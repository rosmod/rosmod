# Listener
# Accumulate workspace objects while 
# walking the parse tree
#
# Author: Pranav Srinivas Kumar
# Date: 2014.12.16

'''
* create dictionaries for each context menu?
'''


from ROSListener import ROSListener
from collections import OrderedDict

import sys,os
sys.path.append('../../python_gui/')
from CanvasOptions import *
from rosmod_dialogs import *

# ROS_Workspace class
class ROS_Workspace(CanvasObject):

    def __init__(self):
        # Name of ROS workspace
        self.name = ""
        # List of ROS packages in workspace
        self.packages = []
        # Useful Dictionaries
        self.packages_dict = OrderedDict()

    def Edit(self):
        d = EditorDialogPopup(
            parent=self.canvas,
            title="Edit Workspace",
            initValsList = [["Name:",self.name,nameStringChars]]
        )
        if d.result != None:
            self.name = d.result[0]
            return 1
        return 0

    def addPackage(self,package):
        self.deletePackage(package.name)
        self.packages.append(package)
        self.packages_dict[package.name] = package
    def deletePackage(self,name):
        if name in self.packages_dict:
            del self.packages_dict[name]
        self.packages[:] = [pack for pack in self.packages if pack.name != name]

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
class ROS_Package(CanvasObject):

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
        # Code Preservation Variables
        self.cmakelists_packages = ""
        self.cmakelists_add_cpp = ""
        # Useful Dictionaries
        self.msg_dict = OrderedDict()
        self.srv_dict = OrderedDict()
        self.component_definition_dict = OrderedDict()
        self.nodes_dict = OrderedDict()

    def Edit(self):
        d = EditorDialogPopup(
            parent=self.canvas,
            title="Edit Package",
            initValsList = [["Name:",self.name,nameStringChars]]
        )
        if d.result != None:
            self.workspace.deletePackage(self.name)
            self.name = d.result[0]
            self.workspace.addPackage(self)
            return 1
        return 0

    def addMessage(self,message):
        self.deleteMessage(message.name)
        self.messages.append(message)
        self.msg_dict[message.name] = message
    def deleteMessage(self,name):
        if name in self.msg_dict:
            del self.msg_dict[name]
        self.messages[:] = [msg for msg in self.messages if msg.name != name]

    def addService(self,service):
        self.deleteService(service.name)
        self.services.append(service)
        self.srv_dict[service.name] = service
    def deleteService(self,name):
        if name in self.srv_dict:
            del self.srv_dict[name]
        self.services[:] = [srv for srv in self.services if srv.name != name]

    def addComponent(self,component):
        self.deleteComponent(component.name)
        self.components.append(component)
        self.component_definition_dict[component.name] = component
    def deleteComponent(self,name):
        if name in self.component_definition_dict:
            del self.component_definition_dict[name]
        self.components[:] = [comp for comp in self.components if comp.name != name]

    def addNode(self,node):
        self.deleteNode(node.name)
        self.nodes.append(node)
        self.nodes_dict[node.name] = node
    def deleteNode(self,name):
        if name in self.nodes_dict:
            del self.nodes_dict[name]
        self.nodes[:] = [node for node in self.nodes if node.name != name]

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

    def Delete(self):
        for comp in self.package.components:
            comp.deleteMessage(self.name)
        self.package.deleteMessage(self.name)

    def Edit(self):
        d = TypeDialogPopup(
            parent=self.canvas,
            title="Edit Message",
            initValsList = [
                ["Name:",self.name,nameStringChars],
                ["Fields:",self.fields]
            ]
        )
        if d.result != None:
            self.package.deleteMessage(self.name)
            self.name = d.result[0]
            self.package.addMessage(self)
            self.fields = d.result[1]
            return 1
        return 0

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

