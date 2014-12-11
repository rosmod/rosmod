Steps to install ROS Indigo from source and setup a workspace:
=============================================================

Last Updated: 2014.11.21

(0) Setup your sources.list
===========================
Setup your computer to accept software from packages.ros.org. 
Note: ROS Indigo only supports Saucy and Trusty for debian packages

For Ubuntu 13.10 (Saucy):
------------------------
$ sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu saucy main" > /etc/apt/sources.list.d/ros-latest.list'

For Ubuntu 14.04 (Trusty):
-------------------------
$ sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu trusty main" > /etc/apt/sources.list.d/ros-latest.list'

(1) Set up your Keys
====================
$ wget https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -O - | sudo apt-key add -


(2) Install Bootstrap Dependencies
==================================
There are tools required to facilitate (download and) management of ROS packages and their dependencies:

For Ubuntu:
----------
$ sudo apt-get install python-rosdep python-rosinstall-generator python-wstool python-rosinstall build-essential

Note: If apt complains that the python-support package is not installable, make sure that Ubuntu universe repositories is enabled in /etc/apt/sources.list

(3) Initializing rosdep - Used in Step (5) to resolve dependencies
==================================================================
$ sudo rosdep init
$ rosdep update

(4) Building the Catkin Packages
================================
Note: ROS is in the process of converting to a new build system, catkin, but not all packages have been converted and the two build systems cannot be used simultaneously. Therefore, it is necessary to build the core ROS packages first (catkin packages) and then the rest. 

Create a catkin workspace:
-------------------------
$ mkdir ~/ros_catkin_ws
$ cd ~/ros_catkin_ws

Now, fetch the core packages so we can build them. Using the wstool,

(OPTION 1) Desktop Install (This is the recommended one):
--------------------------------------------------------
$ rosinstall_generator desktop_full --rosdistro indigo --deps --wet-only --tar > indigo-desktop-full-wet.rosinstall

$ wstool init -j8 src indigo-desktop-full-wet.rosinstall

(OPTION 2) For just the bare bones ROS-Comm:
-------------------------------------------
$ rosinstall_generator ros_comm --rosdistro indigo --deps --wet-only --tar > indigo-ros_comm-wet.rosinstall

$ wstool init -j8 src indigo-ros_comm-wet.rosinstall

Note: This will add all of the catkin or wet packages in the given variant and then fetch the sources into the ~/rot_catkin_ws/src directory.

(5) Resolving Dependencies
==========================
Note: Before you can build catkin workspace, make sure that you have all the required dependencies by:

$ wstool init -j8 src indigo-ros_comm-wet.rosinstall

This looks at all the packages in src directory and find all of the dependencies they have. Then, it will recursively install the dependencies. 

(6) Building the Catkin Workspace
=================================
Note: Once it has completed downloading the packages and resolving the dependencies, you are ready to build the catkin packages. Use the catkin_make_isolated command because there are both catkin and plain cmake packages in the base install, when developing on your catkin only workspace you should use catkin/commands/catkin_make

$ ./src/catkin/bin/catkin_make_isolated --install -DCMAKE_BUILD_TYPE=Release

Other CMake Build Types: http://www.cmake.org/cmake/help/v2.8.12/cmake.html#variable:CMAKE_BUILD_TYPE

Note: The Default catkin installation is in ~/ros_catkin_ws/install_isolated. 
If you want to install elsewhere, use the --install-space /opt/ros/indigo argument to your catkin_make_isolated call.

Note: In the above command we are running the catkin_make_isolated command from the catkin source folder because it has not been installed yet, once installed it can be called directly. 

Source setup.bash
-----------------
If you look in the ~/ros_catkin_ws/install_isolated (or whereever you have installed it), you will see a setup.bash file that is generated. Source this file to utilize things installed there. 

$ source ~/ros_catkin_ws/install_isolated/setup.bash

Environment Setup
-----------------
It's convenient if the ROS environment variables are automatically added to your bash session every time a new shell is launched. 

$ sudo echo "source ~/ros_catkin_ws/install_isolated/setup.bash" >> ~/.bashrc
$ source ~/.bashrc

Confirm this worked by opening a new terminal and running:

$ echo $ROS_ROOT
/home/pranavsrinivas/ros_catkin_ws/install_isolated/share/ros

$ echo $ROS_PACKAGE_PATH
/home/pranavsrinivas/ros_catkin_ws/install_isolated/share:/home/pranavsrinivas/ros_catkin_ws/install_isolated/stacks

$ echo $PYTHONPATH
/home/pranavsrinivas/ros_catkin_ws/install_isolated/lib/python2.7/dist-packages:/opt/ros/indigo/lib/python2.7/dist-packages

$ echo $PATH
/home/pranavsrinivas/ros_catkin_ws/install_isolated/bin:/opt/ros/indigo/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games

(7) Eclipse Setup
=================
$ sudo apt-get install eclipse-cdt

Create a new C++ project and point the directory to: ~/ros_catkin_ws/src

(8) Source Directories of Interest
==================================
ROS-Comm Core
-------------
*.h:     ~/ros_catkin_ws/src/ros_comm/roscpp/include/ros
*.cpp:   ~/ros_catkin_ws/src/ros_comm/roscpp/src/libros

Action Lib
----------
*.h:     ~/ros_catkin_ws/src/actionlib/include/actionlib
*.cpp:   ~/ros_catkin_ws/src/actionlib/src






 
