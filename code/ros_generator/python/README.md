ROS Workspace Generator
=======================
* rosgen generates a complete ROS (Robot Operating System) workspace from a .rosml model file. 
* The model provides means for abstractly specifying the properties of all ROS packages in the design. This includes specifying the structural properties of all the messages, services and nodes in a ROS package. 
* Once modeled, rosgen uses Antlr4 for grammar parsing and the Cheetah template engine to generate the necessary source files, header files, CMakeLists.txt files and package.xml files. 

Component-based Design
----------------------
* Each ROS node contains one or more instances of a "component" definition. This adds a layer of abstraction within ROS nodes to enable component-based software development. Every component in a ROS node consists of (1) zero or more publisher ports, each publishing on a previously defined ROS msg, (2) zero or more subscriber ports, each subscribing to a previously defined ROS msg, (3) zero or more provided services, each being a server for a previously defined ROS srv, (4) zero or more required services, each being a client for a previously defined ROS srv, and finally (5) one component-level queue that receives event-based triggers to all callback functions in the component. 

* Whenever a component timer expires, a timer callback event is enqueued into the message queue and then handled by the component whenever the component thread is scheduled. Similarly, each time a component service request is received from an external client, the request is enqueued onto the message queue and handled when the component is ready. One of the motivating factors to including this semantic is to allow for extensions to future queuing behaviors e.g. a priority-based queuing where each enqueue operation on incoming events is based on request priority so that a higher priority incoming event is always handled first. 

Grammar - Robot Operating System Workspace
------------------------------------------
* Every ROS model begins with a declaration of the workspace
```cpp
workspace WORKSPACE_NAME;
```
* Once declared, a developer can start describing ROS packages as follows
```cpp
/*
 * This is a comment
 */
package PACKAGE_NAME
{
    // Start of message descriptions
    // Comment out region if there are no messages
    messages
    {
        msg MESSAGE_NAME
        {
            // All msg fields here
        }
    }
    // End of message descriptions
    
    // Start of service descriptions
    // Comment out region if there are no services
    services
    {
        srv SERVICE_NAME
        {
            request 
            {
                // All request arguments here
            }
 
            response
            {
                // All response arguments here
            }
        }
    }
    // End of message descriptions

    // Start of component descriptions
    components
    {
        component COMPONENT_NAME
        {
            // Publisher port
            publisher<MSG_NAME> PUBLISHER_NAME;

            // Subscriber port
            subscriber<MSG_NAME> SUBSCRIBER_NAME;             
	    
            // Service provision - Server
            provides SERVICE_NAME;

            // Service requirement - Client
            requires SERVICE_NAME;

            timer TIMER_NAME
            {
                // Timer period
                period = TIMER_PERIOD;
            }
        }
    }
    // End of component descriptions

    // Start of node descriptions
    nodes
    {
        node NODE_NAME
        {
            // Instantiate previously defined components here
            component<COMPONENT_DEFINITION> COMPONENT_INSTANCE_NAME;
        }
    }
    // End of node descriptions
} 
```

Example Model File
------------------
* The following example can be found here: https://github.com/pranav-srinivas-kumar/ROS-Workspace-Generator/blob/master/tests/sample_1/input.rosml
* This example workspace consists of one ROS package called "package_1".
* package_1 contains:
  * ROS msg - ComponentName
  * ROS srv - ComponentService
  * Three ROS Components: Component_1, Component_2 & Component_3
  * ROS node - my_node instantiating the above component threads

```cpp
workspace ROS_Workspace;

//New ROS Package - Package_1
package package_1
{
    // Set of ROS messages
    messages 
    {
        // ROS msg - ComponentName
        msg ComponentName
        {
            string name;	    
        }
    }

    // Set of ROS services
    services
    {
        // ROS srv - ComponentService
        srv ComponentService
        {
             request
             {
                string operation;
             }

             response
             {
                string name;
             }		
        }
    }

    // Set of ROS Components
    components
    {
        // ROS component - Component_1
        component Component_1
        {
            publisher<ComponentName> Component_1_publisher;
            subscriber<ComponentName> Component_1_subscriber;	
	
            timer Timer0
            {
                period = 0.5s;	        		
            }
        }

        // ROS component - Component_2
        component Component_2
        {
            provides ComponentService;
            publisher<ComponentName> Component_2_publisher;	
	
            timer Timer0
            {
                period = 1.0s;	        		
            }
        }

        // ROS component - Component_3
        component Component_3
        {
            requires ComponentService;
	
            timer Timer0
            {
                period = 2.0s;	        		
            }
        }
    }

    // Set of ROS Nodes in this package
    nodes 
    {
        node my_node
        {
            // Instantiating components in ROS node
            component<Component_1> Component_1_i;
            component<Component_2> Component_2_i;
            component<Component_3> Component_3_i;	
        }
    }

}
```

