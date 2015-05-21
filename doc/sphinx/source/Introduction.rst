Introduction
============

`ROSMOD <https://github.com/finger563/rosmod>`_ is a tool suite for rapid prototyping component-based software applications using the `Robot Operating System <http://www.ros.org>`_ (ROS). Using ROSMOD, an application developers can create and manage *projects* for distributed real-time embedded systems. Each ROSMOD Project consists of *models* that represent the structure and behavior of the system:

* Software Model : One or more ROS packages in the workspace.
* Hardware Model: One or more Hardware devices.
* Deployment Model: A Mapping between ROS nodes/processes & Hardware devices.

Using these models, ROSMOD can:

* Generate a skeleton ROS workspace, including build system files.
* Preserve already generated work-in-progress ROS workspaces using code-preservation markers.
* Generate deployment-specific XML files for ROS node lifecycle management. 
* Generate timing analysis models from abstract business logic specification.
* Perform network analysis to admit/reject applications based on network profiles.

ROSMOD significantly improves the time taken to prototype ROS packages since much of the *boring* skeleton code e.g. port & timer initialization, callbacks for timers, servers & subscribers etc. are automatically generated from the Software Model. Once generated, a developer need only add the *"business logic"* of the generated callbacks to complete the package. A detailed tutorial for this workflow can be found :ref:`here<tutorial>`.

Component-based Software Development
------------------------------------

Software development using ROSMOD is inspired by the principles of Component-based Software Engineering. Design and implementation of component-based software applications rests on the principle of assembly: *Complex systems are built by composing re-useable interacting components*. Components contain functional, business-logic code that implements operations/callbacks on state variables. Ports facilitate interactions between communicating components. A component-level message queue controls the scheduling of operations/callbacks. 

ROSMOD Components
^^^^^^^^^^^^^^^^^

A ROSMOD Component is a re-useable unit/piece of software in an application. Each component can contain one or more of the following:

* Publishers: A publisher port publishes (without blocking) on a message/topic (msg in ROS)
* Subscribers: A subscriber port subscribes to a message/topic (msg in ROS)
* Servers: A server port provides an "operation" to the external world
* Clients: A client port requires/uses an operation provided by a server (potentially on another component)
* Timers: A timer is used to trigger the component. Timer callbacks are invoked when a timer expires

.. image:: ./_images/ros_component.png


