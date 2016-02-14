#include "multi_node_sample/receiverC.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void receiverC::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering receiverC::init_timer_operation");
#endif
  // Initialize Here
  srand (time(NULL));
  double tg_duration = -1;
  std::string fName;
  bool enable_oob = false;
  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--tg_time"))
	{
	  tg_duration = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i], "--enable_oob"))
	{
	  enable_oob = true;
	}
    }
  uint64_t capacityBits = 0;
  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--buffer_capacity_bits"))
	{
	  capacityBits = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i], "--buffer_capacity_bytes"))
	{
	  capacityBits = atoi(node_argv[i+1]) * 8;
	}
    }
  if (config.profileMap.find("subscriberC") != config.profileMap.end())
    {
      subscriberC_recv_mw.init(node_argc,
					     node_argv,
					     config.profileMap["subscriberC"],
					     capacityBits,
					     enable_oob);
      if ( tg_duration < 0 )
	subscriberC_recv_mw.set_duration(subscriberC_recv_mw.profile.period);
      else
	subscriberC_recv_mw.set_duration(tg_duration);
      fName = config.nodeName + "." + config.compName + ".subscriberC.network.csv";
      subscriberC_recv_mw.set_output_filename(fName);
      subscriberC_recv_mw.set_recv_done_callback(boost::bind(&receiverC::mw_recv_done_operation, this, &subscriberC_recv_mw));
      subscriberC_id = 0;
    }
  if (config.portSenderMap.find("subscriberC") != config.portSenderMap.end())
    {
      for (auto it=config.portSenderMap["subscriberC"].begin();
	   it != config.portSenderMap["subscriberC"].end(); ++it)
	{
	  subscriberC_recv_mw.add_sender( it->first, it->second );
	}
    }
  
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting receiverC::init_timer_operation");
#endif  
}
//# End Init Marker


void receiverC::mw_recv_done_operation(Network::receiver* receiver_mw)
{
  logger->log("DEBUG","Writing middleware log.");
  logger->log("DEBUG","Max middleware buffer: %lu bits", receiver_mw->buffer.maxBits());
  receiver_mw->record();
}

// Subscriber Operation - subscriberC
//# Start subscriberC_operation Marker
void receiverC::subscriberC_operation(const multi_node_sample::messageC::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering receiverC::subscriberC_operation");
#endif
  // Business Logic for subscriberC_operation

  uint64_t uuid = received_data->uuid;
  uint64_t msgBytes = ros::serialization::Serializer<multi_node_sample::messageC>::serializedLength(*received_data);
  ros::Time now = ros::Time::now();
  subscriberC_recv_mw.update_sender_stream(uuid, now, msgBytes * 8);
  Network::Message new_msg;
  new_msg.Bytes(msgBytes);
  new_msg.Id(subscriberC_id++);
  new_msg.TimeStamp();
  subscriberC_recv_mw.buffer.send(new_msg, msgBytes * 8);
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting receiverC::subscriberC_operation");
#endif
}
//# End subscriberC_operation Marker


// Destructor - Cleanup Ports & Timers
receiverC::~receiverC()
{
  subscriberC.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void receiverC::startUp()
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

  logger->create_file(pwd + config.nodeName + "." + config.compName + ".log");
  logger->set_is_periodic(config.is_periodic_logging);
  logger->set_max_log_unit(config.periodic_log_unit);

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->create_file(pwd + "ROSMOD_DEBUG." + config.nodeName + "." + config.compName + ".log");
  comp_queue.ROSMOD_LOGGER->set_is_periodic(config.is_periodic_logging);
  comp_queue.ROSMOD_LOGGER->set_max_log_unit(config.periodic_log_unit);
#endif    
  
#ifdef USE_ROSMOD 
  this->comp_queue.scheduling_scheme = config.schedulingScheme;
  rosmod::ROSMOD_Callback_Options callback_options;
#endif  

  // Synchronize components now that all publishers and servers have been initialized
  this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);

  ros::Duration(0.5).sleep();

#ifdef USE_ROSMOD  
  rosmod::SubscribeOptions comp_sync_sub_options;
  rosmod::ROSMOD_Callback_Options sync_callback_options;
#else
  ros::SubscribeOptions comp_sync_sub_options;
#endif
  
  comp_sync_sub_options = NAMESPACE::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&receiverC::component_sync_operation, this, _1),
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
  ros::Duration(0.5).sleep();
  this->comp_sync_sub.shutdown();  
  this->comp_sync_pub.shutdown();

  // Configure all subscribers associated with this component
#ifdef USE_ROSMOD 
  callback_options.alias = "subscriberC_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 5000000;
#endif  
  // Component Subscriber - subscriberC
  advertiseName = "messageC";
  if (config.portGroupMap.find("subscriberC") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["subscriberC"];
  NAMESPACE::SubscribeOptions subscriberC_options;
  subscriberC_options = NAMESPACE::SubscribeOptions::create<multi_node_sample::messageC>
      (advertiseName.c_str(),
       1000,
       boost::bind(&receiverC::subscriberC_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->subscriberC = nh.subscribe(subscriberC_options);

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
     boost::bind(&receiverC::init_timer_operation, this, _1),
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
    return new receiverC(config,argc,argv);
  }
}
