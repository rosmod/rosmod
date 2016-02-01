#!/usr/bin/python
# Plot Updater
# Author: Pranav Srinivas Kumar
# Date: 2015.07.20

import os
import sys
import subprocess

pwd = os.path.dirname(os.path.realpath(__file__))
pwd_files = [f for f in os.listdir('.') if os.path.isfile(f)]
plotter = ""
if 'rosmod_log_plotter.py' not in pwd_files:
    print "ERROR::Cannot find rosmod_log_plotter.py in " + pwd
else:
    plotter = os.path.join(pwd, 'rosmod_log_plotter.py')
    repo = os.path.abspath(os.path.join(pwd, os.pardir))
    tests = os.path.join(repo, "tests")
    for root, subdirs, files in os.walk(tests):
        for log in files:
            if "ROSMOD_DEBUG." in log:
                log_path = str(os.path.join(root, log))
                os.chdir(root)
                p = subprocess.Popen(['python',
                                      plotter,
                                      '--log',
                                      log_path,
                                      '--save',
                                      root])
                p.wait()
                p = subprocess.Popen(['python',
                                      plotter,
                                      '--save',
                                      root])
                p.wait()
    os.chdir(pwd)
                
