# ROSMOD Classes
# Author: Pranav Srinivas Kumar
# Date: 2015.07.22

from collections import OrderedDict

class Attribute(object):
    """Generic Attributes class

    Each Attribute has the following:
    kind -- The datatype of the attribute e.g. string, float, bool etc.
    value -- The value of the attributes e.g. "my_component", 1.642 etc.
    """
    def __init__(self, kind, value):
        self.kind = kind
        self.value = value

class Model(object):
    """Generic Model/Container class

    Every Model has the following:
    kind -- The Domain-specific kind of Model e.g. Component, Port etc.
    parent -- A parent Model Object.
    children -- A list of children (Model) objects.
    attributes -- A dictionary of attributes.
    """
    def __init__(self):
        self.kind = ""
        self.parent = None
        self.children = []
        self.attributes = OrderedDict() 

    def __getitem__(self, key):
        return self.attributes[key]

    def add_child(self, child_model):
        self.children.append(child_model)

    def add_attribute(self, name, kind, value):
        self.attributes[name] = Attribute(kind, value)

    def get_children(self, kind):
        return [child for child in self.children if child.kind == kind]

class Project(Model):
    """Project Class
    
    The Project class is the root of a ROSMOD model.
    This object contains the following types of children:
    Software -- Describes the software workspace
    Hardware -- Describes the hardware configuration
    Deployment -- Maps software instances and hardware computers
    """
    def __init__(self, name):
        self.kind = "Project"
        self.add_attribute(name.__name__, 
                           type(name),
                           name)

class Software(Model):
    """Software Class

    The Software class represents a ROS catkin workspace.
    This model contains:
    Messages -- Define a ROS Message
    Services -- Define a ROS Service
    Components -- Define ROSMOD component building blocks for applications
    """
    def __init__(self, name):
        self.kind = "Software"
        self.attributes["name"] = name

class Package(Model):
    def __init__(self, name):
        self.kind = "Package"
        self.attributes["name"] = name

class Message(Model):
    def __init__(self, name, definition):
        self.kind = "Message"
        self.attributes["name"] = name
        self.attributes["definition"] = definition

class Service(Model):
    def __init__(self, name, definition):
        self.kind = "Service"
        self.attributes["name"] = name
        self.attributes["definition"] = definition

class Component(Model):
    def __init__(self, name, component_type):
        self.kind = "Component"
        self.attributes["name"] = name
        self.attributes["type"] = component_type

class Client(Model):
    def __init__(self, name, service_reference, network_profile):
        self.kind = "Client"
        self.attributes["name"] = name
        self.attributes["service_reference"] = service_reference
        self.attributes["network_profile"] = network_profile

class Server(Model):
    def __init__(self, name, service_reference, priority, deadline, business_logic):
        self.kind = "Server"
        self.attributes["name"] = name
        self.attributes["service_reference"] = service_reference
        self.attributes["priority"] = priority
        self.attributes["deadline"] = deadline
        self.attributes["business_logic"] = business_logic

class Publisher(Model):
    def __init__(self, name, message_reference, network_profile):
        self.kind = "Publisher"
        self.attributes["name"] = name
        self.attributes["message_reference"] = message_reference
        self.attributes["network_profile"] = network_profile

class Subscriber(Model):
    def __init__(self, message_reference, priority, deadline, business_logic):
        self.kind = "Subscriber"
        self.attributes["name"] = name
        self.attributes["message_reference"] = message_reference
        self.attributes["priority"] = priority
        self.attributes["deadline"] = deadline
        self.attributes["business_logic"] = business_logic

class Timer(Model):
    def __init__(self, name, period, priority, deadline, business_logic):
        self.kind = "Timer"
        self.attributes["name"] = name
        self.attributes["period"] = period
        self.attributes["priority"] = priority
        self.attributes["deadline"] = deadline
        self.attributes["business_logic"] = business_logic

class Hardware(Model):
    def __init__(self, name):
        self.kind = "Hardware"
        self.attributes["name"] = name

class Computer(Model):
    def __init__(self, name, ip_address, username, sshkey, deployment_path,
                 ros_install_path, init_script, arch, network_profile):
        self.kind = "Computer"
        self.attributes["name"] = name
        self.attributes["ip_address"] = ip_address
        self.attributes["username"] = username
        self.attributes["sshkey"] = sshkey
        self.attributes["deployment_path"] = deployment_path
        self.attributes["ros_install_path"] = ros_install_path
        self.attributes["init_script"] = init_script
        self.attributes["arch"] = arch
        self.attributes["network_profile"] = network_profile

class Deployment(Model):
    def __init__(self):
        self.kind = "Deployment"

class Node(Model):
    def __init__(self, name, hardware_reference, priority, cmd_args, 
                 deployment_path):
        self.kind = "Node"
        self.attributes["name"] = name
        self.attributes["hardware_reference"] = hardware_reference
        self.attributes["priority"] = priority
        self.attributes["cmd_args"] = cmd_args
        self.attributes["deployment_path"] = deployment_path

class Component_Instance(Model):
    def __init__(self, name, component_reference, scheduling_scheme, logging):
        self.kind = "Component_Instance"
        self.attributes["name"] = name
        self.attributes["component_reference"] = component
        self.attributes["scheduling_scheme"] = scheduling_scheme
        self.attributes["logging"] = logging
        


