#!/usr/bin/python

import sys, os, copy, glob, subprocess, datetime, time
from collections import OrderedDict

class DeployedObject:
    def __init__(self,node=None,host=None,process=None):
        self.node = node
        self.host = host
        self.process = process  # process.pid = PID

class DeploymentManager:
    '''
    Deployment Manager handles the deployment of nodes (processes)
    onto the hardware hosts
    '''
    def __init__(self,
                hosts = [] # hosts is a list of host objects which contain all relevant properties
             ):
        self.hosts = hosts
        self.nodeMap = OrderedDict() # map node to DeployedObject

    def startNode(self,node,host,name):
        if name not in self.nodeMap or self.nodeMap[name] == None or self.nodeMap[name].process == None:
            cmd = ['ssh']
            if host.properties['ssh_key'] != None:
                cmd.extend(['-i',host.properties['ssh_key']])
            cmd.append(host.properties['user_name']+"@"+host.properties['ip_address'])
            cmd.extend(node.GetExecutable())
            process = subprocess.Popen(cmd)
            '''
            NEED TO FIX THIS TO GET REMOTE PID
            HOW TO ACTUALLY GET PID FROM NODE INSTEAD OF PID ON THIS NODE?
            '''
            self.nodeMap[name] = DeployedObject(node,host,process)
            print 'Started node {} on host {} as {}'.format(node.properties['name'],host.properties['name'],name)
        else:
            print 'Node {} already running'.format(name)
        return

    def killNode(self, name):
        if name in self.nodeMap:
            process = self.nodeMap[name].process
            if process != None:
                '''
                NEED TO FIX THIS TO KILL REMOTE PROCESS
                '''
                process.kill()
                print 'Killed node {}, pid {}'.format(name,process.pid)
                self.nodeMap[name] = None
            else:
                print 'Node {} not currently running!'.format(name)
        else:
            print 'Node {} not currently managed!'.format(name)
        return

    def shutDown(self):
        for name in self.nodeMap.keys():
            self.killNode(name)
        return
