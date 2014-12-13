using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows;
using System.Linq;

namespace ROS_Generator
{
    // Extending CMakeLists template class
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

    // Extending Package.xml template class
    public partial class package_xml
    {
        public string project_name;

        public package_xml()
        {
            project_name = "";
        }
    }

    // Extending Component base class template
    public partial class Component_Base_cpp
    {
        public string project_name;

        public Component_Base_cpp()
        {
            project_name = "";
        }
    }

    // Extending Component hpp template
    public partial class Component_hpp
    {
        public string define_guard;
        public string project_name;
        public string node_name;
        public string comp_name;
        public List<Visitor.Publisher> publishers;
        public List<Visitor.Subscriber> subscribers;
        public List<Visitor.Service> provided_services;
        public List<Visitor.Service> required_services;
        public List<Visitor.Timer> timers;
        public List<String> topics;
        public List<String> services;

        public Component_hpp()
        {
            node_name = "";
            comp_name = "";
            publishers = new List<Visitor.Publisher>();
            subscribers = new List<Visitor.Subscriber>();
            provided_services = new List<Visitor.Service>();
            required_services = new List<Visitor.Service>();
            timers = new List<Visitor.Timer>();
            topics = new List<String>();
            services = new List<String>();
        }
    }

    class Generator
    {

        public void generate(string input_directory, List<ROS_Package> ros_packages)
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


                // Generating header file and cpp file for each component
                foreach (var node in package.nodes)
                {
                    foreach (var component in node.components)
                    {
                        // HPP GENERATION
                        Component_hpp new_hpp = new Component_hpp();
                        new_hpp.project_name = package.name;

                        new_hpp.define_guard = component.name.ToUpper();

                        char[] node_txt = { '.', 't', 'x', 't' };
                        new_hpp.node_name = node.name.TrimEnd(node_txt);
                        new_hpp.comp_name = component.name;

                        if (component.timers.Count > 0)
                        {
                            foreach (var timer in component.timers)
                            {
                                new_hpp.timers.Add(timer);
                            }
                        }

                        if (component.subscribers.Count > 0)
                        {
                            foreach (var subscriber in component.subscribers)
                            {
                                char[] msg = {'.', 'm', 's', 'g'};
                                string topic_name = subscriber.topic.TrimEnd(msg);
                                new_hpp.topics.Add(topic_name);
                                new_hpp.subscribers.Add(new Visitor.Subscriber { name = subscriber.name, topic = topic_name });
                            }
                        }

                        if (component.publishers.Count > 0)
                        {
                            foreach (var publisher in component.publishers)
                            {
                                char[] msg = { '.', 'm', 's', 'g' };
                                string topic_name = publisher.topic.TrimEnd(msg);
                                new_hpp.topics.Add(topic_name);
                                new_hpp.publishers.Add(new Visitor.Publisher { name = publisher.name, topic = topic_name});
                            }
                        }

                        // Find unique topics
                        new_hpp.topics = new_hpp.topics.Select(x => x).Distinct().ToList();
                        //Notes.Select(x => x.Author).Distinct();

                        if (component.provided_services.Count > 0)
                        {
                            foreach (var provided in component.provided_services)
                            {
                                char[] srv = { '.', 's', 'r', 'v' };
                                string srv_name = provided.name.TrimEnd(srv);
                                new_hpp.services.Add(srv_name);
                                new_hpp.provided_services.Add(provided);
                            }
                        }

                        if (component.required_services.Count > 0)
                        {
                            foreach (var required in component.required_services)
                            {
                                char[] srv = { '.', 's', 'r', 'v' };
                                string srv_name = required.name.TrimEnd(srv);
                                new_hpp.services.Add(srv_name);
                                new_hpp.required_services.Add(required);
                            }
                        }

                        // Find unique services
                        new_hpp.services = new_hpp.services.Select(x => x).Distinct().ToList();

                        generated_text = new_hpp.TransformText();
                        System.IO.File.WriteAllText(package_main + "\\include\\" + component.name + ".hpp", generated_text);

                        // CPP

                    }

                }

            }

        }
    }
}
