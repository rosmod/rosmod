# ROSMOD Commandline Interface
# Author: Pranav Srinivas Kumar
# Date: 2015.05.29

import os, subprocess, project

class ROSMOD_Commandline: 

    def __init__(self):
        self.project = None
        os.system('clear')
        self.path = str(os.getcwd())
        self.deployed = False
        print "__________ ________    _________   _____   ________  ________    "
        print "\______   \\_____  \  /   _____/  /     \  \_____  \ \______ \   "
        print " |       _/ /   |   \ \_____  \  /  \ /  \  /   |   \ |    |  \  "
        print " |    |   \/    |    \/        \/    Y    \/    |    \|    `   \ "
        print " |____|_  /\_______  /_______  /\____|__  /\_______  /_______  / "
        print "        \/         \/        \/         \/         \/        \/  "

    def run(self):
        print "                                                                 "
        print " Welcome to the ROSMOD Commandline Interface!"
        print " [1] Create a new ROSMOD Project"
        print " [2] Open an existing ROSMOD Project"
        print " [0] Quit ROSMOD"
        print "                                                                 "
        choice = raw_input("Please choose an option: ")
        if choice == '0':
            return
        elif choice == '1':
            self.new_project()
        elif choice == '2':
            self.open_project()

    def new_project(self):
        project_name = raw_input("Enter an name for the new ROSMOD Project: ")
        project_path = raw_input("Enter the path to the new ROSMOD Project [Default: ' + str(os.getcwd())  + ']: ") or str(os.getcwd())
        software_name = raw_input("Enter a name for the Software Model: ")
        hardware_name = raw_input("Enter a name for the Hardware Model: ")
        deployment_name = raw_input("Enter a name for the Deployment Model: ")
        self.project = project.ROSMOD_Project()
        self.project.new(project_name, 
                         project_path,
                         software_name,
                         hardware_name,
                         deployment_name)
        self.use_project()

    def open_project(self):
        project_path = raw_input('Enter the path to an existing ROSMOD Project [Default: ' + str(os.getcwd())  + ']: ') or str(os.getcwd())
        self.project = project.ROSMOD_Project()
        return_value = self.project.open(project_path)
        if return_value != -1:
            self.use_project()
        else:
            return

    def use_project(self):
        os.chdir(self.path)
        print "Project name: " + self.project.project_name
        print "Project home: " + self.project.project_path
        print "                                                                 "
        print " [1] Generate ROS Workspace"
        print " [2] Generate Deployment-specific XML files"
        print " [3] Build ROS Workspace" 
        print " [4] Clean & Rebuild ROS Workspace"
#        print " [5] Copy deployment units to target devices"
#        print " [6] Run the deployment"
#        print " [7] Print..."
        print " [0] Quit ROSMOD"
        print "                                                                 "
        choice = raw_input("Please choose an option: ")
        if choice == '0':
            return
        elif choice == '1':
            self.project.generate_workspace()
            self.use_project()
        elif choice == '2':
            self.project.generate_xml()
            self.use_project()
        elif choice == '3':
            self.build_workspace()
            self.use_project()
        elif choice == '4':
            self.rebuild_workspace()
            self.use_project()
        elif choice == '5':
            pass
        elif choice == '6':
            pass
        elif choice == '7':
            pass

    def build_workspace(self):
        rosmod_path = str(os.getcwd())
        if self.project.workspace_dir == "":
            build_path = self.project.project_path\
                         + '/01-Software/'\
                         + self.project.getChildrenByKind('rml')[0].properties['name']
            if not os.path.exists(build_path):
                print "ROSMOD::ERROR::No Workspace Found! Generate a ROS workspace first"
            else:
                os.chdir(build_path + '/')
                source_space = build_path + '/src'
                build_space = build_path + '/build'
                devel_prefix = '-DCATKIN_DEVEL_PREFIX=' + build_path + '/devel'
                install_prefix = '-DCMAKE_INSTALL_PREFIX=' + build_path + '/install'
                p = subprocess.Popen(['catkin_make', 
                                      '--directory',
                                      build_path,
                                      '--source', 
                                      source_space, 
                                      '--build',
                                      build_space,
                                      devel_prefix, 
                                      install_prefix])
                p.wait()
        else:
            if not os.path.exists(self.project.workspace_dir):
                print "ROSMOD::ERROR::Unexpected error! Please regenerate ROS workspace"
            else:
                os.chdir(self.project.workspace_dir)
                source_space = self.project.workspace_dir + '/src'
                build_space = build_path + '/build'
                devel_prefix = '-DCATKIN_DEVEL_PREFIX=' + build_path + '/devel'
                install_prefix = '-DCMAKE_INSTALL_PREFIX=' + build_path + '/install'
                p = subprocess.Popen(['catkin_make', 
                                      '--directory',
                                      build_path,
                                      '--source',
                                      source_space, 
                                      '--build',
                                      build_space,
                                      devel_prefix,
                                      install_prefix])
                p.wait()

    def rebuild_workspace(self):
        rosmod_path = str(os.getcwd())
        build_path = self.project.project_path\
                     + '/01-Software/'\
                     + self.project.getChildrenByKind('rml')[0].properties['name']
        if not os.path.exists(build_path):
            print "ROSMOD::ERROR::No Workspace Found! Generate a ROS workspace first"
        else:
            os.chdir(build_path)
            p = subprocess.Popen(['rm', '-rf', 'build'])
            p.wait()
            p = subprocess.Popen(['rm', '-rf', 'devel'])
            p.wait()
            self.build_workspace()


                                                     


