# ROSMOD Classes
# Author: Pranav Srinivas Kumar
# Date: 2015.07.22

from collections import OrderedDict, MutableSequence

class Children(MutableSequence):
    """Children List
    
    _inner -- Contents of the list
    _allowed -- Type Constraints on the list entries
    The list will accept only object types contained in _allowed
    """
    def __init__(self, it=(), allowed=(), cardinality=()):
        self._inner = list(it)
        self._allowed = list(type(a) for a in allowed)
        self._cardinality = cardinality

    def __len__(self):
        return len(self._inner)
    def __iter__(self):
        return iter(self._inner)
    def __contains__(self, item):
        return item in self._inner
    def __getitem__(self, index):
        return self._inner[index]
    def __setitem__(self, index, value):
        self._inner[index] = value
    def __delitem__(self, index):
        del self._inner[index]
    def __repr__(self):
        return 'Children({})'.format(self._inner)
    def insert(self, index, item):
        if type(item) in self._allowed:
            if item not in self._inner:
                item_cardinality = self._cardinality[str(type(item))]
                children_types = [str(type(val)) for val in self._inner]
                if item_cardinality == '1':
                    if str(type(item)) not in children_types:
                        return self._inner.insert(index, item)
                    else:
                        print "ERROR::Cardinality Error!"
                else:
                    return self._inner.insert(index, item)
        else:
            print "ERROR::Cannot add child: " + str(item)
            return self._inner

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

class Name(Attribute):
    """Name Attribute"""
    def __init__(self, value):
        self.kind = "string"
        self.value = value

class Path(Attribute):
    """Path Attribute"""
    def __init__(self, value):
        self.kind = "string"
        self.value = value

class Message_Definition(Attribute):
    """Definition Attribute"""
    def __init__(self, value):
        self.kind = "string"
        self.value = value

class Service_Definition(Attribute):
    """Definition Attribute"""
    def __init__(self, value):
        self.kind = "string"
        self.value = value

class Component_Type(Attribute):
    """Type Attribute"""
    def __init__(self, value):
        self.kind = "string"
        self.value = value

class Network_Profile(Attribute):
    """Network_Profile Attribute"""
    def __init__(self, value):
        self.kind = "string"
        self.value = value

class Abstract_Business_Logic(Attribute):
    """Abstract_Business_Logic Attribute"""
    def __init__(self, value):
        self.kind = "string"
        self.value = value

class Priority(Attribute):
    """Priority Attribute"""
    def __init__(self, value):
        self.kind = "integer"
        self.value = value

class Deadline(Attribute):
    """Deadline Attribute"""
    def __init__(self, value):
        self.kind = "double"
        self.value = value

class Period(Attribute):
    """Period Attribute"""
    def __init__(self, value):
        self.kind = "double"
        self.value = value

class IP_Address(Attribute):
    """IP_Address Attribute"""
    def __init__(self, value):
        self.kind = "string"
        self.value = value

class Username(Attribute):
    """Username Attribute"""
    def __init__(self, value):
        self.kind = "string"
        self.value = value

class SSH_Key(Attribute):
    """SSH_Key Attribute"""
    def __init__(self, value):
        self.kind = "string"
        self.value = value

class Deployment_Path(Attribute):
    """Deployment_Path Attribute"""
    def __init__(self, value):
        self.kind = "string"
        self.value = value

class ROS_Install_Path(Attribute):
    """ROS_Install_Path Attribute"""
    def __init__(self, value):
        self.kind = "string"
        self.value = value

class Init_Script(Attribute):
    """Init_Script Attribute"""
    def __init__(self, value):
        self.kind = "string"
        self.value = value

class Arch(Attribute):
    """Architecture Attribute"""
    def __init__(self, value):
        self.kind = "string"
        self.value = value

class Cmd_Args(Attribute):
    """Cmd_Args Attribute"""
    def __init__(self, value):
        self.kind = "string"
        self.value = value

