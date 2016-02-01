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

      LOGGER.INFO("Published Low Resolution Image from satellite %s", hostName.c_str());
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
    if ( portGroupMap != NULL && portGroupMap->find("lrImage_pub") != portGroupMap->end() )
        advertiseName += "_" + (*portGroupMap)["lrImage_pub"];
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


    /*
     * Identify present working directory of node executable
     */
    std::string s = node_argv[0];
    std::string exec_path = s;
    std::string delimiter = "/";
    std::string exec, pwd;
    size_t pos = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        exec = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
    }
    exec = s.substr(0, pos);
    pwd = exec_path.erase(exec_path.find(exec), exec.length());
    // Establish the log file name
    std::string log_file_path = pwd + nodeName + "." + compName + ".log"; 

    // Create the log file & open file stream
    LOGGER.CREATE_FILE(log_file_path);

    // Establish log levels of LOGGER
    LOGGER.SET_LOG_LEVELS(groupParser.logging);
}
