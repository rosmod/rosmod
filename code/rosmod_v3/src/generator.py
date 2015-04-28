# ROSMOD Generator
# Author: Pranav Srinivas Kumar
# Date: 2015.02.28

from Cheetah.Template import Template
import os, sys, inspect

# Template Compile Step -- Compiling tmpl files in templates
# Generate template python files
generator_dir = os.path.dirname(os.path.realpath(__file__))
template_dir = os.path.join(generator_dir + "/templates")

# Recursively compile on template files in templates directory
os.system("/usr/local/bin/cheetah compile " + template_dir + "/*.tmpl > /dev/null 2>&1")
ros_templates = os.path.realpath(os.path.abspath
                                 (os.path.join
                                  (os.path.split
                                   (inspect.getfile
                                    (inspect.currentframe()
                                 )
                                )[0], "templates")
                              ))
if ros_templates not in sys.path:
    sys.path.insert(0, ros_templates)
from package_xml import *

from rapidxml_hpp import *
from rapidxml_utils_hpp import *
from xmlParser_hpp import *
from Logger_cpp import *
from Logger_hpp import *
from node_groups_xml import *
from base_component_hpp import *
from base_component_cpp import *
from msg import *
from srv import *
from component_hpp import *
from component_cpp import *
from nodeMain import *
from CMakeLists import *
from rml import *
from rhw import *
from rdp import *