class ROS_Server(CanvasObject):
    
    def __init__(self):
        CanvasObject.__init__(self)
        # Name change is not saved currently, not supported by grammar
        self.name = ""
        # reference to the actual service object
        self.service = None
        self.parentComp = None

    def Delete(self):
        self.parentComp.deleteServer(self.name)

    def Edit(self):
        options = OrderedDict()
        options = self.package.srv_dict
        if self.service == None:
            self.service = options[options.keys()[0]]
        d = ReferenceDialogPopup(
            parent=self.canvas,
            title="Edit Server",
            initValsList = [
                ["Name:",self.name,nameStringChars],
                ["Service:",self.service.name,options]
            ]
        )
        if d.result != None:
            self.parentComp.deleteServer(self.name)
            self.name = d.result[0]
            self.parentComp.addServer(self)
            self.service = d.result[1]
            return 1
        return 0

class ROS_Client(CanvasObject):
    
    def __init__(self):
        CanvasObject.__init__(self)
        # Name change is not saved currently, not supported by grammar
        self.name = ""
        # reference to the actual service object
        self.service = None
        self.parentComp = None

    def Delete(self):
        self.parentComp.deleteClient(self.name)

    def Edit(self):
        options = OrderedDict()
        options = self.package.srv_dict
        if self.service == None:
            self.service = options[options.keys()[0]]
        d = ReferenceDialogPopup(
            parent=self.canvas,
            title="Edit Client",
            initValsList = [
                ["Name:",self.name,nameStringChars],
                ["Service:",self.service.name,options]
            ]
        )
        if d.result != None:
            self.parentComp.deleteClient(self.name)
            self.name = d.result[0]
            self.parentComp.addClient(self)
            self.service = d.result[1]
            return 1
        return 0

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
        # Business Logic
        self.business_logic = ""

    def Delete(self):
        for comp in self.package.components:
            comp.deleteService(self.name)
        self.package.deleteService(self.name)

    def Edit(self):
        d = TypeDialogPopup(
            parent=self.canvas,
            title="Edit Service",
            initValsList = [
                ["Name:",self.name,nameStringChars],
                ["Requests:",self.request_fields],
                ["Responses:",self.response_fields]
            ]
        )
        if d.result != None:
            self.package.deleteService(self.name)
            self.name = d.result[0]
            self.package.addService(self)
            self.request_fields = d.result[1]
            self.response_fields = d.result[2]
            return 1
        return 0
        
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
            srv_str += "\n            }"
        srv_str += "\n        }"
        return srv_str

