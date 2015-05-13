# ROSML Editor V2
=================

A ROSML model can be viewed from three aspects -
  * ROS Packages (have message definitions, service definitions, node definitions, ROSML components definitions)
  * Hardware (has description of hardware platform(s) or **host(s)** on which instances of ROS package will run)
  * Deployment (has information about which instance of ROS package will run on which hardware)

  This codebase controls the graphical editor for generating *.rml, *.rhw and *.rdp files for your ROSML model. These files describe the three aspects of a ROSML model - **ROSML Packages**, **Hardware** and **Deployment** respectively. Thus, the editor allows for the creation, alteration, and deletion of elements within a ROSML model such as - 

  1. The Project
  2. The workspace
  3. Packages
  4. Messages
  4. Services
  5. Component definitions
  6. Component timers
  7. Component message subscribers
  8. Component message publishers
  9. Component service providers (servers)
  10. Component service users (clients)
  11. Nodes
  12. Node component instances
  13. Hardware characteristics of host(s)
  14. Deployment of ROSML Components on host(s)

Furthermore, the editor allows for the interpretation and analysis of the ROSML model. Interpretation of the model involves generating component based skeleton code following the design of the model. Analysis of the model entails either analyzing the model's network quality of service characteristics or analyzing the business logic model's timing and scheduling characteristics.
  
This document gives guidelines to create a ROSML model in all the three aspects, generating the code and deploying it on the hosts. 

## Using the Editor
-------------------
This section describes how to use the ROSML editor to create, open, edit, save, and interpret a ROSML model files.  Most of the user interface for the editor is based around right-click context menus which provide the relevant actions according to the given context.  

