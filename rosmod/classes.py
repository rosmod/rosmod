#!/usr/bin/env python

"""This file defines the ROSMOD metamodel."""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

import os
import jsonpickle
from collections import OrderedDict, MutableSequence
from string import Template

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

class Artifact(object):
    """Generic Artifact object
    
    Each Artifact has the following:
    kind -- the type of artifact e.g. code, build_system, analysis, file etc.
    value -- The value of the artifact e.g. "#include <...", "<?xml version" etc.
    name -- file/folder name of the artifact
    destination -- destination directory of the artifact
    source -- source directory of a "file/static" artifact if any e.g. node_main.cpp
    """
    tooltip = ''
    display = ''
    def __init__(self, kind=None, value="", name=None, destination=None, 
                 source=None, order=[]):
        assert kind != None, "Artifact kind is None!"
        assert isinstance(value, basestring) == True,\
            "Artifact value is not a string!"

        self.kind = kind
        self.value = value
        self.name = name
        self.destination = destination
        self.source = source
        self.order = order

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

    def __setitem__(self, key, val):
        self.value[key] = val

    def __getitem__(self, key):
        return self.value[key]

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
        super(Message_Definition, self).__init__("code",value)

class Service_Definition(Attribute):
    """Definition Attribute"""
    tooltip = "Describes a ROS service interface"
    display = "Service Definition"
    def __init__(self, value):
        super(Service_Definition, self).__init__("code",value)

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
        self.artifacts = []

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
        if self.kind == kind:
            return [self]
        else:
            kids = []
            for c in self.children:
                kids.extend(c.get_children(kind))
            return kids

    def get_artifact(self, name):
        for a in self.artifacts:
            if a.name == name:
                return a

    def update_artifacts(self):
        return

    def resolve_artifacts(self, artifact_list=None):
        if artifact_list == None:
            artifact_list = self.artifacts
        for artifact in artifact_list:
            if artifact.kind == "code":
                for o in artifact.order:
                    sub_artifact = self.get_artifact(o)
                    if sub_artifact != None:
                        artifact.value += sub_artifact.value
                    else:
                        for child in self.children:
                            child_artifact = child.get_artifact(o)
                            if child_artifact != None:
                                artifact.value += child_artifact.value

    def generate(self):
        self.update_artifacts()
        self.resolve_artifacts(self.artifacts)
        if self.artifacts != None:
            for artifact in self.artifacts:
                if artifact.kind == "folder":
                    folder = os.path.join(artifact.destination, artifact.name)
                    if not os.path.exists(folder):
                        os.makedirs(folder)

        for child in self.children:
            child.generate()

        if self.artifacts != None:
            for artifact in self.artifacts:
                if artifact.kind == "code":                    
                    print artifact.value
                    file_artifact = os.path.join(artifact.destination, artifact.name)
                    with open(file_artifact, 'w') as art_file:
                        art_file.write(artifact.value)

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

    def update_artifacts(self):
        src_dest = os.path.join(self.parent.get_artifact("workspace").destination, 
                               "workspace")
        src_dir = os.path.join(src_dest, "src")

        # workspace/src directory artifact
        self.artifacts = [Artifact(kind="folder", name="src", 
                                   destination=src_dest)]
        for child in self.children:
            # package directory artifact
            self.artifacts.append(Artifact(kind="folder", 
                                           name=child['name'].value,
                                           destination=src_dir))

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

    def update_artifacts(self):
        package_dir = os.path.join(self.parent.get_artifact(self["name"].value).\
                                    destination, self["name"].value)
        if self.get_children("Message") != []:
            # Message files directory
            self.artifacts.append(Artifact(kind="folder",
                                           name="msg",
                                           destination=package_dir))
        if self.get_children("Service") != []:
            # Service files directory
            self.artifacts.append(Artifact(kind="folder",
                                           name="srv",
                                           destination=package_dir))

        # Source directory
        self.artifacts.append(Artifact(kind="folder", 
                                        name="src", 
                                        destination=package_dir))

        # Include directory
        self.artifacts.append(Artifact(kind="folder", 
                                       name="include", 
                                       destination=package_dir))

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

    def update_artifacts(self):
        message_dir = os.path.join(self.parent.get_artifact("msg").destination,
                                   "msg")
        self.artifacts.append(Artifact(kind="file", 
                                       name=self['name'].value + ".msg",
                                       destination=message_dir, 
                                       value=self['definition'].value))

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

    def update_artifacts(self):
        service_dir = os.path.join(self.parent.get_artifact("srv").destination,
                                   "srv")
        self.artifacts.append(Artifact(kind="file", 
                                       name=self['name'].value + ".srv",
                                       destination=service_dir, 
                                       value=self['definition'].value))        

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


    def create_init_artifact(self):
        init_artifact = Artifact(kind="snippet",
                                 name="initialization")

        # Template for a init
        init_template = Template("$return_type $component::init($args) {\n" +\
                                 "$comment" +\
                                 "\n\n}" +\
                                 "\n\n")

        # Resolving the template 
        return_type = "void"
        component_name = self['name'].value
        callback_args = "const rosmod::TimerEvent& event"
        comment = "  // Business Logic for init_timer operation"
        init_artifact.value = init_template.substitute(return_type=return_type,
                                                       component=component_name,
                                                       args=callback_args,
                                                       comment=comment)
        self.artifacts.append(init_artifact)

    def create_destructor_artifact(self):
        destructor_artifact = Artifact(kind="snippet",
                                       name="destructor")
        destructor_template\
            = Template("$component::~$component() {\n" +\
                       "$comment" +\
                       "$timer_stop" +\
                       "$pub_shutdown" +\
                       "$sub_shutdown" +\
                       "$server_shutdown" +\
                       "$client_shutdown" +\
                       "}\n\n")

        # Resolving the template
        component_name = self['name'].value
        comment = "  // Clean up all timers and ports\n"
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
                                             client_shutdown=clients)

        self.artifacts.append(destructor_artifact)     
        
    def update_artifacts(self):
        """Update all artifact objects required for artifact generation"""
        self.create_directory_artifacts()
        self.create_init_artifact()
        self.create_destructor_artifact()

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

        self.artifacts.append(Artifact(kind="snippet",
                                       name="headers",
                                       value="#include \"" +\
                                       self.parent["name"].value +\
                                       '/' + self['name'].value + ".hpp\"\n\n"))

        for child in self.children:
            child.update_artifacts()                      

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

    def create_server_artifact(self):
        server_artifact = Artifact(kind="snippet", 
                                   name="server_callback")

        service = self['service_reference'].value
        service_name = service['name'].value
        package_name = service.parent['name'].value

        # Template for a server operation
        server_template\
            = Template('$return_type $component::$callback($args) {\n' +\
                       '$comment' +\
                       '\n\n}' +\
                       '\n\n')

        # Resolving the template 
        return_type = "bool"
        component_name = self.parent['name'].value
        callback_name = "{}_callback".format(self['name'].value)
        service_name = self['service_reference'].value['name'].value
        service_parent = self['service_reference'].value.parent['name'].value
        callback_args = "const {0}::{1}::Request& req,\n  {0}::{1}::Response &res"\
            .format(service_parent, service_name)
        comment = "  // Business Logic for {} operation".format(self['name'].value)
        server_artifact.value = server_template.\
                                substitute(return_type=return_type,
                                           component=component_name,
                                           callback=callback_name, 
                                           args=callback_args,
                                           comment=comment)
        
        self.artifacts.append(server_artifact)

    def update_artifacts(self):
        self.create_server_artifact()

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

    def create_subscriber_artifact(self):
        subscriber_artifact = Artifact(kind="snippet", 
                                       name="subscriber_callback")

        # Template for a subscriber operation
        subscriber_template\
            = Template('$return_type $component::$callback($args) {\n' +\
                       '$comment' +\
                       '\n\n}' +\
                       '\n\n')

        # Resolving the template 
        return_type = "void"
        component_name = self.parent['name'].value
        callback_name = "{}_callback".format(self['name'].value)
        message_name = self['message_reference'].value['name'].value
        message_parent = self['message_reference'].value.parent['name'].value
        callback_args = "const {}::{}::ConstPtr& received_data".format\
                        (message_parent, message_name)
        comment = "  // Business Logic for {} operation".format(self['name'].value)
        subscriber_artifact.value = subscriber_template.\
                                    substitute(return_type=return_type,
                                               component=component_name,
                                               callback=callback_name, 
                                               args=callback_args,
                                               comment=comment)

        self.artifacts.append(subscriber_artifact)

    def update_artifacts(self):
        self.create_subscriber_artifact()

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

    def create_timer_artifact(self):
        timer_artifact = Artifact(kind="snippet", 
                                  name="timer_callback")

        # Template for a timer operation
        timer_template\
            = Template('$return_type $component::$callback($args) {\n' +\
                       '$comment' +\
                       '\n\n}' +\
                       '\n\n')

        # Resolving the template 
        return_type = "void"
        component_name = self.parent['name'].value
        callback_name = "{}_callback".format(self['name'].value)
        callback_args = "const rosmod::TimerEvent& event"
        comment = "  // Business Logic for {} operation".format(self['name'].value)
        timer_artifact.value = timer_template.substitute(return_type=return_type,
                                                         component=component_name,
                                                         callback=callback_name, 
                                                         args=callback_args,
                                                         comment=comment)

        self.artifacts.append(timer_artifact)

    def update_artifacts(self):
        self.create_timer_artifact()

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

    def update_artifacts(self):
        self.artifacts = [Artifact(kind="folder", name="workspace", 
                                   destination=self['path'].value +\
                                   self['name'].value)]  
              
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

    def generate_artifacts(self):
        self.generate()

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
    message = Message(Name("msg1"),
                      Message_Definition("int8 test\nfloat64 soemthing"))
    service = Service(Name("srv1"),
                      Service_Definition("int8 test\n---\nbool retVal"))
    timer_component = Component(Name("Timer_Component"), 
                                Component_Type("BASE"))
    sender_component = Component(Name("Sender_Component"),
                                 Component_Type("BASE"))
    receiver_component = Component(Name("Receiver_Component"),
                                   Component_Type("BASE"))
    client = Client(Name("client"),
                    Service_Reference(service))
    server = Server(Name("server"),
                    Service_Reference(service))
    pub = Publisher(Name("pub"),
                    Message_Reference(message))
    sub = Subscriber(Name("sub"),
                     Service_Reference(message))
    timer = Timer(name = Name("periodic_timer"),
                  period = Period(1.0),
                  priority = Priority(50),
                  deadline = Deadline(0.01))
    timer2 = Timer(name = Name("sender_timer"),
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
    sender_component.add_child(timer2)
    sender_component.add_child(pub)
    sender_component.add_child(client)
    receiver_component.add_child(server)
    receiver_component.add_child(sub)
    timer_package.add_child(service)
    timer_package.add_child(message)
    timer_package.add_child(receiver_component)
    timer_package.add_child(sender_component)
    timer_package.add_child(timer_component)
    my_software.add_child(timer_package)
    my_hardware.add_child(bbb_111)
    timer_node.add_child(timer_component_i)
    my_deployment.add_child(timer_node)

    project = Project()
    project.new(name=Name("timer_example"),
                path=Path("/home/jeb/samples/"),
                software=my_software,
                hardware=my_hardware,
                deployment=my_deployment)
    return project

if __name__ == '__main__':

    project = test_project()
    project.save()
    project.open("/home/jeb/samples/timer_example/timer_example.rml")
    project.generate_artifacts()

    comp = project.get_children("Component")
#    for c in comp:
#        for a in c.artifacts:
#            if a.kind == "code":
#                print a.value
