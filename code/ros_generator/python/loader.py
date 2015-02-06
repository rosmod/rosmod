# Loader
#
# Loads business logic for component operations
# in the input model path
#
# Author: Pranav Srinivas Kumar
# Date: 2015.02.05

import sys
import os

class Loader:

    def load_business_logic(self, listener, path):

        self.workspace_dir = path + "/" + listener.workspace.name
        
        if os.path.exists(self.workspace_dir):
            self.src_path = self.workspace_dir + "/src"

            for package in listener.workspace.packages:
                self.package_path = self.src_path + "/" + package.name

                if os.path.exists(self.package_path):
                    print "Preserving code for Package: ", self.package_path

                    self.include = self.package_path + "/include"
                    self.src = self.package_path + "/src"
                    self.cpp = self.src + "/" + package.name
                    self.hpp = self.include + "/" + package.name

                    for component in package.components:

                        # FIND ALL COMPONENT HPP FILES
                        if(os.path.isfile(os.path.join(self.hpp, component.name + ".hpp")) == True):

                            with open(os.path.join(self.hpp, component.name + ".hpp"), 'r') as hpp_file:
                                # CHECK USER INCLUDES
                                incl_marker = False
                                incl_text = ""
                                for num, line in enumerate(hpp_file, 1):
                                    if incl_marker == True and "//# End User Includes Marker" not in line:
                                        incl_text += line
                                    if incl_marker == False and "//# Start User Includes Marker" in line:
                                        incl_marker = True
                                    if incl_marker == True and "//# End User Includes Marker" in line:
                                        incl_marker = False
                                component.user_includes = incl_text

                            with open(os.path.join(self.hpp, component.name + ".hpp"), 'r') as hpp_file:
                                # CHECK USER Private Variables
                                pv_marker = False
                                pv_text = ""
                                for num, line in enumerate(hpp_file, 1):
                                    if pv_marker == True and "//# End User Private Variables Marker" not in line:
                                        pv_text += line
                                    if pv_marker == False and "//# Start User Private Variables Marker" in line:
                                        pv_marker = True
                                    if pv_marker == True and "//# End User Private Variables Marker" in line:
                                        pv_marker = False
                                component.user_private_variables = pv_text
                                                    
                        # FIND ALL COMPONENT CPP FILES
                        if(os.path.isfile(os.path.join(self.cpp, component.name + ".cpp")) == True):
                            
                            # OPEN THE CPP FILE
                            with open(os.path.join(self.cpp, component.name + ".cpp"), 'r') as cpp_file:
                                # CHECK INIT BUSINESS LOGIC
                                init_marker = False
                                init_text = ""
                                for num, line in enumerate(cpp_file, 1):
                                    if init_marker == True and "//# End Init" not in line:
                                        init_text += line
                                    if init_marker == False and "//# Start Init Marker" in line:
                                        init_marker = True
                                    if init_marker == True and "//# End Init Marker" in line:
                                        init_marker = False
                                component.init_business_logic = init_text
                                
                            # CHECK SUBSCRIBER BUSINESS LOGIC
                            for sub in component.subscribers:

                                # OPEN THE CPP FILE
                                with open(os.path.join(self.cpp, component.name + ".cpp"), 'r') as cpp_file:
                                    
                                    sub_marker = False
                                    sub_text = ""
                                    callback_name = sub.name + "_OnOneData"
                                    start_callback = "//# Start " + callback_name
                                    end_callback = "//# End " + callback_name
                                    for num, line in enumerate(cpp_file, 1):
                                        if sub_marker == True and end_callback not in line:
                                            sub_text += line
                                        if sub_marker == False and start_callback in line:
                                            sub_marker = True
                                        if sub_marker == True and end_callback in line:
                                            sub_marker = False
                                    sub.business_logic = sub_text
                                    
                            # CHECK TIMER BUSINESS LOGIC
                            for timer in component.timers:

                                # OPEN THE CPP FILE
                                with open(os.path.join(self.cpp, component.name + ".cpp"), 'r') as cpp_file:
                                    
                                    timer_marker = False
                                    timer_text = ""
                                    callback_name = timer.name + "Callback"
                                    start_callback = "//# Start " + callback_name
                                    end_callback = "//# End " + callback_name
                                    for num, line in enumerate(cpp_file, 1):
                                        if timer_marker == True and end_callback not in line:
                                            timer_text += line
                                        if timer_marker == False and start_callback in line:
                                            timer_marker = True
                                        if timer_marker == True and end_callback in line:
                                            timer_marker = False
                                    timer.business_logic = timer_text
                                    
                            # CHECK SERVICE BUSINESS LOGIC
                            for service in component.provided_services:

                                # OPEN THE CPP FILE
                                with open(os.path.join(self.cpp, component.name + ".cpp"), 'r') as cpp_file:
                                    
                                    service_marker = False
                                    service_text = ""
                                    callback_name = service.name + "Callback"
                                    start_callback = "//# Start " + callback_name
                                    end_callback = "//# End " + callback_name
                                    for num, line in enumerate(cpp_file, 1):
                                        if service_marker == True and end_callback not in line:
                                            service_text += line
                                        if service_marker == False and start_callback in line:
                                            service_marker = True
                                        if service_marker == True and end_callback in line:
                                            service_marker = False
                                    service.service.business_logic = service_text
                                    
                                    
