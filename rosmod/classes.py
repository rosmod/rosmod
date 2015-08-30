# ROSMOD Classes
# Author: Pranav Srinivas Kumar
# Date: 2015.07.22

from collections import OrderedDict, MutableSequence
import os
import jsonpickle

class Children(MutableSequence):
    """Children List
    
    _inner -- Contents of the list
    _allowed -- The list will accept only object types contained in _allowed
    _cardinality -- Cardinality of each accepted type
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
    tooltip = ''
    display = ''
    options = []
    editable = True
    def __init__(self, kind, value):
        super(Attribute, self).__init__()
        self.kind = kind
        self.value = value

class Name(Attribute):  
    """Name Attribute"""
    tooltip = "A Name is a word or term used for identification"
    display = "Name"
    def __init__(self, value):
        super(Name, self).__init__("string",value)

class Path(Attribute):
    """Path Attribute"""
    tooltip = "Path refers to an absolute path to a folder/file"
    display = "Path"
    def __init__(self, value):
        super(Path, self).__init__("string",value)

class Message_Definition(Attribute):
    """Definition Attribute"""
    tooltip = "Describes a message that ROS nodes publish"
    display = "Message Definition"
    def __init__(self, value):
        super(Message_Definition, self).__init__("string",value)

class Service_Definition(Attribute):
    """Definition Attribute"""
    tooltip = "Describes a ROS service interface"
    display = "Service Definition"
    def __init__(self, value):
        super(Service_Definition, self).__init__("string",value)

class Component_Type(Attribute):
    """Type Attribute"""
    tooltip = "Type of the ROSMOD Component"
    display = "Component Type"
    options = ["BASE", "KSP", "SUMO"]
    def __init__(self, value):
        super(Component_Type, self).__init__("list_entry",value)

class Service_Reference(Attribute):
    """Service Reference Attribute"""
    tooltip = "Reference to a predefined service"
    display = "Service Reference"
    def __init__(self, value):
        super(Service_Reference, self).__init__("reference", value)

class Message_Reference(Attribute):
    """Message Reference Attribute"""
    tooltip = "Reference to a predefined message"
    display = "Message Reference"
    def __init__(self, value):
        super(Message_Reference, self).__init__("reference", value)

class Network_Profile(Attribute):
    """Network_Profile Attribute"""
    tooltip = "Network Profile of a port/system"
    display = "Network Profile"
    def __init__(self, value):
        super(Network_Profile, self).__init__("code",value)

class Abstract_Business_Logic(Attribute):
    """Abstract_Business_Logic Attribute"""
    tooltip = "Abstract Business Logic of a Component Operation"
    display = "Abstract Business Logic"
    def __init__(self, value):
        super(Abstract_Business_Logic, self).__init__("code",value)

class Priority(Attribute):
    """Priority Attribute"""
    tooltip = "Priority (1-99) of a ROS node or component operation "
    display = "Priority"
    def __init__(self, value):
        super(Priority, self).__init__("integer",value)

class Deadline(Attribute):
    """Deadline Attribute"""
    tooltip = "Deadline of a component operation expressed in seconds"
    display = "Deadline"
    def __init__(self, value):
        super(Deadline, self).__init__("double",value)

class Logging(Attribute):
    """Logging Attribute"""
    tooltip = "Levels of logging enforced in a component instance"
    display = "Logging Levels"
    options = ["DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"]
    def __init__(self, value):
        super(Logging, self).__init__("dictionary_bool", value)

class Period(Attribute):
    """Period Attribute"""
    tooltip = "Period of a Timer expressed in seconds"
    display = "Period"
    def __init__(self, value):
        super(Period, self).__init__("double",value)

class Hardware_Reference(Attribute):
    """Hardware Reference Attribute"""
    tooltip = "Reference to a predefined hardware model"
    display = "Hardware Reference"
    def __init__(self, value):
        super(Hardware_Reference, self).__init__("reference", value)

class Computer_Reference(Attribute):
    """Computer Reference Attribute"""
    tooltip = "Reference to a predefined computer"
    display = "Computer Reference"
    def __init__(self, value):
        super(Computer_Reference, self).__init__("reference", value)

class Component_Reference(Attribute):
    """Component Reference Attribute"""
    tooltip = "Reference to a predefined component"
    display = "Component Reference"
    def __init__(self, value):
        super(Component_Reference, self).__init__("reference", value)

class IP_Address(Attribute):
    """IP_Address Attribute"""
    tooltip = "IP Address of a computer"
    display = "IP Address"
    def __init__(self, value):
        super(IP_Address, self).__init__("string",value)

class Username(Attribute):
    """Username Attribute"""
    tooltip = "Name of the user who is deploying on a computer"
    display = "Name"
    def __init__(self, value):
        super(Username, self).__init__("string",value)

class SSH_Key(Attribute):
    """SSH_Key Attribute"""
    tooltip = "Absolute path to an SSH Key to access a computer"
    display = "SSH Key"
    def __init__(self, value):
        super(SSH_Key, self).__init__("string",value)

class Deployment_Path(Attribute):
    """Deployment_Path Attribute"""
    tooltip = "Absolute path on the target where deployment files are to be copied"
    display = "Target Deployment Path"
    def __init__(self, value):
        super(Deployment_Path, self).__init__("string",value)

class ROS_Install_Path(Attribute):
    """ROS_Install_Path Attribute"""
    tooltip = "Absolute path to ROS Install directory e.g. /opt/ros/indigo"
    display = "ROS Installation Path"
    def __init__(self, value):
        super(ROS_Install_Path, self).__init__("string",value)

class Arch(Attribute):
    """Architecture Attribute"""
    tooltip = "Architecture of the target system e.g. arm"
    display = "Target Architecture"
    def __init__(self, value):
        super(Arch, self).__init__("string",value)

class Cmd_Args(Attribute):
    """Cmd_Args Attribute"""
    tooltip = "Commandline arguments to a ROS node spawn"
    display = "Commandline Arguments"
    def __init__(self, value):
        super(Cmd_Args, self).__init__("string",value)

class Scheduling_Scheme(Attribute):
    """Scheduling_Scheme Attribute"""
    tooltip = "Scheduling scheme of the component-level scheduler"
    display = "Scheduling Scheme"
    options = ["FIFO", "PFIFO", "EDF"]
    def __init__(self, value):
        super(Scheduling_Scheme, self).__init__("list_entry",value)

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
        child_model.parent = self
        self.children.append(child_model)

    def add_attribute(self, name, kind, value):
        self[name] = Attribute(kind, value)

    def get_children(self, kind):
        return [child for child in self.children if child.kind == kind]

class Software(Model):
    """Software Class

    The Software class represents a ROS catkin workspace.
    This model contains:
    Messages -- Define a ROS Message
    Services -- Define a ROS Service
    Components -- Define ROSMOD component building blocks for applications
    """
    def __init__(self, name=Name(""), parent=None):
        super(Software, self).__init__()
        self.kind = "Software"

        assert name != None, "Software name is None!"

        self.parent = parent
        self["name"] = name

        self.children = Children(allowed=[Package()], 
                                 cardinality = {str(type(Package()))\
                                                : '1..*'})

class Package(Model):
    def __init__(self, name=Name(""), parent=None):
        super(Package, self).__init__()
        self.kind = "Package"

        assert name != None, "Package name is None!"

        self.parent = parent
        self["name"] = name

        self.children = Children(allowed=[Message(), 
                                          Service(), 
                                          Component()],
                                 cardinality={str(type(Message()))\
                                              : '0..*', 
                                              str(type(Service()))\
                                              : '0..*',
                                              str(type(Component()))\
                                              : '1..*'})

class Message(Model):
    def __init__(self, name=Name(""), definition=Message_Definition(""), 
                 parent=None):
        super(Message, self).__init__()
        self.kind = "Message"

        assert name != None, "Message name is None!"
        assert definition != None, "Message definition is None!"

        self.parent = parent
        self["name"] = name
        self["definition"] = definition

        self.children = Children(allowed=[], cardinality={})

class Service(Model):
    def __init__(self, name=Name(""), definition=Service_Definition(""), 
                 parent=None):
        super(Service, self).__init__()
        self.kind = "Service"

        assert name != None, "Service name is None!"
        assert definition != None, "Service definition is None!"

        self.parent = parent
        self["name"] = name
        self["definition"] = definition

        self.children = Children(allowed=[], cardinality={})

class Component(Model):
    def __init__(self, name=Name(""), component_type=Component_Type("BASE"), 
                 parent=None):
        super(Component, self).__init__()
        self.kind = "Component"

        assert name != None, "Component name is None!"
        assert component_type != None, "Component type is None!"

        self.parent = parent
        self["name"] = name
        self["type"] = component_type

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

class Client(Model):
    def __init__(self, name=Name(""), service_reference=Service_Reference(None), 
                 network_profile=Network_Profile(""), parent=None):
        super(Client, self).__init__()
        self.kind = "Client"

        assert name != None, "Client name is None!"
        assert service_reference != None, "Client service reference is None!"
        assert network_profile != None, "Client network profile is None!"

        self.parent = parent
        self["name"] = name
        self["service_reference"] = service_reference
        self["network_profile"] = network_profile

        self.children = Children(allowed=[], cardinality={})

class Server(Model):
    def __init__(self, name=Name(""), service_reference=Service_Reference(None), 
                 priority=Priority(0), deadline=Deadline(0.0), 
                 business_logic=Abstract_Business_Logic(""),
                 network_profile=Network_Profile(""), parent=None):
        super(Server, self).__init__()
        self.kind = "Server"

        assert name != None, "Server name is None!"
        assert service_reference != None, "Server service reference is None!"
        assert priority != None, "Server priority is None!"
        assert deadline != None, "Server deadline is None!"
        assert business_logic != None, "Server abstract business logic is None!"
        assert network_profile != None, "Server network profile is None!"

        self.parent = parent
        self["name"] = name
        self["service_reference"] = service_reference
        self["priority"] = priority
        self["deadline"] = deadline
        self["business_logic"] = business_logic
        self["network_profile"] = network_profile

        self.children = Children(allowed=[], cardinality={})

class Publisher(Model):
    def __init__(self, name=Name(""), message_reference=Message_Reference(None), 
                 network_profile=Network_Profile(""), parent=None):
        super(Publisher, self).__init__()
        self.kind = "Publisher"

        assert name != None, "Publisher name is None!"
        assert message_reference != None, "Publisher message reference is None!"
        assert network_profile != None, "Publisher network profile is None!"

        self.parent = parent
        self["name"] = name
        self["message_reference"] = message_reference
        self["network_profile"] = network_profile

        self.children = Children(allowed=[], cardinality={})

class Subscriber(Model):
    def __init__(self, name=Name(""), message_reference=Message_Reference(None),
                 priority=Priority(0), deadline=Deadline(0.0), 
                 business_logic=Abstract_Business_Logic(""),
                 network_profile=Network_Profile(""), parent=None):
        super(Subscriber, self).__init__()
        self.kind = "Subscriber"

        assert name != None,  "Subscriber name is None!"
        assert message_reference != None, "Subscriber message reference is None!"
        assert priority != None, "Subscriber priority is None!"
        assert deadline != None, "Subscriber deadline is None!"
        assert business_logic != None, "Subscriber abstract business logic is None!"
        assert network_profile != None, "Subscriber network profile is None!"

        self.parent = parent
        self["name"] = name
        self["message_reference"] = message_reference
        self["priority"] = priority
        self["deadline"] = deadline
        self["business_logic"] = business_logic
        self["network_profile"] = network_profile

        self.children = Children(allowed=[], cardinality={})

class Timer(Model):
    def __init__(self, name=Name(""), period=Period(0.0), priority=Priority(0), 
                 deadline=Deadline(0.0), business_logic=Abstract_Business_Logic(""), 
                 parent=None):
        super(Timer, self).__init__()
        self.kind = "Timer"

        assert name != None,  "Timer name is None!"
        assert period != None, "Timer period is None!"
        assert priority != None, "Timer priority is None!"
        assert deadline != None, "Timer deadline is None!"
        assert business_logic != None, "Timer abstract business logic is None!"

        self.parent = parent
        self["name"] = name
        self["period"] = period
        self["priority"] = priority
        self["deadline"] = deadline
        self["business_logic"] = business_logic

        self.children = Children(allowed=[], cardinality={})

class Hardware(Model):
    def __init__(self, name=Name(""), parent=None):
        super(Hardware, self).__init__()
        self.kind = "Hardware"

        assert name != None, "Hardware name is None!"

        self.parent = parent
        self["name"] = name

        self.children = Children(allowed=[Computer()], 
                                 cardinality={str(type(Computer()))\
                                              : '1..*'})

class Computer(Model):
    def __init__(self, name=Name(""), ip_address=IP_Address(""), 
                 username=Username(""), sshkey=SSH_Key(""), 
                 deployment_path=Deployment_Path(""), 
                 ros_install_path=ROS_Install_Path(""), arch=Arch(""), 
                 network_profile=Network_Profile(""), parent=None):
        super(Computer, self).__init__()
        self.kind = "Computer"

        assert name != None, "Computer name is None!"
        assert ip_address != None, "Computer IP address is None!"
        assert username != None, "Computer username is None!"
        assert sshkey != None, "Computer sshkey is None!"
        assert deployment_path != None, "Computer deployment path is None!"
        assert ros_install_path != None, "Computer ros install path is None!"
        assert arch != None, "Computer architecture is None!"
        assert network_profile != None, "Computer network profile is None!"

        self.parent = parent
        self["name"] = name
        self["ip_address"] = ip_address
        self["username"] = username
        self["sshkey"] = sshkey
        self["deployment_path"] = deployment_path
        self["ros_install_path"] = ros_install_path
        self["arch"] = arch
        self["network_profile"] = network_profile

        self.children = Children(allowed=[], cardinality={})

class Deployment(Model):
    def __init__(self, name=Name(""), hardware_reference=Hardware_Reference(None),
                 parent=None):
        super(Deployment, self).__init__()
        self.kind = "Deployment"

        assert name != None, "Deployment name is None!"
        assert hardware_reference != None, "Deployment hardware reference is None!"

        self.parent = parent
        self['name'] = name
        self["hardware_reference"] = hardware_reference

        self.children = Children(allowed=[Node()], 
                                 cardinality={str(type(Node())) : '1..*'})

class Node(Model):
    def __init__(self, name=Name(""), computer_reference=Computer_Reference(None), 
                 priority=Priority(0), cmd_args=Cmd_Args(""), 
                 deployment_path=Deployment_Path(""), parent=None):
        super(Node, self).__init__()
        self.kind = "Node"

        assert name != None, "Node name is None!"
        assert computer_reference != None, "Node computer reference is None!"
        assert priority != None, "Node priority is None!"
        assert cmd_args != None, "Node cmdline args is None!"
        assert deployment_path != None, "Node deployment path is None!"

        self.parent = parent
        self["name"] = name
        self['computer_reference'] = computer_reference

        self["priority"] = priority
        self["cmd_args"] = cmd_args
        self["deployment_path"] = deployment_path

        self.children = Children(allowed=[Component_Instance()], 
                                 cardinality=\
                                 {str(type(Component_Instance()))\
                                  : '1..*'})

class Component_Instance(Model):
    def __init__(self, name=Name(""), component_reference=Component_Reference(None), 
                 scheduling_scheme=Scheduling_Scheme("FIFO"), logging=Logging(None), 
                 parent=None):
        super(Component_Instance, self).__init__()
        self.kind = "Component_Instance"

        assert name != None, "Component instance name is None!"
        assert component_reference != None, "Component component reference is None!"
        assert scheduling_scheme != None, "Component scheduling scheme is None!"
        assert logging != None, "Component logging is None!"

        self.parent = parent
        self["name"] = name
        self["component_reference"] = component_reference
        self["scheduling_scheme"] = scheduling_scheme
        self["logging"] = logging

        self.children = Children(allowed=[], cardinality={})

class Project(Model):
    """Project Class
    
    The Project class is the root of a ROSMOD model.
    This object contains the following types of children:
    Software -- Describes the software workspace
    Hardware -- Describes the hardware configuration
    Deployment -- Maps software instances and hardware computers
    """
    def __init__(self, name=Name("NewProject"), path=Path(""), parent=None):
        super(Project, self).__init__()
        self.kind = "Project"

        assert name != None, "Project name is None!"
        assert path != None, "Project path is None!"

        self.parent = parent
        self['name'] = name
        self['path'] = path

        self.children = Children(allowed=[Software(), 
                                          Hardware(), 
                                          Deployment()], 
                                 cardinality={str(type(Software())) : '1',
                                              str(type(Hardware()))\
                                              : '1..*',
                                              str(type(Deployment()))\
                                              : '1..*'})

    def new(self, 
            name=Name("NewProject"), 
            path=Path(""),
            software=Software(Name("NewSoftware")),
            hardware=Hardware(Name("NewHardware")),
            deployment=Deployment(Name("NewDeployment")) ):

        assert name != None, "Project name is None!"
        assert path != None, "Project path is None!"        
        assert software != None, "Project Software Model is None!"
        assert hardware != None, "Project Hardware Model is None!"
        assert deployment != None, "Project Deployment Model is None!"

        assert name.value != "", "Project name is empty!"
        assert path.value != "", "Project path is empty!"

        self['name'] = name
        self['path'] = path

        self.add_child(software)
        self.add_child(hardware)
        self.add_child(deployment)

        project_dir = os.path.join(path.value, name.value)
        if not os.path.exists(project_dir):
            os.makedirs(project_dir)

    def save(self):
        jsonpickle.set_encoder_options('simplejson',indent=4)
        encoder_output = jsonpickle.encode(self)
        project_dir = os.path.join(self['path'].value, self['name'].value) 
        model = os.path.join(project_dir, self['name'].value + '.rml')
        with open(model, 'w') as model_file:
            model_file.write(encoder_output)

    def save_as(self, name=None, path=None):
        assert name != None and name.value != "", "Project name is None!"
        assert path != None and path.value != "", "Project path is None!"

        self['name'] = name
        self['path'] = path

        project_dir = os.path.join(path.value, name.value)
        if not os.path.exists(project_dir):
            os.makedirs(project_dir)
        self.save()

    def open(self, model=""):        
        assert model != "", "Project path is empty!"
        with open(model, 'r') as input_model:
            self = jsonpickle.decode(input_model.read())
        
def test_project():
    # Simple Timer Example
    my_software = Software(Name("software"))
    timer_package = Package(Name("timer_package"))
    timer_component = Component(Name("Timer_Component"), 
                                Component_Type("BASE"))
    timer = Timer(name = Name("periodic_timer"),
                  period = Period(1.0),
                  priority = Priority(50),
                  deadline = Deadline(0.01))

    my_hardware = Hardware(Name("hardware"))
    bbb_111 = Computer(name = Name("BBB_111"),
                       ip_address = IP_Address("10.1.1.1"),
                       username = Username("ubuntu"),
                       sshkey = SSH_Key("/home/jeb/.ssh/id_rsa_jetsontk1"),
                       deployment_path = Deployment_Path("/home/ubuntu"),
                       ros_install_path = ROS_Install_Path("/opt/ros/indigo"),
                       arch = Arch("arm"))

    my_deployment = Deployment(name = Name("deployment"), 
                               hardware_reference = Hardware_Reference(my_hardware))
    timer_node = Node(name = Name("Timer_Node"),
                      computer_reference = Computer_Reference(bbb_111),
                      priority = Priority(80))
    timer_component_i = Component_Instance(name = Name("Timer_Component_i"),
                                           component_reference = \
                                           Component_Reference(timer_component),
                                           scheduling_scheme =\
                                           Scheduling_Scheme("PFIFO"),
                                           logging = Logging({"DEBUG" : True,
                                                              "INFO" : False,
                                                              "WARNING" : False,
                                                              "ERROR" : False,
                                                              "CRITICAL" : False}))
    # Establish Tree
    timer_component.add_child(timer)
    timer_package.add_child(timer_component)
    my_software.add_child(timer_package)
    my_hardware.add_child(bbb_111)
    timer_node.add_child(timer_component_i)
    my_deployment.add_child(timer_node)

    project = Project()
    project.new(name=Name("timer_example"),
                path=Path("/home/jeb/pranav/rosmod/samples/"),
                software=my_software,
                hardware=my_hardware,
                deployment=my_deployment)
    return project

if __name__ == '__main__':

    project = test_project()
    project.save()
    project.open("/home/jeb/pranav/rosmod/samples/timer_example/timer_example.rml")

