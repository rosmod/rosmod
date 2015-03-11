#!/usr/bin/python

from fabric.api import *

from collections import OrderedDict

from wx.lib.pubsub import Publisher

env.use_ssh_config = False

class deployed_node():
    def __init__(self,executable,name,cmdArgs='',pid=-1):
        self.name = name
        self.executable = executable
        self.cmdArgs = cmdArgs
        self.pid = pid

class deployed_host():
    def __init__(self,userName,ipAddress,keyFile,nodes=[],envVars=OrderedDict()):
        self.userName = userName
        self.ipAddress = ipAddress
        self.keyFile = keyFile
        self.nodes = nodes
        self.envVars = envVars

def getStatusFromPS(psString, name):
    pass

def getPIDsFromPS(psString, name):
    pids = []
    psString = psString.split('\n')
    psList = []
    for e in psString:
        if name in e and "grep" not in e and "dtach" not in e:
            psList.append(e)
    for e in psList:
        e.strip('\t')
        e.strip('\n')
        e.strip('\r')
    if psList == []:
        print "No running process for {}".format(name)
    else:
        for ps in psList:
            tmp = ps.split(' ')
            i = 1
            while '' == tmp[i] or ' ' == tmp[i]:
                i+=1
            pids.append(tmp[i])
            print "process {} has PID {}".format(name,pids[-1])
    return pids

@parallel
def parallelDeploy(hostDict,updateQ):
    host = hostDict[env.host_string]
    env.key_filename = host.keyFile
    env.host_string = "{}@{}".format(host.userName,host.ipAddress)
    envVarStr = ""
    for key,value in host.envVars:
        envVarStr += " export {}={}".format(key,value)
    with prefix(envVarStr):
        for node in host.nodes:
            executableString = '/home/{}/{}'.format(host.userName,node.executable)
            run('dtach -n `mktemp -u /tmp/dtach.XXXX` {} {}'.format(executableString,node.cmdArgs))
            pgrep = run('ps aux | grep {}'.format(executableString))
            pids = getPIDsFromPS(pgrep,executableString)
            node.pid = pids[0]
            updateQ.put("Deployed {}".format(node.name))
    return host

@parallel
def parallelStop(hostDict,updateQ):
    host = hostDict[env.host_string]
    env.key_filename = host.keyFile
    env.host_string = "root@{}".format(host.ipAddress)
    for node in host.nodes:
        if node.pid != -1:
            try:
                run('kill -9 {}'.format(node.pid))
            except SystemExit:
                node.pid = -1
        updateQ.put("Killed {}".format(node.name))
    return host

@parallel
def parallelMonitor(hostDict,updateQ):
    host = hostDict[env.host_string]
    env.key_filename = host.keyFile
    env.host_string = "root@{}".format(host.ipAddress)
    for node in host.nodes:
        if node.pid != -1:
            try:
                status = run('ps --no-headers -p {}'.format(node.pid))
                updateQ.put("{} UP".format(node.name))
            except SystemExit:
                updateQ.put("{} DOWN".format(node.name))
                node.pid = -1
    return host

def deployTest(hostDict, host_topic, progress_q):
    newHosts = execute(parallelDeploy,hostDict,progress_q)
    Publisher().sendMessage(host_topic,newHosts)

def stopTest(hostDict, host_topic, progress_q):
    newHosts = execute(parallelStop,hostDict,progress_q)
    Publisher().sendMessage(host_topic,newHosts)
    
def monitorTest(hostDict,  host_topic, progress_q):
    newHosts = execute(parallelMonitor,hostDict,progress_q)
    Publisher().sendMessage(host_topic,newHosts)
