# Listener
# Accumulate workspace objects while 
# walking the parse tree
#
# Author: Pranav Srinivas Kumar
# Date: 2014.12.16

'''
* create dictionaries for each type of drawing:
** child of each object
*** need to have: text location, color, connectFrom
** type of each object
* create dictionaries for each context menu?

'''


from ROSListener import ROSListener
from collections import OrderedDict
from Tkinter import * 
import tkFileDialog
import tkMessageBox

class PaddingOptions():
    def __init__(self, childOffset = (5,10), childPadding=(0,15)):
        self.childOffset = childOffset # (x,y) tuple
        self.childPadding = childPadding # (x,y) tuple

class FontOptions():
    def __init__(self, height=10, width=8, fg="black",bg=""):
        self.height = height
        self.width = width
        self.fg = fg
        self.bg = bg

class DrawOptions():
    def __init__(self, color, minSize, textPosition="TOP", image=None, drawChildren=True, connectFrom=False):
        self.color = color
        self.minSize = minSize
        self.image = image
        self.textPosition = textPosition
        self.drawChildren = drawChildren
        self.connectFrom = connectFrom        

class CanvasOptions():
    def __init__(self, paddingOptions, fontOptions, drawOptions, tags=()):
        self.paddingOptions = paddingOptions
        self.fontOptions = fontOptions
        self.drawOptions = drawOptions
        self.tags = tags

    def copy(self,canvasOptions):
        self.paddingOptions = canvasOptions.paddingOptions
        self.fontOptions = canvasOptions.fontOptions
        self.drawOptions = canvasOptions.drawOptions
        self.tags = canvasOptions.tags

# Anything that can be drawn on a Tkinter canvas should inherit from this class 
class CanvasObject(CanvasOptions):
    def __init__(self, name = "", isObjRef = False, objRef = None):
        self.maxChildNameLen = 0
        self.connectionPoint = [-1,-1]
        self.size = [0,0]
        self.children = OrderedDict()
        self.isObjRef = isObjRef
        self.objRef = objRef
        self.name = name

    def setCanvasParams(self, canvas, position, canvasOptions):
        CanvasOptions.copy(self,canvasOptions)
        # these need to be provided
        self.position = position
        self.canvas = canvas
        self.tags += (self.name,self)
        
    def buildChildList(self):
        pass

    def addChild(self, name, child):
        if len(name) > self.maxChildNameLen:
            self.maxChildNameLen = len(name)
        numChildren = len(self.children)
        offsetX = self.paddingOptions.childOffset[0] + self.paddingOptions.childOffset[0]
        offsetY = numChildren * self.paddingOptions.childPadding[1] + self.paddingOptions.childOffset[1]
        child.position = (self.position[0] + offsetX, self.position[1] + offsetY)
        self.children[name] = child

    def Draw(self):
        self.size[0] = self.drawOptions.minSize[0]
        self.size[1] = self.drawOptions.minSize[1]
        if self.drawOptions.drawChildren == True and len(self.children) > 0:
            self.size[0] += self.maxChildNameLen * self.fontOptions.width + self.paddingOptions.childOffset[0] * 2
            self.size[1] += len(self.children) * self.paddingOptions.childPadding[1] + self.paddingOptions.childOffset[1]
        self.connectionPoint = [
            self.position[0] + self.size[0],
            self.position[1] + self.size[1]/2
        ]
        # need to take into account image here!
        objectID = self.canvas.create_rectangle(
            self.position[0], self.position[1], 
            self.position[0] + self.size[0], self.position[1] + self.size[1], 
            outline=self.drawOptions.color, fill=self.drawOptions.color, tags=self.tags,
            activeoutline="black",
            activewidth=3.0
        )
        # need to take into account text position here!
        if self.drawOptions.textPosition == "TOP":
            anchor = CENTER
            textPos = (self.position[0] + self.size[0]/2, self.position[1] - self.fontOptions.height)
        elif self.drawOptions.textPosition == "BOTTOM":
            anchor = CENTER
            textPos = (self.position[0] + self.size[0]/2, self.position[1] + self.size[1] + self.fontOptions.height)
        elif self.drawOptions.textPosition == "RIGHT":
            anchor = W
            textPos = (self.connectionPoint[0] + 3, self.connectionPoint[1])
        elif self.drawOptions.textPosition == "LEFT":
            anchor = E
            textPos = (self.position[0] - 3, self.connectionPoint[1])

        textID = self.canvas.create_text(
            textPos,
            text=self.name,
            state=DISABLED,
            tags = self.tags,
            anchor = anchor
        )
        if self.drawOptions.connectFrom == True and self.objRef.connectionPoint[0] > 0:
            self.canvas.create_line(
                self.position[0],self.position[1]+self.size[1]/2,
                self.objRef.connectionPoint[0],self.objRef.connectionPoint[1],
                arrow=FIRST
            )
        if self.drawOptions.drawChildren == True:
            for name,child in self.children.iteritems():
                child.Draw()
        return self.size

