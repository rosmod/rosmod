#!/usr/bin/env python

"""This file defines the ROSMOD Client Model"""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

from base import Model, Artifact, Children
from attributes import Name, Service_Reference, Network_Profile

class Client(Model):
    def __init__(self, name=Name(""), service_reference=Service_Reference(None), 
                 network_profile=Network_Profile(""), parent=None):
        super(Client, self).__init__()
        self.kind = "Client"

        assert name != None, "Client name is None!"
        assert service_reference != None, "Client service reference is None!"
        assert network_profile != None, "Client network profile is None!"

        self.parent = parent
        self["name"] = name
        self["service_reference"] = service_reference
        self["network_profile"] = network_profile

        self.children = Children(allowed=[], cardinality={})

    def create_client_decl_artifact(self):
        client_decl = "  /**\n   * @brief Component client port\n"\
                      .format(self['name'].value)
        client_decl += "   */\n"
        client_decl += "  rosmod::Client {};\n\n".format(self['name'].value)
        client_decl_artifact = Artifact(kind="snippet",
                                        name="client_declaration",
                                        value=client_decl)
        self.artifacts.append(client_decl_artifact)

    def update_artifacts(self, **kwargs):
        self.create_client_decl_artifact()
