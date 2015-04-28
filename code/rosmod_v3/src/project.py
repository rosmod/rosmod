# ROSMOD Project Class
# Author: Pranav Srinivas Kumar
# Date: 2015.04.22

import sys, os

# Find Drawable_Object
exeName = sys.argv[0]
dirName = os.path.abspath(exeName)
head,tail = os.path.split(dirName)
head,tail = os.path.split(head)
#sys.path.append(head + '/editor_v2/')
from drawable import Drawable_Object

from builder import *
from generator import *

def init(self):
    Drawable_Object.__init__(self)

# ROS Project class
class ROS_Project(Drawable_Object):
    # Initialize ROS Project
    def __init__(self, **kwargs):
        Drawable_Object.__init__(self)

        # Name of the ROS Project
        self.project_name = kwargs.pop("name", "")
        # Path to ROS Project
        self.project_path = kwargs.pop("path", "")

        # Create a ROS Workspace Object
        self.workspace = type("ROS_Workspace", 
                              ( object, Drawable_Object, ), { '__init__' : Drawable_Object.__init__ })()
        self.workspace.kind = "rml"
        # Workspace Path
        self.workspace_path = os.path.join(self.project_path, "01-Software")
        self.workspace_dir = ""
        # ROS Workspace Builder
        self.workspace_builder = ROS_Workspace_Builder(self.workspace, self)

        # Hardware Configurations Path
        self.hardware_path = os.path.join(self.project_path, "02-Hardware")
        # Hardware Configurations - List of all rhw objects in Project
        self.hardware_files = []
        # Hardware Builder
        self.hardware_builder = ROS_Hardware_Builder(self.hardware_files, self)

        # Deployment Path
        self.deployment_path = os.path.join(self.project_path, "03-Deployment")
        # Deployment Configurations - List of all rdp objects in Project
        self.deployment_files = []
        # Deployment Builder
        self.deployment_builder = ROS_Deployment_Builder(self.deployment_files, self)

    # Open an existing ROSMOD Project
    def open(self, project_path):
        valid_project = False
        project_directories = []
        for prj_file in os.listdir(project_path):
            if prj_file.endswith(".rosmod"):
                print "ROSTOOLS::Opening ROSMOD Project:", project_path
                valid_project = True

        if valid_project == True:
            print "ROSTOOLS::Project Name:", os.path.basename(project_path)
            print "ROSTOOLS::Project Path:", project_path
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
                self.parse_models()
            else:
                print "ROSTOOLS::ERROR::Invalid Project!"

    # Parse .rml software model
    def parse_rml(self, filename):

        print "ROSTOOLS::Parsing File:", filename
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
        self.workspace.properties = OrderedDict()
        self.workspace.children = []
        self.workspace.properties["name"] = os.path.basename(filename.split(".")[0])
        print "ROSTOOLS::Reading ROS Workspace:", self.workspace.properties["name"]
        # Walk the parse tree
        walker.walk(self.workspace_builder, tree)
        return self.workspace

    # Parse .rhw hardware configurations model
    def parse_rhw(self, filename):

        print "ROSTOOLS::Parsing File:", filename
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

        # Walk the parse tree
        walker.walk(self.hardware_builder, tree)

        self.hardware_builder.rhw.properties["name"] = os.path.basename(filename.split(".")[0])
        print "ROSTOOLS::Reading Hardware Model:", self.hardware_builder.rhw.properties["name"]
        self.hardware_files.append(self.hardware_builder.rhw)
        #self.hardware_builder = ROS_Hardware_Builder(self.hardware_files, self)

    # Parse .rdp software deployment model
    def parse_rdp(self, filename):
        print "ROSTOOLS::Parsing File:", filename
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

        # Walk the parse tree
        walker.walk(self.deployment_builder, tree)

        self.deployment_builder.rdp.properties["name"] = os.path.basename(filename.split(".")[0])
        print "ROSTOOLS::Reading Deployment Model:", self.deployment_builder.rdp.properties["name"]
        self.deployment_files.append(self.deployment_builder.rdp)
        #return self.deployments

    # Parse all model files in all aspects of Project
    def parse_models(self):
        count = 0
        for rml in os.listdir(self.workspace_path):
            if rml.endswith(".rml"):
                rml_file = os.path.join(self.workspace_path, rml)
                count += 1
                
        if count == 0:
            print "ROSTOOLS::No ROSMOD (.rml) file found in", self.workspace_path
        elif count > 1:
            print "ROSTOOLS::ERROR::There can only be one .rml file in 01-ROS-Workspace!"
        else:
            self.parse_rml(rml_file)
        count = 0

        for rhw in os.listdir(self.hardware_path):
            if rhw.endswith(".rhw"):
                rhw_file = os.path.join(self.hardware_path, rhw)
                self.parse_rhw(rhw_file)
                count += 1

        if count == 0:
            print "ROSTOOLS::No ROS Hardware Configurations (.rhw) files found in", self.hardware_path

        count = 0

        for rdp in os.listdir(self.deployment_path):
            if rdp.endswith(".rdp"):
                rdp_file = os.path.join(self.deployment_path, rdp)
                self.parse_rdp(rdp_file)
                count += 1
                
        if count == 0:
            print "ROSTOOLS::No ROS Deployment (.rdp) files found in", self.deployment_path

        #self.resolve_references()

    # Generate the ROS workspace corresponding to the input model
    def generate_workspace(self):
        # Check for an existing workspace in workspace_path
        # self.check_workspace()
        # Instantiate a Generator Object
        workspace_generator = ROSMOD_Generator()
        # Use listener_object to generate ROS workspace
        print "_________________BEFORE GENERATION"
        print self.workspace.children
        print self.workspace_path
        print self.deployment_files
        print self.deployment_path
        self.workspace_dir = workspace_generator.generate(self.workspace, self.workspace_path, self.deployment_files, self.deployment_path)

