#!/usr/bin/python

from fabric.api import *

from collections import OrderedDict

env.use_ssh_config = False

class dep_node():
    def __init__(self,userName,ipAddress,keyFile):
        self.userName = userName
        self.ipAddress = ipAddress
        self.keyFile = keyFile

hostDict = OrderedDict()
hostDict['jetson'] = dep_node('ubuntu','129.59.79.182','~/.ssh/id_rsa_jetsontk1')
hostDict['bbb'] = dep_node('ubuntu','129.59.79.66','~/.ssh/id_rsa_jetsontk1')

env.hosts = ['jetson','bbb']

@parallel
def jetson_testfunc():
    host = hostDict[env.host_string]
    env.key_filename = host.keyFile
    env.host_string = "{}@{}".format(host.userName,host.ipAddress)
    run('uname -a')

execute(jetson_testfunc)
