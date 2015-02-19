# ROS Generator - Version 2
# Author: Pranav Srinivas Kumar
# Date: 2015.02.06

import os
import sys
import inspect

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

# Find ROS Lexer, Parser and Visitor
ros_grammar = os.path.realpath(os.path.abspath
                               (os.path.join
                                (os.path.split
                                 (inspect.getfile
                                  (inspect.currentframe()
                               )
                              )[0], "grammar/01-Workspace/")
                            ))

if ros_grammar not in sys.path:
    sys.path.insert(0, ros_grammar)
from ROSLexer import ROSLexer
from ROSParser import ROSParser
from ROSListener import ROSListener
from ROSVisitor import ROSVisitor

# Find Hosts Lexer, Parser and Visitor
hosts_grammar = os.path.realpath(os.path.abspath
                               (os.path.join
                                (os.path.split
                                 (inspect.getfile
                                  (inspect.currentframe()
                               )
                              )[0], "grammar/02-Hosts/")
                            ))

if hosts_grammar not in sys.path:
    sys.path.insert(0, hosts_grammar)
from HostsLexer import HostsLexer
from HostsParser import HostsParser
from HostsListener import HostsListener

# Find Hosts Lexer, Parser and Visitor
deployment_grammar = os.path.realpath(os.path.abspath
                               (os.path.join
                                (os.path.split
                                 (inspect.getfile
                                  (inspect.currentframe()
                               )
                              )[0], "grammar/03-Deployment/")
                            ))

if deployment_grammar not in sys.path:
    sys.path.insert(0, deployment_grammar)
from DeploymentLexer import DeploymentLexer
from DeploymentParser import DeploymentParser
from DeploymentListener import DeploymentListener

from listener import *
from generator import *
from loader import *

# Create and return a workspace object corresponding to the input model
def parse_model(filename):

    # Read the input model
    model = FileStream(filename)
    # Instantiate the ROSLexer
    lexer = ROSLexer(model)
    # Generate Tokens
    stream = CommonTokenStream(lexer)
    # Instantiate the ROSParser
    parser = ROSParser(stream)
    # Parse from starting point of grammar
    tree = parser.start()
    # Instantiate a Listener Object
    listener = Listener()
    # Instantiate a Parse Tree Walker
    walker = ParseTreeWalker()

    # Walk the parse tree
    walker.walk(listener, tree)

    return listener

# Check workspace directory for existing code that may
# require preservation
def check_workspace(listener, model_path):
    
    # Instantiate a Loader Object
    loader = Loader()
    # Use load_business_logic to load existing business logic
    loader.load_business_logic(listener, model_path)
    
# Generate the ROS workspace corresponding to the input model
def generate_workspace(listener, model_path):
    
    # Instantiate a Generator Object
    generator = Generator()
    # Use listener_object to generate ROS workspace
    generator.generate(listener, model_path)

# Generate a ROS model from a workspace object
# Used to save an edited model
def generate_model(workspace, model_name, model_path):
    
    model = str(workspace)
    with open(os.path.join(model_path, model_name), 'w') as temp_file:
        temp_file.write(model)
    return model
    
if __name__ == "__main__":
    
    # Obtain the model filename
    model = sys.argv[1]
    # Obtain the model path
    model_path = os.path.abspath(os.path.dirname(sys.argv[1]))

    # Parse the input model
    listener_object = parse_model(model)

    # Check the workspace directory for existing code that may require
    # preservation
    check_workspace(listener_object, model_path)

    # Generate the ROS workspace pertaining to the input model
    generate_workspace(listener_object, model_path)

