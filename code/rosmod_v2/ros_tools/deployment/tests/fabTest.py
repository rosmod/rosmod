#!/usr/bin/python

from fabric.api import *

from collections import OrderedDict

env.use_ssh_config = False

class dep_node():
    def __init__(self,userName,ipAddress,keyFile,executables=[],envVars=OrderedDict()):
        self.userName = userName
        self.ipAddress = ipAddress
        self.keyFile = keyFile
        self.executables = executables
        self.envVars = envVars

hostDict = OrderedDict()
hostDict['jetson'] = dep_node(userName = 'ubuntu',
                              ipAddress = '129.59.79.182',
                              keyFile = '~/.ssh/id_rsa_jetsontk1',
                              envVars = OrderedDict([('ROS_MAIN','testSetting')])
                          )
hostDict['bbb'] = dep_node(userName = 'ubuntu',
                           ipAddress = '129.59.79.66',
                           keyFile = '~/.ssh/id_rsa_jetsontk1',
                           envVars = OrderedDict([('ROS_MAIN','testSetting')])
                       )

env.hosts = ['jetson','bbb']

@parallel
def jetson_testfunc():
    host = hostDict[env.host_string]
    env.key_filename = host.keyFile
    env.host_string = "{}@{}".format(host.userName,host.ipAddress)
    with prefix(' '.join('{}={}'.format(key,value) for key,value in host.envVars.iteritems())):
        run('uname -a')
        run('echo $ROS_MAIN')

execute(jetson_testfunc)
