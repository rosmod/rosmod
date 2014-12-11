using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ROS_Generator
{
    class Printer
    {

        public void print_ros_node(List<Visitor.ROS_Component> ros_components)
        {
            foreach (var component in ros_components)
            {
                // Print Component Name
                Console.Out.WriteLine("Component: " + component.name);
                Console.Out.WriteLine("---------");

                // Print Provided Services
                if (component.provided_services.Count != 0)
                {
                    Console.Out.WriteLine("Provided Services: ");
                    Console.Out.WriteLine("-----------------");
                    foreach (var service in component.provided_services)
                    {
                        if (service.type == "Provides")
                        {
                            Console.Out.WriteLine("* " + service.name);
                        }
                    }
                }

                // Print Required Services
                if (component.required_services.Count != 0)
                {
                    Console.Out.WriteLine("Required Services: ");
                    Console.Out.WriteLine("-----------------");
                    foreach (var service in component.required_services)
                    {
                        if (service.type == "Requires")
                        {
                            Console.Out.WriteLine("* " + service.name);
                        }
                    }
                }

                // Print Publishers
                if (component.publishers.Count != 0)
                {
                    Console.Out.WriteLine("Publishers: ");
                    Console.Out.WriteLine("----------");
                    foreach (var publishers in component.publishers)
                    {
                        Console.Out.WriteLine("* Port: " + publishers.name + " on Topic: " + publishers.topic);
                    }
                }

                // Print Subscribers
                if (component.subscribers.Count != 0)
                {
                    Console.Out.WriteLine("Subscribers: ");
                    Console.Out.WriteLine("-----------");
                    foreach (var subscriber in component.subscribers)
                    {
                        Console.Out.WriteLine("* Port: " + subscriber.name + " on Topic: " + subscriber.topic);
                    }
                }

                // Print Timers
                if (component.timers.Count != 0)
                {
                    Console.Out.WriteLine("Timers: ");
                    Console.Out.WriteLine("------");
                    foreach (var timer in component.timers)
                    {
                        Console.Out.WriteLine("* Timer: " + timer.name +
                                              "; Period: " + timer.period +
                                              " " + timer.period_unit +
                                              "; Offset: " + timer.offset +
                                              " " + timer.offset_unit);
                    }
                }
                Console.Out.WriteLine("\n");
            }
        }
    }
}
