#!/usr/bin/env python

"""This file defines the ROSMOD Component Model"""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

import os
import textwrap
from string import Template
from base import Model, Artifact, Children
from attributes import Name, Component_Type, Detailed_Description
from client import Client
from server import Server
from publisher import Publisher
from subscriber import Subscriber
from timer import Timer

class Component(Model):
    def __init__(self, name=Name(""), component_type=Component_Type("BASE"), 
                 detailed_description=Detailed_Description(""),
                 parent=None):
        super(Component, self).__init__()
        self.kind = "Component"

        assert name != None, "Component name is None!"
        assert component_type != None, "Component type is None!"
        assert detailed_description != None,\
                                       "Component detailed description is None!"

        self.parent = parent
        self["name"] = name
        self["type"] = component_type
        self["detailed_description"] = detailed_description

        self.children = Children(allowed=[Client(), 
                                          Server(), 
                                          Publisher(), 
                                          Subscriber(), 
                                          Timer()],
                                 cardinality={str(type(Client()))\
                                              : '0..*', 
                                              str(type(Server()))\
                                              : '0..*', 
                                              str(type(Publisher()))\
                                              : '0..*', 
                                              str(type(Subscriber()))\
                                              : '0..*', 
                                              str(type(Timer()))\
                                              : '0..*'}) 

    def create_directory_artifacts(self):
        src_dir = os.path.join(self.parent.get_artifact("src").destination, 
                               "src")
        src_artifact =  Artifact(kind="folder", 
                                 destination=src_dir,
                                 name=self.parent['name'].value)        
        self.artifacts.append(src_artifact)

        include_dir = os.path.join(self.parent.get_artifact("include").destination, 
                               "include")
        include_artifact =  Artifact(kind="folder", 
                                     destination=include_dir,
                                     name=self.parent['name'].value)
        self.artifacts.append(include_artifact)
        return src_dir, include_dir
        
    def create_init_artifact(self):
        init_artifact = Artifact(kind="snippet",
                                 name="initialization")

        # Template for a init
        init_template\
            = Template("$comment" +\
                       "$return_type $component::init_timer_callback($args) {\n" +\
                       "$bl_start_marker\n" +\
                       "$preserved_bl" +\
                       "$bl_end_marker\n}" +\
                       "\n\n")

        # Resolving the template
        init_comment  = "/**\n * @brief Component Initialization\n"
        init_comment\
            += " * @param event Time-triggered event during component startup\n"
        init_comment += " * @see startup()\n"
        init_comment += " */\n"

        comment = init_comment
        return_type = "void"
        component_name = self['name'].value
        callback_args = "const rosmod::TimerEvent& event"
        bl_start_marker = "  //# Start init_timer business logic"
        bl_end_marker = "  //# End init_timer business logic"
        if "init_timer_business_logic" in self.kwargs.keys():
            preserved_bl = self.kwargs["init_timer_business_logic"]
        else:
            preserved_bl = ""
        init_artifact.value\
            = init_template.substitute(comment=comment,
                                       return_type=return_type,
                                       component=component_name,
                                       args=callback_args,
                                       bl_start_marker=bl_start_marker,
                                       preserved_bl=preserved_bl,
                                       bl_end_marker=bl_end_marker)
        self.artifacts.append(init_artifact)

    def create_destructor_artifact(self):
        destructor_artifact = Artifact(kind="snippet",
                                       name="destructor")
        destructor_template\
            = Template("$comment" +\
                       "$component::~$component() {\n" +\
                       "$timer_stop" +\
                       "$pub_shutdown" +\
                       "$sub_shutdown" +\
                       "$server_shutdown" +\
                       "$client_shutdown" +\
                       "$bl_start_marker\n" +\
                       "$preserved_bl"
                       "$bl_end_marker\n" +\
                       "}\n\n")

        # Resolving the template
        dest_comment =  "/**\n * @brief Component Shutdown\n"
        dest_comment += " * Cleans up all component ports and timers\n"
        dest_comment += " */\n"

        comment = dest_comment
        component_name = self['name'].value
        bl_start_marker = "  //# Start destructor business logic"
        bl_end_marker = "  //# End destructor business logic"
        if "destructor_business_logic" in self.kwargs.keys():
            preserved_bl = self.kwargs["destructor_business_logic"]
        else:
            preserved_bl = ""
        timers = ""
        for timer in self.get_children("Timer"):
            timers += "  {}.stop()\n".format(timer['name'].value)
        publishers = ""
        for pub in self.get_children("Publisher"):
            publishers += "  {}.shutdown()\n".format(pub['name'].value)
        subscribers = ""
        for sub in self.get_children("Subscriber"):
            subscribers += "  {}.shutdown()\n".format(sub['name'].value)
        servers = ""
        for server in self.get_children("Server"):
            servers += "  {}.shutdown()\n".format(server['name'].value)
        clients = ""
        for client in self.get_children("Client"):
            clients += "  {}.shutdown()\n".format(client['name'].value)
        destructor_artifact.value\
            = destructor_template.substitute(comment=comment,
                                             component=component_name,
                                             timer_stop=timers,
                                             pub_shutdown=publishers,
                                             sub_shutdown=subscribers,
                                             server_shutdown=servers,
                                             client_shutdown=clients,
                                             bl_start_marker=bl_start_marker,
                                             preserved_bl=preserved_bl,
                                             bl_end_marker=bl_end_marker)

        self.artifacts.append(destructor_artifact)     

    def create_headers_artifact(self):
        leading_comment = "/** @file {}.cpp\n".format(self['name'].value)
        leading_comment += " *  @brief {} class definition\n"\
            .format(self['name'].value)
        if self['detailed_description'].value == "":
            leading_comment += " * \n"
        else:
            leading_comment += " *\n"
            textwrapped\
                = textwrap.fill(self['detailed_description'].value, 50).split('\n')  
            for line in textwrapped:
                leading_comment += " *  {}\n".format(line)   
            leading_comment += " *\n"
        for author in self.parent.parent.parent['authors'].value:
            leading_comment += " *  @author {}\n".format(author)
        leading_comment += " */\n\n"

        package = self.parent['name'].value
        component = self['name'].value
        include_header = leading_comment
        include_header += "#include \"{}/{}.hpp\"\n\n".format(package, component)
        self.artifacts.append(Artifact(kind="snippet",
                                       name="headers",
                                       value=include_header)) 

    def create_startup_artifact(self):
        startup_comment =  "/**\n * @brief Component Startup Routine\n"
        startup_comment += " * Instantiates all component ports and timers\n"
        startup_comment += " */\n"
        startup_value = startup_comment
        
        startup_artifact = Artifact(kind="snippet", 
                                    name="startup",
                                    value=startup_value)
        self.artifacts.append(startup_artifact)

    def create_cpp_artifact(self):
        src_dir = os.path.join(self.parent.get_artifact("src").destination, 
                               "src")
        cpp_dir = os.path.join(src_dir, self.parent['name'].value)
        cpp_artifact = Artifact(kind="code", 
                                destination=cpp_dir,
                                name=self['name'].value + ".cpp")
        cpp_artifact.order = ["headers", 
                              "initialization", 
                              "timer_callback", 
                              "server_callback",
                              "subscriber_callback",
                              "destructor",
                              "startup"]
        self.artifacts.append(cpp_artifact)

    def create_header_define_artifact(self):
        leading_comment = "/** @file {}.hpp\n".format(self['name'].value)
        leading_comment += " *  @brief {} class declaration\n"\
            .format(self['name'].value)
        if self['detailed_description'].value == "":
            leading_comment += " * \n"
        else:
            leading_comment += " *\n"
            textwrapped\
                = textwrap.fill(self['detailed_description'].value, 50).split('\n')  
            for line in textwrapped:
                leading_comment += " *  {}\n".format(line)   
            leading_comment += " *\n"
        for author in self.parent.parent.parent['authors'].value:
            leading_comment += " *  @author {}\n".format(author)
        leading_comment += " */\n\n"
        define_value = "{0}_HPP".format(self['name'].value.upper())

        header_define_value = leading_comment
        header_define_value += "#ifndef {0}\n#define {0}\n".format(define_value)
        header_define_artifact = Artifact(kind="snippet",
                                          name="header_define",
                                          value=header_define_value) 
        self.artifacts.append(header_define_artifact)

    def create_includes_artifact(self):
        includes_value = "#include \"rosmod/rosmod_ros.h\"\n" +\
                         "#include \"Component.hpp\"\n"
        messages = []
        services = []
        for child in self.children:
            if child.kind in ["Server", "Client"]:
                service = child["service_reference"].value
                if service not in services:
                    services.append(service)
            elif child.kind in ["Publisher", "Subscriber"]:
                message = child['message_reference'].value
                if message not in messages:
                    messages.append(message)
        for msg in messages:
            package = msg.parent['name'].value
            msg_name = msg['name'].value
            includes_value += "#include \"{0}/{1}.h\"\n".format(package, msg_name)
        for srv in services:
            package = srv.parent['name'].value
            srv_name = srv['name'].value
            includes_value += "#include \"{0}/{1}.h\"\n".format(package, srv_name)
        includes_value += "\n"

        if "header_includes" in self.kwargs.keys():
            preserved_includes = self.kwargs["header_includes"]
        else:
            preserved_includes = ""
        includes_value += "//# Start header includes\n"
        includes_value += preserved_includes
        includes_value += "//# End header includes\n\n"

        includes_artifact = Artifact(kind="snippet",
                                     name="includes",
                                     value=includes_value)
        self.artifacts.append(includes_artifact)

    def create_globals_artifact(self):
        if "header_globals" in self.kwargs.keys():
            preserved_globals = self.kwargs["header_globals"]
        else:
            preserved_globals = ""
        globals_value = "//# Start global variables\n"
        globals_value += preserved_globals
        globals_value += "//# End global variables\n\n"
        globals_artifact = Artifact(kind="",
                                    name="globals",
                                    value=globals_value)
        self.artifacts.append(globals_artifact)

    def create_class_artifact(self):
        class_value\
            = "class {0} : public Component\n{1}\npublic:\n"\
                .format(self['name'].value, "{")

        constructor_comment =  "  /**\n   * @brief Component Creation\n"
        constructor_comment += "   * @param config The component configuration\n"
        constructor_comment += "   * @param argc The ROS node argument count\n"
        constructor_comment += "   * @param argv The ROS node array of arguments\n"
        constructor_comment += "   */\n"
        class_value += constructor_comment
        class_value += "  {}(ComponentConfig& config, int argc, char **argv)"\
            .format(self['name'].value) +\
            "\n     : Component(config, argc, argv) {}\n\n".format("{}")
        init_comment  = "  /**\n   * @brief Component Initialization\n"
        init_comment\
            += "   * @param event Time-triggered event during component startup\n"
        init_comment += "   * @see startup()\n"
        init_comment += "   */\n"
        class_value += init_comment
        class_value += "  void init_timer_callback(const {});\n\n"\
                       .format("rosmod::TimerEvent& event")

        class_artifact = Artifact(kind="snippet",
                                  name="class",
                                  value=class_value)
        self.artifacts.append(class_artifact)

    def create_startup_decl_artifact(self):
        startup_comment =  "  /**\n   * @brief Component Startup Routine\n"
        startup_comment += "   * Instantiates all component ports and timers\n"
        startup_comment += "   */\n"
        startup_value = startup_comment
        startup_value += "  void startup();\n\n"
        startup_artifact = Artifact(kind="snippet",
                                    name="startup_declaration",
                                    value=startup_value)
        self.artifacts.append(startup_artifact)

    def create_destructor_decl_artifact(self):
        dest_comment =  "  /**\n   * @brief Component Shutdown\n"
        dest_comment += "   * Cleans up all component ports and timers\n"
        dest_comment += "   */\n"
        dest_value = dest_comment
        dest_value += "  ~{}();\n\n".format(self['name'].value)
        dest_value += "private:\n"
        dest_artifact = Artifact(kind="snippet",
                                 name="destructor_declaration",
                                 value=dest_value)
        self.artifacts.append(dest_artifact)

    def create_privates_artifact(self):
        if "header_privates" in self.kwargs.keys():
            preserved_privates = self.kwargs["header_privates"]
        else:
            preserved_privates = ""

        privates_value = "  //# Start Private Variables\n"
        privates_value += preserved_privates
        privates_value += "  //# End Private Variables\n\n"
        privates_value += "};\n\n#endif"
        privates_artifact = Artifact(kind="snippet",
                                     name="privates",
                                     value=privates_value)
        self.artifacts.append(privates_artifact)

    def create_hpp_artifact(self):
        include_dir = os.path.join(self.parent.get_artifact("include").destination, 
                               "include")
        hpp_dir = os.path.join(include_dir, self.parent['name'].value)
        hpp_artifact = Artifact(kind="code",
                                destination=hpp_dir,
                                name=self['name'].value + ".hpp")
        hpp_artifact.order = ["header_define",
                              "includes",
                              "globals",
                              "class",
                              "timer_callback_declaration",
                              "server_callback_declaration",
                              "subscriber_callback_declaration",
                              "startup_declaration",
                              "destructor_declaration",
                              "timer_declaration",
                              "publisher_declaration",
                              "subscriber_declaration",
                              "server_declaration",
                              "client_declaration",
                              "privates"]
        self.artifacts.append(hpp_artifact)

    def preserve_code(self, file_path, key, start_marker, end_marker):
        with open(file_path, 'r') as component_src:
            preserved_code = ""
            found = False
            for line in component_src:
                if found:
                    if end_marker in line.strip():
                        break
                    preserved_code += line
                else:
                    if start_marker in line.strip():
                        found = True
                self.kwargs[key] = preserved_code

    def parse_component_source(self):
        """Preserve all code between code-preservation markers"""
        package_src\
            = self.kwargs["{}_src_dir".format(self['name'].value)] +\
            '/' + self.parent['name'].value 
        package_include\
            = self.kwargs["{}_include_dir".format(self['name'].value)] +\
            '/' + self.parent['name'].value 
        for file in os.listdir(package_src):
            if file == "{}.cpp".format(self['name'].value):
                file_path = os.path.join(package_src, file)
                self.preserve_code(file_path,
                                   "init_timer_business_logic",
                                   "//# Start init_timer business logic",
                                   "//# End init_timer business logic")
                self.preserve_code(file_path,
                                   "destructor_business_logic",
                                   "//# Start destructor business logic",
                                   "//# End destructor business logic")
                for child in self.children:
                    if child.kind in ["Timer", "Subscriber", "Server"]:
                        child_name = child['name'].value
                        self.preserve_code\
                            (file_path,
                             "{}_operation".format(child_name),
                             "//# Start {} business logic".format(child_name),
                             "//# End {} business logic".format(child_name))

        for file in os.listdir(package_include):
            if file == "{}.hpp".format(self['name'].value):
                file_path = os.path.join(package_include, file)
                self.preserve_code(file_path,
                                   "header_includes",
                                   "//# Start header includes",
                                   "//# End header includes")
                self.preserve_code(file_path,
                                   "header_globals",
                                   "//# Start global variables",
                                   "//# End global variables")
                self.preserve_code(file_path,
                                   "header_privates",
                                   "//# Start Private Variables",
                                   "//# End Private Variables")

        print self.kwargs

    def update_artifacts(self, **kwargs):
        """Update all artifact objects required for artifact generation"""

        # Start Generation
        src_dir, include_dir = self.create_directory_artifacts()
        self.kwargs = kwargs
        self.kwargs["{}_src_dir".format(self['name'].value)] = src_dir
        self.kwargs["{}_include_dir".format(self['name'].value)] = include_dir
        

        # Parse existing workspace to preserve operation business logic
        self.parse_component_source()

        # Create component CPP
        self.create_headers_artifact()
        self.create_init_artifact()
        self.create_destructor_artifact()
        self.create_startup_artifact()
        self.create_cpp_artifact()

        # Create component HPP
        self.create_header_define_artifact()
        self.create_includes_artifact()
        self.create_globals_artifact()
        self.create_class_artifact()
        self.create_startup_decl_artifact()
        self.create_destructor_decl_artifact()
        self.create_privates_artifact()
        self.create_hpp_artifact()

        self.kwargs = kwargs

        for child in self.children:
            child.update_artifacts(**kwargs)   
