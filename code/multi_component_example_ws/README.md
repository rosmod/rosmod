Multi-Component Example:
========================

Description:
------------
This example shows a way of running multiple components inside a node in an object-oriented fashion.  Each component has its own component operation queue.  The code has been written and commented in such a way as to show which parts are generated each time, which parts are generated based on the model, and which parts much be written by the developer.  

Design:
-------
The Component class is a generic base class as so:

    class Component
    {   
    public:
    	// StartUp will be completely generated
    	virtual void startUp() = 0;
    	// Init will be generated with BL supplied by user
    	virtual void Init(const ros::TimerEvent& event);
    	// queueThread processes queue actions
    	void processQueue();
    	// required for clean shutdown
    	~Component();
    protected:
    	ros::Timer initOneShotTimer;  // timer for calling init
    	ros::CallbackQueue compQueue; // single callbackQueue for the component
    };

Where startUp() must be overridden by all derived component implementations.  In the derived classes, the startUp() function configures any timers, publishers, subscribers, clients, and servers the component may have.  The entirety of startUp() will be automatically generated.

The processQueue() function simply processes all operations present in compQueue.  

Init() has a default implementation which does nothing, but if it is overridden by the derived implementation (as would be the case if the user specified a need for it), then the initOneShotTimer would be configured in startUp() so that Init() will fire once and will be managed by compQueue.  

Any callbacks required, e.g. timer callbacks, subscriber callbacks and service callbacks, are going to be automatically generated in the component implementation's class, along with their associated objects.  These callback funtions will be generated empty and the developer will write the business logic for them.  Again, the setup/configuration of these callbacks will be automatically generated into startUp().  

Finally, the destructor is required to gracefully exit (i.e. without errors) when the process is terminated.  

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