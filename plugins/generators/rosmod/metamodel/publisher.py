#!/usr/bin/env python

"""This file defines the ROSMOD Publisher Model"""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

from base import Model, Artifact, Children
from attributes import Name, Message_Reference, Network_Profile

class Publisher(Model):
    def __init__(self, name=Name(""), message_reference=Message_Reference(None), 
                 network_profile=Network_Profile(""), parent=None):
        super(Publisher, self).__init__()
        self.kind = "Publisher"

        assert name != None, "Publisher name is None!"
        assert message_reference != None, "Publisher message reference is None!"
        assert network_profile != None, "Publisher network profile is None!"

        self.parent = parent
        self["name"] = name
        self["message_reference"] = message_reference
        self["network_profile"] = network_profile

        self.children = Children(allowed=[], cardinality={})

    def create_pub_decl_artifact(self):
        pub_decl = "  /**\n   * @brief Component Publisher port - {}\n"\
                   .format(self['name'].value)
        pub_decl += "   */\n"
        pub_decl += "  rosmod::Publisher {};\n\n".format(self['name'].value)
        pub_decl_artifact = Artifact(kind="snippet",
                                       name="publisher_declaration",
                                       value=pub_decl)
        self.artifacts.append(pub_decl_artifact)

    def update_artifacts(self, **kwargs):
        self.create_pub_decl_artifact()