# ROS Component class
class ROS_Component(CanvasObject):
    
    def __init__(self,isObjRef=False):
        CanvasObject.__init__(self,isObjRef=isObjRef)
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
        # Init Business Logic
        self.init_business_logic = ""
        self.user_includes = ""
        self.user_private_variables = ""
        # type of component, i.e. its definition
        self.comp_type = None
        self.parentNode = None

    def Delete(self):
        if self.isObjRef == True:
            # is an instance in a node
            if self.parentNode != None:
                self.parentNode.deleteComponent(self.name)
        else:
            # is a definition
            for node in self.package.nodes:
                node.deleteCompDef(self)
            self.package.deleteComponent(self.name)

    def Edit(self):
        if self.isObjRef == True:
            options = self.package.component_definition_dict
            if self.comp_type == None:
                self.comp_type = options[options.keys()[0]]
            d = ReferenceDialogPopup(
                parent=self.canvas,
                title="Edit Component Instance",
                initValsList=[
                    ["Name:",self.name,nameStringChars],
                    ["Definition:",self.comp_type.name,options]
                ]
            )
            if d.result != None:
                self.parentNode.deleteComponent(self.name)
                self.name = d.result[0]
                self.parentNode.addComponent(self)
                self.comp_type = d.result[1]
                return 1
        else:
            d = EditorDialogPopup(
                parent=self.canvas,
                title="Edit Component Definition",
                initValsList = [["Name:",self.name,nameStringChars]]
            )
            if d.result != None:
                self.package.deleteComponent(self.name)
                self.name = d.result[0]
                self.package.addComponent(self)
                return 1
        return 0

    def deleteService(self,name):
        self.required_services[:] = [srv for srv in self.required_services if srv.service.name != name]
        self.provided_services[:] = [srv for srv in self.provided_services if srv.service.name != name]

    def deleteMessage(self,name):
        self.publishers[:] = [msg for msg in self.publishers if msg.topic.name != name]
        self.subscribers[:] = [msg for msg in self.subscribers if msg.topic.name != name]

    def addTimer(self,timer):
        self.deleteTimer(timer.name)
        self.timers.append(timer)
    def deleteTimer(self,name):
        self.timers[:] = [tmr for tmr in self.timers if tmr.name != name]

    def addServer(self,server):
        self.deleteServer(server.name)
        self.provided_services.append(server)
    def deleteServer(self,name):
        self.provided_services[:] = [srv for srv in self.provided_services if srv.name != name]

    def addClient(self,client):
        self.deleteClient(client.name)
        self.required_services.append(client)
    def deleteClient(self,name):
        self.required_services[:] = [srv for srv in self.required_services if srv.name != name]

    def addPublisher(self,pub):
        self.deletePublisher(pub.name)
        self.publishers.append(pub)
    def deletePublisher(self,name):
        self.publishers[:] = [msg for msg in self.publishers if msg.name != name]

    def addSubscriber(self,sub):
        self.deleteSubscriber(sub.name)
        self.subscribers.append(sub)
    def deleteSubscriber(self,name):
        self.subscribers[:] = [msg for msg in self.subscribers if msg.name != name]

    def buildChildList(self):
        self.cleanChildren()
        for obj in self.subscribers:
            objType = 'subscriber'
            obj.isObjRef = True
            obj.objRef = obj.topic
            obj.setCanvasParams(
                canvas = self.canvas,
                position = self.position,
                canvasOptions = canvasOptionsDict[objType]
            )
            objDrawOptions = canvasOptionsDict[objType].drawOptions
            obj.drawOptions = DrawOptions(
                color = objDrawOptions.color,
                minSize = objDrawOptions.minSize,
                textPosition = "RIGHT", connectFrom = True
            )
            self.addChild(obj.name,obj)
        for obj in self.publishers:
            objType = 'publisher'
            obj.isObjRef = True
            obj.objRef = obj.topic
            obj.setCanvasParams(
                canvas = self.canvas,
                position = self.position,
                canvasOptions = canvasOptionsDict[objType]
            )
            objDrawOptions = canvasOptionsDict[objType].drawOptions
            obj.drawOptions = DrawOptions(
                color = objDrawOptions.color,
                minSize = objDrawOptions.minSize,
                textPosition = "RIGHT", connectFrom = True
            )
            self.addChild(obj.name,obj)
        for obj in self.timers:
            objType = 'timer'
            obj.setCanvasParams(
                canvas = self.canvas,
                position = self.position,
                canvasOptions = canvasOptionsDict[objType]
            )
            objDrawOptions = canvasOptionsDict[objType].drawOptions
            obj.drawOptions = DrawOptions(
                color = objDrawOptions.color,
                minSize = objDrawOptions.minSize,
                textPosition = "RIGHT", connectFrom = True
            )
            self.addChild(obj.name,obj)
        for obj in self.provided_services:
            objType = 'server'
            obj.isObjRef = True
            obj.objRef = obj.service
            obj.setCanvasParams(
                canvas = self.canvas,
                position = self.position,
                canvasOptions = canvasOptionsDict[objType]
            )
            objDrawOptions = canvasOptionsDict[objType].drawOptions
            obj.drawOptions = DrawOptions(
                color = objDrawOptions.color,
                minSize = objDrawOptions.minSize,
                textPosition = "RIGHT", connectFrom = True
            )
            self.addChild(obj.name,obj)
        for obj in self.required_services:
            objType = 'client'
            obj.isObjRef = True
            obj.objRef = obj.service
            obj.setCanvasParams(
                canvas = self.canvas,
                position = self.position,
                canvasOptions = canvasOptionsDict[objType]
            )
            objDrawOptions = canvasOptionsDict[objType].drawOptions
            obj.drawOptions = DrawOptions(
                color = objDrawOptions.color,
                minSize = objDrawOptions.minSize,
                textPosition = "RIGHT", connectFrom = True
            )
            self.addChild(obj.name,obj)

    def __str__(self):
        comp = "\n        component " + self.name
        comp += "\n        {"
        if len(self.provided_services) > 0:
            for provided in self.provided_services:
                comp += "\n            provides " + provided.service.name + ";"
        if len(self.required_services) > 0:
            for required in self.required_services:
                comp += "\n            requires " + required.service.name + ";"
        if len(self.publishers) > 0:
            for publisher in self.publishers:
                comp += "\n            publisher<" + publisher.topic.name + "> " + publisher.name + ";" 
        if len(self.subscribers) > 0:
            for subscriber in self.subscribers:
                comp += "\n            subscriber<" + subscriber.topic.name + "> " + subscriber.name + ";"
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
        self.topic = None
        self.parentComp = None

    def Delete(self):
        self.parentComp.deletePublisher(self.name)

    def Edit(self):
        options = OrderedDict()
        options = self.package.msg_dict
        if self.topic == None:
            self.topic = options[options.keys()[0]]
        d = ReferenceDialogPopup(
            parent=self.canvas,
            title="Edit Publisher",
            initValsList = [
                ["Name:",self.name,nameStringChars],
                ["Topic:",self.topic.name,options]
            ]
        )
        if d.result != None:
            self.parentComp.deletePublisher(self.name)
            self.name = d.result[0]
            self.parentComp.addPublisher(self)
            self.topic = d.result[1]
            return 1
        return 0

