#include "pubsub/comm.hpp"

//# Start User Globals Marker
#include <unistd.h>
bool starter = false;
double multiplier = 0.3;
NAMESPACE::Timer pubTimer;
double publish_period = -1;
bool ender = false;
int sleep_time = 0;

void comm::pub_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering comm::pub_timer_operation");
#endif

  ros::Time now = ros::Time::now();
  timespec current_time;
  current_time.tv_sec = now.sec;
  current_time.tv_nsec = now.nsec;
  double offset = pub_send_mw.profile.getOffset(current_time);
  double period = pub_send_mw.profile.period;
  uint64_t message_len = max_data_length + sin(offset * 2 * M_PI / period) * max_data_length * multiplier;

  pubsub::pubsubTopic msg;
  msg.uuid = sub_id++;
  msg.bytes.resize(message_len,0);
  try
    {
      pub_send_mw.send<pubsub::pubsubTopic>(pub, msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      logger->log("DEBUG","Prevented from sending on the network!");
    }
  
  if ( ros::Time::now() >= pub_send_mw.get_end_time() )
    {
      logger->log("DEBUG","writing output\n");
      pub_send_mw.record();
      pubTimer.stop();
    }

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting comm::pub_timer_operation");
#endif
}
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void comm::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering comm::init_timer_operation");
#endif
  // Initialize Here
  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--starter"))
	{
	  starter = true;
	}
      else if (!strcmp(node_argv[i], "--ender"))
	{
	  ender = true;
	}
      else if (!strcmp(node_argv[i], "--multiplier"))
	{
	  multiplier = atof(node_argv[++i]);
	}
      else if (!strcmp(node_argv[i], "--publish_period"))
	{
	  publish_period = atof(node_argv[++i]);
	}
      else if (!strcmp(node_argv[i], "--sleep_time"))
	{
	  sleep_time = atoi(node_argv[++i]);
	}
    }

  srand (time(NULL));
  double tg_duration = -1;
  std::string fName;
  bool enable_oob = false;
  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--tg_time"))
	{
	  tg_duration = atof(node_argv[++i]);
	}
      else if (!strcmp(node_argv[i], "--enable_oob"))
	{
	  enable_oob = true;
	}
    }
  uint64_t capacityBits = 0;
  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--buffer_capacity_bits"))
	{
	  capacityBits = atoi(node_argv[++i]);
	}
      else if (!strcmp(node_argv[i], "--buffer_capacity_bytes"))
	{
	  capacityBits = atoi(node_argv[++i]) * 8;
	}
    }
  if (config.profileMap.find("sub") != config.profileMap.end())
    {
      sub_recv_mw.init(node_argc,
					     node_argv,
					     config.profileMap["sub"],
					     capacityBits,
					     enable_oob);
      if ( tg_duration < 0 )
	sub_recv_mw.set_duration(sub_recv_mw.profile.period);
      else
	sub_recv_mw.set_duration(tg_duration);
      fName = "/var/log/" + config.nodeName + "." +
	config.compName + ".sub.network.csv";
      sub_recv_mw.set_output_filename(fName);
      sub_recv_mw.set_recv_done_callback(boost::bind(&comm::mw_recv_done_operation, this, &sub_recv_mw));
      sub_id = 0;
    }
  if (config.portSenderMap.find("sub") != config.portSenderMap.end())
    {
      for (auto it=config.portSenderMap["sub"].begin();
	   it != config.portSenderMap["sub"].end(); ++it)
	{
	  sub_recv_mw.add_sender( it->first, it->second );
	}
    }
  
  max_data_length = 8192;
  tg_misbehave = false;
  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--max_data_length_bytes"))
	{
	  max_data_length = atoi(node_argv[++i]);
	}
      else if (!strcmp(node_argv[i], "--max_data_length_bits"))
	{
	  max_data_length = atoi(node_argv[++i]) / 8;
	}
      else if (!strcmp(node_argv[i], "--tg_misbehave"))
	{
	  tg_misbehave = true;
	}
    }
  if (config.profileMap.find("pub") != config.profileMap.end())
    {
      pub_send_mw.init(node_argc,
					     node_argv,
					     config.profileMap["pub"],
					     config.uuidMap["pub"],
					     enable_oob);
      if ( tg_duration < 0 )
	pub_send_mw.set_duration(pub_send_mw.profile.period);
      else
	pub_send_mw.set_duration(tg_duration);
      fName = "/var/log/" + config.nodeName + "." + config.compName + ".pub.network.csv";
      pub_send_mw.set_output_filename(fName);
    }

  // If required, start the publishing cycle
  if (starter)
    {
      if ( publish_period < 0 )
	{
	  ros::Time now = ros::Time::now();
	  timespec current_time;
	  current_time.tv_sec = now.sec;
	  current_time.tv_nsec = now.nsec;
	  double offset = pub_send_mw.profile.getOffset(current_time);
	  double period = pub_send_mw.profile.period;
	  uint64_t message_len = max_data_length + sin(offset * 2 * M_PI / period) * max_data_length * multiplier;

	  pubsub::pubsubTopic msg;
	  msg.uuid = sub_id++;
	  msg.bytes.resize(message_len,0);
	  try
	    {
	      usleep(5000000);
	      pub_send_mw.send<pubsub::pubsubTopic>(pub, msg);
	    }
	  catch ( Network::Exceeded_Production_Profile& ex )
	    {
	      logger->log("DEBUG","Prevented from sending on the network!");
	    }
	}
      else
	{
	  NAMESPACE::NodeHandle nh;
#ifdef USE_ROSMOD    
	  rosmod::ROSMOD_Callback_Options callback_options;
	  callback_options.alias = "pub_timer_operation";
	  callback_options.priority = 90;
	  callback_options.deadline.sec = 0;
	  callback_options.deadline.nsec = 2000000;
#endif
	  NAMESPACE::TimerOptions timer_options;
	  timer_options = 
	    NAMESPACE::TimerOptions
	    (ros::Duration(publish_period),
	     boost::bind(&comm::pub_timer_operation, this, _1),
	     &this->comp_queue,
#ifdef USE_ROSMOD     
	     callback_options,
#endif     
	     false,
	     false); 
	  pubTimer = nh.createTimer(timer_options);
	  pubTimer.start();
	}
    }

  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting comm::init_timer_operation");
