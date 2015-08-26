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
        super(Attribute, self).__init__()
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
        super(Model, self).__init__()
        self.kind = ""
        self.parent = None
        self.children = []
        self.attributes = OrderedDict() 

    def __getitem__(self, key):
        return self.attributes[key]

    def __setitem__(self, key, value):
        self.attributes[key] = value

    def add_child(self, child_model):
        self.children.append(child_model)

    def add_attribute(self, name, kind, value):
        self[name] = Attribute(kind, value)

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
        super(Project, self).__init__()
        self.kind = "Project"
        self['name'] = name

class Software(Model):
    """Software Class

    The Software class represents a ROS catkin workspace.
    This model contains:
    Messages -- Define a ROS Message
    Services -- Define a ROS Service
    Components -- Define ROSMOD component building blocks for applications
    """
    def __init__(self, name):
        super(Software, self).__init__()
        self.kind = "Software"
        self["name"] = name

class Package(Model):
    def __init__(self, name):
        super(Package, self).__init__()
        self.kind = "Package"
        self["name"] = name

class Message(Model):
    def __init__(self, name, definition):
        super(Message, self).__init__()
        self.kind = "Message"
        self["name"] = name
        self["definition"] = definition

class Service(Model):
    def __init__(self, name, definition):
        super(Service, self).__init__()
        self.kind = "Service"
        self["name"] = name
        self["definition"] = definition

class Component(Model):
    def __init__(self, name, component_type):
        super(Component, self).__init__()
        self.kind = "Component"
        self["name"] = name
        self["type"] = component_type

class Client(Model):
    def __init__(self, name, service_reference, network_profile):
        super(Client, self).__init__()
        self.kind = "Client"
        self["name"] = name
        self["service_reference"] = service_reference
        self["network_profile"] = network_profile

class Server(Model):
    def __init__(self, name, service_reference, priority, deadline, business_logic):
        super(Server, self).__init__()
        self.kind = "Server"
        self["name"] = name
        self["service_reference"] = service_reference
        self["priority"] = priority
        self["deadline"] = deadline
        self["business_logic"] = business_logic

class Publisher(Model):
    def __init__(self, name, message_reference, network_profile):
        super(Publisher, self).__init__()
        self.kind = "Publisher"
        self["name"] = name
        self["message_reference"] = message_reference
        self["network_profile"] = network_profile

class Subscriber(Model):
    def __init__(self, message_reference, priority, deadline, business_logic):
        super(Subscriber, self).__init__()
        self.kind = "Subscriber"
        self["name"] = name
        self["message_reference"] = message_reference
        self["priority"] = priority
        self["deadline"] = deadline
        self["business_logic"] = business_logic

class Timer(Model):
    def __init__(self, name, period, priority, deadline, business_logic):
        super(Timer, self).__init__()
        self.kind = "Timer"
        self["name"] = name
        self["period"] = period
        self["priority"] = priority
        self["deadline"] = deadline
        self["business_logic"] = business_logic

class Hardware(Model):
    def __init__(self, name):
        super(Hardware, self).__init__()
        self.kind = "Hardware"
        self["name"] = name

class Computer(Model):
    def __init__(self, name, ip_address, username, sshkey, deployment_path,
                 ros_install_path, init_script, arch, network_profile):
        super(Computer, self).__init__()
        self.kind = "Computer"
        self["name"] = name
        self["ip_address"] = ip_address
        self["username"] = username
        self["sshkey"] = sshkey
        self["deployment_path"] = deployment_path
        self["ros_install_path"] = ros_install_path
        self["init_script"] = init_script
        self["arch"] = arch
        self["network_profile"] = network_profile

class Deployment(Model):
    def __init__(self, name):
        super(Deployment, self).__init__()
        self.kind = "Deployment"
        self['name'] = name

class Node(Model):
    def __init__(self, name, hardware_reference, priority, cmd_args, 
                 deployment_path):
        super(Node, self).__init__()
        self.kind = "Node"
        self["name"] = name
        self["hardware_reference"] = hardware_reference
        self["priority"] = priority
        self["cmd_args"] = cmd_args
        self["deployment_path"] = deployment_path

class Component_Instance(Model):
    def __init__(self, name, component_reference, scheduling_scheme, logging):
        super(Component_Instance, self).__init__()
        self.kind = "Component_Instance"
        self["name"] = name
        self["component_reference"] = component
        self["scheduling_scheme"] = scheduling_scheme
        self["logging"] = logging
        


