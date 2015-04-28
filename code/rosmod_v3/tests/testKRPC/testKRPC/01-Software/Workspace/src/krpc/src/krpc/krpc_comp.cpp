#include "krpc/krpc_comp.hpp"

//# Start User Globals Marker
boost::asio::io_service io_service;
//# End User Globals Marker

// -------------------------------------------------------
// BUSINESS LOGIC OF THESE FUNCTIONS SUPPLIED BY DEVELOPER
// -------------------------------------------------------

// Init Function
//# Start Init Marker
void krpc_comp::Init(const ros::TimerEvent& event)
{
    // Initialize Component
  tcp::resolver resolver(io_service);
  tcp::socket socket = tcp::socket(io_service);
  tcp::resolver::query query("127.0.0.1", "50000");
  tcp::resolver::iterator iterator = resolver.resolve(query);
  tcp::endpoint endpoint = *iterator;
    // Stop Init Timer
    initOneShotTimer.stop();
}
//# End Init Marker

// Callback for timer0 timer
//# Start timer0Callback Marker
void krpc_comp::timer0Callback(const ros::TimerEvent& event)
{
    // Business Logic for timer0 

}
//# End timer0Callback Marker

// ---------------------------------------------
// EVERYTHING BELOW HERE IS COMPLETELY GENERATED
// ---------------------------------------------

// Destructor - required for clean shutdown when process is killed
krpc_comp::~krpc_comp()
{
    timer0.stop();
//# Start Destructor Marker

//# End Destructor Marker
}

void krpc_comp::startUp()
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

    // Create Init Timer
    ros::TimerOptions timer_options;
    timer_options = 
	ros::TimerOptions
	    (ros::Duration(-1),
	     boost::bind(&krpc_comp::Init, this, _1),
	     &this->compQueue,
             true);
    this->initOneShotTimer = nh.createTimer(timer_options);  
  
    // Create all component timers
    // timer: timer.properties["name"]
    timer_options = 
	ros::TimerOptions
             (ros::Duration(1.0),
	     boost::bind(&krpc_comp::timer0Callback, this, _1),
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
