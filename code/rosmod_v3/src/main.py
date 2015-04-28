#!/usr/bin/python
#
# ROS Tools v3
# Author: Pranav Srinivas Kumar
# Date: 2015.04.22

from collections import OrderedDict
from project import *
from builder import *
from Cheetah.Template import Template

if __name__ == "__main__":

    # Testing ROS Tools Features
    My_Project = ROS_Project()
    My_Project.parse_rml("/home/kelsier/Repositories/rosmod/code/rosmod_v3/tests/testKRPC/testKRPC/01-Software/Workspace.rml")
    My_Project.parse_rhw("/home/kelsier/Repositories/rosmod/code/rosmod_v3/tests/testKRPC/testKRPC/02-Hardware/Hardware.rhw")
    My_Project.parse_rdp("/home/kelsier/Repositories/rosmod/code/rosmod_v3/tests/testKRPC/testKRPC/03-Deployment/Deployment.rdp")