# ROS_Workspace class
class ROS_Workspace:

    def __init__(self):
        # Name of ROS workspace
        self.name = ""
        # List of ROS packages in workspace
        self.packages = []
        # Useful Dictionaries
        self.packages_dict = OrderedDict()

    def __str__(self):
        workspace = "workspace " + self.name + ";"
        if len(self.packages) > 0:
            workspace += ""
            for package in self.packages:
                workspace += "\npackage " + package.name
                workspace += "\n{"
                workspace += str(package)
                workspace += "\n}"
        return workspace

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
        self.msg_dict = OrderedDict()
        self.srv_dict = OrderedDict()
        self.component_definition_dict = OrderedDict()
        self.nodes_dict = OrderedDict()

    def __str__(self):
        package = ""
        if len(self.messages) > 0:
            package += "\n    messages"
            package += "\n    {"
            for msg in self.messages:
                package += str(msg)
            package += "\n    }"
        if len(self.services) > 0:
            package += "\n    services"
            package += "\n    {"
            for srv in self.services:
                package += str(srv)
            package += "\n    }"
        if len(self.components) > 0:
            package += "\n    components"
            package += "\n    {"
            for component in self.components:
                package += str(component)
            package += "\n    }"
        if len(self.nodes) > 0:
            package += "\n    nodes"
            package += "\n    {"
            for node in self.nodes:
                package += str(node)
            package += "\n    }"

        return package
        

# ROS Message class
class ROS_Message(CanvasObject):

    def __init__(self):
        CanvasObject.__init__(self)
        # Name of ROS Message
        self.name = ""
        # List of msg fields
        self.fields = []

    def __str__(self):
        msg = "\n        msg " + self.name
        msg += "\n        {"
        for field in self.fields:
            if len(field) > 2:
                msg += "\n            " + field[0] + " " + field[1] + "=" + field[2] + ";"
            else:
                msg += "\n            " + field[0] + " " + field[1] + ";"
        msg += "\n        }"
        return msg

# ROS Service class
class ROS_Service(CanvasObject):

    def __init__(self):
        CanvasObject.__init__(self)
        # Name of ROS Service
        self.name = ""
        # List of request fields
        self.request_fields = []
        # List of response fields
        self.response_fields = []
        
    def __str__(self):
        srv_str = "\n        srv " + self.name
        srv_str += "\n        {"
        if len(self.request_fields) > 0:
            srv_str += "\n            request"
            srv_str += "\n            {"
            for field in self.request_fields:
                if len(field) > 2:
                    srv_str += "\n                " 
                    srv_str += field[0] + " " + field[1] + "=" + field[2] + ";"
                else:
                    srv_str += "\n                " 
                    srv_str += field[0] + " " + field[1] + ";"
            srv_str += "\n            }"
        if len(self.response_fields) > 0:
            srv_str += "\n            response"
            srv_str += "\n            {"
            for field in self.response_fields:
                if len(field) > 2:
                    srv_str += "\n                " 
                    srv_str += field[0] + " " + field[1] + "=" + field[2] + ";"
                else:
                    srv_str += "\n                " 
                    srv_str += field[0] + " " + field[1] + ";"
            srv_str += "\n             }"
        srv_str += "\n        }"
        return srv_str

