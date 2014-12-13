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

        // Global List of ROS Package objects
        public static List<ROS_Package> ros_packages = new List<ROS_Package>();

        // Find all ROS Packages in the input directory path
        static void find_packages(string sDir)
        {
            try
            {
                // Find all ROS packages in input directory
                foreach (string d in Directory.GetDirectories(sDir))
                {
                    string package_location = d;
                    string package_name = d.Split('\\').Last();

                    ROS_Package new_package = new ROS_Package(package_name);
                    
                    // Find msg directory
                    foreach (string msg in Directory.GetDirectories(d))
                    {
                        string dir_name = msg.Split('\\').Last();
                        if (dir_name == "msg")
                        {
                            foreach (string f in Directory.GetFiles(msg))
                            {
                                string msg_name = f.Split('\\').Last();
                                string msg_definition = System.IO.File.ReadAllText(f);
                                new_package.messages.Add(new ROS_msg { name = msg_name, definition = msg_definition });
                            }
                        }
                    }

                    // Find srv directory
                    foreach (string srv in Directory.GetDirectories(d))
                    {
                        string dir_name = srv.Split('\\').Last();
                        if (dir_name == "srv")
                        {
                            foreach (string f in Directory.GetFiles(srv))
                            {
                                string srv_name = f.Split('\\').Last();
                                string srv_definition = System.IO.File.ReadAllText(f);
                                new_package.services.Add(new ROS_srv { name = srv_name, definition = srv_definition });
                            }
                        }
                    }

                    // Find nodes directory
                    foreach (string node in Directory.GetDirectories(d))
                    {
                        string dir_name = node.Split('\\').Last();
                        if (dir_name == "nodes")
                        {
                            foreach (string f in Directory.GetFiles(node))
                            {
                                string node_name = f.Split('\\').Last();
                                string node_definition = System.IO.File.ReadAllText(f);

                                // Parse the node definition using the visitor

                                // Convert string input into AntlrInputStream
                                byte[] byteArray = Encoding.UTF8.GetBytes(node_definition);
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

                                // Add the parsed node to new_package.nodes
                                new_package.nodes.Add(new ROS_Node { name = node_name, definition = node_definition , components = visitor.ros_components });
                            }
                        }
                    }
                    ros_packages.Add(new_package);
                }
            }
            catch (System.Exception excpt)
            {
                Console.WriteLine(excpt.Message);
            }
        }



        static void Main(string[] args)
        {

            // Input directory containing ROS package
            string input_directory = @"..\..\samples\";

            // Find legal ROS package in input directory
            find_packages(input_directory);

            // Print ros package information
            Printer printer = new Printer();
            printer.print_ros_packages(ros_packages);

            // Generate all *.cpp, *.hpp and build system files
            Generator generator = new Generator();
            generator.generate(input_directory, ros_packages);


            Console.ReadLine();
        }
    }
}
