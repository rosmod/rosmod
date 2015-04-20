#!/usr/bin/python

import sys, os, copy, glob, subprocess, datetime, time

class Options:
    def __init__(self, 
        nodes = []
        ):
        self.nodes = nodes
        return

    def __repr__(self):
        return "Options()"

    def __str__(self):
        retStr = "Options():\n"
        for node in self.nodes:
            retStr += "\tNode:\t\t{}\n".format(node)
        return retStr

    def printUsage(self):
        print """Usage:\n\t ./cleanup.py
        \t\t-K <Name of node to kill>
        \t\t-?, -h, --help (to display this help information)\n"""
        return

    def parse_args(self,args):
        if len(args) == 1:
            self.printUsage()
            return -1
        argind = 1
        while argind < len(args):
            if args[argind] == "-K":
                self.nodes.append(args[argind+1])
                argind += 2
            elif args[argind] == "-?" or args[argind] == "-h" or args[argind] == "--help":
                self.printUsage()
                return -1
            else:
                self.printUsage()
                return -1
        return 0

def forceNodeDeath(node):
    print "Trying to shutdown node "+node
    pID = []
    command = ["ps","aux"]
    p = subprocess.Popen(command,stdout=subprocess.PIPE)
    pID = p.communicate()[0]
    pID = pID.split('\n')
    processName = node
    nodePID = []
    for e in pID:
        if processName in e and "nodeNodeLauncher.py" not in e:
            nodePID.append(e)
    for e in nodePID:
        e.strip('\t')
        e.strip('\n')
        e.strip('\r')
    if nodePID == []:
        print "No running process for {}".format(node)
        return 1
    nodePIDs = nodePID[0].split(' ')
    i = 1
    while '' == nodePIDs[i] or ' ' == nodePIDs[i]:
        i+=1
    pID = nodePIDs[i]
    print "Node {} process has PID {}".format(node,pID)
    command = ["kill","-9",pID]
    p = subprocess.Popen(command,stdout=subprocess.PIPE)
    print "Issued command: {} {} {} to kill {}".format(command[0],command[1],command[2],node)        
    return 0

def main():
    options = Options()
    if options.parse_args(sys.argv):
        return -1
    for node in options.nodes:
        forceNodeDeath(node)
    return
  
if __name__ == "__main__":
    main()
