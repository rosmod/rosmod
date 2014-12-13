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
    // CMakeLists template variables
    public partial class CMakeLists
    {
        public string project_name;
        public List<string> messages;
        public List<string> services;

        public CMakeLists()
        {
            project_name = "";
            messages = new List<string>();
            services = new List<string>();
        }
    }

    // Package.xml template variables
    public partial class package_xml
    {
        public string project_name;

        public package_xml()
        {
            project_name = "";
        }
    }

    // Component template variables
    public partial class Component_Base_cpp
    {
        public string project_name;

        public Component_Base_cpp()
        {
            project_name = "";
        }
    }

    class Generator
    {

        public void create_directory_structure(string input_directory, List<ROS_Package> ros_packages)
        {
            foreach (var package in ros_packages)
            {
                string package_dir = input_directory + package.name;

                string package_ws = package_dir + "\\" + package.name + "_ws";
                string package_src = package_ws + "\\" + "src";
                string package_main = package_src + "\\" + package.name;

                // Create Workspace directory
                Directory.CreateDirectory(package_ws);

                // Create src directory
                Directory.CreateDirectory(package_src);

                dot_catkin_ws new_ws = new dot_catkin_ws();
                var generated_text = new_ws.TransformText();
                System.IO.File.WriteAllText(package_ws + "\\.catkin_workspace", generated_text);

                // Create the package directory
                Directory.CreateDirectory(package_main);

                // Create the include directory
                Directory.CreateDirectory(package_main + "\\include");

                // Create the src directory
                Directory.CreateDirectory(package_main + "\\src");

                // Create msg directory IF necessary
                if (package.messages.Count > 0)
                {
                    Directory.CreateDirectory(package_main + "\\msg");

                    foreach(var msg in package.messages) 
                    {
                        // Create the message files
                        string filename = msg.name;

                        using (StreamWriter fw = new StreamWriter(package_main + "\\msg\\" + filename, false))
                        {
                            fw.Write(msg.definition);
                        }
                    }
                }

                // Create service directory IF necessary
                if (package.services.Count > 0)
                {
                    Directory.CreateDirectory(package_main + "\\srv");

                    foreach (var srv in package.services)
                    {
                        // Create the service files
                        string filename = srv.name;

                        using (StreamWriter fw = new StreamWriter(package_main + "\\srv\\" + filename, false))
                        {
                            fw.Write(srv.definition);
                        }
                    }
                }

                // Create base Component.cpp
                Component_Base_cpp base_comp = new Component_Base_cpp();
                base_comp.project_name = package.name;
                generated_text = base_comp.TransformText();
                System.IO.File.WriteAllText(package_main + "\\src\\Component.cpp", generated_text);

                // Create base Component.h
                Component_Base_hpp base_comp_hpp = new Component_Base_hpp();
                generated_text = base_comp_hpp.TransformText();
                System.IO.File.WriteAllText(package_main + "\\include\\Component.hpp", generated_text);

                // Create CMakeLists file
                CMakeLists cmake_lists = new CMakeLists();
                cmake_lists.project_name = package.name;
                // Setup all messages
                foreach(var msg in package.messages) 
                {
                    cmake_lists.messages.Add(msg.name);
                }
                // Setup all services
                foreach (var srv in package.services)
                {
                    cmake_lists.services.Add(srv.name);
                }
                generated_text = cmake_lists.TransformText();
                System.IO.File.WriteAllText(package_main + "\\CMakeLists.txt", generated_text);   
             
                // Create package.xml file
                package_xml new_package = new package_xml();
                new_package.project_name = package.name;
                generated_text = new_package.TransformText();
                System.IO.File.WriteAllText(package_main + "\\package.xml", generated_text);
            }

        }
    }
}
