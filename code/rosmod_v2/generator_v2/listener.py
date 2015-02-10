# Listener - Version 2
# Author: Pranav Srinivas Kumar
# Date: 2015.02.06

from ROSListener import ROSListener
from collections import OrderedDict

import sys
import os

from classes import *

class Listener(ROSListener):
    def __init__(self):
        self.workspace = ROS_Workspace()

    # Create a new workspace object
    def enterDefine_workspace(self, ctx):
        self.workspace = ROS_Workspace()
        
    # Save the workspace name
    def enterWorkspace_name(self, ctx):
        self.workspace.properties["name"] = ctx.getText()

    # Create a new ROS Package object
    def enterRos_packages(self, ctx):
        self.package = ROS_Package()
        self.package.parent = self.workspace
        
    # On exit, add the new package to list of packages in workspace
    def exitRos_packages(self, ctx):
        self.workspace.add(self.package)

    # Save the package name
    def enterPackage_name(self, ctx):
        self.package.properties["name"] = ctx.getText()

    # Create a new ROS Message object
    def enterRos_msg(self, ctx):
        self.message = ROS_Message()

    # Save the message name
    def enterMsg_name(self, ctx):
        self.message.properties["name"] = ctx.getText()

    # Save the fields in the ROS msg
    def enterMsg_field(self,ctx):
        field_type = ""
        field_name = ""
        field_value = ""
        for child in ctx.getChildren():
            context = str(type(child))
            # Find the message field type
            if "Msg_field_typeContext" in context:
                field_type = child.getText()
            # Find the message field name
            if "Msg_field_nameContext" in context:
                field_name = child.getText()
            # Find the message field value
            if "Msg_field_valueContext" in context:
                field_value = child.getText()
        if field_type != "":
            if field_name != "":
                if field_value != "":
                    self.message.properties["fields"].append([field_type, field_name, field_value])
                else:
                    self.message.properties["fields"].append([field_type, field_name])

    # On exit, append message to list of messages in package
    def exitRos_msg(self, ctx):
        self.package.add(self.message)

    # Create a new ROS Service object
    def enterRos_srv(self, ctx):
        self.service = ROS_Service()

    # Save the name of the ROS Service
    def enterSrv_name(self, ctx):
        self.service.properties["name"] = ctx.getText()

    # Save the request arguments of the service
    def enterReq_argument(self, ctx):
        field_type = ""
        field_name = ""
        field_value = ""
        for child in ctx.getChildren():
            context = str(type(child))
            # Find request field type
            if "Req_field_typeContext" in context:
                field_type = child.getText()
            # Find request field name
            if "Req_field_nameContext" in context:
                field_name = child.getText()
            # Find request field value
            if "Req_field_valueContext" in context:
                field_value = child.getText()
        if field_type != "":
            if field_name != "":
                if field_value != "":
                    self.service.properties["request"].append([field_type, field_name, field_value])
                else:
                    self.service.properties.["request"].append([field_type, field_name])

    # Save the response arguments of the service
    def enterRes_argument(self, ctx):
        field_type = ""
        field_name = ""
        field_value = ""
        for child in ctx.getChildren():
            context = str(type(child))
            # Find request field type
            if "Res_field_typeContext" in context:
                field_type = child.getText()
            # Find request field name
            if "Res_field_nameContext" in context:
                field_name = child.getText()
            # Find request field value
            if "Res_field_valueContext" in context:
                field_value = child.getText()
        if field_type != "":
            if field_name != "":
                if field_value != "":
                    self.service.properties.["response"].append([field_type, field_name, field_value])
                else:
                    self.service.properties.["response"].append([field_type, field_name])

    # On exit, add the service to the list of services in the package
    def exitRos_srv(self, ctx):
        self.package.add(self.service)
        
    # Create a new component object
    def enterRos_component(self, ctx):
        self.component = ROS_Component()

    # Save the name of the component
    def enterComponent_name(self, ctx):
        self.component.properties["name"] = ctx.getText()

    # Save all provided and required services
    def enterRos_client_server(self, ctx):
        if "provides" in ctx.getText():
            for child in ctx.getChildren():
                context = str(type(child))
                if "Service_nameContext" in context:
                    service_name = child.getText()
                    # CHECK: If this service has been defined 
                    self.server = ROS_Server()
                    self.server.properties["name"] = service_name + "_server"
                    for child in self.packages.children:
                        if child.properties["name"] == service_name:
                            self.server.properties["service_reference"] = child
                    self.component.add(self.server)
        elif "requires" in ctx.getText():
            for child in ctx.getChildren():
                context = str(type(child))
                if "Service_nameContext" in context:
                    service_name = child.getText()
                    # CHECK: If this service has been defined 
                    self.client = ROS_Client()
                    self.client.properties["name"] = service_name + "_client"
                    for child in self.packages.children:
                        if child.properties["name"] == service_name:
                            self.server.properties["service_reference"] = child
                    self.component.add(self.client)

    # Save all publishers and susbcribers
    def enterRos_pub_sub(self, ctx):
        if "publisher" in ctx.getText():
            self.publisher = ROS_Publisher()
            for child in ctx.getChildren():
                context = str(type(child))
                if "TopicContext" in context:
                    # CHECK: If this toic has been defined
                    for child in self.package.children:
                        if child.properties["name"] == child.getText():
                            self.publisher.properties["message_reference"] = child
                if "PublisherContext" in context:
                    self.publisher.properties["name"] = child.getText()
            if self.publisher.name != "":
                if self.publisher.topic != None:
                    self.component.add(self.publisher)

        elif "subscriber" in ctx.getText():
            self.subscriber = ROS_Subscriber()
            for child in ctx.getChildren():
                context = str(type(child))
                if "TopicContext" in context:
                    for child in self.package.children:
                        if child.properties["name"] == child.getText():
                            self.subscriber.properties["message_reference"] = child
                if "SubscriberContext" in context:
                    self.subscriber.properties["name"] = child.getText()
            if self.subscriber.name != "":
                if self.subscriber.topic != None:
                    self.component.add(self.subscriber)

    # Save all component timers
    def enterRos_timer(self, ctx):
        for child in ctx.getChildren():
            context = str(type(child))
            if "Timer_nameContext" in context:
                name = child.getText()
            elif "Timer_periodContext" in context:
                period = child.getText()
            elif "Period_unitContext" in context:
                period_unit = child.getText()
        self.timer = ROS_Timer()
        self.timer.properties["name"] = name
        self.timer.properties["period"] = period
        self.timer.properties["unit"] = period_unit
        self.component.add(self.timer)

    # On exit, add component to list of components in package
    def exitRos_component(self, ctx):
        self.package.children.append(self.component)

    # Create a new ROS node object
    def enterRos_node(self, ctx):
        self.node = ROS_Node()

    # Save ROS node name
    def enterNode_name(self, ctx):
        self.node.properties["name"] = ctx.getText()

    # Save the component instances in each node
    def enterComponent_instances(self, ctx):
        comp_type = ""
        instance = ""
        for child in ctx.getChildren():
            context = str(type(child))
            if "Component_typeContext" in context:
                comp_type = child.getText()

            elif "Component_instanceContext" in context:
                instance = child.getText()
        self.component_instance = ROS_Component_Instance()
        self.component_instance.properties["name"] = instance
        for child in self.package.children:
            if child.properties["name"] == comp_type:
                self.component_instance.properties["component_reference"] = child 
        self.node.add(self.component_instance)

    # On exit, add the node to the list of nodes in package
    def exitRos_node(self, ctx):
        self.package.children.append(self.node)
