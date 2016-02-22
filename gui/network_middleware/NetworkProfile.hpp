#ifndef NETWORK_PROFILE_HPP
#define NETWORK_PROFILE_HPP

#include "CSVIterator.hpp"
#include "Message.hpp"

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <iomanip>
#include <algorithm>    // std::swap
#include <streambuf>
#include <list>
#include <vector>
#include <memory>

#include <boost/algorithm/string.hpp>

#include <time.h>

namespace Network {

  static const int ipv4_header_bytes = 20;
  static const int ipv4_route_bytes = 11;
  static const int ipv4_header_padding_bytes = 1;
  static const int udp_header_bytes = 8;
  static const int tcp_header_bytes = 20;

  class Exceeded_Production_Profile
  {
  };

  struct profileMemBuf : std::streambuf {
    profileMemBuf(char* begin, char* end) {
      this->setg(begin, begin, end);
    }
  };

  class ResourceEntry {
  public:
    double time;                      // sec
    uint64_t bandwidth;     // bits / sec
    uint64_t max_bandwidth; // bits / sec
    uint64_t data;          // bits
    double latency;                   // sec

    std::string toString() {
      char charBuf[100];
      sprintf(charBuf,"%f, %lu, %lu, %lu, %f",
	      time, bandwidth, max_bandwidth, data, latency);
      std::string retStr = charBuf;
      return retStr;
    }
  };

  class NetworkProfile {
  private:
    bool initialized;
  public:
    std::vector<ResourceEntry> resources;
    timespec start_time;
    double period;
    uint64_t priority;
    uint64_t uuid;

  public:

    NetworkProfile()
      : initialized(false),
	resources (0),
	period (0),
	priority (0),
	uuid (0)
    {
      start_time.tv_sec = 0;
      start_time.tv_nsec = 0;
    }

    NetworkProfile (const NetworkProfile &s)
      : initialized(s.initialized),
	resources(s.resources),
	period(s.period),
	priority(s.priority),
	uuid(s.uuid)
    {
      start_time.tv_sec = s.start_time.tv_sec;
      start_time.tv_nsec = s.start_time.tv_nsec;
    }

    ~NetworkProfile()
    {
    }

    NetworkProfile & operator= (const NetworkProfile &s)
    {
      if (&s != this)
	{
	  NetworkProfile tmp (s);
	  swap (tmp);
	}
      return  *this;
    }

    NetworkProfile* clone() const {
      return new NetworkProfile( *this );
    }

    void swap (NetworkProfile &s) {
      std::swap (initialized, s.initialized);
      std::swap (resources, s.resources);
      std::swap (period, s.period);
      std::swap (priority, s.priority);
      std::swap (uuid, s.uuid);
      std::swap (start_time.tv_sec, s.start_time.tv_sec);
      std::swap (start_time.tv_nsec, s.start_time.tv_nsec);
    }

    std::string toString() {
      std::string retStr = "";
      for (auto it=resources.begin(); it != resources.end(); ++it)
	{
	  retStr += it->toString() + "\n";
	}
      return retStr;
    }

    int setStartTime(double t)
    {
      double fractpart,intpart;
      fractpart = modf(t,&intpart);
      start_time.tv_sec = (unsigned long)(intpart);
      start_time.tv_nsec = (unsigned long)(fractpart*1000000000.0);
      return 0;
    }

    void setPeriod(double t) { period = t; }

    void setPriority(uint64_t p) { priority = p; }

    void setUUID(uint64_t u) { uuid = u; }

    int initializeFromFile(const char* fname) {
      std::ifstream file(fname);
      if ( !file.is_open() ) {
	return -1;
      }
      return initializeFromIStream(file);
    }

    int initializeFromString(char* buffer) {
      profileMemBuf sbuf(buffer,buffer + strlen(buffer));
      std::istream file(&sbuf);
      return initializeFromIStream(file);
    }

