# ROSMOD Commandline Interface
# Author: Pranav Srinivas Kumar
# Date: 2015.05.29

import os, subprocess, project
from worker import *
from deployment import *

class ROSMOD_Commandline: 

    def __init__(self):
        self.project = None
        os.system('clear')
        self.path = str(os.getcwd())
        self.deployed = False
        self.build_path = ""
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
        else:
            print "                                                    "
            print "ROSMOD::ERROR::Invalid Option! "
            self.run()

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
        print "                                                                 "
        print "Project name: " + self.project.project_name
        print "Project home: " + self.project.project_path
        print "                                                                 "
        print " [1] Generate ROS Workspace"
        print " [2] Generate Deployment-specific XML files"
        print " [3] Build ROS Workspace" 
        print " [4] Clean & Rebuild ROS Workspace"
        print " [5] Copy Deployment Artifacts to target devices"
        print " [6] Run a Deployment"
        print " [7] Stop a Deployment"
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
            self.copy_deployment_files()
            self.use_project()
        elif choice == '6':
            self.start_deployment()
            self.use_project()
        elif choice == '7':
            self.stop_deployment()
            self.use_project()
        else:
            print "                                                    "
            print "ROSMOD::ERROR::Invalid Option! "
            self.use_project()

    def build_workspace(self):
        rosmod_path = str(os.getcwd())
        if self.project.workspace_dir == "":
            self.build_path = self.project.project_path\
                              + '/01-Software/'\
                              + self.project.getChildrenByKind('rml')[0].properties['name']
            if not os.path.exists(self.build_path):
                print "ROSMOD::ERROR::No Workspace Found! Generate a ROS workspace first"
            else:
                os.chdir(self.build_path + '/')
                source_space = self.build_path + '/src'
                build_space = self.build_path + '/build'
                devel_prefix = '-DCATKIN_DEVEL_PREFIX=' + self.build_path + '/devel'
                install_prefix = '-DCMAKE_INSTALL_PREFIX=' + self.build_path + '/install'
                p = subprocess.Popen(['catkin_make', 
                                      '--directory',
                                      self.build_path,
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
                build_space = self.project.workspace_dir + '/build'
                devel_prefix = '-DCATKIN_DEVEL_PREFIX=' + self.project.workspace_dir + '/devel'
                install_prefix = '-DCMAKE_INSTALL_PREFIX='\
                                 + self.project.workspace_dir + '/install'
                p = subprocess.Popen(['catkin_make', 
                                      '--directory',
                                      self.project.workspace_dir,
                                      '--source',
                                      source_space, 
                                      '--build',
                                      build_space,
                                      devel_prefix,
                                      install_prefix])
                p.wait()

    def rebuild_workspace(self):        
        rosmod_path = str(os.getcwd())
        self.build_path = self.project.project_path\
                          + '/01-Software/'\
                          + self.project.getChildrenByKind('rml')[0].properties['name']
        if not os.path.exists(self.build_path):
            print "ROSMOD::ERROR::No Workspace Found! Generate a ROS workspace first"
        else:
            os.chdir(self.build_path)
            p = subprocess.Popen(['rm', '-rf', 'build'])
            p.wait()
            p = subprocess.Popen(['rm', '-rf', 'devel'])
            p.wait()
            self.build_workspace()

    def copy_deployment_files(self):
        deployments = self.project.getChildrenByKind('rdp')
        if deployments != []:
            count = 1
            print "                                                             "
            print "Available Deployments:"
            for dpl in deployments:
                print " [" + str(count) + "] " + dpl.properties['name']
            print "                                                             "
            choice = raw_input("Please choose a deployment: ")
            deployment_object = deployments[count-1]
            self.BuildHostDict(deployment_object)
            workerThread = WorkerThread(
                func = lambda : deployment.copyTest(
                    self,
                    self.project.project_path\
                    + "/01-Software/"\
                    + self.project.getChildrenByKind("rml")[0].properties['name']\
                    + "/devel/lib/",
                    self.project.project_path\
                    + "/03-Deployment/"+\
                    deployment_object.properties['name'], None)
            )
            workerThread.start()
            workerThread.join()
        else:
            print "ROSMOD::ERROR::No Deployments Found"

    def BuildHostDict(self, dep, rosCoreIP=""):
        #env.warn_only = False
        env.use_ssh_config = False
        self.hostDict = {}
        env.hosts = []
        hostToNodeListMap = {}
        numNodes = 0
        for node in dep.getChildrenByKind("Node"):
            host = node.properties['hardware_reference']
            numNodes += 1
            deploymentPath = node.properties['deployment_path']
            if deploymentPath == "":
                deploymentPath = host.properties['deployment_path']
            cmdArgs = node.properties['cmd_args']
            cmdArgs += " -config {} -nodename {} -hostname {}".format(
                deploymentPath + "/" + node.properties['name'] + ".xml",
                node.properties['name'],
                host.properties['name']
            )
            libs = []
            for child in node.children:
                libs.append("lib" + child.properties['component_reference'].properties['name'] + ".so")
            newNode = deployment.deployed_node(
                name = node.properties['name'],
                executable = deploymentPath + '/node_main',
                libs = libs,
                config = node.properties['name'] + ".xml",
                deploymentDir = deploymentPath,
                userName = host.properties['username'],
                keyFile = host.properties['sshkey'],
                cmdArgs = cmdArgs
            )
            if host in hostToNodeListMap.keys():
                hostToNodeListMap[host].append( newNode )
            else:
                hostToNodeListMap[host] = [ newNode ]
        for host,nodeList in hostToNodeListMap.iteritems():
            self.hostDict[host.properties['name']] = deployment.deployed_host(
                name = host.properties['name'],
                userName = host.properties['username'],
                deploymentDir = host.properties['deployment_path'],
                ipAddress = host.properties['ip_address'],
                keyFile = host.properties['sshkey'],
                nodes = nodeList,
                envVars = []
            )
            self.hostDict[host.properties['name']].envVars.append(
                ["LD_LIBRARY_PATH","{}:$LD_LIBRARY_PATH".format(host.properties['deployment_path'])]
            )
            if rosCoreIP != "":
                self.hostDict[host.properties['name']].envVars.append(
                    ['ROS_MASTER_URI','http://{}:11311/'.format(rosCoreIP)])
                self.hostDict[host.properties['name']].envVars.append(
                    ['ROS_IP',host.properties['ip_address']]
                )
            env.hosts.append(host.properties['name'])
        return numNodes

    def start_deployment(self):
        deployments = self.project.getChildrenByKind('rdp')
        if deployments != []:
            count = 1
            print "                                                             "
            print "Available Deployments:"
            for dpl in deployments:
                print " [" + str(count) + "] " + dpl.properties['name']
            print "                                                             "
            choice = raw_input("Please choose a deployment: ")
            deployment_object = deployments[count-1]
            rosCoreIP = ""
            testName = "NewTest"
            print "                                                             "
            print "Preparing Deployment: " + choice
            properties = OrderedDict()
            properties['name'] = raw_input('Provide a name for the test [Default: test]: ')\
                                 or "test"
            self.workTimerPeriod = 2.0
            properties['period'] = raw_input('Provide a monitoring period [Default: '\
                                             + str(self.workTimerPeriod) + ']: ')\
                                   or str(self.workTimerPeriod)
            print "                                                             "
            print "Available Hardware Devices: "
            hw_count = 0
            hws = deployment_object.properties['rhw_reference'].children
            for hw in hws:
                print " [" + str(hw_count+1) + "] " + hw.properties['name']                
            print "                                                             "
            roscore_hardware = int(raw_input('Choose the Hardware Device running roscore: '))
            properties['hardware_reference'] = hws[roscore_hardware-1]
            referenceDict = OrderedDict()
            referenceDict['hardware_reference'] = deployment_object.properties['rhw_reference'].children
            rosCoreIP = properties['hardware_reference'].properties['ip_address']
            testName = properties['name']
            
            self.workTimerPeriod = float(properties['period'])
            #self.workTimer.Start(self.workTimerPeriod*1000)

            numNodes = self.BuildHostDict(deployment_object, 
                                          rosCoreIP)

            workerThread = WorkerThread(
                func = lambda : deployment.deployTest(
                    self,
                    None
                )
            )
            self.updatedHostDict = False
            workerThread.start()           
            workerThread.join()                                         


    def stop_deployment(self):
        workerThread = WorkerThread(
            func = lambda : deployment.stopTest(
                self,
                None
            )
        )
        workerThread.start()
        workerThread.join()
