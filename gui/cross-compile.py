# Cross-compile Command
# Author: Pranav Srinivas Kumar
# Date: 2015.07.06

import os, subprocess
cc_path = str(os.getcwd())

def build_workspace(project_workspace_dir):
    if not os.path.exists(project_workspace_dir):
        print "ROSMOD::ERROR::Unexpected error! Please regenerate ROS workspace"
    else:
        os.chdir(project_workspace_dir)
        source_space = project_workspace_dir + '/src'
        build_space = project_workspace_dir + '/build'
        devel_prefix = '-DCATKIN_DEVEL_PREFIX=' + project_workspace_dir + '/devel'
        install_prefix = '-DCMAKE_INSTALL_PREFIX='\
                         + project_workspace_dir + '/install'
        p = subprocess.Popen(['catkin_make', 
                              '--directory',
                              project_workspace_dir,
                              '--source',
                              source_space, 
                              '--build',
                              build_space,
                              devel_prefix,
                              install_prefix])
        p.wait()

p = subprocess.Popen(['sh', 
                      'up.sh',
                      'RCPS-Testbed.img'])
p.wait()
p = subprocess.Popen(['sh',
                      'go.sh', 
                      '/home/jeb/Repositories/rosmod/samples/roscpp/timer_example/01-Software/workspace'])
p.wait()
os.chdir(cc_path)
p = subprocess.Popen(['sh', 
                      'dn.sh',
                      'RCPS-Testbed.img'])
p.wait()
