# Meta-Model Classes
# Author: Pranav Srinivas Kumar
# Date: 2015.02.06

from collections import OrderedDict

# ROS Workspace
class ROS_Workspace:
    
    def __init__(self):
        # Name of Workspace
        self.name = ""
        # Dictionary of packages in workspace
        self.packages = OrderedDict()

    def get_packages(self):
        # Return all packages in workspace
        return self.packages

    def add_package(self, package):
        # Remove existing package with same name
        self.remove_package(package.name)
        # Add a new package to workspace
        self.packages[package.name] = this_package

    def remove_package(self, name):
        # Remove an existing package from workspace
        if this_package_name in self.packages:
            del self.packages[name]

# ROS Package
class ROS_Package:

    def __init__(self):
        # Name of Package
        self.name = ""
        # Messages
        self.messages = OrderedDict()
        # Services
        self.services = OrderedDict()
        # Component Definitions
        self.components = OrderedDict()
        # Nodes
        self.nodes = OrderedDict()

    def get_messages(self):
        # Return all messages in package
        return self.messages

    def add_message(self, message):
        # Remove existing message with same name
        self.remove_message(message.name)
        # Add a new message to package
        self.messages[message.name] = message
        
    def remove_message(self, name):
        # Remove an existing message from package
        if name in self.messages:
            del self.messages[name]

    def get_services(self):
        # Return all services in package
        return self.services

    def add_service(self, service):
        # Remove existing service with same name
        self.remove_service(service.name)
        # Add a new service to package
        self.service[service.name] = service

    def remove_service(self, name):
        # Remove an existing service from package
        if name in self.services:
            del self.services[name]
            
    def get_components(self):
        # Return all components in package
        return self.components

    def add_component(self, component):
        # Remove existing component with same name
        self.remove_component(component.name)
        # Add a new component to package
        self.components[component.name] = component

    def remove_component(self, name):
        # Remove existing component from package
        if name in self.components:
            del self.components[name]

    def get_nodes(self):
        # Get all nodes in package
        return self.nodes

    def add_node(self, node):
        # Remove existing node with same name
        self.remove_node(node.name)
        # Add a new node to package
        self.nodes[node.name] = node

    def remove_node(self, name):
        # Remove existing component from package
        if name in self.nodes:
            del self.nodes[name]
        
        
    
        
