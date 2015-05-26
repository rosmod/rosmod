# ROSMOD Loader class
# Author: Pranav Srinivas Kumar
# Date: 2015.04.28

import sys, os

class ROSMOD_Loader:
    # Load the business logic of component operations
    def load(self, workspace, path):
        
        print "ROSMOD::Checking for existing workspace in path:", path
        self.workspace_dir = path + "/" + workspace.properties["name"]

        if os.path.exists(self.workspace_dir):
            self.src_path = self.workspace_dir + "/src"

            for package in workspace.children:
                self.package_path = self.src_path + "/" + package.properties["name"]

                if(os.path.isfile(os.path.join(self.package_path, "CMakeLists.txt")) == True):
                    with open(os.path.join(self.package_path, "CMakeLists.txt"), 'r') as cmakelists:
                        
                        # CHECK CMAKELISTS FILE
                        package_marker = False
                        package_text = ""
                        for num, line in enumerate(cmakelists, 1):
                            if package_marker == True and "## End Package Marker" not in line:
                                package_text += line
                            if package_marker == False and "## Start Package Marker" in line:
                                package_marker = True
                            if package_marker == True and "## End Package Marker" in line:
                                package_marker = False
                        package.properties["cmakelists_packages"] = package_text   

                    with open(os.path.join(self.package_path, "CMakeLists.txt"), 'r') as cmakelists:
                        
                        # CHECK CMAKELISTS FILE
                        functions_marker = False
                        functions_text = ""
                        for num, line in enumerate(cmakelists, 1):
                            if functions_marker == True and "## End Global Marker" not in line:
                                functions_text += line
                            if functions_marker == False and "## Start Global Marker" in line:
                                functions_marker = True
                            if functions_marker == True and "## End Global Marker" in line:
                                functions_marker = False
                        package.properties["cmakelists_functions"] = functions_text  

                    with open(os.path.join(self.package_path, "CMakeLists.txt"), 'r') as cmakelists:
                        
                        # CHECK CMAKELISTS FILE
                        include_marker = False
                        include_text = ""
                        for num, line in enumerate(cmakelists, 1):
                            if include_marker == True and "## End Include Directories Marker" not in line:
                                include_text += line
                            if include_marker == False and "## Start Include Directories Marker" in line:
                                include_marker = True
                            if include_marker == True and "## End Include Directories Marker" in line:
                                include_marker = False
                        package.properties["cmakelists_include_dirs"] = include_text   

                if(os.path.isfile(os.path.join(self.package_path, "CMakeLists.txt")) == True):

                    components = package.getChildrenByKind("Component")
                    for component in components:

                        with open(os.path.join(self.package_path, "CMakeLists.txt"), 'r') as cmakelists:
                            # CHECK CMAKELISTS FILE
                            cpp_marker = False
                            cpp_text = ""
                            start_marker = "## Start " + component.properties["name"] + " CPP Marker"
                            end_marker = "## End " + component.properties["name"] + " CPP Marker"
                            for num, line in enumerate(cmakelists, 1):
                                if cpp_marker == True and end_marker not in line:
                                    cpp_text += line
                                if cpp_marker == False and start_marker in line:
                                    cpp_marker = True
                                if cpp_marker == True and end_marker in line:
                                    cpp_marker = False
                            component.properties["cmakelists_cpp_marker"] = cpp_text  

                        with open(os.path.join(self.package_path, "CMakeLists.txt"), 'r') as cmakelists:
                            # CHECK CMAKELISTS FILE
                            tll_marker = False
                            tll_text = ""
                            start_marker = "## Start " + component.properties["name"] + " Target Link Libraries Marker"
                            end_marker = "## End " + component.properties["name"] + " Target Link Libraries Marker"
                            for num, line in enumerate(cmakelists, 1):
                                if tll_marker == True and end_marker not in line:
                                    tll_text += line
                                if tll_marker == False and start_marker in line:
                                    tll_marker = True
                                if tll_marker == True and end_marker in line:
                                    tll_marker = False
                            component.properties["cmakelists_targetlinklibs_marker"] = tll_text                     

                if os.path.exists(self.package_path):
                    print "ROSMOD::Preserving code for Package: ", self.package_path

                    self.include = self.package_path + "/include"
                    self.src = self.package_path + "/src"
                    self.cpp = self.src + "/" + package.properties["name"]
                    self.hpp = self.include + "/" + package.properties["name"]

                    components = []
                    for child in package.children:
                        if child.kind == "Component":
                            components.append(child)

                    for component in components:

                        # FIND ALL COMPONENT HPP FILES
                        if(os.path.isfile(os.path.join(self.hpp, component.properties["name"] + ".hpp")) == True):

                            with open(os.path.join(self.hpp, component.properties["name"] + ".hpp"), 'r') as hpp_file:
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
                                component.properties["user_includes"] = incl_text

                            with open(os.path.join(self.hpp, component.properties["name"] + ".hpp"), 'r') as hpp_file:
                                # CHECK USER GLOBALS IN HPP
                                hpp_globals_marker = False
                                hpp_globals_text = ""
                                for num, line in enumerate(hpp_file, 1):
                                    if hpp_globals_marker == True and "//# End User Globals Marker" not in line:
                                        hpp_globals_text += line
                                    if hpp_globals_marker == False and "//# Start User Globals Marker" in line:
                                        hpp_globals_marker = True
                                    if hpp_globals_marker == True and "//# End User Globals Marker" in line:
                                        hpp_globals_marker = False
                                component.properties["hpp_globals"] = hpp_globals_text

                            with open(os.path.join(self.hpp, component.properties["name"] + ".hpp"), 'r') as hpp_file:
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
                                component.properties["user_private_variables"] = pv_text
                                                    
                        # FIND ALL COMPONENT CPP FILES
                        if(os.path.isfile(os.path.join(self.cpp, component.properties["name"] + ".cpp")) == True):

                            with open(os.path.join(self.cpp, component.properties["name"] + ".cpp"), 'r') as cpp_file:
                                # CHECK USER GLOBALS
                                globals_marker = False
                                globals_text = ""
                                for num, line in enumerate(cpp_file, 1):
                                    if globals_marker == True and "//# End User Globals Marker" not in line:
                                        globals_text += line
                                    if globals_marker == False and "//# Start User Globals Marker" in line:
                                        globals_marker = True
                                    if globals_marker == True and "//# End User Globals Marker" in line:
                                        globals_marker = False
                                component.properties["user_globals"] = globals_text
                            
                            # OPEN THE CPP FILE
                            with open(os.path.join(self.cpp, component.properties["name"] + ".cpp"), 'r') as cpp_file:
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
                                component.properties["init_business_logic"] = init_text

                            # OPEN THE CPP FILE
                            with open(os.path.join(self.cpp, component.properties["name"] + ".cpp"), 'r') as cpp_file:
                                # CHECK INIT BUSINESS LOGIC
                                destructor_marker = False
                                destructor_text = ""
                                for num, line in enumerate(cpp_file, 1):
                                    if destructor_marker == True and "//# End Destructor" not in line:
                                        destructor_text += line
                                    if destructor_marker == False and "//# Start Destructor Marker" in line:
                                        destructor_marker = True
                                    if destructor_marker == True and "//# End Destructor Marker" in line:
                                        destructor_marker = False
                                component.properties["destructor"] = destructor_text

                            subscribers = []
                            timers = []
                            servers = []
                            for child in component.children:
                                if child.kind == "Subscriber":
                                    subscribers.append(child)
                                elif child.kind == "Timer":
                                    timers.append(child)
                                elif child.kind == "Server":
                                    servers.append(child)
                                
                            # CHECK SUBSCRIBER BUSINESS LOGIC
                            for sub in subscribers:

                                # OPEN THE CPP FILE
                                with open(os.path.join(self.cpp, component.properties["name"] + ".cpp"), 'r') as cpp_file:
                                    
                                    sub_marker = False
                                    sub_text = ""
                                    callback_name = sub.properties["name"] + "_OnOneData"
                                    start_callback = "//# Start " + callback_name
                                    end_callback = "//# End " + callback_name
                                    for num, line in enumerate(cpp_file, 1):
                                        if sub_marker == True and end_callback not in line:
                                            sub_text += line
                                        if sub_marker == False and start_callback in line:
                                            sub_marker = True
                                        if sub_marker == True and end_callback in line:
                                            sub_marker = False
                                    sub.properties["business_logic"] = sub_text
                                    
                            # CHECK TIMER BUSINESS LOGIC
                            for timer in timers:

                                # OPEN THE CPP FILE
                                with open(os.path.join(self.cpp, component.properties["name"] + ".cpp"), 'r') as cpp_file:
                                    
                                    timer_marker = False
                                    timer_text = ""
                                    callback_name = timer.properties["name"] + "Callback"
                                    start_callback = "//# Start " + callback_name
                                    end_callback = "//# End " + callback_name
                                    for num, line in enumerate(cpp_file, 1):
                                        if timer_marker == True and end_callback not in line:
                                            timer_text += line
                                        if timer_marker == False and start_callback in line:
                                            timer_marker = True
                                        if timer_marker == True and end_callback in line:
                                            timer_marker = False
                                    timer.properties["business_logic"] = timer_text
                                    
                            # CHECK SERVICE BUSINESS LOGIC
                            for server in servers:

                                # OPEN THE CPP FILE
                                with open(os.path.join(self.cpp, component.properties["name"] + ".cpp"), 'r') as cpp_file:
                                    
                                    service_marker = False
                                    service_text = ""
                                    callback_name = server.properties["service_reference"].properties["name"] + "Callback"
                                    start_callback = "//# Start " + callback_name
                                    end_callback = "//# End " + callback_name
                                    for num, line in enumerate(cpp_file, 1):
                                        if service_marker == True and end_callback not in line:
                                            service_text += line
                                        if service_marker == False and start_callback in line:
                                            service_marker = True
                                        if service_marker == True and end_callback in line:
                                            service_marker = False
                                    server.properties["business_logic"] = service_text
