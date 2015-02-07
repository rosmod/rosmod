# Workspace Generator - Version 2
# Author: Pranav Srinivas Kumar
# Date: 2015.02.06

import sys
import os
from Cheetah.Template import Template
import inspect
import collections

# Template Compile Step -- Compiling tmpl files in templates
# Generate template python files
generator_dir = os.path.dirname(os.path.realpath(__file__))
template_dir = os.path.join(generator_dir + "/templates")

# Recursively compile on template files in templates directory
os.system("/usr/local/bin/cheetah compile " + template_dir + "/*.tmpl")
ros_templates = os.path.realpath(os.path.abspath
                                 (os.path.join
                                  (os.path.split
                                   (inspect.getfile
                                    (inspect.currentframe()
                                 )
                                )[0], "templates")
                              ))
print ros_templates
if ros_templates not in sys.path:
    sys.path.insert(0, ros_templates)
from package_xml import *
from base_component_hpp import *
from base_component_cpp import *
from msg import *
from srv import *
from component_hpp import *
from component_cpp import *
from nodeMain import *
from CMakeLists import *

from OrderedSet import *

class Generator:

    def generate(self, listener, path):

        print "\n Generating ROS Workspace..."
