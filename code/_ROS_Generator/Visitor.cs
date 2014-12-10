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

        // Visit component
        public override string VisitRos_component(ROSParser.Ros_componentContext context)
        {
            return base.VisitRos_component(context);
        }

        // Visit component name 
        public override string VisitRos_component_name(ROSParser.Ros_component_nameContext context)
        {
            return base.VisitRos_component_name(context);
        }

        // Visit service name
        public override string VisitService_name(ROSParser.Service_nameContext context)
        {
            return base.VisitService_name(context);
        }

        // Visit Publisher/Subscriber port name
        public override string VisitPort_name(ROSParser.Port_nameContext context)
        {
            return base.VisitPort_name(context);
        }

        // Visit topic name
        public override string VisitTopic_name(ROSParser.Topic_nameContext context)
        {
            return base.VisitTopic_name(context);
        }

    }
}
