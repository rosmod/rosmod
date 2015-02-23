#!/usr/bin/python

from fabric.api import *

from collections import OrderedDict

env.use_ssh_config = False

class dep_host():
    def __init__(self,userName,ipAddress,keyFile,executables=[],envVars=OrderedDict()):
        self.userName = userName
        self.ipAddress = ipAddress
        self.keyFile = keyFile
        self.executables = executables
        self.envVars = envVars

hostDict = OrderedDict()
hostDict['jetson'] = dep_host(userName = 'ubuntu',
                              ipAddress = '129.59.79.182',
                              keyFile = '~/.ssh/id_rsa_jetsontk1',
                              executables = ['~/repos/agse2015/code/AGSE_ROSMOD/agse/devel/lib/agse_package/positioning'],
                              envVars = OrderedDict([('ROS_MASTER_URI','http://129.59.79.182:11311'),
                                                     ('ROS_IP','129.59.79.182')])
                          )
hostDict['bbb'] = dep_host(userName = 'ubuntu',
                           ipAddress = '129.59.79.66',
                           keyFile = '~/.ssh/id_rsa_jetsontk1',
                           executables = ['~/repos/agse2015/code/AGSE_ROSMOD/agse/devel/lib/agse_package/arm'],
                           envVars = OrderedDict([('ROS_MASTER_URI','http://129.59.79.182:11311'),
                                                  ('ROS_IP','129.59.79.66')])
                       )

env.hosts = ['jetson','bbb']

@parallel
def testfunc():
    host = hostDict[env.host_string]
    env.key_filename = host.keyFile
    env.host_string = "{}@{}".format(host.userName,host.ipAddress)
    envVarStr = ""
    for key,value in host.envVars.iteritems():
        envVarStr += " export {}={}".format(key,value)
    with prefix(envVarStr):
        for executable in host.executables:
            run('echo $ROS_MASTER_URI')
            run('echo $ROS_IP')
            run('{}'.format(executable))
            run('ps aux | grep {}'.format(executable))
            #run('echo $ROS_MAIN')

execute(testfunc)
