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
            // Type can be either 'Provides' or 'Requires'
            public string type;

            // Name of the service
            public string name;
        }

        // Publisher
        public class Publisher
        {
            // Name of the Publisher
            public string name;

            // Name of the message topic
            public string topic;
        }

        // Subscriber
        public class Subscriber
        {
            // Name of the subscriber
            public string name;

            // Name of the message topic
            public string topic;

            // Add subscriber options here?
        }     

        // Timer
        public class Timer
        {
            // Name of the timer
            public string name;

            // Period of the timer 
            public double period;

            // Unit - can be either 's', 'ms', 'us' or 'ns'
            public string period_unit;

            // Offset of the timer
            public double offset;

            // Unit - can be either 's', 'ms', 'us' or 'ns'
            public string offset_unit;
        }

        // ROS Component
        public class ROS_Component
        {
            // Name of the component
            public string name;

            // List of provided services
            public List<Service> provided_services;

            // List of required services
            public List<Service> required_services;

            // List of publisher ports associated with this component
            public List<Publisher> publishers;

            // List of subscriber ports associated with this component
            public List<Subscriber> subscribers;

            // List of timers associated with this component
            public List<Timer> timers;

            public ROS_Component() 
            {
                name = "";
                provided_services = new List<Service>();
                required_services = new List<Service>();
                publishers = new List<Publisher>();
                subscribers = new List<Subscriber>();
                timers = new List<Timer>();
            }
        }

        // List of all components in a ROS Node
        public List<ROS_Component> ros_components = new List<ROS_Component>();

        // Temporary variables for accumulating from visitor functions
        ROS_Component visitor_component;

        // Visit Node
        public override string VisitRos_node(ROSParser.Ros_nodeContext context)
        {
            base.VisitRos_node(context);
            // Adding the last component in node
            if (visitor_component != null)
            {
                ros_components.Add(visitor_component);
            }
            return "";
        }

        // Visit component
        public override string VisitRos_component(ROSParser.Ros_componentContext context)
        {
            if (visitor_component != null)
            {
                ros_components.Add(visitor_component);
            }

            visitor_component = new ROS_Component();
            return base.VisitRos_component(context);
        }

        // Visit component name 
        public override string VisitRos_component_name(ROSParser.Ros_component_nameContext context)
        {
            visitor_component.name = string.Format(context.GetText());
            return base.VisitRos_component_name(context);
        }

        // Visit Service
        public override string VisitRos_service(ROSParser.Ros_serviceContext context)
        {
            if (context.GetText().Contains("Provides"))
            {
                foreach (var child in context.children)
                {
                    if (child.GetType().ToString() == "ROS_Generator.ROSParser+Service_nameContext")
                    {
                        visitor_component.provided_services.Add(new Service { type = "Provides", name = child.GetText().ToString() });
                    }
                }
            }

            else if (context.GetText().Contains("Requires"))
            {
                foreach (var child in context.children)
                {
                    if (child.GetType().ToString() == "ROS_Generator.ROSParser+Service_nameContext")
                    {
                        visitor_component.required_services.Add(new Service { type = "Requires", name = child.GetText().ToString() });
                    }
                }
            }

            return base.VisitRos_service(context);
        }

        // Visit Publish/Subscribe specifications
        public override string VisitRos_pub_sub(ROSParser.Ros_pub_subContext context)
        {
            string port_name = "";
            string topic = "";
            if (context.GetText().Contains("Publisher"))
            {
                foreach (var child in context.children)
                {
                    if (child.GetType().ToString() == "ROS_Generator.ROSParser+PublisherContext")
                    {
                        port_name = child.GetText().ToString();
                    }

                    if (child.GetType().ToString() == "ROS_Generator.ROSParser+TopicContext")
                    {
                        topic = child.GetText().ToString();
                    }
                }
                visitor_component.publishers.Add(new Publisher { name = port_name, topic = topic });
            }

            else if (context.GetText().Contains("Subscriber"))
            {
                foreach (var child in context.children)
                {
                    if (child.GetType().ToString() == "ROS_Generator.ROSParser+SubscriberContext")
                    {
                        port_name = child.GetText().ToString();
                    }

                    if (child.GetType().ToString() == "ROS_Generator.ROSParser+TopicContext")
                    {
                        topic = child.GetText().ToString();
                    }

                }
                visitor_component.subscribers.Add(new Subscriber { name = port_name, topic = topic });
            }
            return base.VisitRos_pub_sub(context);
        }


        // Visit Timers
        public override string VisitRos_timer(ROSParser.Ros_timerContext context)
        {
            string timer_name = "";
            double timer_period = 0.0;
            string timer_period_unit = "";
            double timer_offset = 0.0;
            string timer_offset_unit = "";

            foreach (var child in context.children)
            {
                if (child.GetType().ToString() == "ROS_Generator.ROSParser+Timer_nameContext")
                {
                    timer_name = child.GetText().ToString();
                }

                if (child.GetType().ToString() == "ROS_Generator.ROSParser+Timer_periodContext")
                {
                    timer_period = Double.Parse(child.GetText().ToString());
                }

                if (child.GetType().ToString() == "ROS_Generator.ROSParser+Period_unitContext")
                {
                    timer_period_unit = child.GetText().ToString();
                }

                if (child.GetType().ToString() == "ROS_Generator.ROSParser+Timer_offsetContext")
                {
                    timer_offset = Double.Parse(child.GetText().ToString());
                }

                if (child.GetType().ToString() == "ROS_Generator.ROSParser+Offset_unitContext")
                {
                    timer_offset_unit = child.GetText().ToString();
                }
            }
            visitor_component.timers.Add(new Timer { name = timer_name, 
                                                     period = timer_period, 
                                                     period_unit = timer_period_unit, 
                                                     offset = timer_offset, 
                                                     offset_unit = timer_offset_unit });
            return base.VisitRos_timer(context);
        }


    }
}
