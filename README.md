# ROSMOD - Quick Start Guide

## Clone Repository

$ git clone --recursive http://github.com/pranav-srinivas-kumar/rosmod-0.3

## This Repository contains:

* ROSMOD GUI v0.3
* ROSMOD Component Model
* Test Samples

## Dependencies

* Antlr Python2 Runtime 4.4.1
* Cheetah 2.4.4
* Fabric 1.10.1
* TermEmulator 1.0
* wxPython 2.8
* dtach

### How to Install Dependencies

```bash
$ cd <REPOSITORY_HOME>/utils
$ sudo ./dependencies.py
```

## How to start ROSMOD GUI

```bash
$ cd <REPOSITORY_HOME>/gui
$ python __main__.py
```

### Create an alias in .bashrc 

* Add the following line to ~/.bashrc

```bash
alias rosmod='python ~/PATH_TO_REPOSITORY_HOME/gui/__main__.py'
```

## ROSMOD Communication Layer

* The ROSMOD Communication Layer is a modified version of the ros_comm communication layer in ROS. 
* ROSMOD introduces PFIFO and EDF-based scheduling schemes to the ROS callback queue.
* ROSMOD also facilitates deadline monitoring i.e. deadline violation detection and logging for all callbacks.

```bash
$ cd <REPOSITORY_HOME>/comm
$ catkin_make -DCMAKE_INSTALL_PREFIX=/opt/ros/<ROS_VERSION> install
```