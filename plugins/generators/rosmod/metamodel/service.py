#!/usr/bin/env python

"""This file defines the ROSMOD Service Model"""

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
from attributes import Name, Service_Definition

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

    def update_artifacts(self, **kwargs):
        service_dir = os.path.join(self.parent.get_artifact("srv").destination,
                                   "srv")
        self.artifacts.append(Artifact(kind="file", 
                                       name=self['name'].value + ".srv",
                                       destination=service_dir, 
                                       value=self['definition'].value))

        kwargs["{}_srv_dir".format(self['name'].value)] = service_dir        
        self.kwargs = kwargs