class Scheduling_Scheme(Attribute):
    """Scheduling_Scheme Attribute"""
    def __init__(self, value):
        self.kind = "string"
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
        self.children = None
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
    def __init__(self, name, path, parent=None):
        super(Project, self).__init__()
        self.kind = "Project"
        self.parent = parent
        self['name'] = name
        self['path'] = path
        self.children = Children(allowed=[Software(), Hardware(), Deployment()], 
                                 cardinality={str(type(Software())) : '1',
                                              str(type(Hardware())) : '1..*',
                                              str(type(Deployment())) : '1..*'})

class Software(Model):
    """Software Class

    The Software class represents a ROS catkin workspace.
    This model contains:
    Messages -- Define a ROS Message
    Services -- Define a ROS Service
    Components -- Define ROSMOD component building blocks for applications
    """
    def __init__(self, name=None, parent=None):
        super(Software, self).__init__()
        self.kind = "Software"
        self.parent = parent
        self["name"] = name
        self.children = Children(allowed=[Package()], 
                                 cardinality = {str(type(Package())) : '1..*'})

class Package(Model):
    def __init__(self, name=None, parent=None):
        super(Package, self).__init__()
        self.kind = "Package"
        self.parent = parent
        self["name"] = name
        self.children = Children(allowed=[Message(), Service(), Component()],
                                 cardinality={str(type(Message())) : '0..*', 
                                              str(type(Service())) : '0..*',
                                              str(type(Component())) : '1..*'})

class Message(Model):
    def __init__(self, name=None, definition=None, parent=None):
        super(Message, self).__init__()
        self.kind = "Message"
        self.parent = parent
        self["name"] = name
        self["definition"] = definition
        self.children = Children(allowed=[], cardinality={})

class Service(Model):
    def __init__(self, name=None, definition=None, parent=None):
        super(Service, self).__init__()
        self.kind = "Service"
        self.parent = parent
        self["name"] = name
        self["definition"] = definition
        self.children = Children(allowed=[], cardinality={})

class Component(Model):
    def __init__(self, name=None, component_type=None, parent=None):
        super(Component, self).__init__()
        self.kind = "Component"
        self.parent = parent
        self["name"] = name
        self["type"] = component_type
        self.children = Children(allowed=[Client(), Server(), Publisher(), Subscriber(), Timer()],
                                 cardinality={str(type(Client())) : '0..*', 
                                              str(type(Server())) : '0..*', 
                                              str(type(Publisher())) : '0..*', 
                                              str(type(Subscriber())) : '0..*', 
                                              str(type(Timer())) : '0..*'}) 

class Client(Model):
    def __init__(self, name=None, service_reference=None, network_profile=None, parent=None):
        super(Client, self).__init__()
        self.kind = "Client"
        self.parent = parent
        self["name"] = name
        self["service_reference"] = service_reference
        self["network_profile"] = network_profile
        self.children = Children(allowed=[], cardinality={})

class Server(Model):
    def __init__(self, name=None, 
                 service_reference=None, 
                 priority=None, deadline=None, business_logic=None, parent=None):
        super(Server, self).__init__()
        self.kind = "Server"
        self.parent = parent
        self["name"] = name
        self["service_reference"] = service_reference
        self["priority"] = priority
        self["deadline"] = deadline
        self["business_logic"] = business_logic
        self.children = Children(allowed=[], cardinality={})

class Publisher(Model):
    def __init__(self, name=None, message_reference=None, network_profile=None, parent=None):
        super(Publisher, self).__init__()
        self.kind = "Publisher"
        self.parent = parent
        self["name"] = name
        self["message_reference"] = message_reference
        self["network_profile"] = network_profile
        self.children = Children(allowed=[], cardinality={})

class Subscriber(Model):
    def __init__(self, name=None, message_reference=None, priority=None, deadline=None, business_logic=None, parent=None):
        super(Subscriber, self).__init__()
        self.kind = "Subscriber"
        self.parent = parent
        self["name"] = name
        self["message_reference"] = message_reference
        self["priority"] = priority
        self["deadline"] = deadline
        self["business_logic"] = business_logic
        self.children = Children(allowed=[], cardinality={})

