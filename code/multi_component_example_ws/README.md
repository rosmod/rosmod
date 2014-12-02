Multi-Component Example:
========================

Description:
------------
This example shows a way of running multiple components inside a node in an object-oriented fashion.  Each component has its own component operation queue.  The code has been written and commented in such a way as to show which parts are generated each time, which parts are generated based on the model, and which parts much be written by the developer.  

Messages:
---------
ComponentName is the only type of message of the form

	      string name

Services:
---------
ComponentService is the only type of service and provides the following interface:

		 string operation
		 ---
		 string name

Nodes:
------
NodeMain is the node executable.  It contains three components (described below), which each have run in their own threads.  The node itself only initializes the ros node (ros::init()) and then starts the components in their own threads.  

NodeMain requires command line arguments:

	 nodeMain <node name>

Components:
-----------
Component0 publishes once (in Init) on the ComponentName topic, and subscribes to the topic.  It also has a timer (Timer0) which does nothing but print to the console.  

Component1 has a timer (Timer0) in which it publishes to the ComponentName topic.  It also provides a service of ComponentService.  In this service it also publishes on the ComponentName topic.

Component2 has a timer (Timer0) in which it invokes the ComponentService service.  