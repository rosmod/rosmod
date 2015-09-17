#!/usr/bin/env python

"""This file defines the ROSMOD metamodel."""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

#from metamodel.base import Model, Children, Artifact

#import os
#import jsonpickle
#from string import Template
#import textwrap

from metamodel.importer import *

def test_project():
    # Simple Timer Example
    my_software = Software(Name("software"))
    timer_package = Package(Name("timer_package"))
    message = Message(Name("msg1"),
                      Message_Definition("int8 test\nfloat64 soemthing"))
    service = Service(Name("srv1"),
                      Service_Definition("int8 test\n---\nbool retVal"))
    timer_component = Component(Name("Timer_Component"), 
                                Component_Type("BASE"),
                                Detailed_Description("This is a periodic timer component that is used to test time-triggered operations"))
    sender_component = Component(Name("Sender_Component"),
                                 Component_Type("BASE"),
                                 Detailed_Description("This is a sendor component that sends stuff over the network"))
    receiver_component = Component(Name("Receiver_Component"),
                                   Component_Type("BASE"),
                                   Detailed_Description("This is a receiver component that receives data sent by the sender component"))
    client = Client(Name("client"),
                    Service_Reference(service))
    server = Server(Name("server"),
                    Service_Reference(service))
    pub = Publisher(Name("pub"),
                    Message_Reference(message))
    sub = Subscriber(Name("sub"),
                     Service_Reference(message))
    timer = Timer(name = Name("periodic_timer"),
                  period = Period(1.0),
                  priority = Priority(50),
                  deadline = Deadline(0.01))
    timer2 = Timer(name = Name("sender_timer"),
                  period = Period(1.0),
                  priority = Priority(50),
                  deadline = Deadline(0.01))

    my_hardware = Hardware(Name("hardware"))
    bbb_111 = Computer(name = Name("BBB_111"),
                       ip_address = IP_Address("10.1.1.1"),
                       username = Username("ubuntu"),
                       sshkey = SSH_Key("/home/jeb/.ssh/id_rsa_jetsontk1"),
                       deployment_path = Deployment_Path("/home/ubuntu"),
                       ros_install_path = ROS_Install_Path("/opt/ros/indigo"),
                       arch = Arch("arm"))

    my_deployment = Deployment(name = Name("deployment"), 
                               hardware_reference = Hardware_Reference(my_hardware))
    timer_node = Node(name = Name("Timer_Node"),
                      computer_reference = Computer_Reference(bbb_111),
                      priority = Priority(80))
    timer_component_i = Component_Instance(name = Name("Timer_Component_i"),
                                           component_reference = \
                                           Component_Reference(timer_component),
                                           scheduling_scheme =\
                                           Scheduling_Scheme("PFIFO"),
                                           logging = Logging({"DEBUG" : True,
                                                              "INFO" : False,
                                                              "WARNING" : False,
                                                              "ERROR" : False,
                                                              "CRITICAL" : False}))
    # Establish Tree
    timer_component.add_child(timer)
    sender_component.add_child(timer2)
    sender_component.add_child(pub)
    sender_component.add_child(client)
    receiver_component.add_child(server)
    receiver_component.add_child(sub)
    timer_package.add_child(service)
    timer_package.add_child(message)
    timer_package.add_child(receiver_component)
    timer_package.add_child(sender_component)
    timer_package.add_child(timer_component)
    my_software.add_child(timer_package)
    my_hardware.add_child(bbb_111)
    timer_node.add_child(timer_component_i)
    my_deployment.add_child(timer_node)

    project = Project()
    project.new(name=Name("timer_example"),
                path=Path("/home/jeb/samples/"),
                authors=Authors(["Pranav Srinivas Kumar", 
                                "William Emfinger"]),
                software=my_software,
                hardware=my_hardware,
                deployment=my_deployment)
    return project

if __name__ == '__main__':

    project = test_project()
    project.generate_artifacts()
    project.save()
    project.open("/home/jeb/samples/timer_example/timer_example.rml")


