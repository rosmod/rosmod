# Listener
# Accumulate workspace objects while 
# walking the parse tree
#
# Author: Pranav Srinivas Kumar
# Date: 2014.12.16

from ROSListener import ROSListener

# ROS_Workspace class
class ROS_Workspace:

    def __init__(self):
        # Name of ROS workspace
        self.name = ""
        # List of ROS packages in workspace
        self.packages = []
        # Useful Dictionaries
        self.packages_dict = {}

# ROS_Package class
class ROS_Package:

    def __init__(self):
        # Name of ROS package
        self.name = ""
        # List of messages in package
        self.messages = []
        # List of services in package
        self.services = []
        # List of components in package
        self.components = []
        # List of nodes in package
        self.nodes = []
        # Useful Dictionaries
        self.msg_dict = {}
        self.srv_dict = {}
        self.component_definition_dict = {}
        self.nodes_dict = {}

# ROS Message class
class ROS_Message:

    def __init__(self):
        # Name of ROS Message
        self.name = ""
        # List of msg fields
        self.fields = []

# ROS Service class
class ROS_Service:

    def __init__(self):
        # Name of ROS Service
        self.name = ""
        # List of request fields
        self.request_fields = []
        # List of response fields
        self.response_fields = []

# ROS Component class
class ROS_Component:
    
    def __init__(self):
        # Name of the ROS component
        self.name = ""
        # List of provided services
        self.provided_services = []
        # List of required services
        self.required_services = []
        # List of publisher ports
        self.publishers = []
        # List of subscriber ports
        self.subscribers = []
        # List of timers
        self.timers = []
        # Useful Dictionaries
        self.provided_services_dict = {}
        self.required_services_dict = {}
        self.publishers_dict = {}
        self.subscribers_dict = {}
        self.timers_dict = {}

# ROS Publisher
class ROS_Publisher:
    
    def __init__(self):
        # Name of publisher port
        self.name = ""
        # Name of msg topic
        self.topic = ""

# ROS Subscriber
class ROS_Subscriber:
    
    def __init__(self):
        # Name of subscriber port
        self.name = ""
        # Name of msg topic
        self.topic = ""

# ROS Timer
class ROS_Timer:
    
    def __init__(self):
        # Name of timer
        self.name = ""
        # Period of timer
        self.period = "0.0"
        # Unit of timer period
        self.period_unit = ""

# ROS Node
class ROS_Node:
    
    def __init__(self):
        # Name of the ROS node
        self.name = ""
        # List of components
        self.components = []
        # List of component defs
        self.comp_defs = []
        # Useful Dictionary
        self.component_instance_dict = {}

