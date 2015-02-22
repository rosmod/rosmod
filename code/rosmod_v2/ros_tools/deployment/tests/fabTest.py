#!/usr/bin/python

from fabric.api import env, run, sudo

env.user = 'ubuntu'
env.use_ssh_config = False
env.key_filename = "~/.ssh/id_rsa_jetsontk1"
def jetson_testfunc():
    env.host_string = "ubuntu@129.59.79.182"
    run('uname -a')

jetson_testfunc()
