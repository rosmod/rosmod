#include "New_Package/dlcomp.hpp"

//# Start User Globals Marker

//# End User Globals Marker

// -------------------------------------------------------
// BUSINESS LOGIC OF THESE FUNCTIONS SUPPLIED BY DEVELOPER
// -------------------------------------------------------


extern "C" {
Component *maker(std::string hostName, std::string nodeName, std::string compName, int argc, char **argv){
  return new dlcomp(hostName,nodeName,compName,argc,argv);
}
}

// Init Function
//# Start Init Marker
void dlcomp::Init(const ros::TimerEvent& event)
{
    // Initialize Component

    // Stop Init Timer
    initOneShotTimer.stop();
}
//# End Init Marker

// OnOneData Subscription handler for sub subscriber
//# Start sub_OnOneData Marker
void dlcomp::sub_OnOneData(const New_Package::dlmsg::ConstPtr& received_data)
{
    // Business Logic for sub subscriber subscribing to topic dlmsg callback 

}
//# End sub_OnOneData Marker

// Component Service Callback
//# Start dlsrvCallback Marker
bool dlcomp::dlsrvCallback(New_Package::dlsrv::Request  &req,
    New_Package::dlsrv::Response &res)
{
    // Business Logic for server Server providing dlsrv Service

    return true;
}
//# End dlsrvCallback Marker

// Callback for timer0 timer
//# Start timer0Callback Marker
void dlcomp::timer0Callback(const ros::TimerEvent& event)
{
    // Business Logic for timer0 
  printf("TEST\n");
}
//# End timer0Callback Marker

// ---------------------------------------------
// EVERYTHING BELOW HERE IS COMPLETELY GENERATED
// ---------------------------------------------

// Destructor - required for clean shutdown when process is killed
dlcomp::~dlcomp()
{
    timer0.stop();
    pub.shutdown();
    sub.shutdown();
    dlsrv_server.shutdown();
    dlsrv_client.shutdown();
//# Start Destructor Marker

//# End Destructor Marker
}

void dlcomp::startUp()
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
    // subscriber: sub
    advertiseName = "dlmsg";
    if ( portGroupMap != NULL && portGroupMap->find("sub") != portGroupMap->end() )
        advertiseName += "_" + (*portGroupMap)["sub"];
    ros::SubscribeOptions sub_options;
    sub_options = 
	ros::SubscribeOptions::create<New_Package::dlmsg>
	    (advertiseName.c_str(),
	     1000,
	     boost::bind(&dlcomp::sub_OnOneData, this, _1),
	     ros::VoidPtr(),
             &this->compQueue);
    this->sub = nh.subscribe(sub_options);

    // Configure all publishers associated with this component
    // publisher: pub
    advertiseName = "dlmsg";
    if ( portGroupMap != NULL && portGroupMap->find("pub") != portGroupMap->end() )
        advertiseName += "_" + (*portGroupMap)["pub"];
    this->pub = nh.advertise<New_Package::dlmsg>
	(advertiseName.c_str(), 1000);	

    // Configure all provided services associated with this component
    // server: server
    advertiseName = "dlsrv";
    if ( portGroupMap != NULL && portGroupMap->find("dlsrv_server") != portGroupMap->end() )
        advertiseName += "_" + (*portGroupMap)["dlsrv_server"];
    ros::AdvertiseServiceOptions dlsrv_server_options;
    dlsrv_server_options = 
	ros::AdvertiseServiceOptions::create<New_Package::dlsrv>
	    (advertiseName.c_str(),
             boost::bind(&dlcomp::dlsrvCallback, this, _1, _2),
	     ros::VoidPtr(),
             &this->compQueue);
    this->dlsrv_server = nh.advertiseService(dlsrv_server_options);
 
    // Configure all required services associated with this component
    // client: dlsrv_client
    advertiseName = "dlsrv";
    if ( portGroupMap != NULL && portGroupMap->find(advertiseName+"_client") != portGroupMap->end() )
        advertiseName += "_" + (*portGroupMap)[advertiseName+"_client"];
    this->dlsrv_client = nh.serviceClient<New_Package::dlsrv>
	(advertiseName.c_str()); 

    // Create Init Timer
    ros::TimerOptions timer_options;
    timer_options = 
	ros::TimerOptions
	    (ros::Duration(-1),
	     boost::bind(&dlcomp::Init, this, _1),
	     &this->compQueue,
             true);
    this->initOneShotTimer = nh.createTimer(timer_options);  
  
    // Create all component timers
    // timer: timer.properties["name"]
    timer_options = 
	ros::TimerOptions
             (ros::Duration(1.0),
	     boost::bind(&dlcomp::timer0Callback, this, _1),
	     &this->compQueue);
    this->timer0 = nh.createTimer(timer_options);


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