class Listener(ROSListener):

    # Create a new workspace object
    def enterDefine_workspace(self, ctx):
        self.workspace = ROS_Workspace()

    # Save the workspace name
    def enterWorkspace_name(self, ctx):
        self.workspace.name = ctx.getText()

    # Create a new ROS package object
    def enterRos_packages(self, ctx):
        self.package = ROS_Package()

    # On exit, add the new package to list of packages in workspace
    def exitRos_packages(self, ctx):
        self.workspace.packages.append(self.package)
        self.workspace.packages_dict[self.package.name] = self.package

    # Save the package name
    def enterPackage_name(self, ctx):
        self.package.name = ctx.getText()

    # Create a new ROS message object
    def enterRos_msg(self, ctx):
        self.message = ROS_Message()

    # Save the message name
    def enterMsg_name(self, ctx):
        self.message.name = ctx.getText()

    # Save the fields in the ROS msg
    def enterMsg_field(self,ctx):
        field_type = ""
        field_name = ""
        for child in ctx.getChildren():
            context = str(type(child))
            # Find the message field type
            if "Msg_field_typeContext" in context:
                field_type = child.getText()
            # Find the message field name
            if "Msg_field_nameContext" in context:
                field_name = child.getText()
        if field_type != "":
            if field_name != "":
                self.message.fields.append([field_type, field_name])

    # On exit, append message to list of messages in package
    def exitRos_msg(self, ctx):
        # Append new ros msg onto list of package messages
        self.package.messages.append(self.message)
        self.package.msg_dict[self.message.name] = self.message

    # Create a new ROS Service object
    def enterRos_srv(self, ctx):
        self.service = ROS_Service()

    # Save the name of the ROS Service
    def enterSrv_name(self, ctx):
        self.service.name = ctx.getText()

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
                    self.service.request_fields.append([field_type, field_name, field_value])
                else:
                    self.service.request_fields.append([field_type, field_name])

                
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
                    self.service.response_fields.append([field_type, field_name, field_value])
                else:
                    self.service.response_fields.append([field_type, field_name])

    # On exit, add the service to the list of services in the package
    def exitRos_srv(self, ctx):
        self.package.services.append(self.service)
        self.package.srv_dict[self.service.name] = self.service
        
    # Create a new component object
    def enterRos_component(self, ctx):
        self.component = ROS_Component()

    # Save the name of the component
    def enterComponent_name(self, ctx):
        self.component.name = ctx.getText()

    # Save all provided and required services
    def enterRos_service(self, ctx):
        if "provides" in ctx.getText():
            for child in ctx.getChildren():
                context = str(type(child))
                if "Service_nameContext" in context:
                    service_name = child.getText()
                    # CHECK: If this service has been defined 
                    self.component.provided_services.append(service_name)
                    server_name = service_name + "_server"
                    self.component.provided_services_dict[server_name] = service_name
        elif "requires" in ctx.getText():
            for child in ctx.getChildren():
                context = str(type(child))
                if "Service_nameContext" in context:
                    service_name = child.getText()
                    # CHECK: If this service has been defined 
                    self.component.required_services.append(service_name)
                    client_name = service_name + "_client"
                    self.component.required_services_dict[client_name] = service_name

    # Save all publishers and susbcribers
    def enterRos_pub_sub(self, ctx):
        if "publisher" in ctx.getText():
            self.publisher = ROS_Publisher()
            for child in ctx.getChildren():
                context = str(type(child))
                if "TopicContext" in context:
                    self.publisher.topic = child.getText()
                    # CHECK: If this toic has been defined
                if "PublisherContext" in context:
                    self.publisher.name = child.getText()
            if self.publisher.name != "":
                if self.publisher.topic != "":
                    self.component.publishers.append(self.publisher)
                    self.component.publishers_dict[self.publisher.name] = self.publisher
        elif "subscriber" in ctx.getText():
            self.subscriber = ROS_Subscriber()
            for child in ctx.getChildren():
                context = str(type(child))
                if "TopicContext" in context:
                    self.subscriber.topic =  child.getText()
                    # CHECK: If this topic has been defined
                if "SubscriberContext" in context:
                    self.subscriber.name = child.getText()
            if self.subscriber.name != "":
                if self.subscriber.topic != "":
                    self.component.subscribers.append(self.subscriber)
                    self.component.subscribers_dict[self.subscriber.name] = self.subscriber

    # Save all component timers
    def enterRos_timer(self, ctx):
        self.timer = ROS_Timer()
        for child in ctx.getChildren():
            context = str(type(child))
            if "Timer_nameContext" in context:
                self.timer.name = child.getText()
            elif "Timer_periodContext" in context:
                self.timer.period = child.getText()
            elif "Period_unitContext" in context:
                self.timer.period_unit = child.getText()
        self.component.timers.append(self.timer)
        self.component.timers_dict[self.timer.name] = self.timer

    # On exit, add component to list of components in package
    def exitRos_component(self, ctx):
        self.package.components.append(self.component)
        self.package.component_definition_dict[self.component.name] = self.component

    # Create a new ROS node object
    def enterRos_node(self, ctx):
        self.node = ROS_Node()

    # Save ROS node name
    def enterNode_name(self, ctx):
        self.node.name = ctx.getText()

    # Save the component instances in each node
    def enterComponent_instances(self, ctx):
        comp_type = ""
        instance = ""
        for child in ctx.getChildren():
            context = str(type(child))
            if "Component_typeContext" in context:
                # CHECK: If this component has been defined
                comp_type = child.getText()

            elif "Component_instanceContext" in context:
                instance = child.getText()
        self.node.components.append([comp_type, instance])
        self.node.comp_defs.append(comp_type)
        self.node.component_instance_dict[instance] = self.package.component_definition_dict[comp_type]

    # On exit, add the node to the list of nodes in package
    def exitRos_node(self, ctx):
        self.package.nodes.append(self.node)
        self.package.nodes_dict[self.node.name] = self.node
