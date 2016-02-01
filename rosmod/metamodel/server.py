#!/usr/bin/env python

"""This file defines the ROSMOD Server Model"""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

from string import Template
from base import Model, Artifact, Children
from attributes import Name, Service_Reference, Priority, Deadline,\
    Abstract_Business_Logic, Network_Profile

class Server(Model):
    def __init__(self, name=Name(""), service_reference=Service_Reference(None), 
                 priority=Priority(0), deadline=Deadline(0.0), 
                 business_logic=Abstract_Business_Logic(""),
                 network_profile=Network_Profile(""), parent=None):
        super(Server, self).__init__()
        self.kind = "Server"

        assert name != None, "Server name is None!"
        assert service_reference != None, "Server service reference is None!"
        assert priority != None, "Server priority is None!"
        assert deadline != None, "Server deadline is None!"
        assert business_logic != None, "Server abstract business logic is None!"
        assert network_profile != None, "Server network profile is None!"

        self.parent = parent
        self["name"] = name
        self["service_reference"] = service_reference
        self["priority"] = priority
        self["deadline"] = deadline
        self["business_logic"] = business_logic
        self["network_profile"] = network_profile

        self.children = Children(allowed=[], cardinality={})

    def create_server_callback_decl_artifact(self):
        server_comment =  "  /**\n   * @brief Server Operation - {}\n"\
            .format(self['name'].value)
        server_comment += "   * @param req Received service request\n"
        server_comment += "   * @param res Returned service response\n"
        server_comment += "   * @return Success/Failure of server operation\n"
        server_comment += "   */\n"
        srv_callback_decl = server_comment
        signature = Template('  $return_type $component::$callback($args);\n\n')
        return_type = "bool"
        component_name = self.parent['name'].value
        callback_name = "{}_callback".format(self['name'].value)
        service_name = self['service_reference'].value['name'].value
        service_parent = self['service_reference'].value.parent['name'].value
        callback_args\
            = "const {0}::{1}::Request& req,\n    {0}::{1}::Response &res"\
                .format(service_parent, service_name)
        srv_callback_decl += signature.substitute(return_type=return_type,
                                                  component=component_name,
                                                  callback=callback_name,
                                                  args=callback_args)
        srv_callback_decl_artifact\
            = Artifact(kind="snippet",
                       name="server_callback_declaration",
                       value=srv_callback_decl)
        self.artifacts.append(srv_callback_decl_artifact)

    def create_server_decl_artifact(self):
        server_decl = "  /**\n   * @brief Component server port - {}\n"\
                      .format(self['name'].value)
        server_decl += "   */\n"
        server_decl += "  rosmod::Server {};\n\n".format(self['name'].value)
        server_decl_artifact = Artifact(kind="snippet",
                                        name="server_declaration",
                                        value=server_decl)
        self.artifacts.append(server_decl_artifact)

    def create_server_artifact(self):
        server_artifact = Artifact(kind="snippet", 
                                   name="server_callback")

        service = self['service_reference'].value
        service_name = service['name'].value
        package_name = service.parent['name'].value

        # Template for a server operation
        server_template\
            = Template('$comment' +\
                       '$return_type $component::$callback($args) {\n' +\
                       '$bl_start_marker\n' +\
                       '$preserved_bl' +\
                       '$bl_end_marker\n}' +\
                       '\n\n')

        # Resolving the template
        server_comment =  "/**\n * @brief Server Operation - {}\n"\
            .format(self['name'].value)
        server_comment += " * @param req Received service request\n"
        server_comment += " * @param res Returned service response\n"
        server_comment += " * @return Success/Failure of server operation\n"
        server_comment += " */\n"

        comment = server_comment
        return_type = "bool"
        component_name = self.parent['name'].value
        callback_name = "{}_callback".format(self['name'].value)
        service_name = self['service_reference'].value['name'].value
        service_parent = self['service_reference'].value.parent['name'].value
        callback_args = "const {0}::{1}::Request& req,\n  {0}::{1}::Response &res"\
            .format(service_parent, service_name)
        bl_start_marker = "  //# Start {} business logic".format(self['name'].value)
        bl_end_marker = "  //# End {} business logic".format(self['name'].value)

        srv_key = "{}_operation".format(self['name'].value)
        if srv_key in self.parent.kwargs.keys():
            preserved_bl = self.parent.kwargs[srv_key]
        else:
            preserved_bl = ""
        server_artifact.value = server_template.\
                                substitute(comment=comment,
                                           return_type=return_type,
                                           component=component_name,
                                           callback=callback_name, 
                                           args=callback_args,
                                           bl_start_marker=bl_start_marker,
                                           preserved_bl=preserved_bl,
                                           bl_end_marker=bl_end_marker)
        
        self.artifacts.append(server_artifact)

    def update_artifacts(self, **kwargs):
        self.create_server_artifact()
        self.create_server_callback_decl_artifact()
        self.create_server_decl_artifact()

