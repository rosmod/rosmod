#!/usr/bin/python
'''
'''

class BaseROSInterface:
    def __init__(self, name, definition):
        self.name = name
        self.definition = definition

class Service(BaseROSInterface):
    def __init__(self, name, definition = ''):
        BaseROSInterface.__init__(self,name,definition)

class Message(BaseROSInterface):
    def __init__(self, name, definition = ''):
        BaseROSInterface.__init__(self,name,definition)

class Timer:
    def __init__(self, period, oneShot=False):
        self.period = period
        self.oneShot = oneShot

class Component:
    def __init__(self):
        self.publishers = {}
        self.subscribers = {}
        self.clients = {}
        self.servers = {}
        self.timers = {}

    def addPub(self, name, topic):
        self.publishers[name] = topic
    def addSub(self, name, topic):
        self.subscribers[name] = topic
    def addClient(self, name, service):
        self.clients[name] = service
    def addServer(self, name, service):
        self.servers[name] = service
    def addTimer(self, name, timer):
        self.timers[name] = timer

class Node:
    def __init__(self):
        self.components = {}
    
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

    def addPackage(self, name, package):
        self.packages[name] = package
