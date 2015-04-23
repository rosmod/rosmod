# Find ROSMOD Grammar files & import classes
# Author: Pranav Srinivas Kumar
# Date: 2015.04.22

import sys, os, inspect

# Find ANTLR4 python runtime
antlr4 = os.path.realpath(os.path.abspath
                          (os.path.join
                           (os.path.split
                            (inspect.getfile
                             (inspect.currentframe()
                          )
                         )[0], "/../Antlr4")
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
                              )[0], "../grammar/01-Software/")
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
                              )[0], "../grammar/02-Hardware/")
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
                              )[0], "../grammar/03-Deployment/")
                            ))

if rosmod_deployment not in sys.path:
    sys.path.insert(0, rosmod_deployment)
from ROSMOD_DeploymentLexer import ROSMOD_DeploymentLexer
from ROSMOD_DeploymentParser import ROSMOD_DeploymentParser
from ROSMOD_DeploymentListener import ROSMOD_DeploymentListener
