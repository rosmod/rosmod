#!/usr/bin/python
#
# ROS Tools v3
# Author: Pranav Srinivas Kumar
# Date: 2015.04.22

from collections import OrderedDict
from project import *
from builder import *
from Cheetah.Template import Template

import os,sys

if __name__ == "__main__":

    if len(sys.argv) != 2:
        print "ERROR, usage: ./main.py <path to project>"
        sys.exit(1)

    path = sys.argv[1]

    # Testing ROSMOD Tools Features
    My_Project = ROSMOD_Project()
    My_Project.open(path)
    My_Project.generate_workspace()
    My_Project.generate_xml()
    My_Project.generate_cpn()
    My_Project.save()

