# ROSMOD Project Class
# Author: Pranav Srinivas Kumar
# Date: 2015.04.22

import sys, os

# Find Drawable_Object
exeName = sys.argv[0]
dirName = os.path.abspath(exeName)
head,tail = os.path.split(dirName)
head,tail = os.path.split(head)
from drawable import Drawable_Object

from builder import *
from loader import *
from generator import *

def ros_tools_log(q,logStr,amount=1,total=None):
    if q:
        if total:
            q.put([logStr,amount,total])
        else:
            q.put([logStr,amount])            
    else:
        print logStr

# ROSMOD Project class
class ROSMOD_Project(Drawable_Object):
    # Initialize ROS Project
    def __init__(self, **kwargs):
        Drawable_Object.__init__(self)
        
        self.kind = "Project"

        # Name of the ROS Project
        self.project_name = kwargs.pop("name", "")
        # Path to ROS Project
        self.project_path = kwargs.pop("path", "")

        # Workspace Path
        self.workspace_path = os.path.join(self.project_path, "01-Software")
        self.workspace_dir = ""

        # Hardware Configurations Path
        self.hardware_path = os.path.join(self.project_path, "02-Hardware")

        # Deployment Path
        self.deployment_path = os.path.join(self.project_path, "03-Deployment")

        # Binaries path
        self.binaries_path = os.path.join(self.project_path, "04-Binaries")
        self.x86_path = os.path.join(self.binaries_path, "x86")
        self.arm_path = os.path.join(self.binaries_path, "arm")

    # Create a new ROSMOD Project
    def new(self, 
            project_name = "",
            project_path = "",
            workspace_name = "",
            hardware_name = "",
            deployment_name = "",
            q = None):

        # Check provided project name and path
        if project_name != "":
            self.project_name = project_name
        else:
            ros_tools_log( q, "ROSMOD::ERROR::Invalid Project Name")
        if project_path != "":
            self.project_path = project_path
        else:
            ros_tools_log(q, "ROSMOD::ERROR::Invalid Project Path")
        self.project_path = os.path.join(self.project_path, self.project_name)

        self.children = []

        # Notify user
        ros_tools_log(q, "ROSMOD::Creating Project " + project_name + " at " + project_path)

        # Create the .rosmod file
        if not os.path.exists(self.project_path):
            os.makedirs(self.project_path)
        project = project_name + ".rosmod"
        with open(os.path.join(self.project_path, project), 'w') as temp_file:
            temp_file.write("This is a ROSMOD Project")
            temp_file.close() 

        # Establish the paths to directory structure for new project
        self.workspace_path = os.path.join(self.project_path, "01-Software")
        self.hardware_path = os.path.join(self.project_path, "02-Hardware")
        self.deployment_path = os.path.join(self.project_path, "03-Deployment")
        self.binaries_path = os.path.join(self.project_path, "04-Binaries")
        self.x86_path = os.path.join(self.binaries_path, "x86")
        self.arm_path = os.path.join(self.binaries_path, "arm")

        # Create the directory structure & relevant objects
        if not os.path.exists(os.path.join(self.project_path, "01-Software")):
            os.makedirs(self.workspace_path)

        # Create sub directories in Software
        software_dirs = ["msg", "srv", "abl", "pnp"]
        for sub_dir in software_dirs:
            if not os.path.exists(os.path.join(self.workspace_path, sub_dir)):
                os.makedirs(os.path.join(self.workspace_path, sub_dir))
                
        # Create a Workspace Builder Object - and therefore a workspace object
        self.workspace_builder = ROSMOD_Software_Builder(self)
        
        # Initialize Workspace name and create a template .rml file
        if workspace_name != "":
            self.workspace_builder.rml.properties["name"] = workspace_name
            with open(os.path.join(self.workspace_path, workspace_name + ".rml"), 'w') as temp_file:
                temp_file.write("/*\n * ROSMOD Software Model\n */\n\npackage NewPackage\n{\n\n}")
        else:
            self.workspace_builder.rml.properties["name"] = "New_Workspace"
            with open(os.path.join(self.workspace_path, "New_Workspace.rml"), 'w') as temp_file:
                temp_file.write("/*\n * ROSMOD Software Model\n */\n\npackage New_Package\n{\n\n}")

        # Create a new package in workspace and assign package as child of workspace
        new_package = type("ROS_Package", 
                           ( object, Drawable_Object, ), 
                           { '__init__' : Drawable_Object.__init__ })()
        new_package.kind = "Package"
        for prop in model_dict["Package"].properties:
            new_package.properties[prop] = ""
        new_package.properties["name"] = "New_Package"
        new_package.parent = self.workspace_builder.rml
        self.workspace_builder.rml.add(new_package)

        self.children.append(self.workspace_builder.rml)
        
        if not os.path.exists(os.path.join(self.project_path, "02-Hardware")):
            os.makedirs(self.hardware_path)

        # Create sub directries in Hardware
        hardware_dirs = ["snp"]
        for sub_dir in hardware_dirs:
            if not os.path.exists(os.path.join(self.hardware_path, sub_dir)):
                os.makedirs(os.path.join(self.hardware_path, sub_dir))

        # Create a hardware builder object - ready to ready new hardware models
        self.hardware_builder = ROSMOD_Hardware_Builder(self)

        # Create a new template .rhw file
        if hardware_name != "":
            self.hardware_builder.rhw.properties["name"] = hardware_name
            with open(os.path.join(self.hardware_path, hardware_name + ".rhw"), 'w') as temp_file:
                temp_file.write("/*\n * ROSMOD Hardware Model\n */") 
        else:
            self.hardware_builder.rhw.properties["name"] = "New_Hardware"
            with open(os.path.join(self.hardware_path, "New_Hardware" + ".rhw"), 'w') as temp_file:
                temp_file.write("/*\n * ROSMOD Hardware Model\n */")         


        self.add(self.hardware_builder.rhw)

        if not os.path.exists(os.path.join(self.project_path, "03-Deployment")):
            os.makedirs(self.deployment_path)

        # Create a deployment builder object - ready to ready new deployment models
        self.deployment_builder = ROSMOD_Deployment_Builder(self)

        # Create a new template .rdp file
        if deployment_name != "":
            self.deployment_builder.rdp.properties["name"] = deployment_name
            with open(os.path.join(self.deployment_path, deployment_name + ".rdp"), 'w') as temp_file:
                if hardware_name == "":
                    temp_file.write("/*\n * ROSMOD Deployment Model\n */\n\n// ROSMOD Hardware Model - Hardware\nusing New_Hardware;") 
                else:
                    temp_file.write("/*\n * ROSMOD Deployment Model\n */\n\n// ROSMOD Hardware Model - " + hardware_name + "\nusing " + hardware_name + ";") 
        else:
            self.deployment_builder.rdp.properties["name"] = "New_Deployment"
            with open(os.path.join(self.deployment_path, "New_Deployment" + ".rdp"), 'w') as temp_file:
                if hardware_name == "":
                    temp_file.write("/*\n * ROSMOD Deployment Model\n */\n\n// ROSMOD Hardware Model - Hardware\nusing New_Hardware;") 
                else:
                    temp_file.write("/*\n * ROSMOD Deployment Model\n */\n\n// ROSMOD Hardware Model - " + hardware_name + "\nusing " + hardware_name + ";") 

        self.deployment_builder.rdp.properties["rhw_reference"]=self.hardware_builder.rhw
        self.add(self.deployment_builder.rdp)

        if not os.path.exists(os.path.join(self.project_path, "04-Binaries")):
            os.makedirs(self.binaries_path)
        if not os.path.exists(os.path.join(self.binaries_path, "x86")):
            os.makedirs(self.x86_path) 
        if not os.path.exists(os.path.join(self.binaries_path, "arm")):
            os.makedirs(self.arm_path)

    # Open an existing ROSMOD Project
    def open(self, project_path, progressQ = None):
        self.children = []
        valid_project = False
        project_directories = []
           
        for prj_file in os.listdir(project_path):
            if prj_file.endswith(".rosmod"):
                logStr = "ROSMOD::Opening ROSMOD Project: {}".format(project_path)
                ros_tools_log(progressQ,logStr)
                valid_project = True
        if valid_project == False:
            print "ROSMOD::ERROR::No valid ROSMOD Project Found!"
            return -1
        if valid_project == True:
            logStr = "ROSMOD::Project Name: {}".format(os.path.basename(project_path))
            ros_tools_log(progressQ, logStr)
            logStr = "ROSMOD::Project Path: {}".format(project_path)
            ros_tools_log(progressQ, logStr)
            sub_directories = [x[0] for x in os.walk(project_path)]
            for sd in reversed(sub_directories):
                project_directories.append(sd)
            del project_directories[-1]
            
            # Find subdirectories
            sd_count = 0
            for pd in project_directories:
                if "01-Software" in pd:
                    sd_count += 1
                elif "02-Hardware" in pd:
                    sd_count += 1
                elif "03-Deployment" in pd:
                    sd_count += 1
            # If all is well, reinitialize the project with the right name & path
            if sd_count >= 3:
                self.__init__(name=os.path.basename(project_path), path=project_path)
                self.parse_models(progressQ)
            else:
                ros_tools_log(progressQ, "ROSMOD::ERROR::Invalid Project!")

        if not os.path.exists(os.path.join(self.project_path, "04-Binaries")):
            os.makedirs(self.binaries_path)
        if not os.path.exists(os.path.join(self.binaries_path, "x86")):
            os.makedirs(self.x86_path) 
        if not os.path.exists(os.path.join(self.binaries_path, "arm")):
            os.makedirs(self.arm_path)
                
    # Go through dirname and load in all .msg files into message objects
    # load them into the ref dict according to their filename pkgName.msgName.msg
    def parse_msg(self, dirname):
        if not os.path.exists(dirname):
            return
        #print "ROSMOD::Parsing Message Files!"
        for filename in os.listdir(dirname):
            if filename.endswith('.msg'):
                package_name = filename.split('.')[0]
                msg_name = filename.split('.')[1]
                msg_file = open(os.path.join(dirname, filename), 'r')
                msg = type("ROS_Message",
                           ( object, Drawable_Object, ), 
                           { '__init__' : Drawable_Object.__init__ })()
                msg.properties["name"] = msg_name
                msg.properties["definition"]= msg_file.read()
                msg.kind = "Message"
                for package in self.workspace_builder.rml.children:
                    if package.properties["name"] == package_name:
                        msg.parent = package
                        package.add(msg)
                        reference_dict[package.properties["name"] + "/" + msg.properties["name"]] = msg
    
    # Go through dirname and load in all .srv files into service objects
    # load them into the ref dict according to their filename pkgName.srvName.srv
    def parse_srv(self, dirname):
        if not os.path.exists(dirname):
            return
        #print "ROSMOD::Parsing Service Files!"
        for filename in os.listdir(dirname):
            if filename.endswith('.srv'):
                package_name = filename.split('.')[0]
                srv_name = filename.split('.')[1]
                srv_file = open(os.path.join(dirname, filename), 'r')
                srv = type("ROS_Service",
                           ( object, Drawable_Object, ), 
                           { '__init__' : Drawable_Object.__init__ })()
                srv.properties["name"] = srv_name
                srv.properties["definition"]= srv_file.read()
                srv.kind = "Service"
                for package in self.workspace_builder.rml.children:
                    if package.properties["name"] == package_name:
                        srv.parent = package
                        package.add(srv)
                        reference_dict[package.properties["name"] + "/" + srv.properties["name"]] = srv

    # Go through dirname and load in all .abl files into business_logic properties for their ports
    # load them into the ref dict according to their filename pkgName.compName.portName.abl
    def parse_abl(self, dirname):
        if not os.path.exists(dirname):
            return
        #print "ROSMOD::Parsing Abstract Business Logic Model Files!"
        for filename in os.listdir(dirname):
            if len(filename.split('.')) == 4:
                package_name = filename.split('.')[0]
                comp_name = filename.split('.')[1]
                port_name = filename.split('.')[2]
                abl_file = open(os.path.join(dirname, filename), 'r')
                for package in self.workspace_builder.rml.children:
                    if package.properties["name"] == package_name:
                        for comp in package.getChildrenByKind("Component"):
                            if comp.properties["name"] == comp_name:
                                for port in comp.children:
                                    if port.properties["name"] == port_name:
                                        if "abstract_business_logic" not in port.properties.keys():
                                            port.properties["abstract_business_logic"] = ""
                                        port.properties["abstract_business_logic"] = abl_file.read()

    # Go through dirname and load in all .pnp files into network profile properties for their ports
    # load them into the ref dict according to their filename pkgName.compName.portName.pnp
    def parse_pnp(self, dirname):
        if not os.path.exists(dirname):
            return
        #print "ROSMOD::Parsing Port Network Profiles!"
        for filename in os.listdir(dirname):
            if len(filename.split('.')) == 4:
                package_name = filename.split('.')[0]
                comp_name = filename.split('.')[1]
                port_name = filename.split('.')[2]
                pnp_file = open(os.path.join(dirname, filename), 'r')
                for package in self.workspace_builder.rml.children:
                    if package.properties["name"] == package_name:
                        for comp in package.getChildrenByKind("Component"):
                            if comp.properties["name"] == comp_name:
                                for port in comp.children:
                                    if port.properties["name"] == port_name:
                                        if "port_network_profile" not in port.properties.keys():
                                            port.properties["port_network_profile"] = ""
                                        port.properties["port_network_profile"] = pnp_file.read()

    # Go through dirname and load in all .snp files into system profile properties in hardware
    def parse_snp(self, dirname):
        if not os.path.exists(dirname):
            return
        #print "ROSMOD::Parsing System Network Profiles!"
        for filename in os.listdir(dirname):
            if len(filename.split('.')) == 2:
                rhw_name = filename.split('.')[0]
                snp_file = open(os.path.join(dirname, filename), 'r')
                for rhw in self.getChildrenByKind("rhw"):
                    if rhw.properties['name'] == rhw_name:
                        rhw.properties['system_network_profile'] = snp_file.read()
            elif len(filename.split('.')) == 3:
                rhw_name = filename.split('.')[0]
                hardware_name = filename.split('.')[1]
                snp_file = open(os.path.join(dirname, filename), 'r')
                for rhw in self.getChildrenByKind("rhw"):
                    if rhw.properties["name"] == rhw_name:
                        for hardware in rhw.children:
                            if hardware.properties["name"] == hardware_name:
                                hardware.properties["system_network_profile"] = snp_file.read()

    # Parse .rml software model
    def parse_rml(self, filename):

        #print "ROSMOD::Parsing File:", filename
        # Read the input model
        model = FileStream(filename)
        # Instantiate the ROSLexer
        lexer = ROSMOD_SoftwareLexer(model)
        # Generate Tokens
        stream = CommonTokenStream(lexer)
        # Instantiate the ROSParser
        parser = ROSMOD_SoftwareParser(stream)
        # Parse from starting point of grammar
        tree = parser.start()
        # Instantiate a Parse Tree Walker
        walker = ParseTreeWalker()
        
        self.workspace_builder = ROSMOD_Software_Builder(self)
        self.workspace_builder.rml.properties["name"] = os.path.basename(filename.split(".")[0])
        #print "ROSMOD::Reading ROS Workspace:", self.workspace_builder.rml.properties["name"]

        # Walk the parse tree
        walker.walk(self.workspace_builder, tree)

        self.children.append(self.workspace_builder.rml)

    # Parse .rhw hardware configurations model
    def parse_rhw(self, filename):

        #print "ROSMOD::Parsing File:", filename
        # Read the hardware configurations model
        model = FileStream(filename)
        # Instantiate the HostsLexer
        lexer = ROSMOD_HardwareLexer(model)
        # Generate Tokens
        stream = CommonTokenStream(lexer)
        # Instantiate the HostsParser
        parser = ROSMOD_HardwareParser(stream)
        # Parse from starting point of grammar
        tree = parser.start()
        # Instantiate a Parse Tree Walker
        walker = ParseTreeWalker()

        # Hardware Builder
        self.hardware_builder = ROSMOD_Hardware_Builder(self)
        self.hardware_builder.rhw.properties["name"] = os.path.basename(filename.split(".")[0])
        #print "ROSMOD::Reading Hardware Model:", self.hardware_builder.rhw.properties["name"]

        # Walk the parse tree
        walker.walk(self.hardware_builder, tree)

        self.add(self.hardware_builder.rhw)
        reference_dict[self.children[-1].properties["name"]] = self.children[-1]

    # Parse .rdp software deployment model
    def parse_rdp(self, filename):
        #print "ROSMOD::Parsing File:", filename
        # Read the hardware configurations model
        model = FileStream(filename)
        # Instantiate the DeploymentLexer
        lexer = ROSMOD_DeploymentLexer(model)
        # Generate Tokens
        stream = CommonTokenStream(lexer)
        # Instantiate the DeploymentParser
        parser = ROSMOD_DeploymentParser(stream)
        # Parse from starting point of grammar
        tree = parser.start()
        # Instantiate a Parse Tree Walker
        walker = ParseTreeWalker()    

        # Deployment Builder
        self.deployment_builder = ROSMOD_Deployment_Builder(self)
        self.deployment_builder.rdp.properties["name"] = os.path.basename(filename.split(".")[0])
        #print "ROSMOD::Reading Deployment Model:", self.deployment_builder.rdp.properties["name"]

        # Walk the parse tree
        walker.walk(self.deployment_builder, tree)

        self.add(self.deployment_builder.rdp)

    # Parse all model files in all aspects of Project
    def parse_models(self, progressQ = None):
        count = 0
        for rml in os.listdir(self.workspace_path):
            if rml.endswith(".rml"):
                rml_file = os.path.join(self.workspace_path, rml)
                count += 1
                
        if count == 0:
            ros_tools_log(progressQ,"ROSMOD::No ROSMOD (.rml) file found in " + self.workspace_path)
        elif count > 1:
            ros_tools_log(progressQ,"ROSMOD::ERROR::There can only be one .rml file in 01-ROS-Workspace!")
        else:
            ros_tools_log(progressQ,"ROSMOD::Parsing Software files")
            self.parse_rml(rml_file)
        count = 0

        ros_tools_log(progressQ,"ROSMOD::Parsing Message files")
        self.parse_msg(self.workspace_path + "/msg")

        ros_tools_log(progressQ,"ROSMOD::Parsing Service files")
        self.parse_srv(self.workspace_path + "/srv")

        ros_tools_log(progressQ,"ROSMOD::Parsing Abstract Business Logic files")
        self.parse_abl(self.workspace_path + "/abl")

        ros_tools_log(progressQ,"ROSMOD::Parsing Port Network Profile files")
        self.parse_pnp(self.workspace_path + "/pnp")

        ros_tools_log(progressQ,"ROSMOD::Parsing Hardware files")
        for rhw in os.listdir(self.hardware_path):
            if rhw.endswith(".rhw"):
                rhw_file = os.path.join(self.hardware_path, rhw)
                self.parse_rhw(rhw_file)
                count += 1
        if count == 0:
            ros_tools_log(progressQ,"ROSMOD::No ROS Hardware Configurations (.rhw) files found in "+ self.hardware_path)

        count = 0

        ros_tools_log(progressQ,"ROSMOD::Parsing System Network Profile files")
        self.parse_snp(self.hardware_path + "/snp")

        ros_tools_log(progressQ,"ROSMOD::Parsing Deployment files")
        for rdp in os.listdir(self.deployment_path):
            if rdp.endswith(".rdp"):
                rdp_file = os.path.join(self.deployment_path, rdp)
                self.parse_rdp(rdp_file)
                count += 1
                
        if count == 0:
            ros_tools_log(progressQ,"ROSMOD::No ROS Deployment (.rdp) files found in" + self.deployment_path)

        ros_tools_log(progressQ,"ROSMOD::Resolving References")
        self.resolve_references()

    # Check workspace directory for existing code that may
    # require preservation
    def check_workspace(self):
        # Instantiate a Loader Object
        business_logic = ROSMOD_Loader()
        # Use load to load existing business logic
        business_logic.load(self.workspace_builder.rml, self.workspace_path)

    # Generate the ROS workspace corresponding to the input model
    def generate_workspace(self, comm, trafficGen=False):
        # Check for an existing workspace in workspace_path
        self.check_workspace()
        # Instantiate a Generator Object
        workspace_generator = ROSMOD_Generator()
        # Use listener_object to generate ROS workspace
        self.workspace_dir = workspace_generator.generate_workspace(self.workspace_builder.rml, 
                                                                    self.workspace_path,
                                                                    comm,
                                                                    trafficGen) 

    # Generate Deployment XML files and relevant directories
    def generate_xml(self, num_comps_to_sync, sync_timeout):
        # Instantiate a Generator Object
        deployment_generator = ROSMOD_Generator()
        # Use existing deployment objects to generate necessary xml files
        deployment_generator.generate_xml(self.getChildrenByKind("rdp"),
                                          self.deployment_path,
                                          num_comps_to_sync,
                                          sync_timeout)
        print "ROSMOD::Generated Deployment-Specific XML files at: " + self.deployment_path

    # Generate CPN Timing Analysis Model for each deployment
    def generate_cpn(self):
        # Instantiate a Generator Object
        cpn_generator = ROSMOD_Generator()
        # Use existing deployment objects to generate necessary xml files
        cpn_generator.generate_cpn(self.workspace_builder.rml,
                                   self.getChildrenByKind("rdp"),
                                   self.deployment_path)

