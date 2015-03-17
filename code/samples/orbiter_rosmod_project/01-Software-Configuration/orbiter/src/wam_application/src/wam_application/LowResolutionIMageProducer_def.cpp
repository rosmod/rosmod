#include "wam_application/LowResolutionIMageProducer_def.hpp"

//# Start User Globals Marker

//# End User Globals Marker

// -------------------------------------------------------
// BUSINESS LOGIC OF THESE FUNCTIONS SUPPLIED BY DEVELOPER
// -------------------------------------------------------

// Init Function
//# Start Init Marker
void LowResolutionIMageProducer_def::Init(const ros::TimerEvent& event)
{
    // Initialize Component

    // Stop Init Timer
    initOneShotTimer.stop();
}
//# End Init Marker

// Callback for Timer0 timer
//# Start Timer0Callback Marker
void LowResolutionIMageProducer_def::Timer0Callback(const ros::TimerEvent& event)
{
    // Business Logic for Timer0 

      wam_application::LRImageVector lowResImgVec;

      lowResImgVec.img[0] = 400;

      lrImage_pub.publish(lowResImgVec);

      ROS_INFO("Published Low Resolution Image from satellite %s", hostName.c_str());
}
//# End Timer0Callback Marker

// ---------------------------------------------
// EVERYTHING BELOW HERE IS COMPLETELY GENERATED
// ---------------------------------------------

// Destructor - required for clean shutdown when process is killed
LowResolutionIMageProducer_def::~LowResolutionIMageProducer_def()
{
    Timer0.stop();
    lrImage_pub.shutdown();
//# Start Destructor Marker

//# End Destructor Marker
}

void LowResolutionIMageProducer_def::startUp()
{
    ros::NodeHandle nh;

    // Need to read in and parse the group configuration xml if it exists
    GroupXMLParser groupParser;
    std::map<std::string,std::string> *portGroupMap = NULL;
    std::string configFileName = nodeName + "." + compName + ".xml";
    if (groupParser.Parse(configFileName))
    {
	portGroupMap = &groupParser.portGroupMap;
    }

    std::string advertiseName;

    // Configure all publishers associated with this component
    // publisher: lrImage_pub
    advertiseName = "LRImageVector";
    if ( portGroupMap != NULL && portGroupMap->find(advertiseName) != portGroupMap->end() )
        advertiseName += "_" + (*portGroupMap)[advertiseName];
    this->lrImage_pub = nh.advertise<wam_application::LRImageVector>
	(advertiseName.c_str(), 1000);	

    // Create Init Timer
    ros::TimerOptions timer_options;
    timer_options = 
	ros::TimerOptions
	    (ros::Duration(-1),
	     boost::bind(&LowResolutionIMageProducer_def::Init, this, _1),
	     &this->compQueue,
             true);
    this->initOneShotTimer = nh.createTimer(timer_options);  
  
    // Create all component timers
    // timer: timer.properties["name"]
    timer_options = 
	ros::TimerOptions
             (ros::Duration(5.0),
	     boost::bind(&LowResolutionIMageProducer_def::Timer0Callback, this, _1),
	     &this->compQueue);
    this->Timer0 = nh.createTimer(timer_options);

}
