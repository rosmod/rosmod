# Generator
#
# Generates the ROS Workspace
#
# Author: Pranav Srinivas Kumar
# Date: 2014.12.16

import sys
import os
from Cheetah.Template import Template
import inspect
# Template Compile step -- Compiling tmpl files in templates
# Generates equivalent .py files
generator_dir = os.path.dirname(os.path.realpath(__file__))
template_dir = os.path.join(generator_dir + "/templates")

# Recursively compile on template files in templates directory
os.system("/usr/local/bin/cheetah compile " + template_dir + "/*.tmpl")
ros_templates = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile(inspect.currentframe()))[0], "templates")))
if ros_templates not in sys.path:
    sys.path.insert(0, ros_templates)
from package_xml import *
from base_component_hpp import *
from base_component_cpp import *
from msg import *
from srv import *
from component_hpp import *
from component_cpp import *
from nodeMain import *
from CMakeLists import *

class Generator:
    
    def generate(self, listener, path):

        print "\nGenerating ROS Workspace..."
        # Make the workspace directory
        self.workspace_dir = path + "/" + listener.workspace.name
        print "Workspace Path:", self.workspace_dir
        if not os.path.exists(self.workspace_dir):
            os.makedirs(self.workspace_dir)

        # Generate the .catkin_ws file in the workspace directory
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
        for package in listener.workspace.packages:
            # Create the package directory
            self.package_path = self.src_path + "/" + package.name
            print package.name, "Path:", self.package_path
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

            # Create the msg directory if there are messages
            if (len(package.messages) > 0):
                self.msg = self.package_path + "/msg"
                if not os.path.exists(self.msg):
                    os.makedirs(self.msg)

            # Create the srv directory if there are services
            if (len(package.services) > 0):
                self.srv = self.package_path + "/srv"
                if not os.path.exists(self.srv):
                    os.makedirs(self.srv)

            # Create package.xml
            package_xml_namespace = {'package_name': package.name}
            t = package_xml(searchList=[package_xml_namespace])
            self.package_xml = str(t)
            with open(os.path.join(self.package_path, "package.xml"), 'w') as temp_file:
                temp_file.write(self.package_xml)

            # Create Component.cpp and Component.hpp
            self.cpp = self.src + "/" + package.name
            self.hpp = self.include + "/" + package.name

            if not os.path.exists(self.cpp):
                os.makedirs(self.cpp)
            base_cpp_namespace = {'hash_include': "#include", 
                                      'package_name': package.name}
            # Populate Base Component cpp template
            t = base_component_cpp(searchList=[base_cpp_namespace])
            self.base_cpp = str(t)
            # Write Component.cpp
            with open(os.path.join(self.cpp, "Component.cpp"), 'w') as temp_file:
                temp_file.write(self.base_cpp)

            if not os.path.exists(self.hpp):
                os.makedirs(self.hpp)
            base_hpp_namespace = {'hash_include': '#include'}
            # Populate Base Component hpp template
            t = base_component_hpp(searchList=[base_hpp_namespace])
            self.base_hpp = str(t)
            # Write Component.hpp
            with open(os.path.join(self.hpp, "Component.hpp"), 'w') as temp_file:
                temp_file.write(self.base_hpp)

            # Create all package messages in msg folder
            for message in package.messages:
                msg_namespace = {'fields': message.fields}
                msg_filename = message.name + ".msg"
                t = msg(searchList=[msg_namespace])
                self.msg_fields = str(t)
                # Write msg file
                with open(os.path.join(self.msg, msg_filename), 'w') as temp_file:
                    temp_file.write(self.msg_fields)

            # Create all package services in srv folder
            for service in package.services:
                srv_filename = service.name + ".srv"
                srv_namespace = {'request_fields': service.request_fields, 
                                 'response_fields': service.response_fields}
                t = srv(searchList=[srv_namespace])
                self.srv_fields = str(t)
                # Write the srv file
                with open(os.path.join(self.srv, srv_filename), 'w') as temp_file:
                    temp_file.write(self.srv_fields)


            # Create a .hpp and .cpp per component definition in package
            for component in package.components:
                component_name  = component.name
                define_guard = component_name.upper()
                topics = []
                for publisher in component.publishers:
                    topics.append(publisher.topic)
                for subscriber in component.subscribers:
                    topics.append(subscriber.topic)
                topics = set(topics)
                services = []
                for provided in component.provided_services:
                    services.append(provided)
                for required in component.required_services:
                    services.append(required)
                services = set(services)
                hash_include = "#include"
                component_namespace = {'define_guard': define_guard, 
                                       'hash_include': hash_include, 
                                       'package_name': package.name, 
                                       'topics': topics, 
                                       'services': services, 
                                       'component_name': component.name, 
                                       'publishers': component.publishers, 
                                       'subscribers': component.subscribers, 
                                       'provided_services': component.provided_services, 
                                       'required_services': component.required_services, 
                                       'timers': component.timers}
                t = component_hpp(searchList=[component_namespace])
                self.component_hpp_str = str(t)
                # Write the component hpp file
                hpp_filename = component_name + ".hpp"

                # CODE PRESERVATION FOR COMPONENT HEADER FILES

                existing_file_h = []
                line_number_h = 0
                overwrite_h = True
                new_file_h = self.component_hpp_str.splitlines(True)
                new_text_h = ""

                if(os.path.isfile(os.path.join(self.hpp, hpp_filename)) == False):
                    with open(os.path.join(self.hpp, hpp_filename), 'w') as temp_file_h:
                        temp_file_h.write(self.component_hpp_str)
                else:
                    with open (os.path.join(self.hpp, hpp_filename), "r") as existing_h:
                        existing_file_h = existing_h.readlines()
                        for line_h in existing_file_h:
                            if "//#PRESERVE CODE" in line_h:
                                overwrite_h = False
                            if overwrite_h == False:
                                new_file_h.insert(line_number_h, line_h)
                            if "//#END PRESERVE CODE" in line_h:
                                overwrite_h = True
                            line_number_h += 1
                        for line_h in new_file_h:
                            new_text_h += line_h
                        with open(os.path.join(self.hpp, hpp_filename), 'w') as new_file_handle_h:
                            new_file_handle_h.write(new_text_h)

                # GENERATING CPP FILES

                t = component_cpp(searchList=[component_namespace])
                self.component_cpp_str = str(t)
                # Write the component cpp file
                cpp_filename = component_name + ".cpp"

                # CODE PRESERVATION FOR COMPONENT CPP FILES

                existing_file = []
                line_number = 0
                overwrite = True
                new_file = self.component_cpp_str.splitlines(True)
                new_text = ""
                preserve_count = 0
                text_count = 0

                if(os.path.isfile(os.path.join(self.cpp, cpp_filename)) == False):
                    with open(os.path.join(self.cpp, cpp_filename), 'w') as temp_file:
                        temp_file.write(self.component_cpp_str)
                else:
                    with open (os.path.join(self.cpp, cpp_filename), "r") as existing:
                        existing_file = existing.readlines()
                        for line in existing_file:
                            if "//#PRESERVE CODE" in line:
                                overwrite = False
                            if overwrite == False:
                                new_file.insert(line_number + preserve_count, line)
                                text_count += 1
                            line_number += 1
                            if "//#END PRESERVE CODE" in line:
                                overwrite = True
                                preserve_count += text_count
                        for line in new_file:
                            new_text += line
                        with open(os.path.join(self.cpp, cpp_filename), 'w') as new_file_handle:
                            new_file_handle.write(new_text)

            for node in package.nodes:
                node_name = node.name
                hash_include = "#include"
                node_namespace = {'hash_include': hash_include, 
                                  'package_name': package.name,
                                  'node_name': node_name, 
                                  'components': node.components}
                t = nodeMain(searchList=[node_namespace])
                self.nodeMain_str = str(t)
                # Write node main.cpp file
                node_filename = node_name + "_main.cpp"

                existing_file_nm = []
                line_number_nm = 0
                overwrite_nm = True
                new_file_nm = self.nodeMain_str.splitlines(True)
                new_text_nm = ""

                if(os.path.isfile(os.path.join(self.cpp, node_filename)) == False):
                    with open(os.path.join(self.cpp, node_filename), 'w') as temp_file_nm:
                        temp_file_nm.write(self.nodeMain_str)
                else:
                    with open (os.path.join(self.cpp, node_filename), "r") as existing_nm:
                        existing_file_nm = existing_nm.readlines()
                        for line_nm in existing_file_nm:
                            if "//#PRESERVE CODE" in line_nm:
                                overwrite_nm = False
                            if overwrite_nm == False:
                                new_file_nm.insert(line_number_nm, line_nm)
                            if "//#END PRESERVE CODE" in line_nm:
                                overwrite_nm = True
                            line_number_nm += 1
                        for line_nm in new_file_nm:
                            new_text_nm += line_nm
                        with open(os.path.join(self.cpp, node_filename), 'w') as new_file_handle_nm:
                            new_file_handle_nm.write(new_text_nm)

            cmake_lists_namespace = {'package_name': package.name, 
                                     'messages': package.messages, 
                                     'services': package.services, 
                                     'catkin_INCLUDE_DIRS': "${catkin_INCLUDE_DIRS}",
                                     'PROJECT_NAME': "${PROJECT_NAME}",
                                     'catkin_LIBRARIES': '${catkin_LIBRARIES}',
                                     'CATKIN_PACKAGE_BIN_DESTINATION':
                                           "${CATKIN_PACKAGE_BIN_DESTINATION}",
                                     'CATKIN_PACKAGE_LIB_DESTINATION': 
                                           "${CATKIN_PACKAGE_LIB_DESTINATION}",
                                     'CATKIN_PACKAGE_INCLUDE_DESTINATION':
                                           "${CATKIN_PACKAGE_INCLUDE_DESTINATION}",
                                     'CATKIN_PACKAGE_SHARE_DESTINATION':
                                           "${CATKIN_PACKAGE_SHARE_DESTINATION}",
                                     'nodes': package.nodes}
            t = CMakeLists(searchList=[cmake_lists_namespace])
            self.cmake_lists = str(t)
            # Write CMakeLists file
            with open(os.path.join(self.package_path, "CMakeLists.txt"), 'w') as temp_file:
                temp_file.write(self.cmake_lists)
            
                