    int initializeFromIStream(std::istream& stream)
    {
      std::vector<std::vector<double> > csv;

      for (CSVIterator loop(stream);loop != CSVIterator();++loop)
	{
	  std::vector<double> rowvec;
	  if ( (*loop).size() > 0 && (*loop)[0].c_str()[0] != '%' )
	    {
	      if ( (*loop)[0].c_str()[0] != '#' )
		{
		  for (int i=0;i<(*loop).size();i++)
		    rowvec.push_back( atof( (*loop)[i].c_str() ) );
		  csv.push_back(rowvec);
		}
	      else
		{
		  std::string row = "";
		  for (int i=0;i<(*loop).size();i++)
		    row += (*loop)[i];
		  boost::trim_if(row, boost::is_any_of("# "));
		  std::vector<std::string> strs;
		  boost::split(strs, row, boost::is_any_of("="));
		  for (auto it = strs.begin(); it!=strs.end(); ++it)
		    boost::trim(*it);
		  if (!strcmp("period", strs[0].c_str()) )
		    setPeriod( atof(strs[1].c_str()) );
		  else if (!strcmp("start", strs[0].c_str()) )
		    setStartTime( atof(strs[1].c_str()) );
		  else if (!strcmp("priority", strs[0].c_str()) )
		    setPriority( atoi(strs[1].c_str()) );
		  else if (!strcmp("uuid", strs[0].c_str()) )
		    setUUID( atoi(strs[1].c_str()) );
		}
	    }
	}
      if ( parse_csv(csv) )
	return -1;
      else {
	initialized = true;
	return 0;
      }
    }

    int parse_csv(std::vector<std::vector<double> > csv) {
      // each csv row contains time,bandwidth,latency
      for (int i=0;i<csv.size();i++) {
	ResourceEntry entry;
	entry.time = csv[i][0];                              // s
	entry.bandwidth = (uint64_t) (csv[i][1]);  // bps
	if (csv[i].size() == 3)
	  {
	    entry.latency = csv[i][2];    // s
	    entry.max_bandwidth = entry.bandwidth;
	  }
	else if (csv[i].size() == 4)
	  {
	    entry.max_bandwidth = (uint64_t) (csv[i][2]); // bps
	    entry.latency = csv[i][3];    // s
	  }

	if ( HasEntries() ) {
	  entry.data = resources.back().data +
	    resources.back().bandwidth *
	    (entry.time - resources.back().time);
	}
	else {
	  entry.data = 0;
	}
	//printf("Interval %d: %s\n", i, entry.toString().c_str());
	resources.push_back(entry);
      }
      if (HasEntries() && (resources.back().time < period)) {
	ResourceEntry entry;
	entry.time = period;
	entry.bandwidth = 0;
	entry.latency = resources[0].latency;
	entry.data = resources.back().data +
	  resources.back().bandwidth *
	  (entry.time - resources.back().time);
	resources.push_back(entry);
      }
      return 0;
    }

    inline double getOffset(timespec& t) {
      double _start = (double)(start_time.tv_sec) +
	(double)(start_time.tv_nsec)/1000000000.0;
      double _time = (double)(t.tv_sec) + (double)(t.tv_nsec)/1000000000.0;
      double offset = 0;
      offset += (double)(t.tv_sec - start_time.tv_sec);
      offset += ((double)(t.tv_nsec - start_time.tv_nsec)/1000000000.0);
      offset = fmod(offset,period);
      offset = fabs(offset);
      if ( _time < _start ) {
	offset = period - offset;
      }
      return offset;
    }

    uint64_t getDataAtTime( timespec t )
    {
      if (!HasEntries())
	return 0;
      uint64_t retData = 0;
      double offset = getOffset(t);
      int i = 0;
      for ( i = 0; i < resources.size(); i++)
	{
	  if ( resources[i].time > offset )
	    break;
	}
      if ( i < resources.size() - 1 )
	i = i - 1;
      double end = resources[i].time;
      uint64_t bw = resources[i].bandwidth;
      uint64_t endData = resources[i].data;
      retData = endData - bw * (end-offset);
      return retData;
    }

    int getCurrentInterval( uint64_t& bandwidth, double& latency ) {
      if (!HasEntries())
	return -1;
      timespec currentTime;
      int returnCode = clock_gettime (CLOCK_REALTIME, &currentTime);
      double offset = getOffset(currentTime);
      bandwidth = resources[0].bandwidth;
      latency = resources[0].latency;
      for ( int i=0; i < resources.size(); i++ ) {
	if ( resources[i].time > offset ) {
	  bandwidth = resources[i-1].bandwidth;
	  latency = resources[i-1].latency;
	  break;
	}
      }
      //printf("current: %f, %lu, %f\n", offset, bandwidth, latency);
      return 0;
    }

