William
=======
* Source code study - Threading behavior
* An example for what the generated code is going to be
* Sign up as ROS developer - Get access to mailing list

Pranav
======
* Get ROS source code compiling
* GME Meta Model of ROS Component Model semantics
* Sign up as ROS developer - Get access to mailing list

Examples
========
* RMI Example - Two components on two different nodes (even different computers) showing RMI interaction
* Pub/Sub Example
* AMI Example - clear, working example using actionlib

General Design
==============
* Facet/Receptacle
* Component Lifecycle Management - Parameter server and state management
* Init method in the modeling tools - Like having a timer that fires at time zero - Must manifest as a message on the callback queue 
* Errno setting for RMI stuff - How do we return errors?
* Component Threading Behavior 
* Actor <-> ROS Node mapping
* Deadling Monitoring
* Priority Queuing in Component-level scheduling
* What is an operation?
* Network Management for queue operations, Network interface for components (to manage n/w resources and profiles and what not)
* Don't worry about temporal partitioning or DM
