#!/usr/bin/python
import os

from fabric.api import *

from collections import OrderedDict

from wx.lib.pubsub import Publisher

env.use_ssh_config = False

class deployed_node():
    def __init__(self,executable,name,cmdArgs='',pids=[]):
        self.name = name
        self.executable = executable
        self.cmdArgs = cmdArgs
        self.pids = pids

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
            #executableString = '/home/{}/{}'.format(host.userName,node.executable)
            #if 'roscore' in node.executable:
            executableString = node.executable
            run('dtach -n `mktemp -u /tmp/dtach.XXXX` {} {}'.format(executableString,node.cmdArgs))
            pgrep = run('ps aux | grep {}'.format(executableString))
            pids = getPIDsFromPS(pgrep,executableString)
            node.pids = pids
            updateQ.put("Deployed {}".format(node.name))
    return host

@parallel
def parallelCopy(hostDict, exec_folder_path, deployment_folder_path, updateQ):
    host = hostDict[env.host_string]
    env.key_filename = host.keyFile
    env.host_string = "{}@{}".format(host.userName,host.ipAddress)

    source = os.path.join(exec_folder_path, "*")
    dest = "/home/" + host.userName + "/."
    put(source, dest)    
    source = os.path.join(deployment_folder_path, "*.xml")
    dest = "/home/" + host.userName + "/."
    put(source, dest)
    updateQ.put("Copied files to {}".format(env.host_string))

@parallel
def parallelCommand(hostDict, command, updateQ):
    host = hostDict[env.host_string]
    env.key_filename = host.keyFile
    env.host_string = "{}@{}".format(host.userName,host.ipAddress)
    run(command)
    updateQ.put("Ran {} on host {}".format(command,env.host_string))

@parallel
def parallelStop(hostDict,updateQ):
    host = hostDict[env.host_string]
    env.key_filename = host.keyFile
    env.host_string = "root@{}".format(host.ipAddress)
    for node in host.nodes:
        if node.pids != [] and len(node.pids) > 0:
            for pid in node.pids:
                try:
                    run('kill -9 {}'.format(pid))
                except SystemExit:
                    pass
        updateQ.put("Killed {}".format(node.name))
        node.pids = []
    return host

@parallel
def parallelMonitor(hostDict,updateQ):
    host = hostDict[env.host_string]
    env.key_filename = host.keyFile
    env.host_string = "root@{}".format(host.ipAddress)
    for node in host.nodes:
        if node.pids != [] and len(node.pids) > 0:
            for pid in node.pids:
                try:
                    status = run('ps --no-headers -p {}'.format(pid))
                    updateQ.put("{} UP".format(node.name))
                except SystemExit:
                    updateQ.put("{} DOWN".format(node.name))
                    node.pids = []
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

def copyTest(hostDict, exec_folder_path, xml_folder_path, progress_q):
    execute(parallelCopy, hostDict, exec_folder_path, xml_folder_path, progress_q)

def runCommandTest(hostDict, command, progress_q):
    execute(parallelCommand, hostDict, command, progress_q)
