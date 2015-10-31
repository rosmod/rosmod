#include "tlc/controller.hpp"

//# Start User Globals Marker
std::string NSGREEN;
std::string WEGREEN;

int step = 0;

void controller::clock_value(const std::string& ns_state,
			     int& clock_WE,
			     int& clock_NS,
			     std::string& tl_state)
{
  if (!tl_state.compare(ns_state))
    {
      clock_NS = clock_NS + 1;
      clock_WE = 0;
    }
  else
    {
      clock_WE = clock_WE + 1;
      clock_NS = 0;
    }
}

void controller::controller_main(std::string& tl_state,
				 const std::string& we_state,
				 const std::string& ns_state,
				 int queue_WE,
				 int queue_NS,
				 int& clock_WE,
				 int& clock_NS)
{
  if ((queue_WE < _s_WE && queue_NS < _s_NS) || (queue_WE >= _s_WE && queue_NS >= _s_NS))
    {
      if ( !tl_state.compare(we_state) && clock_WE > _Light_Max )
	{
	  tl_state = ns_state;
	  clock_WE = 0;
	}
      if ( !tl_state.compare(ns_state) && clock_NS > _Light_Max )
	{
	  tl_state = we_state;
	  clock_NS = 0;
	}
    }
  else if (queue_WE >= _s_WE && queue_NS < _s_NS)
    {
      if ( !tl_state.compare(ns_state) && clock_NS > _Light_Min )
	{
	  tl_state = we_state;
	  clock_NS = 0;
	}
      if ( !tl_state.compare(we_state) && clock_WE > _Light_Max )
	{
	  tl_state = ns_state;
	  clock_WE = 0;
	}
    }
  else
    {
      if ( !tl_state.compare(we_state) && clock_WE > _Light_Min )
	{
	  tl_state = ns_state;
	  clock_WE = 0;
	}
      if ( !tl_state.compare(ns_state) && clock_NS > _Light_Max )
	{
	  tl_state = we_state;
	  clock_NS = 0;
	}
    }
  tlc::ryg_control new_control;
  if (_id.length() > 0)
    {
      logger->log("DEBUG","Publishing new TL state:: %s = %s", _id.c_str(), tl_state.c_str());
      new_control.intersection_name = _id;
      new_control.state = tl_state;
      ryg_control_pub.publish(new_control);
    }
}
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void controller::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering controller::init_timer_operation");
#endif
  // Initialize Here
  int num_lanes_north = 1;
  int num_lanes_south = 1;
  int num_lanes_east = 1;
  int num_lanes_west = 1;
  _id = "IK";
  _Light_Min = 300;   // step size is 0.1 seconds
  _Light_Max = 1200;  // step size is 0.1 seconds
  _s_NS = 4;
  _s_WE = 15;
  _clock[0] = 0; _clock[1] = 0;
  _queue[0] = 0; _queue[1] = 0;
  std::vector<std::string> sensors = {"north",
				      "south",
				      "west",
				      "east"};
  for (auto it = sensors.begin(); it != sensors.end(); ++it)
    {
      _num_vehicles_map[*it] = 0;
    }
  for (int i=0;i<node_argc;i++)
    {
      if (!strcmp(node_argv[i],"--id"))
	{
	  _id = node_argv[i+1];
	}
      if (!strcmp(node_argv[i],"--light_min"))
	{
	  _Light_Min = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--light_max"))
	{
	  _Light_Max = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--s_NS"))
	{
	  _s_NS = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--s_WE"))
	{
	  _s_WE = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--num_lanes_north"))
	{
	  num_lanes_north = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--num_lanes_south"))
	{
	  num_lanes_south = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--num_lanes_east"))
	{
	  num_lanes_east = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--num_lanes_west"))
	{
	  num_lanes_west = atoi(node_argv[i+1]);
	}
      for (auto it = sensors.begin(); it != sensors.end(); ++it)
	{
	  std::string cmpstr = "--";
	  cmpstr += *it;
	  if (!cmpstr.compare(node_argv[i]))
	    {
	      _sensor_id_map[node_argv[i+1]] = *it;
	      logger->log("DEBUG","using sensor ID %s as %s sensor",
			  node_argv[i+1],
			  it->c_str());
	      break;
	    }
	}
    }
  for (int i = 0; i < num_lanes_east; i++)
    {
      NSGREEN += 'r';
      WEGREEN += 'G';
    }
  for (int i = 0; i < num_lanes_west; i++)
    {
      NSGREEN += 'r';
      WEGREEN += 'g';
    }
  for (int i = 0; i < num_lanes_north; i++)
    {
      NSGREEN += 'G';
      WEGREEN += 'r';
    }
  for (int i = 0; i < num_lanes_south; i++)
    {
      NSGREEN += 'g';
      WEGREEN += 'r';
    }
  for (int i = 0; i < num_lanes_east; i++)
    {
      NSGREEN += 'r';
      WEGREEN += 'G';
    }
  for (int i = 0; i < num_lanes_west; i++)
    {
      NSGREEN += 'r';
      WEGREEN += 'g';
    }
  for (int i = 0; i < num_lanes_north; i++)
    {
      NSGREEN += 'G';
      WEGREEN += 'r';
    }
  for (int i = 0; i < num_lanes_south; i++)
    {
      NSGREEN += 'g';
      WEGREEN += 'r';
    }
  logger->log("DEBUG","using NSGREEN: %s", NSGREEN.c_str());
  logger->log("DEBUG","using WEGREEN: %s", WEGREEN.c_str());
  _state = WEGREEN;
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting controller::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - ryg_state_sub
//# Start ryg_state_sub_operation Marker
void controller::ryg_state_sub_operation(const tlc::ryg_state::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering controller::ryg_state_sub_operation");
#endif
  // Business Logic for ryg_state_sub_operation
  if (received_data->state.length() && !_id.compare(received_data->intersection_name))
    _current_state = received_data->state;
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting controller::ryg_state_sub_operation");
#endif
}
//# End ryg_state_sub_operation Marker
// Subscriber Operation - sensor_state_sub
//# Start sensor_state_sub_operation Marker
void controller::sensor_state_sub_operation(const tlc::sensor_state::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering controller::sensor_state_sub_operation");
#endif
  // Business Logic for sensor_state_sub_operation
  if ( _sensor_id_map.find(received_data->sensor_name) != _sensor_id_map.end() )
    {
      _num_vehicles_map[_sensor_id_map[received_data->sensor_name]] = received_data->num_vehicles;
    }
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting controller::sensor_state_sub_operation");
#endif
}
//# End sensor_state_sub_operation Marker

// Timer Callback - controller_timer
//# Start controller_timer_operation Marker
void controller::controller_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
	  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering controller::controller_timer_operation");
#endif
  // Business Logic for controller_timer_operation

  //First we compute the queue length of West-East direction
  int queue_l1, queue_l2;
  _queue[0] = _num_vehicles_map["east"] + _num_vehicles_map["west"];
  logger->log("DEBUG","EW Q :: %d",_queue[0]);
  //Then we compute the length in North-South direction
  _queue[1] = _num_vehicles_map["north"] + _num_vehicles_map["south"];
  logger->log("DEBUG","NS Q :: %d",_queue[1]);
  //Now we compute the clock value of the traffic lights(value k in the paper)
  clock_value ( NSGREEN, _clock[0], _clock[1], _state );
  //Now we need to design the traffic light control logic
  controller_main( _state, WEGREEN, NSGREEN, _queue[0], _queue[1], _clock[0], _clock[1] );

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting controller::controller_timer_operation");
#endif
}
//# End controller_timer_operation Marker


// Destructor - Cleanup Ports & Timers
controller::~controller()
{
  controller_timer.stop();
  ryg_control_pub.shutdown();
  ryg_state_sub.shutdown();
  sensor_state_sub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void controller::startUp()
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

  // Component Publisher - ryg_control_pub
  advertiseName = "ryg_control";
  if (config.portGroupMap.find("ryg_control_pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["ryg_control_pub"];
  this->ryg_control_pub = nh.advertise<tlc::ryg_control>(advertiseName.c_str(), 1000);


  this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
#ifdef USE_ROSMOD  
  rosmod::SubscribeOptions comp_sync_sub_options;
  rosmod::ROSMOD_Callback_Options sync_callback_options;
#else
  ros::SubscribeOptions comp_sync_sub_options;
#endif
  
  comp_sync_sub_options = NAMESPACE::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&controller::component_sync_operation, this, _1),
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



#ifdef USE_ROSMOD 
  callback_options.alias = "ryg_state_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - ryg_state_sub
  advertiseName = "ryg_state";
  if (config.portGroupMap.find("ryg_state_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["ryg_state_sub"];
  NAMESPACE::SubscribeOptions ryg_state_sub_options;
  ryg_state_sub_options = NAMESPACE::SubscribeOptions::create<tlc::ryg_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller::ryg_state_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->ryg_state_sub = nh.subscribe(ryg_state_sub_options);
#ifdef USE_ROSMOD 
  callback_options.alias = "sensor_state_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - sensor_state_sub
  advertiseName = "sensor_state";
  if (config.portGroupMap.find("sensor_state_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["sensor_state_sub"];
  NAMESPACE::SubscribeOptions sensor_state_sub_options;
  sensor_state_sub_options = NAMESPACE::SubscribeOptions::create<tlc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller::sensor_state_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->sensor_state_sub = nh.subscribe(sensor_state_sub_options);

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
     boost::bind(&controller::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "controller_timer_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif
  // Component Timer - controller_timer
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&controller::controller_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->controller_timer = nh.createTimer(timer_options);

  this->init_timer.start();
  this->controller_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new controller(config,argc,argv);
  }
}
