#!/usr/bin/python
'''
'''

class BaseROSType:
    def __init__(self, name, definition):
        self.name = name
        self.definition = definition

class Service(BaseROSType):
    def __init__(self, name, definition = ''):
        BaseROSType.__init__(self,name,definition)

class Message(BaseROSType):
    def __init__(self, name, definition = ''):
        BaseROSType.__init__(self,name,definition)

class Timer:
    def __init__(self, name, period, oneShot=False):
        self.name = name
        self.period = period
        self.oneShot = oneShot

class Component:
    def __init__(self):
        self.publishers = {}
        self.subscribers = {}
        self.clients = {}
        self.servers = {}
        self.timers = {}

class Node:
    def __init__(self):
        self.components = {}

class Package:
    def __init__(self):
        self.nodes = {}

class Model:
    def __init__(self):
        self.packages = {}
