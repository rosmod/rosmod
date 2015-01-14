#include "Package_1/Component_2.hpp"

// -------------------------------------------------------
// BUSINESS LOGIC OF THESE FUNCTIONS SUPPLIED BY DEVELOPER
// -------------------------------------------------------

// Init Function
void Component_2::Init(const ros::TimerEvent& event)
{
    // Initialize Component

    // Stop Init Timer
    initOneShotTimer.stop();
}

// Component Service Callback
bool Component_2::ComponentServiceCallback(Package_1::ComponentService::Request  &req,
    Package_1::ComponentService::Response &res)
{
    // Business Logic for ComponentService Service
    Package_1::ComponentName compName;
    compName.name = "Component_2";
    ROS_INFO("Component_2 : Publishing component name %s from compServ callback",compName.name.c_str());
    //Component_2_publisher.publish(compName); 
    res.name = "Component_2";
    return true;
}

// Callback for Timer0 timer
void Component_2::Timer0Callback(const ros::TimerEvent& event)
{
    // Business Logic for Timer0 
    Package_1::ComponentName compName;
    compName.name = "Component_2";
    ROS_INFO("Component_2 : Publishing component name %s from Timer0 callback",compName.name.c_str());
    //Component_2_publisher.publish(compName); 
}

// ---------------------------------------------
// EVERYTHING BELOW HERE IS COMPLETELY GENERATED
// ---------------------------------------------

// Destructor - required for clean shutdown when process is killed
Component_2::~Component_2()
{
    Timer0.stop();
    Component_2_publisher.shutdown();
    ComponentService_server.shutdown();
}

void Component_2::startUp()
{
    ros::NodeHandle2 nh;

    // Configure all publishers associated with this component
    //this->Component_2_publisher = nh.advertise<Package_1::ComponentName>
	//("ComponentName", 1000);	

    // Configure all provided services associated with this component
    ros::AdvertiseServiceOptions2 ComponentService_server_options;
    ComponentService_server_options = 
	ros::AdvertiseServiceOptions2::create<Package_1::ComponentService>
	    ("ComponentService",
             boost::bind(&Component_2::ComponentServiceCallback, this, _1, _2),
	     ros::VoidPtr(),
             &this->cmq);

    this->ComponentService_server = nh.advertiseService(ComponentService_server_options);
 
    // Create Init Timer
    ros::TimerOptions2 timer_options;
    timer_options = 
	ros::TimerOptions2
	    (ros::Duration(-1),
	     boost::bind(&Component_2::Init, this, _1),
	     &this->cmq,
	     10,
         true);
    this->initOneShotTimer = nh.createTimer(timer_options);  
  
      // Create all component timers
    timer_options = 
	ros::TimerOptions2
             (ros::Duration(1.0),
	     boost::bind(&Component_2::Timer0Callback, this, _1),
	     &this->cmq, 11);
    this->Timer0 = nh.createTimer(timer_options);

}
