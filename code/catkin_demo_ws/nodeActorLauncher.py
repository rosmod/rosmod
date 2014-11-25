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

class Options:
    def __init__(self, 
        node = 'alpha',
        log = 'nodeActorLauncher.log',
        redirect = False,
        actors = []
        ):
        self.node = node
        self.log_filename = log
        self.redirect_to_file = redirect
        self.actors = actors
        return

    def __repr__(self):
        return "Options()"

    def __str__(self):
        retStr = "Options():\n"
        retStr += "\tNode:\t\t{0}\n".format(self.node)
        retStr += "\tLog file:\t{0}\n".format(self.log_filename)
        retStr += "\tRedirect?:\t{0}\n".format(self.redirect_to_file)
        for actor in self.actors:
            retStr += "\tActor:\t\t{0}\n".format(actor)
        return retStr

    def parse_args(self,args):
        argind = 1
        while argind < len(args):
            if args[argind] == "-N":
                self.node = args[argind+1]
                argind += 2
            elif args[argind] == "-A":
                tmpind = argind + 2
                cmd = [args[argind+1]]
                while tmpind < len(args) and args[tmpind][0] != '-':
                    cmd.append(args[tmpind])
                    tmpind += 1
                self.actors.append(cmd)
                argind = tmpind
            elif args[argind] == "-L":
                self.log_filename = args[argind+1]
                argind += 2
            elif args[argind] == "-r":
                self.redirect_to_file = True
                argind += 1
            elif args[argind] == "-?" or args[argind] == "-h" or args[argind] == "--help":
                print "Usage:\n\tpython ",args[0],"""
                \t\t-N <(N)ode name>
                \t\t-L <program (L)og filename>
                \t\t-r ((p)edirect program output to log file)
                \t\t-A <actor executable with path and cmd line arguments>\n"""
                return -1
            else:
                print """Usage:\n\t""",args[0],"""
                \t\t-N <(N)ode name>
                \t\t-L <program (L)og filename>
                \t\t-r ((p)edirect program output to log file)
                \t\t-A <actor executable with path and cmd line arguments>\n"""
                return -1
        return 0

def main():
    options = Options()

    if options.parse_args(sys.argv):
        return -1

    if options.redirect_to_file == True:
        sys.stdout = open(options.log_filename, "w")

    print 'Managing actors on node {0}'.format(options.node)

    actorProcessMap = {}

    for actor in options.actors:
        actorProcessMap[actor[0]] = subprocess.Popen(actor)
        print 'Started actor {0} on node {1}'.format(actor[0],options.node)

    time.sleep(10)
    
    for actor,process  in actorProcessMap.iteritems():
        process.kill()
        print 'Killed actor {0}, pid {1} on node {2}'.format(actor,process.pid,options.node)

    return
  
if __name__ == "__main__":
    main()
