#include "Package_1/Component_1.hpp"

// -------------------------------------------------------
// BUSINESS LOGIC OF THESE FUNCTIONS SUPPLIED BY DEVELOPER
// -------------------------------------------------------

// Init Function
void Component_1::Init(const ros::TimerEvent& event)
{
    // Initialize Component
    Package_1::ComponentName compName;
    compName.name = "Component_1";
    ROS_INFO("Component_1 : Publishing component name %s",compName.name.c_str());
    //Component_1_publisher.publish(compName);

    // Stop Init Timer
    initOneShotTimer.stop();
}

// OnOneData Subscription handler for Component_1_subscriber subscriber
void Component_1::OnOneData(const Package_1::ComponentName::ConstPtr& received_data)
{
    // Business Logic for Component_1_subscriber subscriber callback  
    ROS_INFO("Component_1 : Got component name %s",received_data->name.c_str());
}

// Callback for Timer0 timer
void Component_1::Timer0Callback(const ros::TimerEvent& event)
{
    // Business Logic for Timer0 
    ROS_INFO("Component_1 : Timer0 callback has triggered!");
}

// ---------------------------------------------
// EVERYTHING BELOW HERE IS COMPLETELY GENERATED
// ---------------------------------------------

// Destructor - required for clean shutdown when process is killed
Component_1::~Component_1()
{
    Timer0.stop();
    Component_1_publisher.shutdown();
    Component_1_subscriber.shutdown();
}

void Component_1::startUp()
{
    ros::NodeHandle2 nh;

    // Configure all subscribers associated with this component
    ros::SubscribeOptions2 Component_1_subscriber_options;
    Component_1_subscriber_options = 
	ros::SubscribeOptions2::create<Package_1::ComponentName>
	    ("ComponentName",
	     1000,
	     boost::bind(&Component_1::OnOneData, this, _1),
	     ros::VoidPtr(),
         &this->cmq, 99);
    this->Component_1_subscriber = nh.subscribe(Component_1_subscriber_options);

    // Configure all publishers associated with this component
    //this->Component_1_publisher = nh.advertise<Package_1::ComponentName>
	//("ComponentName", 1000);	

    // Create Init Timer
    ros::TimerOptions2 timer_options;
    timer_options = 
	ros::TimerOptions2
	    (ros::Duration(-1),
	     boost::bind(&Component_1::Init, this, _1),
	     &this->cmq,
	     10,
         true);
    this->initOneShotTimer = nh.createTimer(timer_options);  
  
    // Create all component timers
    timer_options = 
	ros::TimerOptions2
             (ros::Duration(0.5),
		      boost::bind(&Component_1::Timer0Callback, this, _1),
		      &this->cmq, 26);
    this->Timer0 = nh.createTimer(timer_options);
}
