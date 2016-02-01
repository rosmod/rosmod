#!/usr/bin/env python

"""This file defines the ROSMOD Timer Model"""

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
from attributes import Name, Period, Priority, Deadline,\
    Abstract_Business_Logic

class Timer(Model):
    def __init__(self, name=Name(""), period=Period(0.0), priority=Priority(0), 
                 deadline=Deadline(0.0), business_logic=Abstract_Business_Logic(""), 
                 parent=None):
        super(Timer, self).__init__()
        self.kind = "Timer"

        assert name != None,  "Timer name is None!"
        assert period != None, "Timer period is None!"
        assert priority != None, "Timer priority is None!"
        assert deadline != None, "Timer deadline is None!"
        assert business_logic != None, "Timer abstract business logic is None!"

        self.parent = parent
        self["name"] = name
        self["period"] = period
        self["priority"] = priority
        self["deadline"] = deadline
        self["business_logic"] = business_logic

        self.children = Children(allowed=[], cardinality={})

    def create_timer_callback_decl_artifact(self):
        timer_comment =  "  /**\n   * @brief Timer Operation - {}\n"\
            .format(self['name'].value)
        timer_comment += "   * @param event Time-triggered event\n"
        timer_comment += "   */\n"
        timer_callback_decl = timer_comment
        timer_callback_decl += "  void {0}_callback({1});\n\n"\
                               .format(self['name'].value,
                                       "const rosmod::TimerEvent& event")
        timer_callback_decl_artifact = Artifact(kind="snippet",
                                                name="timer_callback_declaration",
                                                value=timer_callback_decl)
        self.artifacts.append(timer_callback_decl_artifact)

    def create_timer_decl_artifact(self):
        timer_decl = "  /**\n   * @brief Component timer - {}\n"\
            .format(self['name'].value)
        timer_decl += "   */\n"
        timer_decl += "  rosmod::Timer {};\n\n".format(self['name'].value)
        timer_decl_artifact = Artifact(kind="snippet",
                                       name="timer_declaration",
                                       value=timer_decl)
        self.artifacts.append(timer_decl_artifact)

    def create_timer_artifact(self):
        timer_comment =  "/**\n * @brief Timer Operation - {}\n"\
            .format(self['name'].value)
        timer_comment += " * @param event Time-triggered event\n"
        timer_comment += " */\n"
        timer_artifact = Artifact(kind="snippet", 
                                  name="timer_callback")

        # Template for a timer operation
        timer_template\
            = Template('$comment' +\
                       '$return_type $component::$callback($args) {\n' +\
                       '$bl_start_marker\n' +\
                       '$preserved_bl'
                       '$bl_end_marker\n}\n\n')

        # Resolving the template 
        comment = timer_comment
        return_type = "void"
        component_name = self.parent['name'].value
        callback_name = "{}_callback".format(self['name'].value)
        callback_args = "const rosmod::TimerEvent& event"
        bl_start_marker = "  //# Start {} business logic".format(self['name'].value)
        bl_end_marker = "  //# End {} business logic".format(self['name'].value)

        timer_key = "{}_operation".format(self['name'].value)
        if timer_key in self.parent.kwargs.keys():
            preserved_bl = self.parent.kwargs[timer_key]
        else:
            preserved_bl = ""

        timer_artifact.value\
            = timer_template.substitute(comment=comment,
                                        return_type=return_type,
                                        component=component_name,
                                        callback=callback_name, 
                                        args=callback_args,
                                        bl_start_marker=bl_start_marker,
                                        preserved_bl=preserved_bl,
                                        bl_end_marker=bl_end_marker)
        self.artifacts.append(timer_artifact)

    def update_artifacts(self, **kwargs):
        self.create_timer_artifact()
        self.create_timer_callback_decl_artifact()
        self.create_timer_decl_artifact()
