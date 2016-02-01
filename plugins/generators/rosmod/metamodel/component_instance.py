#!/usr/bin/env python

"""This file defines the ROSMOD Component Instance Model"""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

from base import Model, Children
from attributes import Name, Component_Reference, Scheduling_Scheme, Logging

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
