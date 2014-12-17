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
    ros::NodeHandle nh;

    // Configure all required services associated with this component
    this->ComponentService_client = nh.serviceClient<Package_1::ComponentService>
	("ComponentService"); 

    // Create Init Timer
    ros::TimerOptions timer_options;
    timer_options = 
	ros::TimerOptions
	    (ros::Duration(-1),
	     boost::bind(&Component_3::Init, this, _1),
	     &this->compQueue,
             true);
    this->initOneShotTimer = nh.createTimer(timer_options);  
  
    // Create all component timers
    timer_options = 
	ros::TimerOptions
             (ros::Duration(2.0),
	     boost::bind(&Component_3::Timer0Callback, this, _1),
	     &this->compQueue);
    this->Timer0 = nh.createTimer(timer_options);

}
