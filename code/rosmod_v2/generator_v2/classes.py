# Meta-Model Classes
# Author: Pranav Srinivas Kumar
# Date: 2015.02.06

from collections import OrderedDict

import sys, os
exeName = sys.argv[0]
dirName = os.path.abspath(exeName)
head,tail = os.path.split(dirName)
head,tail = os.path.split(head)
sys.path.append(head + '/editor_v2/')
from drawable import Drawable_Object

# ROS Workspace
class ROS_Workspace(Drawable_Object):
    # Initialize Workspace
    def __init__(self):
        Drawable_Object.__init__(self)
        self.kind = "workspace"
        self.properties["name"] = ""

# ROS Package
class ROS_Package(Drawable_Object):
    # Initialize Package
    def __init__(self):
        Drawable_Object.__init__(self)
        self.kind = "package"
        self.properties["name"] = ""
        self.properties["cmakelists_packages"] = ""

# ROS Message
class ROS_Message(Drawable_Object):
    # Initialize Message
    def __init__(self):
        Drawable_Object.__init__(self)
        self.kind = "message"
        self.properties["name"] = ""
        # Fields e.g. [["int", "position"], ["float", "velocity", "334.5"]]
        self.properties["fields"] = []

# ROS Service
class ROS_Service(Drawable_Object):
    # Initialize Service
    def __init__(self):
        Drawable_Object.__init__(self)
        self.kind = "service"
        self.properties["name"] = ""
        self.properties["request"] = []
        self.properties["response"] = []

# ROS Component
class ROS_Component(Drawable_Object):
    # Initialize Component
    def __init__(self):
        Drawable_Object.__init__(self)
        self.kind = "component"
        self.properties["name"] = ""
        self.properties["init_business_logic"] = ""
        self.properties["user_includes"] = ""
        self.properties["user_globals"] = ""
        self.properties["hpp_globals"] = ""
        self.properties["user_private_variables"] = ""

# ROS Timer
class ROS_Timer(Drawable_Object):
    # Initialize Timer
    def __init__(self):
        Drawable_Object.__init__(self)
        self.kind = "timer"
        self.properties["name"] = ""
        self.properties["period"] = 0.0
        self.properties["unit"] = ""
        self.properties["business_logic"] = ""

# ROS Client
class ROS_Client(Drawable_Object):
    # Initialize Client
    def __init__(self):
        Drawable_Object.__init__(self)
        self.kind = "client"
        self.properties["name"] = ""
        self.properties["service_reference"] = None

# ROS Server
class ROS_Server(Drawable_Object):
    # Initialize Server
    def __init__(self):
        Drawable_Object.__init__(self)
        self.kind = "server"
        self.properties["name"] = ""
        self.properties["service_reference"] = None
        self.properties["business_logic"] = ""

# ROS Publisher
class ROS_Publisher(Drawable_Object):
    # Initialize Publisher
    def __init__(self):
        Drawable_Object.__init__(self)
        self.kind = "publisher"
        self.properties["name"] = ""
        self.properties["message_reference"] = None

# ROS Subscriber
class ROS_Subscriber(Drawable_Object):
    # Initialize Subscriber
    def __init__(self):
        Drawable_Object.__init__(self)
        self.kind = "subscriber"
        self.properties["name"] = ""
        self.properties["message_reference"] = None
        self.properties["business_logic"] = ""

# ROS Component Instance
class ROS_Component_Instance(Drawable_Object):
    # Initialize Instance
    def __init__(self):
        Drawable_Object.__init__(self)
        self.kind = "component_instance"
        self.properties["name"] = ""
        self.properties["component_reference"] = None

# ROS Node
class ROS_Node(Drawable_Object):
    # Initialize Node
    def __init__(self):
        Drawable_Object.__init__(self)
        self.kind = "node"
        self.properties["name"] = ""
        self.properties["cmakelists_add_cpp"] = ""


