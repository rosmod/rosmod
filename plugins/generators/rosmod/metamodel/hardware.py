#!/usr/bin/env python

"""This file defines the ROSMOD Hardware Model"""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

from base import Model, Children
from attributes import Name
from computer import Computer

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
