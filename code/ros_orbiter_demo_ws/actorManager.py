#!/usr/bin/python

import sys, os, copy, glob, subprocess, datetime, time

import socket

class ActorManager:
    def __init__(self,
        nodeName = 'alpha',
        a2pmap = {},
        delimeters = ' ',
        port = 7777,
        ip = ""
        ):
        self.nodeName = nodeName
        self.actorToProcessMap = a2pmap
        self.port = port
        self.ip = ip
        self.delimeters = delimeters
        return

    def __repr__(self):
        return "ActorManager()"

    def __str__(self):
        retStr = "ActorManager():\n"
        for actor,process in self.actorToProcessMap:
            retStr += '\t{0} : {1}\n'.format(actor,process.pid)
        return retStr

    def startActor(self, actor):
        if not (actor[0] in self.actorToProcessMap) or self.actorToProcessMap[actor[0]] == None:
            process = subprocess.Popen(actor)
            self.actorToProcessMap[actor[0]] = process
            print 'Started actor {0} : {1}'.format(actor[0],process.pid)
        else:
            print 'Actor {0} already running'.format(actor[0])
        return

    def killActor(self, actorName):
        if actorName in self.actorToProcessMap:
            process = self.actorToProcessMap[actorName]
            if process != None:
                process.kill()
                print 'Killed actor {0}, pid {1}'.format(actorName,process.pid)
                self.actorToProcessMap[actorName] = None
            else:
                print 'Actor {0} not currently running!'.format(actorName)
        else:
            print 'Actor {0} not currently managed!'.format(actorName)
        return

    def parseCommandString(self,command):
        retCode = 0
        if command[0] == "SHUTDOWN":
            retCode = 1
        elif command[0] == "KILL":
            actorName = command[1]
            self.killActor(actorName)
        elif command[0] == "START":
            del(command[0])
            self.startActor(command)
        return retCode

    def loop(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind((self.ip, self.port))
        print 'Waiting for input on port {0}'.format(self.port)
        while True:
            commandString, addr = self.sock.recvfrom(1024)
            print 'Received \"{0}\" from {1}'.format(commandString,addr)
            command = commandString.split(self.delimeters)
            if self.parseCommandString(command) != 0:
                break
        self.shutDown()
        return

    def shutDown(self):
        for actor,process in self.actorToProcessMap.iteritems():
            self.killActor(actor)
        return
