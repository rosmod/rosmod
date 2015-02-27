#include "wam_application/ImageProcessor_def.hpp"

//# Start User Globals Marker

//# End User Globals Marker

// -------------------------------------------------------
// BUSINESS LOGIC OF THESE FUNCTIONS SUPPLIED BY DEVELOPER
// -------------------------------------------------------

// Init Function
//# Start Init Marker
void ImageProcessor_def::Init(const ros::TimerEvent& event)
{
    // Initialize Component

    // Stop Init Timer
    initOneShotTimer.stop();
}
//# End Init Marker

// OnOneData Subscription handler for HRsub subscriber
//# Start HRsub_OnOneData Marker
void ImageProcessor_def::HRsub_OnOneData(const wam_application::HRImageVector::ConstPtr& received_data)
{
    // Business Logic for HRsub subscriber subscribing to topic HRImageVector callback 
  ROS_INFO("Processing high-resolution image on satellite %s",nodeName.c_str());
}
//# End HRsub_OnOneData Marker
// OnOneData Subscription handler for LRsub subscriber
//# Start LRsub_OnOneData Marker
void ImageProcessor_def::LRsub_OnOneData(const wam_application::LRImageVector::ConstPtr& received_data)
{
    // Business Logic for LRsub subscriber subscribing to topic LRImageVector callback 
  ROS_INFO("Processing low-resolution image on satellite %s",nodeName.c_str());
}
//# End LRsub_OnOneData Marker

// ---------------------------------------------
// EVERYTHING BELOW HERE IS COMPLETELY GENERATED
// ---------------------------------------------

// Destructor - required for clean shutdown when process is killed
ImageProcessor_def::~ImageProcessor_def()
{
    HRsub.shutdown();
    LRsub.shutdown();
//# Start Destructor Marker

//# End Destructor Marker
}

void ImageProcessor_def::startUp()
{
    ros::NodeHandle nh;

    // Configure all subscribers associated with this component
    // subscriber: HRsub
    ros::SubscribeOptions HRsub_options;
    HRsub_options = 
	ros::SubscribeOptions::create<wam_application::HRImageVector>
	    ("HRImageVector",
	     1000,
	     boost::bind(&ImageProcessor_def::HRsub_OnOneData, this, _1),
	     ros::VoidPtr(),
             &this->compQueue);
    this->HRsub = nh.subscribe(HRsub_options);
    // subscriber: LRsub
    ros::SubscribeOptions LRsub_options;
    LRsub_options = 
	ros::SubscribeOptions::create<wam_application::LRImageVector>
	    ("LRImageVector",
	     1000,
	     boost::bind(&ImageProcessor_def::LRsub_OnOneData, this, _1),
	     ros::VoidPtr(),
             &this->compQueue);
    this->LRsub = nh.subscribe(LRsub_options);

    // Create Init Timer
    ros::TimerOptions timer_options;
    timer_options = 
	ros::TimerOptions
	    (ros::Duration(-1),
	     boost::bind(&ImageProcessor_def::Init, this, _1),
	     &this->compQueue,
             true);
    this->initOneShotTimer = nh.createTimer(timer_options);  
  
}
