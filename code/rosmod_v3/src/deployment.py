#!/usr/bin/python
import os

from fabric.api import *

from collections import OrderedDict

from wx.lib.pubsub import Publisher

env.use_ssh_config = False

local_ips = ["localhost","127.0.0.1"]

def InitDeployment(self):
    self.deployed = False
    self.deploying = False
    self.runningDeployment = None
    self.runningDeploymentCanvas = None
    self.runningNodes = 0
    self.hostDict = None
    self.updatedHostDict = False

class deployed_node():
    def __init__(self,executable,libs,config,name,deploymentDir,userName,keyFile,cmdArgs='',pids=[]):
        self.name = name
        self.executable = executable
        self.libs = libs
        self.config = config
        self.deploymentDir = deploymentDir
        self.userName = userName
        self.keyFile = keyFile
        self.cmdArgs = cmdArgs
        self.pids = pids

class deployed_host():
    def __init__(self,name,userName,ipAddress,keyFile,deploymentDir,nodes=[],envVars=OrderedDict()):
        self.name = name
        self.userName = userName
        self.ipAddress = ipAddress
        self.deploymentDir = deploymentDir
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
    envVarStr = ""
    for key,value in host.envVars:
        envVarStr += " export {}={}".format(key,value)
    with prefix(envVarStr):
        for node in host.nodes:
            executableString = node.executable
            if host.ipAddress not in local_ips:
                env.key_filename = host.keyFile
                env.host_string = "{}@{}".format(host.userName,host.ipAddress)
                run('dtach -n `mktemp -u /tmp/dtach.XXXX` {} {}'.format(executableString,node.cmdArgs))
                pgrep = run('ps aux | grep {}'.format(executableString))
            else:
                tmp = local('dtach -n `mktemp -u /tmp/dtach.XXXX` {} {}'.format(executableString,node.cmdArgs))
                pgrep = local('ps aux | grep {}'.format(executableString))
            pids = getPIDsFromPS(pgrep,executableString)
            node.pids = pids
            updateQ.put(["Deployed {}".format(node.name),1])
    return host

@parallel
def parallelCopy(hostDict, exec_folder_path, deployment_folder_path, updateQ):
    host = hostDict[env.host_string]
    copyList = []
    copyList.append( 
        [os.path.join(exec_folder_path, "node/node_main"), host.deploymentDir] 
    )
    for node in host.nodes:
        copyList.append( 
            [os.path.join(deployment_folder_path + "/xml/" + host.name, node.config), node.deploymentDir]
        )
        for lib in node.libs:
            copyList.append(
                [os.path.join(exec_folder_path, lib), node.deploymentDir]
            )
        if node.deploymentDir != host.deploymentDir:
            copyList.append( [os.path.join(exec_folder_path, "node/node_main"), node.deploymentDir] )
    if host.ipAddress not in local_ips:
        env.key_filename = host.keyFile
        env.host_string = "{}@{}".format(host.userName,host.ipAddress)
        for source,dest in copyList:
            put(source,dest)
    else:
        for source,dest in copyList:
            local('cp {} {}'.format(source, dest))
    updateQ.put(["Copied files to {}".format(env.host_string),1])

@parallel
def parallelCommand(hostDict, command, updateQ):
    host = hostDict[env.host_string]
    if host.ipAddress not in local_ips:
        env.key_filename = host.keyFile
        env.host_string = "{}@{}".format(host.userName,host.ipAddress)
        run(command)
    else:
        local(command)
    updateQ.put(["Ran {} on host {}".format(command,env.host_string),1])

@parallel
def parallelStop(hostDict,updateQ):
    host = hostDict[env.host_string]
    if host.ipAddress not in local_ips:
        env.key_filename = host.keyFile
        env.host_string = "root@{}".format(host.ipAddress)
        for node in host.nodes:
            if node.pids != [] and len(node.pids) > 0:
                for pid in node.pids:
                    try:
                        run('kill -9 {}'.format(pid))
                    except SystemExit:
                        pass
            updateQ.put(["Killed {}".format(node.name),1])
            node.pids = []
    else:
        for node in host.nodes:
            if node.pids != [] and len(node.pids) > 0:
                for pid in node.pids:
                    try:
                        local('kill -9 {}'.format(pid))
                    except SystemExit:
                        pass
            updateQ.put(["Killed {}".format(node.name),1])
            node.pids = []
    return host

@parallel
def parallelMonitor(hostDict,updateQ):
    host = hostDict[env.host_string]
    if host.ipAddress not in local_ips:
        env.key_filename = host.keyFile
        env.host_string = "root@{}".format(host.ipAddress)
        for node in host.nodes:
            if node.pids != [] and len(node.pids) > 0:
                for pid in node.pids:
                    try:
                        status = run('ps --no-headers -p {}'.format(pid))
                        updateQ.put(["{} UP".format(node.name),1])
                    except SystemExit:
                        updateQ.put(["{} DOWN".format(node.name),1])
                        node.pids = []
            else:
                updateQ.put(["{} DOWN".format(node.name),1])
    else:
        for node in host.nodes:
            if node.pids != [] and len(node.pids) > 0:
                for pid in node.pids:
                    try:
                        status = local('ps --no-headers -p {}'.format(pid))
                        updateQ.put(["{} UP".format(node.name),1])
                    except SystemExit:
                        updateQ.put(["{} DOWN".format(node.name),1])
                        node.pids = []
            else:
                updateQ.put(["{} DOWN".format(node.name),1])
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