'''
    # Resolving Null references in all workspace packages
    def resolve_references(self):
        print "ROSTOOLS::Checking for unresolved references..."
        object_kind = ""
        remove_list = []
        for obj in self.null_references:
            if "publisher" in obj.kind or "subscriber" in obj.kind:
                package = obj.properties["message_text"].split('/')[0]
                reference = obj.properties["message_text"].split('/')[-1]
                # First, find the package
                for child in self.workspace.children:
                    if child.properties["name"] == package:
                        # Get all its messages
                        for msg in child.getChildrenByKind("message"):
                            if msg.properties["name"] == reference:
                                obj.properties["message_reference"] = msg
                                remove_list.append(obj)
            if "client" in obj.kind or "server" in obj.kind:
                package = obj.properties["service_text"].split('/')[0]
                reference = obj.properties["service_text"].split('/')[-1]
                # First, find the package
                for child in self.workspace.children:
                    if child.properties["name"] == package:
                        # Get all its services
                        for srv in child.getChildrenByKind("service"):
                            if srv.properties["name"] == reference:
                                obj.properties["service_reference"] = srv
                                remove_list.append(obj)
            if "component_instance" in obj.kind:
                package = obj.properties["component_text"].split('/')[0]
                reference = obj.properties["component_text"].split('/')[-1]
                # First, find the package
                for child in self.workspace.children:
                    if child.properties["name"] == package:
                        # Get all components
                        for comp in child.getChildrenByKind("component"):
                            if comp.properties["name"] == reference:
                                obj.properties["component_reference"] = comp
                                remove_list.append(obj)

        # Removed not-null references from list
        for obj in remove_list:
            self.null_references.remove(obj)
        # Check for remaining null references
        if len(self.null_references) > 0 and self.checked_references == False:
            print self.null_references
            print "ROSTOOLS::ERROR::Unable to resolve all null references"
            for obj in self.null_references:
                if obj.kind == "publisher" or obj.kind == "subscriber":
                    print "ROSTOOLS::ERROR::Invalid Reference " + obj.properties["message_text"] + " used when defining " +  obj.kind + " " + obj.properties["name"]
                if obj.kind == "client" or obj.kind == "server":
                    print "ROSTOOLS::ERROR::Invalid Reference " + obj.properties["service_text"] + " used when defining " +  obj.kind + " " + obj.properties["name"]
                if obj.kind == "component_instance":
                    print "ROSTOOLS::ERROR::Invalid Reference " + obj.properties["component_text"] + " used when defining " +  obj.kind + " " + obj.properties["name"]
        self.checked_references = True

    # Check workspace directory for existing code that may
    # require preservation
    def check_workspace(self):
        # Instantiate a Loader Object
        business_logic = Workspace_Loader()
        # Use load to load existing business logic
        business_logic.load(self.workspace, self.workspace_path)

    # Generate a ROS model from a workspace object
    # Used to save an edited model
    def save_rml(self, path=""):
        if path == "":
            path = self.workspace_path
        rml_namespace = {'workspace': self.workspace}
        t = rml(searchList=[rml_namespace])
        self.rml = str(t)
        with open(os.path.join(path, self.workspace.properties["name"] + ".rml"), 'w') as temp_file:
            temp_file.write(self.rml)
        print "ROSTOOLS::Saving " + self.workspace.properties["name"] + ".rml " + "at " + path

    # Generate a ROS Hardware Configurations model (.rhw file) from a ROS_HW Object
    def save_rhw(self, path=""):
        if path == "":
            path = self.hardware_configurations_path
        for hw in self.hardware_configurations:
            rhw_namespace = {'hardware' : hw}
            t = rhw(searchList=[rhw_namespace])
            self.rhw = str(t)
            with open(os.path.join(path, hw.properties["name"] + ".rhw"), 'w') as temp_file:
                temp_file.write(self.rhw)
                temp_file.close()
            print "ROSTOOLS::Saving " + hw.properties["name"] + ".rhw " + "at " + path

    # Generate a ROS Deployment model (.rdp file) from a ROS_Deployment Object
    def save_rdp(self, path=""):
        if path == "":
            path = self.deployment_path
        for dp in self.deployments:
            rdp_namespace = {'deployment' : dp}
            t = rdp(searchList=[rdp_namespace])
            self.rdp = str(t)
            with open(os.path.join(path, dp.properties["name"] + ".rdp"), 'w') as temp_file:
                temp_file.write(self.rdp)
                temp_file.close()
            print "ROSTOOLS::Saving " + dp.properties["name"] + ".rdp " + "at " + path   

    # Save the entire project
    def save(self, project_name = "", project_path = ""):
        if project_path == "":
            project_path = self.project_path
        else:
            self.new(project_name, project_path)

        # Save the ROS Workpace Model
        self.save_rml()
        # Save the ROS Hardware Configurations Model
        self.save_rhw()
        # Save the ROS Deployment Model
        self.save_rdp()

'''
