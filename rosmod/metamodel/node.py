#!/usr/bin/env python

"""This file defines the ROSMOD Node Model"""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

from base import Model, Children
from attributes import Name, Computer_Reference, Priority, Cmd_Args,\
    Deployment_Path
from component_instance import Component_Instance

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
