#!/usr/bin/python
'''
'''

class BaseROSInterface:
    def __init__(self, name, definition):
        self.name = name
        self.definition = definition

    def __str__(self):
        retStr = ""
        retStr += "Name: {0}".format(name)
        retStr += "Def : {0}".format(definition)
        return retStr

class Service(BaseROSInterface):
    def __init__(self, name, definition = ''):
        BaseROSInterface.__init__(self,name,definition)

class Message(BaseROSInterface):
    def __init__(self, name, definition = ''):
        BaseROSInterface.__init__(self,name,definition)

class Component:
    def __init__(self):
        self.publishers = {}
        self.subscribers = {}
        self.clients = {}
        self.servers = {}
        self.timers = {}

    def __str__(self):
        retStr = ""
        retStr += "publishers:{0}".format(self.publishers)
        retStr += "subscribers:{0}".format(self.subscribers)
        retStr += "clients:{0}".format(self.clients)
        retStr += "servers:{0}".format(self.servers)
        retStr += "timers:{0}".format(self.servers)
        return retStr

    def addPub(self, name, topic):
        self.publishers[name] = topic
    def addSub(self, name, topic):
        self.subscribers[name] = topic
    def addClient(self, name, service):
        self.clients[name] = service
    def addServer(self, name, service):
        self.servers[name] = service
    def addTimer(self, name, timerOpts):
        self.timers[name] = timerOpts

class Node:
    def __init__(self):
        self.components = {}

    def __str__(self):
        retStr = ""
        retStr += "components:{0}".format(self.components)
        return retStr
    
    def addComponent(self, name, component):
        self.components[name] = component

class Package:
    def __init__(self):
        self.nodes = {}

    def addNode(self, name, node):
        self.nodes[name] = node

class Model:
    def __init__(self):
        self.packages = {}
        self.nodes = {}
        self.components = {}
        self.messages = {}
        self.services = {}

    def __str__(self):
        retStr = ""
        retStr += "nodes:{0}".format(self.nodes)
        retStr += "components:{0}".format(self.components)
        retStr += "messages:{0}".format(self.messages)
        retStr += "services:{0}".format(self.services)
        return retStr

    def addPackage(self, name, package):
        self.packages[name] = package

    def addNode(self, name, node):
        self.nodes[name] = node

    def addComponent(self, name, component):
        self.components[name] = component

    def addService(self, name, definition = ''):
        self.services[name] = definition

    def addService(self,serv):
        self.services[serv.name]=serv.definition
    
    def addMessage(self, name, definition = ''):
        self.messages[name] = definition

    def addMessage(self, msg):
        self.messages[msg.name]=msg.definition

