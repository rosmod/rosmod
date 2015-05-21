#!/usr/bin/python
"""
""" 

# QoS files have 4 columns: time (s), BW(bps), latency (ms), Network Link (id #)
import sys, os, csv, copy, glob

class Options:
    def __init__(self,
        tdmaPeriod = 0.01,
        tdmaSlots = 2,
        node = "notdma",
        outputFilename = "tdma",
        activeSlot = 0
        ):
        self.tdmaPeriod = tdmaPeriod
        self.tdmaSlots = tdmaSlots
        self.node = node
        self.outputFilename = outputFilename
        self.activeSlot = activeSlot

    def parseArgs(self,args):
        argind = 1
        while argind < len(args):
            if args[argind] == "-P":
                self.tdmaPeriod = float(args[argind+1])
                if self.tdmaPeriod <= 0:
                    print "Error! You must specify a tdma time period > 0"
                    return -1
                argind += 2
            elif args[argind] == "-N":
                self.node = args[argind+1]
                argind += 2
            elif args[argind] == "-O":
                self.outputFilename = args[argind+1]
                argind += 2
            elif args[argind] == "-S":
                self.tdmaSlots = int(args[argind+1])
                if self.tdmaSlots <= 0:
                    print "Error! You must specify a number of tmda slots > 0"
                argind += 2
            elif args[argind] == "-A":
                self.activeSlot = float(args[argind+1])
                if self.activeSlot <= 0:
                    print "Error! You must specify an active TDMA slot > 0"
                    return -1
                argind += 2
            elif args[argind] == "-?" or args[argind] == "-h":
                print """Usage:\n\t""",args[0],"""
                \t\t-N <(N)ode name>
                \t\t-O <(O)utput Filename>
                \t\t-S <number of tdma (S)lots in each period>
                \t\t-A <0-indexed ID of (A)ctive slot>
                \t\t-P <tdma (P)eriod (seconds)>\n"""
                return -1
            else:
                print """Usage:\n\t""",args[0],"""
                \t\t-N <(N)ode name>
                \t\t-O <(O)utput Filename>
                \t\t-S <number of tdma (S)lots in each period>
                \t\t-A <0-indexed ID of (A)ctive slot>
                \t\t-P <tdma (P)eriod (seconds)>\n"""
                return -1
        return 0

class TDMA:
    def __init__(self,period,slots,selectedSlot):
        self.period = period
        self.slots = slots
        self.selectedSlot = selectedSlot

class ProfileEntry:
    def __init__(self,start=-1,end=-1,bandwidth=-1,interface=''):
        self.start = start
        self.end = end
        self.bandwidth = bandwidth
        self.interface = interface 

    def __str__(self):
        retStr = ""
        retStr += "{0},{1},0,{2}".format(self.start,self.bandwidth,self.interface)
        return retStr

    def fromLine(self,line=None):
        if line == None or len(line) == 0:
            self.__init__()
            return
        fields = line.split(',')
        if len(fields) == 0 or fields[0][0] == '%':
            self.__init__()
            return
        self.start = float(fields[0])
        self.bandwidth = float(fields[1])
        self.interface = fields[3]

def generateNewProfile(oldProfile,tdma):
    newProfile = []
    for interval in oldProfile:
        tdmaBandwidth = interval.bandwidth * tdma.slots
        tdmaTime = interval.start
        while tdmaTime < interval.end:
            for slotNum in range(0,tdma.slots):
                selectedBW = 0
                if slotNum == tdma.selectedSlot:
                    selectedBW = tdmaBandwidth
                newProfile.append(
                    ProfileEntry(
                        tdmaTime,
                        tdmaTime + tdma.period / tdma.slots,
                        selectedBW,
                        interval.interface
                        )
                    )
                tdmaTime += tdma.period / tdma.slots
    return newProfile

def get_nodeProfiles(folder):
    profile_dir = os.getcwd()+os.sep+folder
    nodes = {}
    if os.path.isdir(profile_dir):
        print 'Found ',profile_dir
        for file in glob.glob(profile_dir+os.sep+'*config.csv'):
            node_name = file.replace('_crm_config.csv','')
            node_name = node_name.replace(profile_dir+os.sep,'')
            if node_name != 'crm_config.csv':
                with open(file,'r+') as f:
                    content = f.read()
                    nodes[node_name] = content
    else:
        print "ERROR: ",profile_dir," doesn't exist!"
    return nodes

def generateProfile(txtProfile):
    nodeProfile = []
    lines = txtProfile.split('\n')
    for line in lines:
        newEntry = ProfileEntry()
        newEntry.fromLine(line)
        if newEntry.start >= 0:
            if len(nodeProfile) > 0:
                nodeProfile[-1].end = newEntry.start
            nodeProfile.append(newEntry)
    return nodeProfile

def main():    
    args = sys.argv

    options = Options()
    if options.parseArgs(args) < 0:
        return
    tdma = TDMA(options.tdmaPeriod,options.tdmaSlots,options.activeSlot)

    nodes = get_nodeProfiles('scripts')

    print nodes[options.node]

    nodeProfile = generateProfile(nodes[options.node])

    tdmaProfile = generateNewProfile(nodeProfile,tdma)

    with open("./scripts/{0}_crm_config.csv".format(options.outputFilename),'w') as f:
        for interval in tdmaProfile:
            f.write("{0}\n".format(interval))

    return
  
if __name__ == "__main__":
    main()
