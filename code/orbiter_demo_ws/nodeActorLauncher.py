#!/usr/bin/python
"""
This program is designed to launch the ROS Orbiter demo application actors on a specified node.

It takes as input (either from the command line or through a socket):
  * name of the node it is deployed on
  * list of actors which need to be started
  * list of actors which need to be killed

It performs the following actions:
  * kills the processes associated with any actors that need to be killed
  * sets up the environment for all actors which must be started
  * starts every actor in the list of actors to start
""" 

import sys, os, copy, glob, subprocess, datetime, time

from actorManager import *

class Options:
    def __init__(self, 
        socketListen = False,
        dur = 10,
        log = 'nodeActorLauncher.log',
        redirect = False,
        actors = []
        ):
        self.socketListen = socketListen
        self.duration = dur
        self.log_filename = log
        self.redirect_to_file = redirect
        self.actors = actors
        return

    def __repr__(self):
        return "Options()"

    def __str__(self):
        retStr = "Options():\n"
        retStr += "\tListen on Socket?:\t{0}\n".format(self.socketListen)
        retStr += "\tDuration:\t{0}\n".format(self.duration)
        retStr += "\tLog file:\t{0}\n".format(self.log_filename)
        retStr += "\tRedirect?:\t{0}\n".format(self.redirect_to_file)
        for actor in self.actors:
            retStr += "\tActor:\t\t{0}\n".format(actor)
        return retStr

    def printUsage(self):
        print """Usage:\n\t ./nodeActorLauncher.py
        \t\t-l (to (l)isten indefinitely on a socket)
        \t\t-A <(A)ctor executable with path and cmd line arguments>
        \t\t-D <(D)uration to run cli-provided actors>
        \t\t-L <program (L)og filename>
        \t\t-r (to (r)edirect program output to log file)
        \t\t-?, -h, --help (to display this help information)\n"""
        return

    def parse_args(self,args):
        if len(args) == 1:
            self.printUsage()
            return -1
        argind = 1
        while argind < len(args):
            if args[argind] == "-l":
                self.socketListen = True
                argind += 1
            elif args[argind] == "-A":
                tmpind = argind + 2
                cmd = [args[argind+1]]
                while tmpind < len(args) and args[tmpind][0] != '-':
                    cmd.append(args[tmpind])
                    tmpind += 1
                self.actors.append(cmd)
                argind = tmpind
            elif args[argind] == "-D":
                self.duration = float(args[argind+1])
                argind += 2
            elif args[argind] == "-L":
                self.log_filename = args[argind+1]
                argind += 2
            elif args[argind] == "-r":
                self.redirect_to_file = True
                argind += 1
            elif args[argind] == "-?" or args[argind] == "-h" or args[argind] == "--help":
                self.printUsage()
                return -1
            else:
                self.printUsage()
                return -1
        return 0

def main():
    options = Options()

    if options.parse_args(sys.argv):
        return -1

    if options.redirect_to_file == True:
        sys.stdout = open(options.log_filename, "w")

    actorManager = ActorManager()

    if options.socketListen == True:
        actorManager.loop()
    else:
        if len(options.actors) > 0:
            print 'Managing {0} actors for {1} seconds'.format(len(options.actors),options.duration)
            for actor in options.actors:
                actorManager.startActor(actor)
            time.sleep(options.duration)    
            actorManager.shutDown()
        else:
            print 'No actors provided, shutting down.'            

    return
  
if __name__ == "__main__":
    main()
