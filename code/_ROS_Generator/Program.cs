using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows;

namespace ROS_Generator
{
    class Program
    {
        static void Main(string[] args)
        {

            string ros_node = System.IO.File.ReadAllText(@"..\..\test_input.txt");
            // Convert string input into AntlrInputStream
            byte[] byteArray = Encoding.UTF8.GetBytes(ros_node);
            MemoryStream constraints_stream = new MemoryStream(byteArray);
            StreamReader inputStream = new StreamReader(constraints_stream);
            Antlr4.Runtime.AntlrInputStream input = new Antlr4.Runtime.AntlrInputStream(inputStream.ReadToEnd());

            // Create lexer + parser objects and obtain parsed tree
            ROSLexer lexer = new ROSLexer(input);
            Antlr4.Runtime.CommonTokenStream tokens = new Antlr4.Runtime.CommonTokenStream(lexer);
            ROSParser parser = new ROSParser(tokens);
            Antlr4.Runtime.Tree.IParseTree tree = parser.ros_node();

            // Create a Visitor object
            Visitor visitor = new Visitor();

            // Visit nodes on the parsed tree            
            visitor.Visit(tree);


            Console.ReadLine();
        }
    }
}