#endif  
}
//# End Init Marker

void comm::mw_recv_done_operation(Network::receiver* receiver_mw)
{
  logger->log("DEBUG","Writing middleware log.");
  logger->log("DEBUG","Max middleware buffer: %lu bits", receiver_mw->buffer.maxBits());
  receiver_mw->record();
}

// Subscriber Operation - sub
//# Start sub_operation Marker
void comm::sub_operation(const pubsub::pubsubTopic::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering comm::sub_operation");
#endif
  // Business Logic for sub_operation

  uint64_t uuid = received_data->uuid;
  uint64_t msgBytes = ros::serialization::Serializer<pubsub::pubsubTopic>::serializedLength(*received_data);
  ros::Time now = ros::Time::now();
  Network::Message new_msg;
  new_msg.Bytes(msgBytes);
  new_msg.Id(uuid);
  new_msg.TimeStamp();
  sub_recv_mw.buffer.send(new_msg, msgBytes * 8);

  if ( ros::Time::now() >= pub_send_mw.get_end_time() )
    {
      logger->log("DEBUG","writing output\n");
      pub_send_mw.record();
      this->sub.shutdown();
    }
  
  if (!ender)
    {
      timespec current_time;
      current_time.tv_sec = now.sec;
      current_time.tv_nsec = now.nsec;
      double offset = pub_send_mw.profile.getOffset(current_time);
      double period = pub_send_mw.profile.period;
      uint64_t message_len = max_data_length +
	sin(offset * 2 * M_PI / period) * max_data_length * multiplier;

      pubsub::pubsubTopic msg;
      msg.uuid = sub_id++;
      msg.bytes.resize(message_len,0);
      try
	{
	  // Sleep for 10 ms
	  if (sleep_time)
	    usleep(sleep_time);	  
	  pub_send_mw.send<pubsub::pubsubTopic>(pub, msg);
	}
      catch ( Network::Exceeded_Production_Profile& ex )
	{
	  logger->log("DEBUG","Prevented from sending on the network!");
	}
    }
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting comm::sub_operation");
#endif
}
//# End sub_operation Marker