class ROSMOD_Generator:
    # Main Generate Function
    def generate(self, workspace, path, deployments, xml_path):
        print "ROSTOOLS::Generating ROS Workspace..."
        # Make the workspace directory
        # Make the workspace directory
        self.workspace_dir = os.path.join(path, workspace.properties["name"])
        print "ROSTOOLS::Workspace Path:", self.workspace_dir
        if not os.path.exists(self.workspace_dir):
            os.makedirs(self.workspace_dir)

        # Generate the .catkin_ws file
        catkin_ws = "# This file currently only serves to mark the location of a catkin workspace for tool integration "
        filename = ".catkin_workspace"
        with open(os.path.join(self.workspace_dir, filename), 'w') as temp_file:
            temp_file.write(catkin_ws)
            temp_file.close()  

        # Make the src directory
        self.src_path = self.workspace_dir + "/src"
        if not os.path.exists(self.src_path):
            os.makedirs(self.src_path)     

        # For each package in the ros model
        for package in workspace.children:

            # Create the package directory
            self.package_path = os.path.join(self.src_path, package.properties["name"])
            print "ROSTOOLS::" + package.properties["name"] + " Path: " + self.package_path
            if not os.path.exists(self.package_path):
                os.makedirs(self.package_path)

            # Create the include directory
            self.include = self.package_path + "/include"
            if not os.path.exists(self.include):
                os.makedirs(self.include)
                
            # Create the src directory
            self.src = self.package_path + "/src"
            if not os.path.exists(self.src):
                os.makedirs(self.src)

            messages = []
            services = []
            components = []
            nodes = []
            for child in package.children:
                if child.kind == "Message":
                    messages.append(child)
                elif child.kind == "Service":
                    services.append(child)
                elif child.kind == "Component":
                    components.append(child)
                elif child.kind == "Node":
                    nodes.append(child)

            if (len(messages) > 0):
                self.msg = self.package_path + "/msg"
                if not os.path.exists(self.msg):
                    os.makedirs(self.msg)

            # Create the srv directory if there are services
            if (len(services) > 0):
                self.srv = self.package_path + "/srv"
                if not os.path.exists(self.srv):
                    os.makedirs(self.srv)

            # Create package.xml
            package_xml_namespace = {'package_name': package.properties["name"]}
            t = package_xml(searchList=[package_xml_namespace])
            self.package_xml = str(t)
            with open(os.path.join(self.package_path, "package.xml"), 'w') as temp_file:
                temp_file.write(self.package_xml)

            # Create rapidxml.hpp, rapidxml_utils.hpp, and xmlParser.hpp
            self.cpp = self.src + "/" + package.properties["name"]
            self.hpp = self.include + "/" + package.properties["name"]

            if not os.path.exists(self.hpp):
                os.makedirs(self.hpp)
            xml_namespace = {'hash_include': "#include", 
                                      'package_name': package.properties["name"]}

            # Main RapidXML file
            t = rapidxml_hpp(searchList=[xml_namespace])
            self.rapidxml_hpp = str(t)
            with open(os.path.join(self.hpp, "rapidxml.hpp"), 'w') as temp_file:
                temp_file.write(self.rapidxml_hpp)

            # RapidXML Utils file
            t = rapidxml_utils_hpp(searchList=[xml_namespace])
            self.rapidxml_utils_hpp = str(t)
            with open(os.path.join(self.hpp, "rapidxml_utils.hpp"), 'w') as temp_file:
                temp_file.write(self.rapidxml_utils_hpp)

            # XML Parser file
            t = xmlParser_hpp(searchList=[xml_namespace])
            self.xmlParser_hpp = str(t)
            with open(os.path.join(self.hpp, "xmlParser.hpp"), 'w') as temp_file:
                temp_file.write(self.xmlParser_hpp)

            # Create Component.cpp and Component.hpp
            self.cpp = self.src + "/" + package.properties["name"]
            self.hpp = self.include + "/" + package.properties["name"]

            if not os.path.exists(self.cpp):
                os.makedirs(self.cpp)
            base_cpp_namespace = {'hash_include': "#include", 
                                      'package_name': package.properties["name"]}
            # Populate Base Component cpp template
            t = base_component_cpp(searchList=[base_cpp_namespace])
            self.base_cpp = str(t)
            # Write Component.cpp
            with open(os.path.join(self.cpp, "Component.cpp"), 'w') as temp_file:
                temp_file.write(self.base_cpp)

            if not os.path.exists(self.hpp):
                os.makedirs(self.hpp)
            base_hpp_namespace = {'hash_include': "#include", 
                                      'package_name': package.properties["name"]}
            # Populate Base Component hpp template
            t = base_component_hpp(searchList=[base_hpp_namespace])
            self.base_hpp = str(t)
            # Write Component.hpp
            with open(os.path.join(self.hpp, "Component.hpp"), 'w') as temp_file:
                temp_file.write(self.base_hpp)

            # Populate Logger cpp template
            t = Logger_cpp(searchList=[base_cpp_namespace])
            self.logger_cpp = str(t)
            # Write Logger.cpp
            with open(os.path.join(self.cpp, "Logger.cpp"), 'w') as temp_file:
                temp_file.write(self.logger_cpp)

            # Populate Logger hpp template
            t = Logger_hpp(searchList=[base_hpp_namespace])
            self.logger_hpp = str(t)
            # Write Logger.hpp
            with open(os.path.join(self.hpp, "Logger.hpp"), 'w') as temp_file:
                temp_file.write(self.logger_hpp)

            # Create all package messages in msg folder
            for message in messages:
                temp_fields = []
                for child in message.children:
                    if "value" in child.properties.keys():
                        temp_fields.append([child.properties["datatype"],
                                            child.properties["name"],
                                            child.properties["value"]])
                    else:
                        temp_fields.append([child.properties["datatype"],
                                            child.properties["name"]])
                msg_namespace = {'fields': temp_fields}
                msg_filename = message.properties["name"] + ".msg"
                t = msg(searchList=[msg_namespace])
                self.msg_fields = str(t)
                # Write msg file
                with open(os.path.join(self.msg, msg_filename), 'w') as temp_file:
                    temp_file.write(self.msg_fields)

            # Create all package services in srv folder
            for service in services:
                srv_filename = service.properties["name"] + ".srv"
                temp_request = []
                temp_response = []
                for child in service.children:
                    if child.kind == "Request":
                        for field in child.children:
                            if "value" in field.properties.keys():
                                temp_request.append([field.properties["datatype"],
                                                     field.properties["name"],
                                                     field.properties["value"]])
                            else:
                                temp_request.append([field.properties["datatype"],
                                                     field.properties["name"]])                 
                    elif child.kind == "Response":
                        for field in child.children:
                            if "value" in child.properties.keys():
                                temp_response.append([field.properties["datatype"],
                                                      field.properties["name"],
                                                      field.properties["value"]])
                            else:
                                temp_response.append([field.properties["datatype"],
                                                      field.properties["name"]])                 
                srv_namespace = {'request_fields': temp_request, 
                                 'response_fields': temp_response}
                t = srv(searchList=[srv_namespace])
                self.srv_fields = str(t)
                # Write the srv file
                with open(os.path.join(self.srv, srv_filename), 'w') as temp_file:
                    temp_file.write(self.srv_fields)

            cmakelists_services = services

