#!/usr/bin/env python

"""This file defines the ROSMOD metamodel attributes"""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

from base import Attribute

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

class Authors(Attribute):
    """Authors Attribute"""
    tooltip = "Authors refers to the project manager(s)"
    display = "Authors"
    def __init__(self, value):
        super(Authors, self).__init__("mutable_list_string",value)

class Detailed_Description(Attribute):
    """Detailed_Description Attribute"""
    tooltip = "A detailed description of a model"
    display = "Detailed Description"
    def __init__(self, value):
        super(Detailed_Description, self).__init__("string",value)

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