Generating ROS Workspace
------------------------
* Once a ROS model is complete, its time to generate the designed ROS workspace.
* From the terminal, run:
```bash
$ alias rosgen='python <PATH_TO_ROS_GENERATOR_DIRECTORY>/rosgen.py'
$ cd <DIRECTORY_CONTAINING_MODEL_FILE>
$ rosgen <filename>.rosml
```

* This will generate a ROS workspace is the same directory as the model file:

![Alt text](https://github.com/finger563/rosmod/blob/master/code/ros_generator/python/screenshots/rosgen.png?raw=true "ROS Workspace Generator")

Component Skeleton
------------------

**For each component in the model, a skeleton is generated. This is the skeleton for Component_2:**

![Alt text](https://github.com/finger563/rosmod/tree/master/code/ros_generator/python/screenshots/skeleton.png?raw=true "ROS Workspace Generator")

Business Logic
--------------

**It is the developer's responsibility to add "business logic" to the generated callback functions. For Component_2, the following is a sample business logic for the skeleton functions:**

![Alt text](https://github.com/finger563/rosmod/tree/master/code/ros_generator/python/screenshots/skeleton_2.png?raw=true "ROS Workspace Generator")

**After filling out all the callback functions, run "catkin_make" to build all ROS packages in the workspace.**

![Alt text](https://github.com/finger563/rosmod/tree/master/code/ros_generator/python/screenshots/rosgen_2.png?raw=true "catkin_make") 

**If the build is successful, run the ROS node**

**Terminal 1**
```bash
$ roscore
```
**Terminal 2 - Inside generated workspace**
```bash
$ source ./devel/setup.sh
$ rosrun package_1 my_node
```

Example ROS node execution
--------------------------
This example workspace (with business logic) can be found at:

https://github.com/finger563/rosmod/tree/master/code/ros_generator/python/tests/sample_1/After_Business_Logic/ROS_Workspace

![Alt text](https://github.com/finger563/rosmod/tree/master/code/ros_generator/python/tests/sample_1/screenshot.png?raw=true "ROS Workspace Generator") 

Quick Start Guide
=================

Setup Antlr4
------------
**Note**: This step is optional as the ROS lexer, parser and listener python classes are included with ROS.g4

```bash
$ cd /usr/local/lib
$ sudo curl -O http://www.antlr.org/download/antlr-4.4-complete.jar
$ export CLASSPATH=".:/usr/local/lib/antlr-4.4-complete.jar:$CLASSPATH"
$ alias antlr4='java -jar /usr/local/lib/antlr-4.4-complete.jar'
$ alias grun='java org.antlr.v4.runtime.misc.TestRig'
```

Generate ROS Parser, Lexer and Listener:
---------------------------------------
**Note**: This step is optional as the ROS lexer, parser and listener python classes are included with ROS.g4

```bash
$ cd 
$ antlr4 -Dlanguage=Python2 ROS.g4
```

Install the Antlr4 Python Target:
--------------------------------
**Note**: This step is optional as the antlr4 python target is included with ros_generator

1. Download from: https://pypi.python.org/pypi/antlr4-python2-runtime/
2. Extract antlr4-python2-runtime-4.4.1.tar.gz

```bash
$ cd antlr4-python2-runtime-4.4.1.tar.gz
$ sudo python setup.py install
```

Install the Cheetah Templating Engine: 
-------------------------------------
1. Download from: https://pypi.python.org/pypi/Cheetah/2.4.4
2. Extract Cheetah-2.4.4.tar.gz
```bash
$ cd Cheetah-2.4.4
$ sudo python setup.py install
```

Setup alias for rosgen.py
-------------------------
```bash
alias rosgen='python <PATH_TO_ROS_GENERATOR_DIRECTORY>/rosgen.py'
```

Using rosgen:
------------
```bash
$ cd <DIRECTORY_CONTAINING_MODEL_FILE>
$ rosgen <filename>.rosml
```


