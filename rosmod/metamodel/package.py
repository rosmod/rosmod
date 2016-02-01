#!/usr/bin/env python

"""This file defines the ROSMOD Package Model"""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

import os
from base import Model, Artifact, Children
from attributes import Name
from message import Message
from service import Service
from component import Component

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

    def update_artifacts(self, **kwargs):
        package_dir = os.path.join(self.parent.get_artifact(self["name"].value).\
                                    destination, self["name"].value)

        kwargs["{}_pkg_dir".format(self['name'].value)] = package_dir

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

        self.kwargs = kwargs
