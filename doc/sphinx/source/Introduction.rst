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

A ROSMOD Component is a re-useable unit/piece of software in an application. Components can be thought of as LEGO pieces - Each piece has a well-defined shape; Multiple such pieces are connected together to build large structures (applications). Component-based software divides application concerns into smaller, manageable blocks that can be easily composed. In ROSMOD, each component can contain one or more of the following:

* Publishers: A publisher port publishes (without blocking) on a message/topic (msg in ROS)
* Subscribers: A subscriber port subscribes to a message/topic (msg in ROS)
* Servers: A server port provides an "operation" to the external world
* Clients: A client port requires/uses an operation provided by a server (potentially on another component)
* Timers: A timer is used to trigger the component. Timer callbacks are invoked when a timer expires

.. image:: ./_images/ros_component.png

Notes:

* Threading: Every component has a single thread called the component executor thread. This thread handles all requests from external entities and executes the component callbacks e.g. timer callbacks, subscriber callbacks, server callbacks etc. 
* Message Queue: Every Component has a message queue. This queue holds requests from (1) other components and from (2) the underlying infrastructure. When a client port in "Component A" make a procedure call to a server port in "Component B", this requests first lands in the message queue of Component B and is eventually handled by Componetn B's executor thread.


