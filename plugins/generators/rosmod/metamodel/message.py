#!/usr/bin/env python

"""This file defines the ROSMOD Message Model"""

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
from attributes import Name, Message_Definition

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

    def update_artifacts(self, **kwargs):
        message_dir = os.path.join(self.parent.get_artifact("msg").destination,
                                   "msg")
        self.artifacts.append(Artifact(kind="file", 
                                       name=self['name'].value + ".msg",
                                       destination=message_dir, 
                                       value=self['definition'].value))

        kwargs["{}_msg_dir".format(self['name'].value)] = message_dir
        self.kwargs = kwargs
