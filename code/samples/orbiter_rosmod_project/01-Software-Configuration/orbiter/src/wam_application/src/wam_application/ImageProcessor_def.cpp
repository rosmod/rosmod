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
  LOGGER.INFO("Processing high-resolution image on satellite %s",hostName.c_str());
}
//# End HRsub_OnOneData Marker
// OnOneData Subscription handler for LRsub subscriber
//# Start LRsub_OnOneData Marker
void ImageProcessor_def::LRsub_OnOneData(const wam_application::LRImageVector::ConstPtr& received_data)
{
    // Business Logic for LRsub subscriber subscribing to topic LRImageVector callback 
  LOGGER.INFO("Processing low-resolution image on satellite %s",hostName.c_str());
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

    // Need to read in and parse the group configuration xml if it exists
    GroupXMLParser groupParser;
    std::map<std::string,std::string> *portGroupMap = NULL;
    std::string configFileName = nodeName + "." + compName + ".xml";
    if (groupParser.Parse(configFileName))
    {
	portGroupMap = &groupParser.portGroupMap;
    }

    std::string advertiseName;

    // Configure all subscribers associated with this component
    // subscriber: HRsub
    advertiseName = "HRImageVector";
    if ( portGroupMap != NULL && portGroupMap->find("HRsub") != portGroupMap->end() )
        advertiseName += "_" + (*portGroupMap)["HRsub"];
    ros::SubscribeOptions HRsub_options;
    HRsub_options = 
	ros::SubscribeOptions::create<wam_application::HRImageVector>
	    (advertiseName.c_str(),
	     1000,
	     boost::bind(&ImageProcessor_def::HRsub_OnOneData, this, _1),
	     ros::VoidPtr(),
             &this->compQueue);
    this->HRsub = nh.subscribe(HRsub_options);
    // subscriber: LRsub
    advertiseName = "LRImageVector";
    if ( portGroupMap != NULL && portGroupMap->find("LRsub") != portGroupMap->end() )
        advertiseName += "_" + (*portGroupMap)["LRsub"];
    ros::SubscribeOptions LRsub_options;
    LRsub_options = 
	ros::SubscribeOptions::create<wam_application::LRImageVector>
	    (advertiseName.c_str(),
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
    Log_Levels target_log_levels = {.DEBUG = false,
				    .INFO = true,
				    .WARNING = false,
				    .ERROR = true,
				    .CRITICAL = true};
    LOGGER.SET_LOG_LEVELS(target_log_levels);
}
