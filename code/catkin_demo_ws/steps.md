ROS Orbiter Demo:
=================

This file describes the ROS version of the Orbiter demo: how it works and how to make it from scratch.  


Description:
------------
The orbiter demo consists of many components/actors grouped into applications.  We will map actors to ROS NODES and applications to ROS PACKAGES.  


Steps:
------

It is assumed the ROS is already installed on the system.  To create this demo (and set up the workspace) follow these instructions:

(if you haven't done so, tell ROS what the editor you want to use is):
http://wiki.ros.org/ROS/Tutorials/UsingRosEd

To initialize the workspace:
http://wiki.ros.org/ROS/Tutorials/InstallingandConfiguringROSEnvironment

To create the package (which is what we'll call the applications):
http://wiki.ros.org/ROS/Tutorials/CreatingPackage

To create the definitions for the messages (pub/sub) and services (rmi):
http://wiki.ros.org/ROS/Tutorials/CreatingMsgAndSrv

To write the pub/sub code:
http://wiki.ros.org/ROS/Tutorials/WritingPublisherSubscriber%28c%2B%2B%29