# ROS Subscriber
class ROS_Subscriber(CanvasObject):
    
    def __init__(self):
        CanvasObject.__init__(self)
        # Name of subscriber port
        self.name = ""
        # Name of msg topic
        self.topic = None
        self.business_logic = ""
        self.parentComp = None

    def Delete(self):
        self.parentComp.deleteSubscriber(self.name)

    def Edit(self):
        options = OrderedDict()
        options = self.package.msg_dict
        if self.topic == None:
            self.topic = options[options.keys()[0]]
        d = ReferenceDialogPopup(
            parent=self.canvas,
            title="Edit Subscriber",
            initValsList = [
                ["Name:",self.name,nameStringChars],
                ["Topic:",self.topic.name,options]
            ]
        )
        if d.result != None:
            self.parentComp.deleteSubscriber(self.name)
            self.name = d.result[0]
            self.parentComp.addSubscriber(self)
            self.topic = d.result[1]
            return 1
        return 0

# ROS Timer
class ROS_Timer(CanvasObject):
    
    def __init__(self,name="",period="0.0",period_unit="s"):
        CanvasObject.__init__(self)
        # Name of timer
        self.name = name
        # Period of timer
        self.period = period
        # Unit of timer period
        self.period_unit = period_unit
        self.business_logic = ""
        self.parentComp = None

    def Delete(self):
        self.parentComp.deleteTimer(self.name)

    def Edit(self):
        d = EditorDialogPopup(
            parent=self.canvas,
            title="Edit Timer",
            initValsList=[
                ["Name:",self.name,nameStringChars],
                ["Period:",self.period,digitStringChars],
                ["Units:",self.period_unit,unitStringChars]
            ]
        )
        if d.result != None:
            self.parentComp.deleteTimer(self.name)
            self.name = d.result[0]
            self.parentComp.addTimer(self)
            self.period = d.result[1]
            self.period_unit = d.result[2]
            return 1
        return 0

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

    def Edit(self):
        d = EditorDialogPopup(
            parent=self.canvas,
            title="Edit Node",
            initValsList=[
                ["Name:",self.name,nameStringChars]
            ]
        )
        if d.result != None:
            self.package.deleteNode(self.name)
            self.name = d.result[0]
            self.package.addNode(self)
            return 1
        return 0

    def Delete(self):
        self.package.deleteNode(self.name)

    def addComponent(self,component):
        self.deleteComponent(component.name)
        self.components.append(component)
    def deleteComponent(self,name):
        self.components[:] = [comp for comp in self.components if comp.name != name]

    def deleteCompDef(self,compDef):
        self.components[:] = [comp for comp in self.components if comp.comp_type != compDef]
        self.comp_defs[:] = [comp for comp in self.comp_defs if comp != compDef.name]

    def __str__(self):
        node = "\n        node " + self.name
        node += "\n        {"
        for instance in self.components:
            node += "\n            component<" + instance.comp_type.name + "> " + instance.name + ";"
        node += "\n        }"
        return node

    def buildChildList(self):
        self.cleanChildren()
        for obj in self.components:
            objType = 'compInst'
            obj.isObjRef = True
            obj.objRef = obj.comp_type
            obj.setCanvasParams(
                canvas = self.canvas,
                position = self.position,
                canvasOptions = canvasOptionsDict[objType]
            )
            objDrawOptions = canvasOptionsDict[objType].drawOptions
            obj.drawOptions = DrawOptions(
                    color = objDrawOptions.color,
                    minSize = objDrawOptions.minSize,
                    textPosition = "RIGHT", connectFrom = True
                )
            self.addChild(obj.name,obj)

