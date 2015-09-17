#!/usr/bin/env python

"""This file defines the ROSMOD Software Model"""

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
from package import Package

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

    def update_artifacts(self, **kwargs):
        src_dest = os.path.join(self.parent.get_artifact("workspace").destination, 
                               "workspace")
        src_dir = os.path.join(src_dest, "src")

        kwargs["workspace_src_dir"] = src_dir

        # workspace/src directory artifact
        self.artifacts = [Artifact(kind="folder", name="src", 
                                   destination=src_dest)]
        for child in self.children:
            # package directory artifact
            self.artifacts.append(Artifact(kind="folder", 
                                           name=child['name'].value,
                                           destination=src_dir))

        self.kwargs = kwargs
