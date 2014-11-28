#!/usr/bin/python

import sys, os, copy, glob, subprocess, datetime, time

class Options:
    def __init__(self, 
        actors = []
        ):
        self.actors = actors
        return

    def __repr__(self):
        return "Options()"

    def __str__(self):
        retStr = "Options():\n"
        for actor in self.actors:
            retStr += "\tActor:\t\t{0}\n".format(actor)
        return retStr

    def printUsage(self):
        print """Usage:\n\t ./cleanup.py
        \t\t-K <Name of actor to kill>
        \t\t-?, -h, --help (to display this help information)\n"""
        return

    def parse_args(self,args):
        if len(args) == 1:
            self.printUsage()
            return -1
        argind = 1
        while argind < len(args):
            if args[argind] == "-K":
                self.actors.append(args[argind+1])
                argind += 2
            elif args[argind] == "-?" or args[argind] == "-h" or args[argind] == "--help":
                self.printUsage()
                return -1
            else:
                self.printUsage()
                return -1
        return 0

def forceActorDeath(actor):
    print "Trying to shutdown actor "+actor
    pID = []
    command = ["ps","aux"]
    p = subprocess.Popen(command,stdout=subprocess.PIPE)
    pID = p.communicate()[0]
    pID = pID.split('\n')
    processName = actor
    actorPID = []
    for e in pID:
        if processName in e and "nodeActorLauncher.py" not in e:
            actorPID.append(e)
    for e in actorPID:
        e.strip('\t')
        e.strip('\n')
        e.strip('\r')
    if actorPID == []:
        print "No running process for {0}".format(actor)
        return 1
    actorPIDs = actorPID[0].split(' ')
    i = 1
    while '' == actorPIDs[i] or ' ' == actorPIDs[i]:
        i+=1
    pID = actorPIDs[i]
    print "Actor {0} process has PID {1}".format(actor,pID)
    command = ["kill","-9",pID]
    p = subprocess.Popen(command,stdout=subprocess.PIPE)
    print "Issued command: {0} {1} {2} to kill {3}".format(command[0],command[1],command[2],actor)        
    return 0

def main():
    options = Options()
    if options.parse_args(sys.argv):
        return -1
    for actor in options.actors:
        forceActorDeath(actor)
    return
  
if __name__ == "__main__":
    main()
