# ROS Generator
# Author: Pranav Srinivas Kumar
# Date: 2014.12.16

import os
import sys
import inspect

# Find ANTLR4 python runtime
antlr4 = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile(inspect.currentframe()))[0], "Antlr4")))
if antlr4 not in sys.path:
    sys.path.insert(0, antlr4)
from antlr4 import *

# Find ROS Lexer, Parser and Visitor
ros_grammar = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile(inspect.currentframe()))[0], "grammar")))
if ros_grammar not in sys.path:
    sys.path.insert(0, ros_grammar)
from ROSLexer import ROSLexer
from ROSParser import ROSParser
from ROSListener import ROSListener
from ROSVisitor import ROSVisitor

from listener import *
from printer import *
print "Compiling all templates...\n"
from generator import *

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
    workspace = Listener()
    # Instantiate a Parse Tree Walker
    walker = ParseTreeWalker()
    print "Parsing input model...\n"
    # Walk the parse tree
    walker.walk(workspace, tree)

    return workspace
    
# Generate the ROS workspace corresponding to the input model
def generate_workspace(workspace, model_path):
    
    # Instantiate a Generator Object
    generator = Generator()
    # Use listener_object to generate ROS workspace
    generator.generate(workspace, model_path)

# Print the ROS workspace 
def print_workspace(workspace):

    # Instantiate a Printer Object
    printer = Printer()
    # Print the workspace object
    printer.print_ros_workspace(workspace)
    
    
if __name__ == "__main__":
    
    # Obtain the model filename
    model = sys.argv[1]
    # Obtain the model path
    model_path = os.path.abspath(os.path.dirname(sys.argv[1]))

    # Parse the input model
    workspace = parse_model(model)

    # Generate the ROS workspace pertaining to the input model
    generate_workspace(workspace, model_path)

    # Print the ROS workspace
    print_workspace(workspace)