class Timer(Model):
    def __init__(self, name=None, period=None, priority=None, deadline=None, business_logic=None, parent=None):
        super(Timer, self).__init__()
        self.kind = "Timer"
        self.parent = parent
        self["name"] = name
        self["period"] = period
        self["priority"] = priority
        self["deadline"] = deadline
        self["business_logic"] = business_logic
        self.children = Children(allowed=[], cardinality={})

class Hardware(Model):
    def __init__(self, name=None, parent=None):
        super(Hardware, self).__init__()
        self.kind = "Hardware"
        self.parent = parent
        self["name"] = name
        self.children = Children(allowed=[Computer()], 
                                 cardinality={str(type(Computer())) : '1..*'})

class Computer(Model):
    def __init__(self, name=None, ip_address=None, username=None, sshkey=None, deployment_path=None,
                 ros_install_path=None, init_script=None, arch=None, network_profile=None, parent=None):
        super(Computer, self).__init__()
        self.kind = "Computer"
        self.parent = parent
        self["name"] = name
        self["ip_address"] = ip_address
        self["username"] = username
        self["sshkey"] = sshkey
        self["deployment_path"] = deployment_path
        self["ros_install_path"] = ros_install_path
        self["init_script"] = init_script
        self["arch"] = arch
        self["network_profile"] = network_profile
        self.children = Children(allowed=[], cardinality={})

class Deployment(Model):
    def __init__(self, name=None, parent=None):
        super(Deployment, self).__init__()
        self.kind = "Deployment"
        self.parent = parent
        self['name'] = name
        self.children = Children(allowed=[Node()], cardinality={str(type(Node())) : '1..*'})

class Node(Model):
    def __init__(self, name=None, hardware_reference=None, priority=None, cmd_args=None, 
                 deployment_path=None, parent=None):
        super(Node, self).__init__()
        self.kind = "Node"
        self.parent = parent
        self["name"] = name
        self["hardware_reference"] = hardware_reference
        self["priority"] = priority
        self["cmd_args"] = cmd_args
        self["deployment_path"] = deployment_path
        self.children = Children(allowed=[Component_Instance()], cardinality={str(type(Component_Instance())) : '1..*'})

class Component_Instance(Model):
    def __init__(self, name=None, component_reference=None, scheduling_scheme=None, logging=None, parent=None):
        super(Component_Instance, self).__init__()
        self.kind = "Component_Instance"
        self.parent = parent
        self["name"] = name
        self["component_reference"] = component_reference
        self["scheduling_scheme"] = scheduling_scheme
        self["logging"] = logging
        self.children = Children(allowed=[], cardinality={})

import json, jsonpickle

project = Project(Name("NewProject"), 
                  Path(""), 
                  parent=None)

software = Software(Name("Software"), 
                    parent=project)

package = Package(Name("Package"), 
                  parent=software)

message = Message(Name("Message"), 
                  Message_Definition("int64 value\nbool return_value"), 
                  parent=package)

service = Service(Name("Service"), 
                  Service_Definition("float64 request\n---\nfloat64 response"), 
                  parent=package)

component = Component(Name("Component"), 
                      Component_Type("BASE"), 
                      parent=package)

client = Client(name=Name("myClient"),
                service_reference=service,
                network_profile=Network_Profile("myProfile"),
                parent=component)

hardware = Hardware(Name("Hardware"), 
                    parent=project)

deployment = Deployment(Name("Deployment"), 
                        parent=project)

# Establish tree
component.add_child(client)
package.add_child(message)
package.add_child(service)
package.add_child(component)
print type(package)
software.add_child(package)
print software.children
project.add_child(software)
project.add_child(hardware)
project.add_child(deployment)

encoder_output = json.dumps(json.loads(jsonpickle.encode(project)), indent=4)
print encoder_output
with open('model.txt', 'w') as metamodel:
    metamodel.write(encoder_output)


