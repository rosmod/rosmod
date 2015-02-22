#!/usr/bin/python

from fabric.api import env, run, sudo

env.use_ssh_config = False
env.key_filename = "~/.ssh/id_rsa_jetsontk1"
env.hosts = ['ubuntu@129.59.79.182','ubuntu@129.59.79.66']
def jetson_testfunc():
    for host in env.hosts:
        env.host_string = host
        run('uname -a')

jetson_testfunc()
