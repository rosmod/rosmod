#!/usr/bin/env python

"""This file defines the ROSMOD Computer Model"""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

from base import Model, Children
from attributes import Name, IP_Address, Username, SSH_Key, Deployment_Path,\
    ROS_Install_Path, Arch, Network_Profile

class Computer(Model):
    def __init__(self, name=Name(""), ip_address=IP_Address(""), 
                 username=Username(""), sshkey=SSH_Key(""), 
                 deployment_path=Deployment_Path(""), 
                 ros_install_path=ROS_Install_Path(""), arch=Arch(""), 
                 network_profile=Network_Profile(""), parent=None):
        super(Computer, self).__init__()
        self.kind = "Computer"

        assert name != None, "Computer name is None!"
        assert ip_address != None, "Computer IP address is None!"
        assert username != None, "Computer username is None!"
        assert sshkey != None, "Computer sshkey is None!"
        assert deployment_path != None, "Computer deployment path is None!"
        assert ros_install_path != None, "Computer ros install path is None!"
        assert arch != None, "Computer architecture is None!"
        assert network_profile != None, "Computer network profile is None!"

        self.parent = parent
        self["name"] = name
        self["ip_address"] = ip_address
        self["username"] = username
        self["sshkey"] = sshkey
        self["deployment_path"] = deployment_path
        self["ros_install_path"] = ros_install_path
        self["arch"] = arch
        self["network_profile"] = network_profile

        self.children = Children(allowed=[], cardinality={})
