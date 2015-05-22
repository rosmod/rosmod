Modeling Language
=================

ROSMOD Projects are built using the ROSMOD Modeling Language. The tool suite provides a Graphical User Interface to build these models but the state and configuration properties of the project are saved in a set of text files (*models*) that follow a strict set of grammatical rules. The grammatical rules were written using `Antlr 4 <http://www.antlr.org/>`_ and the models are loaded using the generated parser, lexer and listener classes. This section briefly describes the ROSMOD Textual Modeling Language, along with sample model files. 

Software Model
--------------

The Software model completely describes a ROS Workspace. 

.. image:: ./_images/Software_Model.png
   :scale: 70 %
   :align: center

As shown in the above metamodel, every ROS workspace contains one or more ROS packages. Each ROS package contains (1) messages, (2) services and (3) components. Multiple communicating components are used to realize a domain-specific application e.g. an *Image Processor* component can periodically fetch the webcam feed from a *Camera* component and perform image processing tasks. The data structures, ports and timers of these components are described in this Software Model. How these components are grouped into ROS nodes and deployed on specific hardware is described using the :ref:`Deployment Model <deployment_model>`.  

Messages
^^^^^^^^

Messages are data structures used by Publishers and Subscribers. The data structure of messages is described in .msg files. ROSMOD Messages are constructed identical to the textual language used in `ROS msg files <http://wiki.ros.org/msg>`_. Here is a sample msg file:

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

Services are data structures used by Clients and Servers. The data structure for services is described in .srv files. ROSMOD Services are also constructed identical to the textual langugage used in `ROS srv files <http://wiki.ros.org/srv>`_ by ROS. Here is a sample srv file:

.. code-block:: c

   int64 thruster_id
   float64 amount
   float64 duration
   ---
   int64 return_value

Components
^^^^^^^^^^

Valid ROSMOD Packages contain one or more *components*. Each component has zero or more ports/timers. In the following example, the *three_component_example* package has three components: Component_1, Component_2 and Component_3. Component_1 has 

* Publisher: *Name_Publisher* publishing on the *ComponentName* message
* Subscriber: *Name_Subscriber* subscribing to the *ComponentName* message
* Timer: *Timer_1* with a period of 0.5 seconds

Notice that the subscriber and the timer have a *priority* and a *deadline* property. These properties are used by the component message queue when enqueuing requests from external entities. When the component scheduling scheme is PFIFO, the request with the highest priority is chosen first for processing. Similarly, when the scheduling scheme is EDF, the request with the earliest relative deadline is chosen first for processing. So, different callbacks/operations in the same component compete for a processing order. By default, this scheduling scheme is FIFO.

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

.. image:: ./_images/Hardware_Model.png
   :scale: 80 %
   :align: center

Deployment Model
----------------

.. _deployment_model:

.. image:: ./_images/Deployment_Model.png
   :scale: 80 %
   :align: center