The main operations covered in these subsections are

  * [Starting the ROSML Editor](#starting-the-editor)
  * [Creating a New Model in "Packages" Aspect](#creating-a-model-in-"Packages"-aspect)
  * [Creating a New Model in "Hardware" Aspect](#creating-a-model-in-"Hardware"-aspect)
  * [Creating a New Model ]
  * [Opening an Existing Model](#open-an-existing-model)
  * [Editing a Model](#editing-a-model)
  * [Saving a Model](#saving-a-model)
  * [Generating Code for a Model](#generating-code-for-a-model)
  * [Deploying a Model on Hardware](#deploying-a-model-on-hardware)

### Starting the Editor
-----------------------
Before starting the editor, you must [setup the ROSML editor](../README.md) in your system.

For starting the editor, execute the `rosmodv2.py` python script in a shell. 
  
Note: to initialize a model, you must either [Create a new model](#creating-a-model) or [Open an existing model](#open-an-existing-model).

### Creating a Model in "Packages" Aspect
-----------------------------------------

A ROSML model can be viewed from three aspects -
  * ROS Packages
  * Hardware
  * Deployment 
  
This subsection will guide you through creating various entities in a ROSML model in "Packages" aspect.

#### Creating a Workspace
-------------------------

For creating a ROSML model, you must create a ROSML workspace which will serve as a platform to create ROSML components, describe the hardware platform(s) on which they are going to be deployed and deployment scheme which describes which node is deployed on which hardware. 

For creating a Workspace in ROSML editor -
  1. First, open the ROSML editor by executing `rosmodv2.py`.
  2. Click on "New" ![New icon](./icons/toolbar/tnew.png "New") button on toolbar.
  3. Browse the directory in which you want ROSML editor to create the new model.
  4. Enter the name of the model.

#### Creating and Removing Packages
-----------------------------------

In this newly created workspace, you would now create a new Package which will correspond to a ROS package.

For creating a package in ROSML workspace -
  1. First, create a ROSML workspace.
  2. Click on "New Package" ![New icon](./icons/toolbar/tnew.png "New") button on toolbar.
  3. Enter the name of the package.
  
To remove a package from your ROSML workspace -
  1. Click on "Delete Package" ![Delete icon](./icons/toolbar/texit.png "Exit") button on toolbar.
  OR
  1. Right-click on the package pane.
  2. Click on the "Delete" option in the menu.

#### Adding and Removing Services and Messages
----------------------------------------------

ROS nodes communicate with each other using Services and Messages.

For adding a message (or a service) in your ROSML model -
  1. Right-click on the package pane.
  2. Click on the "Add Message" (or "Add Service") option in the menu.
  3. In the window that appears, give a name to the Message (Service) and specify the fields in your Message (or Service).

For removing a message (or a service) from your ROSML model -
  1. Right-click on the Message (or the Service) that you want to remove.
  2. Click on the "Delete" option in the menu.

#### Adding and Removing Components
-----------------------------------

ROSML components are building blocks of the ROSML [Component model](../../../docs/ComponentModelOverview.md). A component can be seen as an entity having terminals of various types such as publisher, subscriber, server, clients. Components can be associated with each other on these terminals based on their type, e.g. a publisher terminal of a component can be associated with the subscriber terminal of another component. To describe these associations, names of the messages and services are used. For example, a component having publisher terminal of message A can be thought of connected with another component having subscriber terminal of message A.

For adding a component in your ROSML model -
  1. Right-click on the package pane.
  2. Click on the "Add Component Definition" option in the menu.
  3. In the window that appears, give a name to the component.

For removing a component from your ROSML model -
  1. Right-click on the component that you want to remove.
  2. Click on the "Delete" option in the menu.

##### Adding and Removing Objects to and from Components
--------------------------------------------------------

ROSML components can contain 5 types of objects -
  * Publisher
  * Subscriber
  * Server
  * Client
  * Timer

For adding these objects to any component -
  1. First, select the component in which you want to add a new object by left-clicking on it. After left-clicking, it will be highlighted with red border.
  2. Right-click on the highlighted object.
  3. Select the appropriate object type you want to add into that component.
  4. Give a name to the object which you want to add.
  5. Additionally -
	* For Publishers and Subscribers, give reference to the message object with which you want to associate the Publisher/Subscriber.
	* For Servers and Clients, give reference to the service object with which you want to associate the Server/Client.
	* For Timers, give period value and unit of the period value.

For removing the objects from any component -
  1. First, select the object which you want to remove from the component by left-clicking on it. After left-clicking, it will be highlighted with red border.
  2. Right-click on the highlighted object.
  3. Click on "Delete" option in the menu.

#### Adding and Removing Nodes
------------------------------

ROSML nodes are collection of components' instances and they correspond to ROS nodes.

For adding a node to your ROSML model -
  1. Right-click on the package pane.
  2. Click on the "Add Node" option in the menu.
  3. In the window that appears, give a name to the node.

For removing a node from your ROSML model -
  1. First, select the node which you want to remove by left-clicking on it. After left-clicking, it will be highlighted with red border.
  2. Right-click on the highlighted node.
  3. Click on "Delete" option in the menu.

##### Adding and Removing Component Instances to and from Nodes
--------------------------------------------------------------

For adding a component instance to a node in your ROSML model -
  1. First, select the node in which you want to add a component instance by left-clicking on it. After left-clicking, it will be highlighted with red border.
  2. Right-click on the highlighted node.
  3. Click on "Add Component Instance" option in the menu.
  4. Give a name to the component instance that you want to add.
  5. Give the reference to the component object which you want to create instance of.

For removing a component instance from a node in your ROSML model -
  1. First, select the component instance which you want to remove from the node by left-clicking on it. After left-clicking, it will be highlighted with red border.
  2. Right-click on the highlighted component instance.
  3. Click on "Delete" option in the menu.

### Creating a Model in "Hardware" Aspect
-----------------------------------------

A ROSML model can be viewed from three aspects -
  * ROS Packages
  * Hardware
  * Deployment 
  
This subsection will guide you through creating various entities in a ROSML model in "Hardware" aspect.

The **Hardware** aspect of a ROSML model specifies the hardware platforms available and attributes associated with them (such as their IP addresses and their architecture e.g. armv7l or x86).

#### Adding and Removing Hosts in "Hardware" Aspect
---------------------------------------------------

For adding a host in "Hardware" aspect in a ROSML model -
  1. First, select the "Hardware" aspect in the "Aspects" menu on top.
  2. Right-click on the hardware pane (empty area of "Hardware" aspect).
  3. Click on "Add Host" option in the menu.
  4. Specify the attributes of the host such as Name, IP address and architecture in the window that will appear.
     (The architecture type is going to be used for the compilation of package code using a cross-compiler installed on the system)

For removing a host in "Hardware" aspect from a ROSML model -
  1. First, select the "Hardware" aspect in the "Aspects" menu on top.
  2. Right-click on the host that you want to delete.
  3. Click on "Delete" option in the menu.

### Creating a Model in "Deployment" Aspect
-------------------------------------------

A ROSML model can be viewed from three aspects -
  * ROS Packages
  * Hardware
  * Deployment 
  
This subsection will guide you through creating various entities in a ROSML model in "Deployment" aspect.

The **Deployment** aspect of the ROSML model declares instances of hardware platforms described in **Hardware** and binds instances of nodes described in **ROS Packages** with them. An instance of a hardware platform can run one or multiple instances of node(s). The **Deployment** aspect can also contain groups of ROS nodes instances running on hardware platform instances, to precisely describe which ROS nodes interact with which ones. This is to avoid ambiguity in communication between multiple instances of similar ROS nodes deployed across the system. E.g. Instance A and Instance B of a ROS publisher node named "pub" want to interact with Instance C and Instance D of a ROSML subscriber node named "sub" (of the same topic) respectively. But, they do not want to broadcast their messages to any other instances of ROSML subscriber node "sub". In implementing such a scenario, you would want to put Instance A and Instance C into one group, and Instance B and Instance D into another group.

#### Adding and Removing Host Instances in "Deployment" Aspect
--------------------------------------------------------------

For adding a host instance in "Deployment" aspect in a ROSML model -
  1. First, select the "Hardware" aspect in the "Aspects" menu on top.
  2. Right-click on the hardware pane (empty area of "Deployment" aspect).
  3. Click on "Add Host Instance" option in the menu.
  4. There are several attributes of a host instance (other than 'name' and 'host_reference') which can be specified in the window that will appear -
	1. username - Username of the user under which you want to run the node instances you are going to add in the next step
	2. ssh_key - For running ROS nodes on the host, ROSML editor has to ssh to the node and run node processes. For that it will need the private ssh key of the host. In 'ssh_key' option, the absolute path to the private ssh key is provided.
	3. init - __TODO__
	4. env_variables - __TODO__
	5. You can add one or more node instance(s) which can be run on this host instance by following the steps given below.
	6. You can also launch an ssh terminal from the editor on the host instance by right-clicking on it and selecting the option "Open SSH Terminal".

For removing a host instance in "Deployment" aspect from a ROSML model -
  1. First, select the host instance which you want to remove by left-clicking on it. After left-clicking, it will be highlighted with red border.
  2. Right-click on the highlighted host instance.
  3. Click on "Delete" option in the menu.

##### Adding and Removing Node Instances in Host Instances
----------------------------------------------------------

For adding a node instance in a host instance -
  1. First, select the host instance by left-clicking on it. After left-clicking, it will be highlighted with red border.
  2. Right-click on the highlighted host instance.
  3. Select "Add Node Instance" to open a pop-up window, in which attributes such as name of the node instance, reference of the node and the command-line arguments which are to be provided while executing the node instance can be given.

For removing a node instance from a host instance -
  1. First, select the node instance which you want to remove by left-clicking on it. After left-clicking, it will be highlighted with red border.
  2. Right-click on the highlighted node instance.
  3. Click on "Delete" option in the menu.

### Opening an Existing Model
-----------------------------

For opening an existing model in the ROSML editor -
  1. First, open the ROSML editor by executing `rosmodv2.py`.
  2. Click on "Open" ![Open icon](./icons/toolbar/topen.png "Open") button on toolbar.
  3. Browse to the directory in which you have your existing ROSML model.
  4. You should be able to see the following contents in the directory -
	1. 01-Software-Configuration - The directory containing .rml files, which describe ROSML components in your model
	2. 02-Hardware-Configuration - The directory containing .rhw files, which describe hardware on which you would want to run your ROSML model
	3. 03-Deployment - The directory containing .rdp files, which describe mapping between ROSML components and hardware described in .rhw files
	4. ROSMOD file - The file bearing the name of the project and having .rosmod extension
  5. Select the file with .rosmod extension and click on "Open".

### Editing a Model
-------------------

For editing a model in ROSML editor, [create a new model](#creating-a-model) or [open an existing model](#open-an-existing-model) and add/remove/edit the ROSML components in the model as per your requirements.

### Saving a Model
------------------

For saving a model in ROSML editor -
  1. First, [create a new model](#creating-a-model) or [open an existing model](#open-an-existing-model).
  2. Click on "Save" ![Save icon](./icons/toolbar/tsave.png "Save") button on toolbar.

### Generating code for a Model
-------------------------------

The ROSML editor can be used for not only developing ROSML models but also generating code which can be run on a [ROS](http://wiki.ros.org/ROS) platform. This means that the ROSML projects are ultimately translated into [ROS packages](http://wiki.ros.org/ROS/Tutorials/CreatingPackage) written in C++ (as we know, a ROS package can be developed in C++ or Python). The generated code can then be build against the target hardware platform, and be deployed on the same (discussed in the next section).

For generating ROS code for a ROSML model in the editor -
  1. First, [create a new model](#creating-a-model) or [open an existing model](#open-an-existing-model).
  2. Select the "Packages" aspect from "Aspects" dropdown menu.
  3. Go to "Tools" dropdown menu, and click on "Generate ROS code" (or alternatively use hotkey **Ctrl+G**).
  
After completing the above steps, the confirmation box saying "Generated ROS Workspace" should appear. The generated code can be found in the "01-Software-Configuration" sub-directory in the Project directory. Previously, the "01-Software-Configuration" directory contained only .rml file describing the ROSML model. After completing the steps for generating the code, it should additionally contain another directory named after the name of the workspace (default name is "Workspace") which contains fully populated ROS workspace, as discussed in [here](http://wiki.ros.org/ROS/Tutorials/CreatingPackage#ROS.2BAC8-Tutorials.2BAC8-catkin.2BAC8-CreatingPackage.Packages_in_a_catkin_Workspace). 

Above mentioned directory (named after ROSML workspace) contains sub-directory called "src" which has sourcecodes associated with all the ROS packages described in your ROSML model. The src sub-directory thus contains directories for individual packages which in turn have CMakeLists.txt (which serves as a toplevel CMake file for a ROS package), include directory (has .hpp files used in the package), msg directory (containing .msg files), srv directory (containing .srv files), package.xml file (which contains information about the package), src directory (containing source code for the package) etc. The overall folder structure looks like following -

```bash
workspace_name
└── src
    ├── package_1
	│	├── CMakeLists.txt
    │   ├── include
    │   │   └── package_1
    │   │       ├── component_1.hpp
    │   │       ├── component_2.hpp
    │   │       ├── Component.hpp
    │   │       ├── Logger.hpp
    │   │       ├── rapidxml.hpp
    │   │       ├── rapidxml_utils.hpp
    │   │       └── xmlParser.hpp
    │   ├── msg
    │   │   └── message_1.msg
    │   ├── package.xml
    │   └── src
    │       └── package_1
    │           ├── component_1.cpp
    │           ├── component_2.cpp
    │           ├── Component.cpp
    │           ├── Logger.cpp
    │           ├── pub_node_1_main.cpp
    │           └── sub_node_1_main.cpp
	├── package_2
	│	...
	├── package_3
	│	...
	.
	.
	.
	└── package_n
```

### Deploying a ROSML Model 
---------------------------

The generated code needs to be deployed on hardware platforms running ROS middleware, before which it needs to be built against the architecture of hardware platform on which it is to be deployed. For building the code against appropriate hardware platform, "Hardware" and "Deployment" aspects of a ROSML model play an important role.

The **ROS Packages** aspect of a ROSML model defines the structure and business logic of the ROS packages which need to run on the targeted hardware platforms.

The **Hardware** aspect of a ROSML model specifies the hardware platforms available and attributes associated with them (such as their IP addresses and their architecture e.g. armv7l or x86).

The **Deployment** aspect of the ROSML model declares instances of hardware platforms described in **Hardware** and binds instances of nodes described in **ROS Packages** with them. After creating this binding, you can deploy your ROS packages (which are built against respective hardware platforms) using two-step mechanism -
  1. Copy deployment files
  2. Deploy system

Copying deployment files will copy all the binaries of ROS packages (which are on the development machine) on the host on which they are going to be executed. This also copies the .xml files which are used to specify the groups of node instances on the hosts.

Deploying system results in the ROSML editor executing ROS nodes on the respective hosts by ssh-ing on to the hosts (using the username and ssh keys provided while creating the host instance). While ROS nodes are executed on the respective hosts, command-line arguments specified while creating node instances are given. ROSML editor uses a python library called (Fabric)[http://www.fabfile.org/] to achieve all the deployment tasks.