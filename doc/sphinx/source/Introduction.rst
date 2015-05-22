Introduction
============

`ROSMOD <https://github.com/finger563/rosmod>`_ is a tool suite for rapid prototyping component-based software applications using the `Robot Operating System <http://www.ros.org>`_ (ROS). Using ROSMOD, an application developer can create and manage *projects* for distributed real-time embedded systems. Each ROSMOD Project consists of *models* that represent the structure and behavior of the system:

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

* *Publishers*: A publisher port publishes (without blocking) on a message/topic (msg in ROS)
* *Subscribers*: A subscriber port subscribes to a message/topic (msg in ROS)
* *Servers*: A server port provides an "operation" (srv in ROS) to the external world 
* *Clients*: A client port requires/uses an operation (srv in ROS) provided by a server (potentially on another component)
* *Timers*: A timer is used to trigger the component. Timer callbacks are invoked when a timer expires

.. image:: ./_images/ROSMOD_Component.png
   :scale: 75 %
   :align: center

Each component has a single thread called the "Component Executor Thread". This thread handles all requests from external entities (other components) and infrastructural triggers (timer expiry). This thread is therefore responsible for executing all triggered callbacks e.g. subscriber callbacks, server callbacks & timer callbacks. To facilitate interactions with other components, each component also has a "Component Message Queue". This queue *holds* requests received from other interacting entities. 

The following figure shows a simple Client-Server component interaction. Component A is periodically triggered by a timer. At each timer expiry, Component A makes a blocking remote procedure call to Component B using its client port. This service request, on reaching Component B, is enqueued onto Component B's message queue. When this request reaches the front of the queue, the corresponding server-side callback is executed by the Component B executor thread and the response is returned back to Component A. This message queue-based interaction is also true for timers; when the timer in Component A expires, a timer callback request is enqueued onto its message queue and eventually handled.

.. image:: ./_images/Component_Message_Queue.png
   :scale: 90 %
   :align: center

Design Notes:
"""""""""""""

* In each component, the message queue is processed by a single executor thread. Multiple components can run concurrently but each component execution is single-threaded.
* The component message queue supports several scheduling schemes including FIFO (first-in first-out), PFIFO (priority first-in first-out) and EDF (earliest deadline first).
* Requests in the message queue are processed using a non-preemptive scheduling scheme. This means that each callback/operation run by the executor thread is run to completion before the next one (in the message queue) is processed. 
* These rules are strictly applied to all ROSMOD components.

ROSMOD Nodes
^^^^^^^^^^^^

ROSMOD Nodes are deployable processes that run the application/package. These Nodes contain one or more *instances* of ROSMOD Components. Therefore, each ROSMOD node contains one or more component threads, each of which (1) uses component ports to push out queries, (2) uses its message queue to receive external requests, and (3) executes triggered callbacks. ROSMOD Nodes are similar to ROS Nodes but follow a strict structure and behavior. 