class Listener(ROSListener):
    def __init__(self):
        self.workspace = ROS_Workspace()
        self.timers = []

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
        self.workspace.addPackage(self.package)

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
                    self.message.fields.append([field_type, field_name, field_value])
                else:
                    self.message.fields.append([field_type, field_name])

    # On exit, append message to list of messages in package
    def exitRos_msg(self, ctx):
        # Append new ros msg onto list of package messages
        self.message.package = self.package
        self.package.addMessage(self.message)

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
        self.service.package = self.package
        self.package.addService(self.service)
        
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
                    self.server = ROS_Server()
                    self.server.name = service_name + "_server"
                    self.server.service = self.package.srv_dict[service_name]
                    self.server.package = self.package
                    self.server.parentComp = self.component
                    self.component.addServer(self.server)
        elif "requires" in ctx.getText():
            for child in ctx.getChildren():
                context = str(type(child))
                if "Service_nameContext" in context:
                    service_name = child.getText()
                    # CHECK: If this service has been defined 
                    self.client = ROS_Client()
                    self.client.name = service_name + "_client"
                    self.client.service = self.package.srv_dict[service_name]
                    self.client.package = self.package
                    self.client.parentComp = self.component
                    self.component.addClient(self.client)

    # Save all publishers and susbcribers
    def enterRos_pub_sub(self, ctx):
        if "publisher" in ctx.getText():
            self.publisher = ROS_Publisher()
            for child in ctx.getChildren():
                context = str(type(child))
                if "TopicContext" in context:
                    self.publisher.topic = self.package.msg_dict[child.getText()]
                    # CHECK: If this toic has been defined
                if "PublisherContext" in context:
                    self.publisher.name = child.getText()
            if self.publisher.name != "":
                if self.publisher.topic != None:
                    self.publisher.package = self.package
                    self.publisher.parentComp = self.component
                    self.component.addPublisher(self.publisher)

        elif "subscriber" in ctx.getText():
            self.subscriber = ROS_Subscriber()
            for child in ctx.getChildren():
                context = str(type(child))
                if "TopicContext" in context:
                    self.subscriber.topic =  self.package.msg_dict[child.getText()]
                    # CHECK: If this topic has been defined
                if "SubscriberContext" in context:
                    self.subscriber.name = child.getText()
            if self.subscriber.name != "":
                if self.subscriber.topic != None:
                    self.subscriber.package = self.package
                    self.subscriber.parentComp = self.component
                    self.component.addSubscriber(self.subscriber)

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
        self.timer = ROS_Timer(name=name,period=period,period_unit=period_unit)
        self.timer.package = self.package
        self.timer.parentComp = self.component
        self.component.addTimer(self.timer)

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
        self.component = ROS_Component()
        self.component.name = instance
        self.component.comp_type = self.package.component_definition_dict[comp_type]
        self.component.parentNode = self.node
        self.node.comp_defs.append(comp_type)
        self.component.package = self.package
        self.node.addComponent(self.component)

    # On exit, add the node to the list of nodes in package
    def exitRos_node(self, ctx):
        self.package.nodes.append(self.node)
        self.package.nodes_dict[self.node.name] = self.node