// Destructor - Cleanup Ports & Timers
comm::~comm()
{
  pub.shutdown();
  sub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void comm::startUp()
{
  NAMESPACE::NodeHandle nh;
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
  std::string log_file_path = pwd + config.nodeName + "." + config.compName + ".log"; 

  logger->create_file("/var/log/" + config.nodeName + "." + config.compName + ".log");
  logger->set_is_periodic(config.is_periodic_logging);
  logger->set_max_log_unit(config.periodic_log_unit);

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->create_file("/var/log/ROSMOD_DEBUG." + config.nodeName + "." + config.compName + ".log");
  comp_queue.ROSMOD_LOGGER->set_is_periodic(config.is_periodic_logging);
  comp_queue.ROSMOD_LOGGER->set_max_log_unit(config.periodic_log_unit);
#endif    
  
#ifdef USE_ROSMOD 
  this->comp_queue.scheduling_scheme = config.schedulingScheme;
  rosmod::ROSMOD_Callback_Options callback_options;
#endif  
  // Configure all publishers associated with this component
  // Component Publisher - pub
  advertiseName = "pubsubTopic";
  if (config.portGroupMap.find("pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["pub"];
  this->pub = nh.advertise<pubsub::pubsubTopic>(advertiseName.c_str(), 1000);


  if (config.num_comps_to_sync > 1 )
    {
      // Synchronize components now that all publishers and servers have been initialized
      this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
#ifdef USE_ROSMOD  
      rosmod::SubscribeOptions comp_sync_sub_options;
      rosmod::ROSMOD_Callback_Options sync_callback_options;
#else
      ros::SubscribeOptions comp_sync_sub_options;
#endif
      ros::Duration(config.comp_sync_timeout/2.0).sleep();
      comp_sync_sub_options = NAMESPACE::SubscribeOptions::create<std_msgs::Bool>
	("component_synchronization",
	 1000,
	 boost::bind(&comm::component_sync_operation, this, _1),
	 NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD     
	 &this->comp_queue,
	 sync_callback_options);
#else
         &this->comp_queue);
#endif
      this->comp_sync_sub = nh.subscribe(comp_sync_sub_options);

      ros::Time now = ros::Time::now();
      while ( this->comp_sync_sub.getNumPublishers() < this->config.num_comps_to_sync &&
	      (ros::Time::now() - now) < ros::Duration(config.comp_sync_timeout))
	ros::Duration(0.1).sleep();
      ros::Duration(config.comp_sync_timeout/2.0).sleep();
      this->comp_sync_sub.shutdown();  
      this->comp_sync_pub.shutdown();
    }

  // Configure all subscribers associated with this component
#ifdef USE_ROSMOD 
  callback_options.alias = "sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 20000000;
#endif  
  // Component Subscriber - sub
  advertiseName = "pubsubTopic";
  if (config.portGroupMap.find("sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["sub"];
  NAMESPACE::SubscribeOptions sub_options;
  sub_options = NAMESPACE::SubscribeOptions::create<pubsub::pubsubTopic>
      (advertiseName.c_str(),
       1000,
       boost::bind(&comm::sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif
  sub_options.transport_hints = NAMESPACE::TransportHints().udp();
  this->sub = nh.subscribe(sub_options);

  // Init Timer
#ifdef USE_ROSMOD    
  callback_options.alias = "init_timer_operation";
  callback_options.priority = 99;
  callback_options.deadline.sec = 1;
  callback_options.deadline.nsec = 0;
#endif
  NAMESPACE::TimerOptions timer_options;
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(-1),
     boost::bind(&comm::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();


  this->init_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new comm(config,argc,argv);
  }
}
