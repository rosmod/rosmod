#include "dsc/tl_sensor_stream_ddos.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void tl_sensor_stream_ddos::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  srand (time(NULL));
  double tg_duration = -1;
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
  if (config.profileMap.find("tl_sensor_stream_ddos_pub") != config.profileMap.end())
    {
      tl_sensor_stream_ddos_pub_send_mw.init(node_argc,
					     node_argv,
					     config.uuidMap["tl_sensor_stream_ddos_pub"],
					     config.profileMap["tl_sensor_stream_ddos_pub"]);
      if ( tg_duration < 0 )
	tl_sensor_stream_ddos_pub_send_mw.set_duration(tl_sensor_stream_ddos_pub_send_mw.profile.period);
      else
	tl_sensor_stream_ddos_pub_send_mw.set_duration(tg_duration);
      fName = nodeName + "." + compName + ".tl_sensor_stream_ddos_pub.network.csv";
      tl_sensor_stream_ddos_pub_send_mw.set_output_filename(fName);

      timer_options = 
	ros::TimerOptions
	(ros::Duration(-1),
	 boost::bind(&tl_sensor_stream_ddos::tl_sensor_stream_ddos_pub_timerCallback, this, _1),
	 &this->compQueue,
	 true);
      tl_sensor_stream_ddos_pub_timer = nh.createTimer(timer_options);
    }
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker


void tl_sensor_stream_ddos::tl_sensor_stream_ddos_pub_timerCallback(const ros::TimerEvent& event)
{
  dsc::ryg_state msg;
  msg.uuid = tl_sensor_stream_ddos_pub_send_mw.get_uuid();
  msg.bytes.resize(max_data_length,0);
  double timerDelay = 0;
  try
    {
      timerDelay =
	tl_sensor_stream_ddos_pub_send_mw.send<dsc::ryg_state>(tl_sensor_stream_ddos_pub, msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      LOGGER.DEBUG("Prevented from sending on the network!");
    }

  if ( ros::Time::now() >= tl_sensor_stream_ddos_pub_send_mw.get_end_time() )
    {
      LOGGER.DEBUG("writing output\n");
      tl_sensor_stream_ddos_pub_send_mw.record();
    }
  else
    {
      if (tg_misbehave)
	timerDelay -= 0.1;
      ros::TimerOptions timer_options;
      timer_options = 
	ros::TimerOptions
	(ros::Duration(timerDelay),
	 boost::bind(&tl_sensor_stream_ddos::tl_sensor_stream_ddos_pub_timerCallback, this, _1),
	 &this->compQueue,
	 true);
      ros::NodeHandle nh;
      tl_sensor_stream_ddos_pub_timer = nh.createTimer(timer_options);
    }
}


// Destructor - Cleanup Ports & Timers
tl_sensor_stream_ddos::~tl_sensor_stream_ddos()
{
  tl_sensor_stream_ddos_pub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void tl_sensor_stream_ddos::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO

  // Component Publisher - tl_sensor_stream_ddos_pub
  advertiseName = "ryg_state";
  if (portGroupMap.find("tl_sensor_stream_ddos_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["tl_sensor_stream_ddos_pub"];
  this->tl_sensor_stream_ddos_pub = nh.advertise<dsc::ryg_state>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&tl_sensor_stream_ddos::Init, this, _1),
     &this->compQueue,
     true,
     false);
  this->initOneShotTimer = nh.createTimer(timer_options);
  this->initOneShotTimer.stop();
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


  this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
  ros::SubscribeOptions comp_sync_sub_options;
  comp_sync_sub_options = ros::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&tl_sensor_stream_ddos::component_synchronization_OnOneData, this, _1),
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
    return new tl_sensor_stream_ddos(config,argc,argv);
  }
}