    int getNextInterval( timespec& start, uint64_t& bandwidth, double& latency ) {
      if (!HasEntries())
	return -1;
      timespec currentTime;
      int returnCode = clock_gettime (CLOCK_REALTIME, &currentTime);
      double offset = getOffset(currentTime);
      double end = period;
      bandwidth = resources[0].bandwidth;
      latency = resources[0].latency;
      for ( int i=0; i < resources.size() - 1; i++ ) {
	if ( resources[i].time > offset ) {
	  end = resources[i].time;
	  bandwidth = resources[i].bandwidth;
	  latency = resources[i].latency;
	  break;
	}
      }
      double timeDiff = end - offset;
      double fractpart, intpart;
      fractpart = modf(timeDiff, &intpart);
      start.tv_sec = (unsigned long) intpart;
      start.tv_nsec = (unsigned long)( fractpart*1000000000.0f );
      start.tv_sec += currentTime.tv_sec;
      start.tv_nsec += currentTime.tv_nsec;
      if ( start.tv_nsec > 999999999 ) {
	start.tv_sec += 1;
	start.tv_nsec = (start.tv_nsec - 1000000000);
      }
      return 0;
    }

    double Delay(uint64_t dataLenBits, timespec sentTime) {
      if (!HasEntries())
	return -1;

      double offset = getOffset(sentTime);

      double start = resources.back().time;
      uint64_t offsetData = resources.back().data;
      uint64_t bandwidth = resources.back().bandwidth;
      int res_id = resources.size() - 1;
      for (int i=0;i<resources.size();i++) {
	if ( resources[i].time > offset ) {
	  res_id = i;
	  start = resources[i-1].time;
	  bandwidth = resources[i-1].bandwidth;
	  offsetData = resources[i-1].data;
	  break;
	}
      }

      offsetData += (uint64_t)((double)(offset-start)*((double)bandwidth));

      double timeDiff = 0;
      uint64_t dataInPeriod = resources.back().data;
      uint64_t dataToEnd = dataInPeriod - offsetData;
      uint64_t numPeriods = dataLenBits / dataInPeriod;
      uint64_t modData = dataLenBits % dataInPeriod;
  
      if ( numPeriods > 0 ) { // will take more than numPeriods to send data
	timeDiff += (double)numPeriods * period;
      }

      if ( dataToEnd < modData ) { // will have to cycle back to beginning to send data
	timeDiff += period - offset;
	offsetData = 0;
	offset = 0;
	res_id = 0;
	modData = modData - dataToEnd;
      }

      uint64_t remainder = modData;
      if ( (resources[res_id].data - offsetData) <= modData ) {
	remainder = modData - (resources[res_id].data - offsetData);
	timeDiff += resources[res_id++].time - offset;
	while ( (resources[res_id].data - offsetData) < modData ) {
	  remainder = modData - (resources[res_id].data - offsetData);
	  timeDiff += resources[res_id].time - resources[res_id-1].time;
	  res_id++;
	}
      }
      res_id--;

      timeDiff += (double)remainder / (double)resources[res_id].bandwidth;

      int  return_code;
      timespec current_time;
      return_code = clock_gettime (CLOCK_REALTIME, &current_time);
      double delta = 0;
      delta += (double)(current_time.tv_sec - sentTime.tv_sec);
      delta += ((double)(current_time.tv_nsec - sentTime.tv_nsec)/1000000000.0);
      timeDiff = timeDiff - delta;

      if (timeDiff < 0)
	timeDiff = 0;

      return timeDiff;
    }
  
    bool Initialized() const { return initialized; }
    bool HasEntries() const { return resources.size() > 0; }
  };

  static std::string write_header(numTimes) {
    std::string retStr;
    retStr << "ID";
    for (int i=0; i<numTimes; i++)
      retStr << ", Time (s)";
    retStr << ", Size (bits)\n";
    return retStr;
  }

  static int write_data(const char* fname, const std::vector<Network::Message> messages) {
    std::string fStr;
    fStr << Network::write_header(messages[0].NumTimes());
    for (auto it=messages.begin(); it != messages.end(); ++it) {
      fStr << it->ToString() << "\n";
    }
    std::ofstream file(fname);
    if ( !file.is_open() )
      return -1;
    file << fStr;
    file.close();
    return 0;
  }
};
#endif
