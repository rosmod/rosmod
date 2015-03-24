# ROSML Editor GUI V2
=====================
This codebase controls the graphical editor for generating *.rosml, *.roshw and *.rosdp files for your ROSML application based on the Component Model discussed earlier. It allows for the creation, alteration, and deletion of elements within a ROSML application such as - 

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

Furthermore, the editor allows for the interpretation and analysis of the ROSML model.  Interpretation of the model involves generating component based skeleton code following the design of the model.  Analysis of the model entails either analyzing the model's network quality of service characteristics or analyzing the business logic model's timing and scheduling characteristics.  

## Using the Editor
-------------------
This section describes how to use the ROSML editor to create, open, edit, save, and interpret a ROSML model files.  Most of the user interface for the editor is based around right-click context menus which provide the relevant actions according to the given context.  

The main operations covered in these subsections are

  * [Starting the ROSML editor](#starting-the-editor)
  * [Creating a New Model](#creating-a-model)
  * [Opening an Existing Model](#open-an-existing-model)
  * [Editing a Model](#editing-a-model)
  * [Saving a Model](#saving-a-model)
  * [Generating Code for a Model](#generating-code-for-a-model)

### Starting the editor
-----------------------
Before starting the editor, you must [setup the ROSML editor](../README.md) in your system.

For starting the editor, execute the `rosmodv2.py` python script in a shell. 
  
Note: to initialize a model, you must either [Create a new model](#creating-a-model) or [Open an existing model](#open-an-existing-model).

### Creating a Model
--------------------

A ROSML model can be viewed from three aspects -
  * ROS Package (has message definitions, service definitions, node definitions, ROSML components definitions)
  * Hardware (has description of hardware platform(s) or **host(s)** on which instances of ROS package will run)
  * Deployment (has information about which instance of ROS package will run on which hardware)
  
This subsection will guide you through creating various entities in a ROSML model.

#### Creating a workspace
-------------------------

For creating a ROSML model, you must create a ROSML workspace which will serve as a platform to create ROSML components, describe the hardware platform(s) on which they are going to be deployed and deployment scheme which describes which node is deployed on which hardware. 

For creating a Workspace in ROSML editor -
  1. First, open the ROSML editor by executing `rosmodv2.py`.
  2. Click on "New" ![New icon](./icons/toolbar/tnew.png "New") button on toolbar.
  3. Browse the directory in which you want ROSML editor to create the new model.
  4. Enter the name of the project.

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
  4. Click on "OK".

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
  4. Click on "OK".

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
  2. Then right-click on the highlighted object.
  3. Select the appropriate object type you want to add into that component.
  4. Give a name to the object which you want to add.
  5. Additionally -
	* For Publishers and Subscribers, give reference to the message object with which you want to associate the Publisher/Subscriber.
	* For Servers and Clients, give reference to the service object with which you want to associate the Server/Client.
	* For Timers, give period value and unit of the period value.
  6. Click on "OK".

For removing the objects from any component -
  1. First, select the object which you want to remove from the component by left-clicking on it. After left-clicking, it will be highlighted with red border.
  2. The right-click on the highlighted object.
  3. Click on "Delete" option in the menu.

#### Adding and Removing Nodes
------------------------------

ROSML nodes are collection of components' instances and they correspond to ROS nodes.

For adding a node to your ROSML model -
  1. Right-click on the package pane.
  2. Click on the "Add Node" option in the menu.
  3. In the window that appears, give a name to the node.
  4. Click on "OK".

For removing a node from your ROSML model -
  1. First, select the node which you want to remove by left-clicking on it. After left-clicking, it will be highlighted with red border.
  2. The right-click on the highlighted node.
  3. Click on "Delete" option in the menu.

##### Adding and Removing Component Instances to and from Nodes
--------------------------------------------------------------

For adding a component instance to a node in your ROSML model -
  1. First, select the node in which you want to add a component instance by left-clicking on it. After left-clicking, it will be highlighted with red border.
  2. Then right-click on the highlighted node.
  3. Click on "Add Component Instance" option in the menu.
  4. Give a name to the component instance that you want to add.
  5. Give the reference to the component object which you want to create instance of.
  6. Click on "OK".

For removing a component instance from a node in your ROSML model -
  1. First, select the component instance which you want to remove from the node by left-clicking on it. After left-clicking, it will be highlighted with red border.
  2. Then right-click on the highlighted component instance.
  3. Click on "Delete" option in the menu.

### Opening an Existing Model
-----------------------------

### Editing a Model
-------------------

### Saving a Model
------------------

### Generating code for a Model
-------------------------------