# ROS Component class
class ROS_Component(CanvasObject):
    
    def __init__(self):
        CanvasObject.__init__(self)
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
        self.provided_services_dict = OrderedDict()
        self.required_services_dict = OrderedDict()
        self.publishers_dict = OrderedDict()
        self.subscribers_dict = OrderedDict()
        self.timers_dict = OrderedDict()
        self.dicts = [
            self.provided_services_dict,
            self.required_services_dict,
            self.publishers_dict,
            self.subscribers_dict,
            self.timers_dict
        ]

    def buildChildList(self):
        for childDict in self.dicts:
            for name, child in childDict.iteritems():
                childObj = CanvasObject( name=name, isObjRef=True, objRef=child)
                childObj.setCanvasParams(
                    canvas = self.canvas,
                    position = self.position,
                    canvasOptions = CanvasOptions(
                        self.paddingOptions,
                        self.fontOptions,
                        self.drawOptions,
                        ('object','component',name)
                    )
                )
                childObj.drawOptions = DrawOptions("black",(10,10),"RIGHT",connectFrom=True)
                self.addChild(name,childObj)

    def __str__(self):
        comp = "\n        component " + self.name
        comp += "\n        {"
        if len(self.provided_services) > 0:
            for provided in self.provided_services:
                comp += "\n            provides " + provided + ";"
        if len(self.required_services) > 0:
            for required in self.required_services:
                comp += "\n            requires " + required + ";"
        if len(self.publishers) > 0:
            for publisher in self.publishers:
                comp += "\n            publisher<" + publisher.topic + "> " + publisher.name + ";" 
        if len(self.subscribers) > 0:
            for subscriber in self.subscribers:
                comp += "\n            subscriber<" + subscriber.topic + "> " + subscriber.name + ";"
        if len(self.timers) > 0:
            for timer in self.timers:
                comp += "\n            timer " + timer.name
                comp += "\n            {"
                comp += "\n                period = " + timer.period + timer.period_unit + ";";
                comp += "\n            }"
        comp += "\n        }"
        return comp

# ROS Publisher
class ROS_Publisher(CanvasObject):
    
    def __init__(self):
        CanvasObject.__init__(self)
        # Name of publisher port
        self.name = ""
        # Name of msg topic
        self.topic = ""

# ROS Subscriber
class ROS_Subscriber(CanvasObject):
    
    def __init__(self):
        CanvasObject.__init__(self)
        # Name of subscriber port
        self.name = ""
        # Name of msg topic
        self.topic = ""

# ROS Timer
class ROS_Timer(CanvasObject):
    
    def __init__(self):
        CanvasObject.__init__(self)
        # Name of timer
        self.name = ""
        # Period of timer
        self.period = "0.0"
        # Unit of timer period
        self.period_unit = ""

# ROS Node
class ROS_Node(CanvasObject):
    
    def __init__(self):
        CanvasObject.__init__(self)
        # Name of the ROS node
        self.name = ""
        # List of components
        self.components = []
        # List of component defs
        self.comp_defs = []
        # Useful Dictionary
        self.component_instance_dict = OrderedDict()

    def __str__(self):
        node = "\n        node " + self.name
        node += "\n        {"
        for instance in self.components:
            node += "\n            component<" + instance[0] + "> " + instance[1] + ";"
        node += "\n        }"
        return node

    def buildChildList(self):
        for name, compInst in self.component_instance_dict.iteritems():
            childObj = CanvasObject( name=name, isObjRef=True, objRef=compInst)
            childObj.setCanvasParams(
                canvas = self.canvas,
                position = self.position,
                canvasOptions = CanvasOptions(
                    self.paddingOptions,
                    self.fontOptions,
                    self.drawOptions,
                    ('object','component',name)
                )
            )
            childObj.drawOptions = DrawOptions("black",(10,10),"RIGHT",connectFrom=True)
            self.addChild(name,childObj)

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
                    self.component.provided_services_dict[server_name] = self.package.srv_dict[service_name]
        elif "requires" in ctx.getText():
            for child in ctx.getChildren():
                context = str(type(child))
                if "Service_nameContext" in context:
                    service_name = child.getText()
                    # CHECK: If this service has been defined 
                    self.component.required_services.append(service_name)
                    client_name = service_name + "_client"
                    self.component.required_services_dict[client_name] = self.package.srv_dict[service_name]

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
                    self.component.publishers_dict[self.publisher.name] = self.package.msg_dict[self.publisher.topic]
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
                    self.component.subscribers_dict[self.subscriber.name] = self.package.msg_dict[self.subscriber.topic]

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
