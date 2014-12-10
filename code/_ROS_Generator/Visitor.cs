using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ROS_Generator
{
    // Derive from the generated base visitor class
    class Visitor : ROSBaseVisitor<string>
    {

        // Service
        public class Service
        {
            public string name;
        }

        // Facet
        public class Facet
        {
            public string name;
            public List<Service> services;
        }

        // Receptacle 
        public class Receptacle
        {
            public string name;
            public List<Service> services;
        }

        // Publisher
        public class Publisher
        {
            public string name;
            public string topic;
        }

        // Subscriber
        public class Subscriber
        {
            public string name;
            public string topic;
            // Add subscriber options here - will be specified in grammar and populated here
        }     

        // Timer
        public class Timer
        {
            public string name;
            // TImer Properties
            public bool one_shot_timer;
            public float period;
            public float offset;
        }

        // ROS Component
        public class Component
        {
            public string name;
            public List<Facet> facets;
            public List<Receptacle> receptacles;
            public List<Publisher> publishers;
            public List<Subscriber> subscribers;
            public List<Timer> timers;
        }

        // List of all components in ROS Node
        public List<Component> ros_components = new List<Component>();


        public override string VisitRos_component(ROSParser.Ros_componentContext context)
        {
            return base.VisitRos_component(context);
        }

    }
}
