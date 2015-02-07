ROS Orbiter Demo:
=================
This file describes the ROS version of the Orbiter demo: how it works and how to make it from scratch.  


Description:
------------
The orbiter demo consists of many components/actors grouped into applications.  We will map actors to ROS NODES and applications to ROS PACKAGES.  


Steps:
------
It is assumed the ROS is already installed on the system.  If it is not installed, you can install it by following:
http://wiki.ros.org/indigo/Installation/Ubuntu

To create this demo (and set up the workspace) follow these instructions:

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

To write the RMI code:
http://wiki.ros.org/ROS/Tutorials/WritingServiceClient%28c%2B%2B%29

To configure the VMs:
* copy binaries to all VMs
* ensure virtualbox VMs are connected through host-only networking
* install ros-indigo-ros-base and edit your .bashrc ( http://wiki.ros.org/indigo/Installation/Ubuntu )
* ensure each vm can ping the others
* start roscore on one VM
* ( http://wiki.ros.org/ROS/NetworkSetup )
* ( http://wiki.ros.org/ROS/Tutorials/MultipleMachines )
* on each VM, $ export ROS_MASTER_URI=http://<ROSCORE VM IP ADDR>:11311
* on each VM, $ export ROS_IP=<THAT VM'S IP ADDR>