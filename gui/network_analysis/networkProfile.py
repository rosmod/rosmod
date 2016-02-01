"""
Network Profile implements the Profile class.  
This class provides all the members and functions neccessary to 
model, compose, and analyze network profiles for applications 
and systems.  
"""

import copy,sys
import utils
from collections import OrderedDict
from decimal import *

class Profile:
    """
    Profile contains the information about a single network profie.
    A network profile has a kind (e.g. 'provided'), a period (in seconds),
    and a lists of relevant data vs time series (e.g. bandwidth, latency, data, etc.).
    """

    #: Sepearates fileds in a line in a profile file
    field_delimeter = ','
    #: Denotes headers (profile properties) in a profile file
    header_delimeter = '#'
    #: Denotes commends in the profile file
    comment_delimeter = '%'
    #: Splits lines in a profile file
    line_delimeter = '\n'
    #: Strip lines starting with these delimeters to get just profile data
    special_delimeters = [header_delimeter, comment_delimeter]
    #: Which profiles are interpolated between points
    interpolated_profiles = ['data','latency']
    
    def __init__(self, kind = None, period = 0, priority = 0,
                 node = 0, flow_type = None, num_periods = 1, sender_names = []):
        """
        :param string kind: what kind of profile is it?
        :param double period: what is the periodicity (in seconds) of the profile
        :param int priority: what is the priority of the flow in the system
        :param int source: what is the node id from which the data on this profile will be sent
        :param int dest: what is the node id to which the data on this profile will be sent
        """
        self.kind = kind              #: The kind of this profile, e.g. 'required'
        self.period = period          #: The length of one period of this profile
        self.priority = priority      #: The priority of the profile; relevant for 'required' profiles
        self.node_id = node           #: The node ID which is the source of this profile
        self.flow_type = flow_type    #: This flow is the reciever for which sender flows?
        self.entries = OrderedDict()  #: Dictionary of 'type name' : 'list of [x,y] points' k:v pairs 

    def ParseHeader(self, header):
        """
        Parses information from the profile's header if it exists:

        * period
        * priority
        * node ID
        * flow_type             (for matching senders <--> receivers)
        * profile kind          (provided, required, receiver, output, leftover)

        A profile header is at the top of the file and has the following syntax::

            # <property> = <value>

        """
        if header:
            for line in header:
                line.strip('#')
                prop, value = line.split('=')
                if "period" in prop:
                    self.period = Decimal(value)
                elif "priority" in prop:
                    self.priority = int(value)
                elif "node ID" in prop:
                    self.node_id = value.strip()
                elif "flow type" in prop:
                    self.flow_type = value.strip()
                elif "kind" in prop:
                    self.kind = value.strip()

    def ParseFromFile(self, prof_fName):
        """
        Builds the entries from a properly formatted CSV file.  
        Internally calls :func:`Profile.ParseFromString`.
        """
        prof_str = None
        try:
            with open(prof_fName, 'r+') as f:
                prof_str = f.read()
        except:
            print >> sys.stderr, "ERROR: Couldn't find/open {}".format(prof_fName)
            return -1
        if prof_str == None:
            return -1
        return self.ParseFromString( prof_str )

    def ParseFromString(self, prof_str):
        """
        Builds the entries from a string (line list of csv's formatted as per
        :func:`ParseEntriesFromLine`).
        """
        if not prof_str:
            print >> sys.stderr, "ERROR: String contains no profile spec!"
            return -1
        lines = prof_str.split(self.line_delimeter)
        header = [l for l in lines if self.header_delimeter in l]
        self.ParseHeader(header)
        p = copy.copy(lines)
        for s in self.special_delimeters:
            p = [l for l in p if s not in l]
        for line in p:
            if self.ParseEntriesFromLine(line):
                return -1
        self.EntriesRemoveDegenerates()
        self.EntriesStartFill()
        return 0

    def ParseEntriesFromLine(self, line_str):
        """
        Builds the [time, value] list for each type of value into entries:
        
        * slope
        * max slope
        * latency

        These values are formatted in the csv as::

            <time>, <slope>, <max slope>, <latency>

        """
        if line_str:
            fields = line_str.split(self.field_delimeter)
            if len(fields) == 4:
                time = Decimal(fields[0])
                slope = Decimal(fields[1])
                maxSlope = Decimal(fields[2])
                latency = Decimal(fields[3])
                self.entries.setdefault('slope',[]).append([time, slope])
                self.entries.setdefault('max slope',[]).append([time, maxSlope])
                self.entries.setdefault('latency',[]).append([time, latency])
            else:
                print >> sys.stderr,"{} must be formatted:".format(line_str)
                print >> sys.stderr,"\t<time>, <slope>, <max slope>, <latency>"
                return -1
        return 0

    def EntriesRemoveDegenerates(self):
        """Remove duplicate entries by time stamp."""
        for key, values in self.entries.iteritems():
            values = utils.remove_degenerates(values)
        
    def AggregateSlopes(self):
        """Remove sequential entries which have the same slope."""
        self.entries['slope'] = utils.aggregate(self.entries['slope'])

    def EntriesStartFill(self):
        """Make sure all entries have a start time of 0."""
        for name, values in self.entries.iteritems():
            if values[0][0] > 0:
                values.insert(0,[0,0])

    def Repeat(self, num_periods):
        """Copy the current profile entries over some number of its periods."""
        keys = ['slope', 'max slope', 'latency']
        for key in keys:
            if key in self.entries:
                self.entries[key] = utils.repeat(self.entries[key], self.period, num_periods)

    def Integrate(self, time):
        """Integrates the slope entries to produce data entries up to *time*"""
        self.AggregateSlopes()
        self.entries['data'] = utils.integrate(self.entries['slope'], time)

    def Derive(self):
        """Derives the slope entries from the data entries"""
        self.entries['slope'] = utils.derive( self.entries['data'] )
        self.AggregateSlopes()

    def IsKind(self, kind):
        """Returns True if the profile is of type *kind*, False otherwise."""
        return kind in self.kind
    
    def Kind(self,kind):
        """Set the kind of the profile."""
        self.kind = kind

    def Shrink(self, t):
        """Shrink the profile to be <= *t*."""
        for key, values in self.entries.iteritems():
            self.entries[key], r = utils.split(values, t)
        del self.entries['slope'][-1]

    def AddProfile(self,profile):
        """
        Compose this profile with an input profile by adding their slopes together.

        :rtype: :class:`Profile`
        """
        new_slopes = utils.add_values(
            self.entries['slope'],
            profile.entries['slope'],
            interpolate = 'slope' in self.interpolated_profiles
        )
        retProf = copy.deepcopy(self)
        retProf.entries['slope'] = new_slopes
        return retProf

    def SubtractProfile(self,profile):
        """
        Compose this profile with an input profile by subtracting the input profile's slopes.

        :rtype: :class:`Profile`
        """
        new_slopes = utils.subtract_values(
            self.entries['slope'],
            profile.entries['slope'],
            interpolate = 'slope' in self.interpolated_profiles
        )
        retProf = copy.deepcopy(self)
        retProf.entries['slope'] = new_slopes
        return retProf

    def MakeGraphPointsSlope(self):
        """Return matplotlib plottable x and y series for the slope of the profile."""
        return utils.convert_values_to_graph(self.entries['slope'], interpolate = 'slope' in self.interpolated_profiles)

    def MakeGraphPointsData(self):
        """Return matplotlib plottable x and y series for the data of the profile."""
        return utils.convert_values_to_graph(self.entries['data'], interpolate = 'data' in self.interpolated_profiles)

    def GetValueAtTime(self, key, t, interpolate = True):
        """Return the value at time *t* from series *key*, optionally interpolating between."""
        return utils.get_value_at_time(self.entries[key], t, interpolate)

    def ToString(self, prefix = ''):
        """
        Returns a string version of the profile, with all values properly tabulated.

        :rtype: :func:`string`

        :param in prefix: string to be prepended to every line of the returned string.
        """
        retstr = ''
        try:
            from tabulate import tabulate
            newDict = OrderedDict()
            times = []
            for key,values in self.entries.iteritems():
                for val in values:
                    if val[0] not in times:
                        times.append(val[0])
            newDict['time(s)'] = sorted(times)
            for key,values in self.entries.iteritems():
                for t in times:
                    newDict.setdefault(key,[]).append(
                        float(utils.get_value_at_time(
                            values,
                            t,
                            interpolate= key in self.interpolated_profiles
                        ))
                    )
            retstr = tabulate(newDict, headers='keys',floatfmt='.1f')
            r = retstr
            retstr = ''
            for line in r.split('\n'):
                retstr += '{}{}\n'.format(prefix,line)
        except ImportError:
            print >> sys.stderr, "Tabulate module should be installed for printing profiles."
        return retstr

    def ConvertToNC(self,filterFunc, step = 0):
        """
        Perform time-window based integration to generate a Network Calculus curve
        from the profile.  The conversion is configurable based on time-window step-size
        and a filter function (e.g. min or max).  Passing :func:`max` will create an arrival
        curve, while passing :func:`min` will create a service curve.

        :rtype: :class:`Profile`, the network-calculus version of the *self* profile

        .. note:: Requires the profile to have been integrated

        """
        time_list = []
        data_list = []
        for t,d in self.entries['data']:
            time_list.append(t)
            data_list.append(-d)
        new_datas = []
        if step <= 0: step = min( [x for x in time_list if x > 0] )
        for tw in time_list:
            extreme_data = -filterFunc(data_list)
            t = tw
            while t <= time_list[-1]:
                start_data = utils.get_value_at_time(self.entries['data'],
                                                     t - tw,
                                                     interpolate = 'data' in self.interpolated_profiles)
                end_data = utils.get_value_at_time(self.entries['data'],
                                                   t,
                                                   interpolate = 'data' in self.interpolated_profiles)
                diff = end_data - start_data
                extreme_data = filterFunc([diff,extreme_data])
                t += step
            new_datas.append([tw, extreme_data])
            
        new_datas = utils.remove_degenerates(new_datas)
        retProf = Profile(kind = self.kind)
        retProf.entries['data'] = new_datas
        retProf.Derive()
        return retProf

    def CalcDelay(self, output):
        """
        Compute the maximum horizontal distance between this profile and the input profile.  

        This function implements the operation (see :ref:`network_math_formalism`):

        .. math::
            delay = sup\{l^{-1}[y]-r^{-1}[y] : y \in \mathbb{N}\}

        Where

        * :math:`l^{-1}[y]` is the inverse map of the ouptut profile, 
          e.g. a function mapping output data to time
        * :math:`r^{-1}[y]` is the inverse map of the required profile, 
          e.g. a function mapping required data to time

        :rtype: :func:`list` of the form::
        
            [ <time>, <data>, <length of delay> ]
        
        :param in output: a :class:`Profile` describing the output profile
        """
        r = self.entries['data']
        o = output.entries['data']
        delay = utils.max_horizontal_difference(r, o,
                                                interpolate = 'data' in self.interpolated_profiles)
        return delay

    def CalcBuffer(self, output):
        """
        Compute the maximum vertical distance between this profile and the input profile.  

        This function implements the operation (see :ref:`network_math_formalism`): 

        .. math::
            buffer= sup\{r[t] - l[t] : t \in \mathbb{N}\}

        Where

        * :math:`l[t]` is the output profile (see :func:`Profile.Convolve`)
        * :math:`r[t]` is the required profile (*self*)

        :rtype: :func:`list` of the form::

            [ <time>, <data>, <size of the buffer> ]
        
        :param in output: a :class:`Profile` describing the output profile
        """
        r = self.entries['data']
        o = output.entries['data']
        buff = utils.max_vertical_difference(r, o,
                                             interpolate = 'data' in self.interpolated_profiles)
        return buff

    def Delay(self, delayProf):
        """
        Compute the delayed profile composed of *self* profile and *delayProf*,
        received by a node for which this *self* profile is the output profile on the sender side.
        The delay profile describes the delay as a function of time for the link.

        This function implements the operation: 

        .. math::
            o[t + \delta[t]] = l[t]

        Where

        * :math:`\delta[t]` is the delay profile
        * :math:`l[t]` is the profile transmitted into the link (*self*)
        * :math:`o[t]` is the output profile received at the other end of the link

        :rtype: :class:`Profile`, :math:`o[t]`

        :param in delayProf: :class:`Profile` describing the delay
        """
        delays = delayProf.entries['latency']
        all0 = True
        for time, delay in delays:
            if delay != 0:
                all0 = False
        if all0: return copy.deepcopy(self)
        datas = self.entries['data']
        endTime = datas[-1][0]
        times = [ x[0] for x in delays ]
        times.extend( [ x[0] for x in datas ] )
        times = sorted(list(set(times)))
        newDatas = []
        for t in times:
            d = utils.get_value_at_time(datas, t)
            delay = utils.get_value_at_time(delays, t, interpolate = 'latency' in self.interpolated_profiles)
            newDatas.append([ t + delay, d ])
        newDatas = utils.remove_degenerates(newDatas)
        newDatas, remainder = utils.split(newDatas, endTime)
        if remainder:
            t = -remainder[0][0]
            utils.shift(remainder, t)
            r_slopes = utils.derive(remainder)
            d_slopes = utils.derive(newDatas)
            d_slopes = utils.add_values(d_slopes,r_slopes)
            newDatas = utils.integrate(d_slopes, endTime)

        retProf = Profile()
        retProf.entries['data'] = newDatas
        retProf.Derive()
        return retProf

    def Convolve(self, provided):
        """
        Use min-plus calculus to convolve this *required* profile with an input *provided* profile.

        This function implements the operation (see :ref:`network_math_formalism`):

        .. math::
            y=l[t] &= (r \otimes p)[t] = min( r[t] , p[t] - (p[t-1] -l[t-1]) )

        Where

        * :math:`r[t]` is the data profile required by the application (*self*)
        * :math:`p[t]` is the data profile provided by the node's link
        * :math:`l[t]` is the data profile transmitted onto the link

        :rtype: :class:`Profile`, :math:`l[t]`

        :param in provided: a :class:`Profile` describing the node's provided link profile
        """
        r = self.entries['data']
        p = provided.entries['data']
        o = []

        times = [ x[0] for x in p ]
        times.extend( [ x[0] for x in r ] )
        times = sorted(list(set(times)))
        offset = 0
        prevDiff = 0
        prevTime = None
        r_prev = None
        p_prev = None
        for t in times:
            r_data = utils.get_value_at_time(r, t, interpolate = 'data' in self.interpolated_profiles)
            p_data = utils.get_value_at_time(p, t, interpolate = 'data' in self.interpolated_profiles) - offset
            diff = p_data - r_data
            if diff > 0:
                offset += diff
                if cmp(diff,0) != cmp(prevDiff,0):
                    intersection = utils.get_intersection(
                        [ prevTime, r_prev ],
                        [ t, r_data ],
                        [ prevTime, p_prev ],
                        [ t, p_data ]
                    )
                    if intersection:
                        o.append(intersection)
            newPoint = [t, p_data - max(0,diff)]
            o.append(newPoint)
            prevDiff = diff
            prevTime = t
            r_prev = r_data
            p_prev = p_data
        o = utils.remove_degenerates(o)

        output = Profile(kind='output')
        output.entries['data'] = o
        output.Derive()
        return output
