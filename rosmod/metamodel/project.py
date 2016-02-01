#!/usr/bin/env python

"""This file defines the ROSMOD Project Model"""

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
from base import Model, Artifact, Children
from attributes import Name, Path, Authors
from software import Software
from hardware import Hardware
from deployment import Deployment

class Project(Model):
    """Project Class
    
    The Project class is the root of a ROSMOD model.
    This object contains the following types of children:
    Software -- Describes the software workspace
    Hardware -- Describes the hardware configuration
    Deployment -- Maps software instances and hardware computers
    """
    def __init__(self, name=Name("NewProject"), path=Path(""), 
                 authors=Authors([]), parent=None):
        super(Project, self).__init__()
        self.kind = "Project"

        assert name != None, "Project name is None!"
        assert path != None, "Project path is None!"
        assert authors != None, "Project authors is None!"

        self.parent = parent
        self['name'] = name
        self['path'] = path
        self['authors'] = authors

        self.children = Children(allowed=[Software(), 
                                          Hardware(), 
                                          Deployment()], 
                                 cardinality={str(type(Software())) : '1',
                                              str(type(Hardware()))\
                                              : '1..*',
                                              str(type(Deployment()))\
                                              : '1..*'})

    def update_artifacts(self, **kwargs):
        self.artifacts = [Artifact(kind="folder", name="workspace", 
                                   destination=self['path'].value +\
                                   self['name'].value)]  