# self.getChildrenByKind("rdp"), 
# self.deployment_path)

    # Resolving References in all workspace packages
    def resolve_references(self, progressQ = None):
        #print "ROSMOD::Fixing unresolved references"
        for package in self.workspace_builder.rml.children:
            for package_child in package.children:
                if package_child.kind == "Component":
                    for port in package_child.children:
                        if port.kind == "Client" or port.kind == "Server":
                            port.properties["service_reference"] = reference_dict[port.properties["reference"]]
                        elif port.kind == "Publisher" or port.kind == "Subscriber":
                            port.properties["message_reference"] = reference_dict[port.properties["reference"]]

        for rdp in self.getChildrenByKind("rdp"):
            rdp.properties["rhw_reference"] = reference_dict[rdp.properties["reference"]]
            for node in rdp.getChildrenByKind("Node"):
                node.properties["hardware_reference"] = reference_dict[node.properties["reference"]]
                for comp_instance in node.children:
                    comp_instance.properties["component_reference"] = reference_dict[comp_instance.properties["reference"]]
                    comp_instance.properties["is_periodic_logging"] = comp_instance.properties["is_periodic_logging"] == "true"
                    for port_instance in comp_instance.children:
                        port_instance.properties["port_reference"] = reference_dict[port_instance.properties["reference"]]

    # Generate a ROS model from a workspace object
    # Used to save an edited model
    def save_rml(self, path=""):
        if path == "":
            path = self.workspace_path
        rml_namespace = {'workspace': self.workspace_builder.rml}
        t = rml(searchList=[rml_namespace])
        self.rml = str(t)
        with open(os.path.join(path, self.workspace_builder.rml.properties["name"] + ".rml"), 'w') as temp_file:
            temp_file.write(self.rml)
        print "ROSMOD::Saving " + self.workspace_builder.rml.properties["name"] + ".rml " + "at " + path
        
        # Save all messages and services in respective folders
        for package in self.workspace_builder.rml.children:
            for msg in package.getChildrenByKind("Message"):
                self.save_msg(msg)
            for srv in package.getChildrenByKind("Service"):
                self.save_srv(srv)
            for comp in package.getChildrenByKind("Component"):
                for port in comp.children:
                    if "abstract_business_logic" in port.properties.keys():
                        self.save_abl(port)
                    if "port_network_profile" in port.properties.keys():
                        self.save_pnp(port)

    # Generate a ROS Hardware Configurations model (.rhw file) from a ROS_HW Object
    def save_rhw(self, path=""):
        if path == "":
            path = self.hardware_path
        for rhw in self.getChildrenByKind("rhw"):
            rhw_namespace = {'rhw' : rhw}
            t = rhw_template.rhw(searchList=[rhw_namespace])
            self.rhw = str(t)
            with open(os.path.join(path, rhw.properties["name"] + ".rhw"), 'w') as temp_file:
                temp_file.write(self.rhw)
                temp_file.close()
            print "ROSMOD::Saving " + rhw.properties["name"] + ".rhw " + "at " + path

            if "system_network_profile" in rhw.properties.keys():
                snp_path = self.hardware_path + "/snp"
                if not os.path.exists(snp_path):
                    os.makedirs(snp_path)
                filename = rhw.properties["name"] + ".snp"
                if rhw.properties["system_network_profile"] != "":
                    with open(os.path.join(snp_path, filename), 'w') as temp_file:
                        temp_file.write(rhw.properties["system_network_profile"])
            
            for hardware in rhw.children:
                self.save_snp(hardware)

    # Generate a ROS Deployment model (.rdp file) from a ROS_Deployment Object
    def save_rdp(self, path=""):
        if path == "":
            path = self.deployment_path
        for rdp in self.getChildrenByKind("rdp"):
            rdp_namespace = {'deployment' : rdp}
            t = rdp_template.rdp(searchList=[rdp_namespace])
            self.rdp = str(t)
            with open(os.path.join(path, rdp.properties["name"] + ".rdp"), 'w') as temp_file:
                temp_file.write(self.rdp)
                temp_file.close()
            print "ROSMOD::Saving " + rdp.properties["name"] + ".rdp " + "at " + path  

    # Save Message files
    def save_msg(self, msg_object):
        path = self.workspace_path + "/msg"
        if not os.path.exists(path):
            os.makedirs(path)
        msg_namespace = {'definition': msg_object.properties["definition"]}
        t = msg_template.msg(searchList=[msg_namespace])
        msg = str(t)
        with open(os.path.join(path, msg_object.parent.properties["name"] + "." + msg_object.properties["name"] + ".msg"), 'w') as temp_file:
            temp_file.write(msg)
        print "ROSMOD::Saving " + msg_object.properties["name"] + ".msg " + "at " + path
        
    # Save Service files
    def save_srv(self, srv_object):
        path = self.workspace_path + "/srv"
        if not os.path.exists(path):
            os.makedirs(path)
        srv_namespace = {'definition': srv_object.properties["definition"]}
        t = srv_template.srv(searchList=[srv_namespace])
        srv = str(t)
        with open(os.path.join(path, srv_object.parent.properties["name"] + "." + srv_object.properties["name"] + ".srv"), 'w') as temp_file:
            temp_file.write(srv)
        print "ROSMOD::Saving " + srv_object.properties["name"] + ".srv " + "at " + path

    # Save Abstract Business Logic files
    def save_abl(self, port_object):
        if "abstract_business_logic" in port_object.properties.keys():
            path = self.workspace_path + "/abl"
            if not os.path.exists(path):
                os.makedirs(path)
            filename = port_object.parent.parent.properties["name"] + "." + port_object.parent.properties["name"] + "." + port_object.properties["name"] + ".abl"
            if port_object.properties["abstract_business_logic"] != "":
                with open(os.path.join(path, filename), 'w') as temp_file:
                    temp_file.write(port_object.properties["abstract_business_logic"])
        
    # Save Port Network Profiles
    def save_pnp(self, port_object):
        if "port_network_profile" in port_object.properties.keys():
            path = self.workspace_path + "/pnp"
            if not os.path.exists(path):
                os.makedirs(path)
            filename = port_object.parent.parent.properties["name"] + "." + port_object.parent.properties["name"] + "." + port_object.properties["name"] + ".pnp"
            if port_object.properties["port_network_profile"] != "":
                with open(os.path.join(path, filename), 'w') as temp_file:
                    temp_file.write(port_object.properties["port_network_profile"])

    # Save System Network Profiles
    def save_snp(self, hardware):
        if "system_network_profile" in hardware.properties.keys():
            path = self.hardware_path + "/snp"
            if not os.path.exists(path):
                os.makedirs(path)
            filename = hardware.parent.properties["name"] + "." + hardware.properties["name"] + "." + "snp"
            if hardware.properties["system_network_profile"] != "":
                with open(os.path.join(path, filename), 'w') as temp_file:
                    temp_file.write(hardware.properties["system_network_profile"])

    # Save the entire project
    def save(self, project_name = "", project_path = ""):
        if project_path == "":
            project_path = self.project_path
            # Save the ROS Workpace Model
            self.save_rml()
            # Save the ROS Hardware Configurations Model
            self.save_rhw()
            # Save the ROS Deployment Model
            self.save_rdp()
        else:
            new_project = ROSMOD_Project()
            new_project.new(project_name, project_path)
            if os.path.isfile(os.path.join(new_project.workspace_path, "New_Workspace.rml")):
                os.remove(os.path.join(new_project.workspace_path, "New_Workspace.rml"))
            if os.path.isfile(os.path.join(new_project.hardware_path, "New_Hardware.rhw")):
                os.remove(os.path.join(new_project.hardware_path, "New_Hardware.rhw"))
            if os.path.isfile(os.path.join(new_project.deployment_path, "New_Deployment.rdp")):
                os.remove(os.path.join(new_project.deployment_path, "New_Deployment.rdp"))
            # Save the ROS Workpace Model
            self.save_rml(new_project.workspace_path)
            # Save the ROS Hardware Configurations Model
            self.save_rhw(new_project.hardware_path)
            # Save the ROS Deployment Model
            self.save_rdp(new_project.deployment_path)


