Modeling Language
=================

The ROSMOD Modeling Language is both textual and graphical. The tool suite provides a Graphical User Interface to build these models but the state and configuration properties of the ROSMOD Project are saved in a set of text files (*models*) that follow a strict set of grammatical rules. The grammatical rules were written using `Antlr 4 <http://www.antlr.org/>`_ and the models are loaded using the generated parser, lexer and listener classes. This section briefly describes the ROSMOD Textual Modeling Language, along with sample model files.

Software Model
--------------

The Software model completely describes a ROS Workspace.

As shown in the above class diagram, every ROS workspace contains one or more ROS packages. Each ROS package contains one or more (1) messages, (2) services and (3) components. In ROSMOD, such packages are built using the Software Modeling Language. 

Messages
^^^^^^^^

Messages are constructed identical to the textual language used in `.msg files <http://wiki.ros.org/msg>`_ by ROS. Here is a sample msg file:

.. code-block:: c

   int64 sat_id
   float64 inclination
   float64 longitudeAN
   float64 argumentOfPeriapsis
   float64 eccentricity
   float64 semimajorAxis
   float64 meanAnomaly

Services
^^^^^^^^

Services are also constructed identical to the textual langugage used in `.srv files <http://wiki.ros.org/srv>`_ by ROS. Here is a sample srv file:

.. code-block:: c

   int64 thruster_id
   float64 amount
   float64 duration
   ---
   int64 return_value

Components
^^^^^^^^^^

.. code-block:: c

   /*
    * ROSMOD Software Model
    */

   // ROSMOD Package - three_component_example
   package three_component_example 
   {
     // ROSMOD Component - Component_1
     component Component_1 : Base 
     {
       // ROSMOD Publisher - Name_Publisher
       publisher <three_component_example/ComponentName> Name_Publisher;
       // ROSMOD Subscriber - Name_Subscriber
       subscriber <three_component_example/ComponentName> Name_Subscriber 
       {
         priority = 50;
         deadline = 0.300;
       }       
       // ROSMOD Timer - Timer_1
       timer Timer_1 
       {
	  period = 0.5;
	  priority = 50;
	  deadline = 0.200;
       }       
     }

     // ROSMOD Component - Component_2
     component Component_2 : Base 
     {
       // ROSMOD Server - Service_Server
       server <three_component_example/ComponentService> Service_Server 
       {
         priority = 50;
         deadline = 0.500;
       }       
       // ROSMOD Publisher - Name_Publisher
       publisher <three_component_example/ComponentName> Name_Publisher;
       // ROSMOD Timer - Timer_2
       timer Timer_2 
       {
	  period = 1.0;
	  priority = 50;
	  deadline = 0.200;
       }       
     }

     // ROSMOD Component - Component_3
     component Component_3 : Base 
     {
       // ROSMOD Client - Service_Client
       client <three_component_example/ComponentService> Service_Client;
       // ROSMOD Timer - Timer_3
       timer Timer_3 
       {
	  period = 2.0;
	  priority = 50;
   	  deadline = 0.100;
       }        
     }
   }


Hardware Model
--------------

Deployment Model
----------------

