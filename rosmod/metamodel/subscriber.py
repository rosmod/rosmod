#!/usr/bin/env python

"""This file defines the ROSMOD Suscriber Model"""

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
from attributes import Name, Message_Reference, Priority, Deadline,\
    Abstract_Business_Logic, Network_Profile

class Subscriber(Model):
    def __init__(self, name=Name(""), message_reference=Message_Reference(None),
                 priority=Priority(0), deadline=Deadline(0.0), 
                 business_logic=Abstract_Business_Logic(""),
                 network_profile=Network_Profile(""), parent=None):
        super(Subscriber, self).__init__()
        self.kind = "Subscriber"

        assert name != None,  "Subscriber name is None!"
        assert message_reference != None, "Subscriber message reference is None!"
        assert priority != None, "Subscriber priority is None!"
        assert deadline != None, "Subscriber deadline is None!"
        assert business_logic != None, "Subscriber abstract business logic is None!"
        assert network_profile != None, "Subscriber network profile is None!"

        self.parent = parent
        self["name"] = name
        self["message_reference"] = message_reference
        self["priority"] = priority
        self["deadline"] = deadline
        self["business_logic"] = business_logic
        self["network_profile"] = network_profile

        self.children = Children(allowed=[], cardinality={})

    def create_subscriber_callback_decl_artifact(self):
        sub_comment =  "  /**\n   * @brief Subscriber Operation - {}\n"\
            .format(self['name'].value)
        sub_comment += "   * @param received_data Received message\n"
        sub_comment += "   */\n"
        sub_callback_decl = sub_comment
        signature = Template('  $return_type $component::$callback($args);\n\n')
        return_type = "void"
        component_name = self.parent['name'].value
        callback_name = "{}_callback".format(self['name'].value)
        message_name = self['message_reference'].value['name'].value
        message_parent = self['message_reference'].value.parent['name'].value
        callback_args = "const {}::{}::ConstPtr& received_data".format\
                        (message_parent, message_name)
        sub_callback_decl += signature.substitute(return_type=return_type,
                                                  component=component_name,
                                                  callback=callback_name,
                                                  args=callback_args)
        sub_callback_decl_artifact\
            = Artifact(kind="snippet",
                       name="subscriber_callback_declaration",
                       value=sub_callback_decl)
        self.artifacts.append(sub_callback_decl_artifact)

    def create_sub_decl_artifact(self):
        sub_decl = "  /**\n   @brief Component subscriber port - {}\n"\
                   .format(self['name'].value)
        sub_decl += "   */\n"
        sub_decl += "  rosmod::Subscriber {};\n\n".format(self['name'].value)
        sub_decl_artifact = Artifact(kind="snippet",
                                       name="subscriber_declaration",
                                       value=sub_decl)
        self.artifacts.append(sub_decl_artifact)

    def create_subscriber_artifact(self):
        sub_comment =  "/**\n * @brief Subscriber Operation - {}\n"\
            .format(self['name'].value)
        sub_comment += " * @param received_data Received message\n"
        sub_comment += " */\n"
        subscriber_artifact = Artifact(kind="snippet", 
                                       name="subscriber_callback")

        # Template for a subscriber operation
        subscriber_template\
            = Template('$comment' +\
                       '$return_type $component::$callback($args) {\n' +\
                       '$bl_start_marker\n' +\
                       '$preserved_bl' +\
                       '$bl_end_marker\n}\n\n')

        # Resolving the template 
        comment = sub_comment
        return_type = "void"
        component_name = self.parent['name'].value
        callback_name = "{}_callback".format(self['name'].value)
        message_name = self['message_reference'].value['name'].value
        message_parent = self['message_reference'].value.parent['name'].value
        callback_args = "const {}::{}::ConstPtr& received_data".format\
                        (message_parent, message_name)
        bl_start_marker = "  //# Start {} business logic".format(self['name'].value)
        bl_end_marker = "  //# End {} business logic".format(self['name'].value)

        sub_key = "{}_operation".format(self['name'].value)
        if sub_key in self.parent.kwargs.keys():
            preserved_bl = self.parent.kwargs[sub_key]
        else:
            preserved_bl = ""

        subscriber_artifact.value = subscriber_template.\
                                    substitute(comment=comment,
                                               return_type=return_type,
                                               component=component_name,
                                               callback=callback_name, 
                                               args=callback_args,
                                               bl_start_marker=bl_start_marker,
                                               preserved_bl=preserved_bl,
                                               bl_end_marker=bl_end_marker)

        self.artifacts.append(subscriber_artifact)

    def update_artifacts(self, **kwargs):
        self.create_subscriber_artifact()
        self.create_subscriber_callback_decl_artifact()
        self.create_sub_decl_artifact()
