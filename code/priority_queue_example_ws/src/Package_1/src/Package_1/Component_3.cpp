#include "Package_1/Component_3.hpp"

// -------------------------------------------------------
// BUSINESS LOGIC OF THESE FUNCTIONS SUPPLIED BY DEVELOPER
// -------------------------------------------------------

// Init Function
void Component_3::Init(const ros::TimerEvent& event)
{
    // Initialize Component

    // Stop Init Timer
    initOneShotTimer.stop();
}

// Callback for Timer0 timer
void Component_3::Timer0Callback(const ros::TimerEvent& event)
{
    // Business Logic for Timer0 
    Package_1::ComponentService srv;
    srv.request.operation = "GetComponentName";
    ROS_INFO("Component_3 : Invoking ComponentService from Timer0 callback");
    if (ComponentService_client.call(srv))
    {
      ROS_INFO("Component_3 : Got name of other component: %s", srv.response.name.c_str());
    }
    else
    {
      ROS_ERROR("Component_3 : Failed to invoke operation");
      ComponentService_client.waitForExistence(ros::Duration(-1));
    }
}

// ---------------------------------------------
// EVERYTHING BELOW HERE IS COMPLETELY GENERATED
// ---------------------------------------------

// Destructor - required for clean shutdown when process is killed
Component_3::~Component_3()
{
    Timer0.stop();
    ComponentService_client.shutdown();
}

void Component_3::startUp()
{
    ros::NodeHandle2 nh;

    // Configure all required services associated with this component
    this->ComponentService_client = nh.serviceClient<Package_1::ComponentService>
	("ComponentService"); 

    // Create Init Timer
    ros::TimerOptions2 timer_options;
    timer_options = 
	ros::TimerOptions2
	    (ros::Duration(-1),
	     boost::bind(&Component_3::Init, this, _1),
	     &this->cmq,
	     20,
         true);
    this->initOneShotTimer = nh.createTimer(timer_options);  

    // Create all component timers
    timer_options = 
	ros::TimerOptions2
             (ros::Duration(2.0),
	     boost::bind(&Component_3::Timer0Callback, this, _1),
	     &this->cmq, 12);
    this->Timer0 = nh.createTimer(timer_options);

}
