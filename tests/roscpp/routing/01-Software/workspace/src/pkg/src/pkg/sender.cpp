#include "pkg/sender.hpp"


//# Start User Globals Marker
double tg_duration = -1;
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void sender::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  srand (time(NULL));
  std::string fName;
  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--tg_time"))
	{
	  tg_duration = atof(node_argv[i+1]);
	}
    }
  max_data_length = 8192;
  tg_misbehave = false;
  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--max_data_length_bytes"))
	{
	  max_data_length = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i], "--max_data_length_bits"))
	{
	  max_data_length = atoi(node_argv[i+1]) / 8;
	}
      if (!strcmp(node_argv[i], "--tg_misbehave"))
	{
	  tg_misbehave = true;
	}
    }
  ros::NodeHandle nh;
  ros::TimerOptions timer_options;
  if (config.profileMap.find("routed_msg_pub") != config.profileMap.end())
    {
      routed_msg_pub_send_mw.init(node_argc,
					     node_argv,
					     config.uuidMap["routed_msg_pub"],
					     config.profileMap["routed_msg_pub"]);
      if ( tg_duration < 0 )
	routed_msg_pub_send_mw.set_duration(routed_msg_pub_send_mw.profile.period);
      else
	routed_msg_pub_send_mw.set_duration(tg_duration);
      fName = nodeName + "." + compName + ".routed_msg_pub.network.csv";
      routed_msg_pub_send_mw.set_output_filename(fName);

      timer_options = 
	ros::TimerOptions
	(ros::Duration(-1),
	 boost::bind(&sender::routed_msg_pub_timerCallback, this, _1),
	 &this->compQueue,
	 true);
      routed_msg_pub_timer = nh.createTimer(timer_options);
    }
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

void sender::routed_msg_pub_timerCallback(const ros::TimerEvent& event)
{
  double timerDelay = 0;
  if ( this->routed_msg_pub.getNumSubscribers() >= 1 )
    {
      pkg::routed_msg msg;
      msg.uuid = routed_msg_pub_send_mw.get_uuid();
      msg.bytes.resize(max_data_length,0);
      try
	{
	  timerDelay =
	    routed_msg_pub_send_mw.send<pkg::routed_msg>(routed_msg_pub, msg);
	}
      catch ( Network::Exceeded_Production_Profile& ex )
	{
	  LOGGER.DEBUG("Prevented from sending on the network!");
	}
    }
  else
    {
      timerDelay = 0.1;
      routed_msg_pub_send_mw.set_duration(tg_duration);
    }
  
  
  if ( ros::Time::now() >= routed_msg_pub_send_mw.get_end_time() )
    {
      LOGGER.DEBUG("writing output\n");
      routed_msg_pub_send_mw.record();
    }
  else
    {
      if (tg_misbehave)
	timerDelay -= 0.1;
      ros::TimerOptions timer_options;
      timer_options = 
	ros::TimerOptions
	(ros::Duration(timerDelay),
	 boost::bind(&sender::routed_msg_pub_timerCallback, this, _1),
	 &this->compQueue,
	 true);
      ros::NodeHandle nh;
      routed_msg_pub_timer = nh.createTimer(timer_options);
    }
}


// Destructor - Cleanup Ports & Timers
sender::~sender()
{
  routed_msg_pub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void sender::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

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

  // Scheduling Scheme is FIFO

  // Component Publisher - routed_msg_pub
  advertiseName = "routed_msg";
  if (portGroupMap.find("routed_msg_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["routed_msg_pub"];
  this->routed_msg_pub = nh.advertise<pkg::routed_msg>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&sender::Init, this, _1),
     &this->compQueue,
     true,
     false);
  this->initOneShotTimer = nh.createTimer(timer_options);
  this->initOneShotTimer.stop();


  this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
  ros::SubscribeOptions comp_sync_sub_options;
  comp_sync_sub_options = ros::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&sender::component_synchronization_OnOneData, this, _1),
     ros::VoidPtr(),
     &this->compQueue);
  this->comp_sync_sub = nh.subscribe(comp_sync_sub_options);

  ros::Time now = ros::Time::now();
  while ( this->comp_sync_sub.getNumPublishers() < this->num_comps_to_sync &&
	  (ros::Time::now() - now) < ros::Duration(comp_sync_timeout) )
    ros::Duration(0.1).sleep();
  ros::Duration(0.5).sleep();
  this->comp_sync_sub.shutdown();
  this->comp_sync_pub.shutdown();

  this->initOneShotTimer.start();
  
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new sender(config,argc,argv);
  }
}
