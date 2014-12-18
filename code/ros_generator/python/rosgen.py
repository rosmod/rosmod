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

# Start of ROS Generator
def main(argv):
	
    # Obtain name and path to input model
    model = argv[1]
    model_path = os.path.abspath(os.path.dirname(argv[1]))
    print "\nModel: ", model
    print "Model Path: ", model_path, "\n"

    # Read the input model
    generator_input = FileStream(model)
    # Instantiate the ROSLexer
    lexer = ROSLexer(generator_input)
    # Generate Tokens
    stream = CommonTokenStream(lexer)
    # Instantiate the ROSParser
    parser = ROSParser(stream)
    # Parse from starting point of grammar
    tree = parser.start()
    # Instantiate a Listener Object
    listener_object = Listener()
    # Instantiate a Parse Tree Walker
    walker = ParseTreeWalker()
    print "Walking Parse Tree...\n"
    # Walk the parse tree
    walker.walk(listener_object, tree)

    # listener_object.workspace has all the information we need
 
    # Instantiate a Printer Object
    printer = Printer()
    # Use listener_object to print workspace information
    printer.print_ros_workspace(listener_object)

    # Instantiate a Generator Object
    generator = Generator()
    # Use listener_object to generate ROS workspace
    generator.generate(listener_object, model_path)

    return listener_object

if __name__ == "__main__":
    main(sys.argv)
        
