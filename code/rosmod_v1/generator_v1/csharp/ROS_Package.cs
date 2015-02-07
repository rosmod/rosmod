using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ROS_Generator
{
    class ROS_Package
    {
        // Name of ROS Package
        public string name;

        // List of ROS msg files
        public List<ROS_msg> messages;

        // List of ROS srv files
        public List<ROS_srv> services;

        // List of ROS nodes
        public List<ROS_Node> nodes;

        public ROS_Package(string package_name)
        {
            name = package_name;
            messages = new List<ROS_msg>();
            services = new List<ROS_srv>();
            nodes = new List<ROS_Node>();
        }
    }

    // A Single ROS Node
    class ROS_Node
    {
        // Name of ROS Node
        public string name;

        // Component Definitions in node txt file
        public string definition;

        // Parsed list of visitor "component" objects
        public List<Visitor.ROS_Component> components;

        public ROS_Node()
        {
            name = "";
            definition = "";
            components = new List<Visitor.ROS_Component>();
        }
    }

    // A Single ROS msg file
    class ROS_msg
    {
        // Name of ROS msg
        public string name;

        // Content of msg file
        public string definition;

        public ROS_msg()
        {
            name = "";
            definition = "";
        }
    }

    // A Single ROS srv file
    class ROS_srv
    {
        // Name of ROS srv
        public string name;

        // Content of srv file
        public string definition;

        public ROS_srv()
        {
            name = "";
            definition = "";
        }
    }
}
