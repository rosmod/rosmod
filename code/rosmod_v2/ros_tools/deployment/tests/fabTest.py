#!/usr/bin/python

from fabric.api import *

from collections import OrderedDict

env.use_ssh_config = False

class dep_node():
    def __init__(self,executable,pid=-1):
        self.executable = executable
        self.pid = pid

class dep_host():
    def __init__(self,userName,ipAddress,keyFile,nodes=[],envVars=OrderedDict()):
        self.userName = userName
        self.ipAddress = ipAddress
        self.keyFile = keyFile
        self.nodes = nodes
        self.envVars = envVars

myhostDict = {}
myhostDict['jetson'] = dep_host(userName = 'ubuntu',
                              ipAddress = '129.59.79.169',
                              keyFile = '~/.ssh/id_rsa_jetsontk1',
                              nodes = [dep_node('/home/ubuntu/Repositories/agse2015/code/agse_rosmod_project/01-Software-Configuration/agse/devel/lib/agse_package/positioning')],
                              envVars = OrderedDict([('ROS_MASTER_URI','http://129.59.79.169:11311'),
                                                     ('ROS_IP','129.59.79.169')])
                          )
myhostDict['bbb'] = dep_host(userName = 'ubuntu',
                           ipAddress = '129.59.79.66',
                           keyFile = '~/.ssh/id_rsa_jetsontk1',
                           nodes = [dep_node('/home/ubuntu/repos/agse2015/code/agse_rosmod_project/01-Software-Configuration/agse/devel/lib/agse_package/arm')],
                           envVars = OrderedDict([('ROS_MASTER_URI','http://129.59.79.169:11311'),
                                                  ('ROS_IP','129.59.79.66')])
                       )

env.hosts = ['jetson','bbb']

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
    for key,value in host.envVars.iteritems():
        envVarStr += " export {}={}".format(key,value)
    with prefix(envVarStr):
        for node in host.nodes:
            run('dtach -n `mktemp -u /tmp/dtach.XXXX` {}'.format(node.executable))
            pgrep = run('ps aux | grep {}'.format(node.executable))
            pids = getPIDsFromPS(pgrep,node.executable)
            node.pid = pids[0]
    return host

@parallel
def parallelStop(hostDict):
    host = hostDict[env.host_string]
    env.key_filename = host.keyFile
    env.host_string = "root@{}".format(host.ipAddress)
    envVarStr = ""
    for key,value in host.envVars.iteritems():
        envVarStr += " export {}={}".format(key,value)
    with prefix(envVarStr):
        for node in host.nodes:
            if node.pid != -1:
                run('kill -9 {}'.format(node.pid))
                node.pid = -1

def deployTest():
    global myhostDict
    newHosts = execute(parallelDeploy,myhostDict)
    myhostDict = newHosts
    retVals = execute(parallelStop,myhostDict)

deployTest()

