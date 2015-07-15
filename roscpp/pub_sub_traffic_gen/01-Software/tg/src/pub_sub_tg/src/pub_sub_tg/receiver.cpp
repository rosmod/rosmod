#include "pub_sub_tg/receiver.hpp"


//# Start User Globals Marker

void receiver::message_sub_wrapper(const pub_sub_tg::message::ConstPtr& received_data)
{
  LOGGER.DEBUG("CHECKING RECEIVED DATA AGAINST SENDER PROFILE");
  // GET SENDER ID
  uint64_t uuid = received_data->uuid;
  // CHECK NETWORK PROFILE HERE FOR SENDER
  Network::NetworkProfile* profile = &profile_map[uuid];
  // IF THE NETWORK PROFILE HAS BEEN EXCEEDED FOR TOO LONG
  // ( TAKE INTO ACCOUNT OTHER SENDERS )
  // THEN SEND A REQUEST BACK TO SENDER(S) MIDDLEWARE TO METER OR STOP
  ros::ServiceClient* sender = oob_map[uuid];
  pub_sub_tg::oob_comm oob;
  oob.request.deactivateSender = true;
  sender->call(oob);
  // MEASURE AND RECORD DATA OUTPUT
  Network::Message new_msg;
  messages.push_back(new_msg);
  messages[id].Bytes(sizeof(uint64_t) + received_data->bytes.size());
  messages[id].Id(id);
  messages[id].TimeStamp();
  id++;
  // FINALLY, PASS DATA THROUGH (IF IT'S ALRIGHT)
  this->message_sub_OnOneData(received_data);
}
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void receiver::Init(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering receiver::Init");
  // Initialize Here
  
  // Stop Init Timer
  initOneShotTimer.stop();
  LOGGER.DEBUG("Exiting receiver::Init");
}
//# End Init Marker

// Subscriber Callback - message_sub
//# Start message_sub_OnOneData Marker
void receiver::message_sub_OnOneData(const pub_sub_tg::message::ConstPtr& received_data)
{
  LOGGER.DEBUG("Entering receiver::message_sub_OnOneData");
  // Business Logic for message_sub Subscriber

  LOGGER.DEBUG("Exiting receiver::message_sub_OnOneData");
}
//# End message_sub_OnOneData Marker


// Destructor - Cleanup Ports & Timers
receiver::~receiver()
{
  message_sub.shutdown();
  oob_client.shutdown();
  //# Start Destructor Marker
  std::string fName = nodeName + "." + compName + ".network.csv";
  Network::write_data(fName.c_str(),messages);
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void receiver::startUp()
{
  LOGGER.DEBUG("Entering receiver::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Subscriber - message_sub
  advertiseName = "message";
  if (portGroupMap.find("message_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["message_sub"];
  ros::SubscribeOptions message_sub_options;
  message_sub_options = ros::SubscribeOptions::create<pub_sub_tg::message>
      (advertiseName.c_str(),
       1000,
       boost::bind(&receiver::message_sub_wrapper, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->message_sub = nh.subscribe(message_sub_options);

  // Configure all required services associated with this component
  // Component Client - oob_client
  advertiseName = "oob_comm";
  if (portGroupMap.find("oob_client") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["oob_client"];
  this->oob_client = nh.serviceClient<pub_sub_tg::oob_comm>(advertiseName.c_str());

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&receiver::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Identify the pwd of Node Executable
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
  std::string log_file_path = pwd + nodeName + "." + compName + ".log"; 
  
  // Create the log file & open file stream
  LOGGER.CREATE_FILE(log_file_path);
  
  // Establish log levels of LOGGER
  LOGGER.SET_LOG_LEVELS(logLevels);
  

  // INITIALIZE N/W MIDDLEWARE HERE
  // GET ALL OOB SERVER UUIDS FOR USE IN CALLBACK
  pub_sub_tg::oob_comm oob_get_uuid;
  oob_get_uuid.request.deactivateSender = false;
  oob_get_uuid.request.meterSender = false;
  oob_client.call(oob_get_uuid);
  uint64_t uuid = oob_get_uuid.response.uuid;
  oob_map[uuid] = &oob_client;
  std::string profileName = oob_get_uuid.response.profileName;
  // LOAD PROFILES
  profile_map[uuid] = Network::NetworkProfile();
  profile_map[uuid].initializeFromFile(profileName.c_str());
  id = 0;
  // SYNCHRONIZE HERE

  LOGGER.DEBUG("Exiting receiver::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new receiver(config,argc,argv);
  }
}
