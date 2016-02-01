#!/usr/bin/env python

"""This file defines the ROSMOD Deployment Model"""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

from base import Model, Children
from attributes import Name, Hardware_Reference
from node import Node

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
