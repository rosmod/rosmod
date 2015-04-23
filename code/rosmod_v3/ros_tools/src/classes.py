# ROSMOD Classes
# Author: Pranav Srinivas Kumar
# Date: 2015.04.22

import sys, os

# Find Drawable_Object
exeName = sys.argv[0]
dirName = os.path.abspath(exeName)
head,tail = os.path.split(dirName)
head,tail = os.path.split(head)
sys.path.append(head + '/../editor_v2/')
from drawable import Drawable_Object

# ROS Workspace
class ROS_Workspace(Drawable_Object):
    # Initialize Workspace
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "workspace"
        self.properties["name"] = ""

# ROS Package
class ROS_Package(Drawable_Object):
    # Initialize Package
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "package"
        self.properties["name"] = ""
        self.properties["cmakelists_packages"] = ""
        self.properties["cmakelists_functions"] = ""
        self.properties["cmakelists_include_dirs"] = ""

# ROS Message
class ROS_Message(Drawable_Object):
    # Initialize Message
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "message"
        self.properties["name"] = ""

# ROS Field

# ROS Service
class ROS_Service(Drawable_Object):
    # Initialize Service
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "service"
        self.properties["name"] = ""

# ROS Service Request

# ROS Service Response

# ROS Component
class ROS_Component(Drawable_Object):
    # Initialize Component
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "component"
        self.properties["name"] = ""
        self.properties["type"] = "basic"
        self.properties["init_business_logic"] = ""
        self.properties["user_includes"] = ""
        self.properties["user_globals"] = ""
        self.properties["hpp_globals"] = ""
        self.properties["user_private_variables"] = ""
        self.properties["destructor"] = ""

# ROS Timer
class ROS_Timer(Drawable_Object):
    # Initialize Timer
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "timer"
        self.properties["name"] = ""
        self.properties["period"] = "0.0"
        self.properties["unit"] = ""
        self.properties["business_logic"] = ""

# ROS Client
class ROS_Client(Drawable_Object):
    # Initialize Client
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "client"
        self.properties["name"] = ""
        self.properties["service_text"] = ""
        self.properties["service_reference"] = None

# ROS Server
class ROS_Server(Drawable_Object):
    # Initialize Server
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "server"
        self.properties["name"] = ""
        self.properties["service_text"] = ""
        self.properties["service_reference"] = None
        self.properties["business_logic"] = ""

# ROS Publisher
class ROS_Publisher(Drawable_Object):
    # Initialize Publisher
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "publisher"
        self.properties["message_text"] = ""
        self.properties["name"] = ""
        self.properties["message_reference"] = None

# ROS Subscriber
class ROS_Subscriber(Drawable_Object):
    # Initialize Subscriber
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "subscriber"
        self.properties["name"] = ""
        self.properties["message_text"] = ""
        self.properties["message_reference"] = None
        self.properties["business_logic"] = ""

# ROS Component Instance
class ROS_Component_Instance(Drawable_Object):
    # Initialize Instance
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "component_instance"
        self.properties["name"] = ""
        self.properties["component_text"] = ""
        self.properties["component_reference"] = None

# ROS Node
class ROS_Node(Drawable_Object):
    # Initialize Node
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "node"
        self.properties["name"] = ""
        self.properties["cmakelists_add_cpp"] = ""
        self.properties["cmakelists_target_link_libs"] = ""

# ROS Hardware
class ROS_Hardware(Drawable_Object):
    # Initialize Host
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "hardware"
        self.properties["name"] = ""
        self.properties["architecture"] = ""

# Hardware Model/Configuration
class ROS_Hardware_Configuration(Drawable_Object):
    # Initialize Hardware Configuration
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "hardware_configuration"
        self.properties["name"] = ""

# ROS Deployment
class ROS_Deployment(Drawable_Object):
    # Initialize Deployment
    def __init__(self):
        self.properties["kind"] = "deployment"
        self.properties["name"] = ""
        self.properties["hardware_configuration_reference"] = None
        self.properties["xml_list"] = []
        self.groups = OrderedDict()

# ROS Port Instance
class ROS_Port_Instance(Drawable_Object):
    # Initialize the port 
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "port_instance"
        self.properties["name"] = ""
        self.properties["node_instance_reference"] = None
        self.properties["component_instance_reference"] = None
        self.properties["port_reference_string"] = ""
        self.properties["port_reference"] = None

class ROS_Group(Drawable_Object):
    # Initialize the group; children are port_instances
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "group"
        self.properties["name"] = ""

# Hardware Instances in a Deployment
class ROS_Hardware_Instance(Drawable_Object):
    # Initialize Hardware Instance
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "hardware_instance"
        self.properties["name"] = ""
        self.properties["hardware_reference"] = None
        self.properties["ip_address"] = ""
        self.properties["username"] = ""
        self.properties["sshkey"] = ""
        self.properties["init"] = ""
        # Environment Variables per host instance
        # [[env_name1, env_value1], [env_name2, env_value2], ..]
        self.properties["env_vars"] = []
 
# Node Instances in Deployment per Host Instance
class ROS_Node_Instance(Drawable_Object):
    # Initialize ROS Node Instance
    def __init__(self):
        Drawable_Object.__init__(self)
        self.properties["kind"] = "node_instance"
        self.properties["name"] = ""
        self.properties["node_reference_string"] = ""
        self.properties["node_reference"] = None
        self.properties["cmdline_arguments"] = "" 


