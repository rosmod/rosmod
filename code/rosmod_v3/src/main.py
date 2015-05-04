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
    My_Project.open("/home/kelsier/Repositories/rosmod/code/rosmod_v3/tests/testKRPC/testKRPC")
    My_Project.save()
#    My_Project.generate_workspace()
#    My_Project.save_rml()
#    My_Project.save_rhw()
#    My_Project.save_rdp()
