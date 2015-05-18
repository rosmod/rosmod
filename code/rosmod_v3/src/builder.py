# Find ROSMOD Grammar Listeners
# Author: Pranav Srinivas Kumar
# Date: 2015.04.22

import sys, os, inspect
from collections import OrderedDict
from metaclass import *

# Find ANTLR4 python runtime
antlr4 = os.path.realpath(os.path.abspath
                          (os.path.join
                           (os.path.split
                            (inspect.getfile
                             (inspect.currentframe()
                          )
                         )[0], "Antlr4")
                       ))
if antlr4 not in sys.path:
    sys.path.insert(0, antlr4)
from antlr4 import *

# Find Software Model 
rosmod_software = os.path.realpath(os.path.abspath
                               (os.path.join
                                (os.path.split
                                 (inspect.getfile
                                  (inspect.currentframe()
                               )
                              )[0], "grammar/01-Software/")
                            ))

if rosmod_software not in sys.path:
    sys.path.insert(0, rosmod_software)
from ROSMOD_SoftwareLexer import ROSMOD_SoftwareLexer
from ROSMOD_SoftwareParser import ROSMOD_SoftwareParser
from ROSMOD_SoftwareListener import ROSMOD_SoftwareListener

# Find Hardware Model 
rosmod_hardware = os.path.realpath(os.path.abspath
                               (os.path.join
                                (os.path.split
                                 (inspect.getfile
                                  (inspect.currentframe()
                               )
                              )[0], "grammar/02-Hardware/")
                            ))

if rosmod_hardware not in sys.path:
    sys.path.insert(0, rosmod_hardware)
from ROSMOD_HardwareLexer import ROSMOD_HardwareLexer
from ROSMOD_HardwareParser import ROSMOD_HardwareParser
from ROSMOD_HardwareListener import ROSMOD_HardwareListener

# Find Deployment Model
rosmod_deployment = os.path.realpath(os.path.abspath
                               (os.path.join
                                (os.path.split
                                 (inspect.getfile
                                  (inspect.currentframe()
                               )
                              )[0], "grammar/03-Deployment/")
                            ))

if rosmod_deployment not in sys.path:
    sys.path.insert(0, rosmod_deployment)
from ROSMOD_DeploymentLexer import ROSMOD_DeploymentLexer
from ROSMOD_DeploymentParser import ROSMOD_DeploymentParser
from ROSMOD_DeploymentListener import ROSMOD_DeploymentListener

# Build ROS Workspace from Software Model
# Use Metaclass to obtain listener functions
class ROSMOD_Software_Builder(ROSMOD_SoftwareListener):
    __metaclass__ = Grammar_MetaClass

    def __init__(self, project):
        # Create a ROS Workspace Object
        self.rml = type("ROS_RML", 
                              ( object, Drawable_Object, ), { '__init__' : Drawable_Object.__init__ })()
        self.rml.parent = project
        self.rml.kind = "rml"
        self.active_objects = [self.rml]

# Build ROS Hardware from Hardware Model
# Use Metaclass to obtain listener functions
class ROSMOD_Hardware_Builder(ROSMOD_HardwareListener):
    __metaclass__ = Grammar_MetaClass

    def __init__(self, project):
        # Create a ROS Deployment Object
        self.rhw = type("ROS_RHW", 
                        ( object, Drawable_Object, ), { '__init__' : Drawable_Object.__init__ })()
        self.rhw.parent = project
        self.rhw.kind = "rhw"
        self.active_objects = [self.rhw]


# Build ROS Deployment from Deployment Model
# Use Metaclass to obtain listener functions
class ROSMOD_Deployment_Builder(ROSMOD_DeploymentListener):
    __metaclass__ = Grammar_MetaClass

    def __init__(self, project):
        # Create a ROS Deployment Object
        self.rdp = type("ROS_RDP",
                        ( object, Drawable_Object, ), { '__init__' : Drawable_Object.__init__ })()
        self.rdp.parent = project
        self.rdp.kind = "rdp"
        self.active_objects = [self.rdp]


