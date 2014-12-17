#!/usr/bin/python
'''
'''

class BaseROSInterface:
    def __init__(self, name, definition):
        self.name = name
        self.definition = definition

    def __str__(self):
        retStr = ""
        retStr += "Name: {0}".format(self.name)
        retStr += "Def : {0}".format(self.definition)
        return retStr

class Service(BaseROSInterface):
    def __init__(self, name, definition = ''):
        BaseROSInterface.__init__(self,name,definition)

class Message(BaseROSInterface):
    def __init__(self, name, definition = ''):
        BaseROSInterface.__init__(self,name,definition)

class Component:
    def __init__(self):
        self.numObjects = 0
        self.maxNameLen = 0
        self.publishers = {}
        self.subscribers = {}
        self.clients = {}
        self.servers = {}
        self.timers = {}

    def __str__(self):
        retStr = "Component Definition:\n"
        retStr += "\tpublishers:{0}\n".format(self.publishers)
        retStr += "\tsubscribers:{0}\n".format(self.subscribers)
        retStr += "\tclients:{0}\n".format(self.clients)
        retStr += "\tservers:{0}\n".format(self.servers)
        retStr += "\ttimers:{0}\n".format(self.servers)
        return retStr

    def addPub(self, name, topic):
        if name not in self.publishers:
            self.numObjects += 1
        if len(name) > self.maxNameLen:
            self.maxNameLen = len(name)
        self.publishers[name] = topic
    def addSub(self, name, topic):
        if name not in self.subscribers:
            self.numObjects += 1
        if len(name) > self.maxNameLen:
            self.maxNameLen = len(name)
        self.subscribers[name] = topic
    def addClient(self, name, service):
        if name not in self.clients:
            self.numObjects += 1
        if len(name) > self.maxNameLen:
            self.maxNameLen = len(name)
        self.clients[name] = service
    def addServer(self, name, service):
        if name not in self.servers:
            self.numObjects += 1
        if len(name) > self.maxNameLen:
            self.maxNameLen = len(name)
        self.servers[name] = service
    def addTimer(self, name, timerOpts):
        if name not in self.timers:
            self.numObjects += 1
        if len(name) > self.maxNameLen:
            self.maxNameLen = len(name)
        self.timers[name] = timerOpts

class Node:
    def __init__(self):
        self.numObjects = 0
        self.maxNameLen = 0
        self.components = {}

    def __str__(self):
        retStr = ""
        for name,comp in self.components.iteritems():
            retStr += "\t{0} :\n\t{1}".format(name,comp)
        return retStr
    
    def addComponent(self, name, component):
        if name not in self.components:
            self.numObjects += 1
        if len(name) > self.maxNameLen:
            self.maxNameLen = len(name)
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

