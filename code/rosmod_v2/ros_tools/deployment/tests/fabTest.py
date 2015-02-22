#!/usr/bin/python

from fabric.api import env, run, sudo

env.use_ssh_config = False

class dep_node():
    def __init__(self,userName,ipAddress,keyFile):
        self.userName = userName
        self.ipAddress = ipAddress
        self.keyFile = keyFile

hosts = [dep_node('ubuntu','129.59.79.182','~/.ssh/id_rsa_jetsontk1'),
         dep_node('ubuntu','129.59.79.66','~/.ssh/id_rsa_jetsontk1')]

def jetson_testfunc():
    for host in hosts:
        env.key_filename = host.keyFile
        env.host_string = "{}@{}".format(host.userName,host.ipAddress)
        run('uname -a')

jetson_testfunc()
