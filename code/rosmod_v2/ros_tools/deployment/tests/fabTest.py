#!/usr/bin/python

from fabric.api import *

from collections import OrderedDict

env.use_ssh_config = False

class deployed_node():
    def __init__(self,executable,cmdArgs='',pid=-1):
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
def parallelDeploy(hostDict):
    host = hostDict[env.host_string]
    env.key_filename = host.keyFile
    env.host_string = "{}@{}".format(host.userName,host.ipAddress)
    envVarStr = ""
    for key,value in host.envVars:
        envVarStr += " export {}={}".format(key,value)
    with prefix(envVarStr):
        for node in host.nodes:
            executableString = '/home/{}/{}'.format(host.userName,node.executable)
            run('dtach -n `mktemp -u /tmp/dtach.XXXX` {}'.format(executableString))
            pgrep = run('ps aux | grep {}'.format(executableString))
            pids = getPIDsFromPS(pgrep,executableString)
            node.pid = pids[0]
    return host

@parallel
def parallelStop(hostDict):
    host = hostDict[env.host_string]
    env.key_filename = host.keyFile
    env.host_string = "root@{}".format(host.ipAddress)
    envVarStr = ""
    for key,value in host.envVars:
        envVarStr += " export {}={}".format(key,value)
    with prefix(envVarStr):
        for node in host.nodes:
            if node.pid != -1:
                run('kill -9 {}'.format(node.pid))
                node.pid = -1

def deployTest(q):
    hostDict = q.get()
    newHosts = execute(parallelDeploy,hostDict)
    hostDict = newHosts
    q.put(hostDict)
    return hostDict

def stopTest(q):
    hostDict = q.get()
    retVals = execute(parallelStop,hostDict)
    q.put(retVals)
    return retVals
    
    

