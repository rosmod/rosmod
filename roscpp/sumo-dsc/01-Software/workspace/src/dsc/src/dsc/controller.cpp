#include "dsc/controller.hpp"


//# Start User Globals Marker
const std::string NSGREEN = "Grr";
const std::string NSYELLOW = "yrr";
const std::string WEGREEN = "rGG";
const std::string WEYELLOW = "ryy";

const std::string NSGREEN1 = "GGrr";
const std::string NSYELLOW1 = "yyrr";
const std::string WEGREEN1 = "rrGG";
const std::string WEYELLOW1 = "rryy";

int step = 0;

void controller::vehicle_number(std::string sensor1,
				std::string sensor2,
				int& queue_length)
{
  int numVehicles = _num_vehicles_map[sensor1];
  if (numVehicles == 0)
    _id_map[sensor1] = "";
  else
    {
      std::vector<std::string> list_T1 = _vehicle_ids_map[sensor1];
      for ( std::vector<std::string>::iterator it = list_T1.begin(); it != list_T1.end(); ++it)
	{
	  if ( *it != _id_map[sensor1] )
	    {
	      _id_map[sensor1] = *it;
	      _sum_map[sensor1] += 1;
	    }
	}
    }
  numVehicles = _num_vehicles_map[sensor2];
  if (numVehicles == 0)
    _id_map[sensor2] = "";
  else
    {
      std::vector<std::string> list_S1 = _vehicle_ids_map[sensor2];
      for ( std::vector<std::string>::iterator it = list_S1.begin(); it != list_S1.end(); ++it)
	{
	  if ( *it != _id_map[sensor2] )
	    {
	      _id_map[sensor2] = *it;
	      _sum_map[sensor2] += 1;
	    }
	}
    }
  queue_length = _sum_map[sensor1]-_sum_map[sensor2] + 1 ;
}

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
  dsc::ryg_control new_control;
  if (_id.length() > 0)
    {
      LOGGER.DEBUG("Publishing new TL state:: %s = %s", _id.c_str(), tl_state.c_str());
      new_control.intersection_name = _id;
      new_control.state = tl_state;
      ryg_control_pub.publish(new_control);
    }
}
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void controller::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  _id = "IK";
  _state = WEGREEN;
  _Light_Min = 30;
  _Light_Max = 120;
  _s_NS = 4;
  _s_WE = 15;
  _clock[0] = 0; _clock[1] = 0;
  _queue[0] = 0; _queue[1] = 0;
  std::vector<std::string> sensors = {"l1_ew_in","l1_ew_out",
				      "l2_ew_in","l2_ew_out",
				      "l1_ns_in","l1_ns_out"};
  for (auto it = sensors.begin(); it != sensors.end(); ++it)
    {
      _sum_map[*it] = 0;
      _num_vehicles_map[*it] = 0;
      _id_map[*it] = std::string();
      _vehicle_ids_map[*it] = std::vector<std::string>();
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
    }
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker



// Subscriber Callback - ryg_state_sub
//# Start ryg_state_sub_OnOneData Marker
void controller::ryg_state_sub_OnOneData(const dsc::ryg_state::ConstPtr& received_data)
{
  // Business Logic for ryg_state_sub Subscriber
  if (received_data->state.length())
    _current_state = received_data->state;
}
//# End ryg_state_sub_OnOneData Marker
// Subscriber Callback - l1_ew_in
//# Start l1_ew_in_OnOneData Marker
void controller::l1_ew_in_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for l1_ew_in Subscriber
  _num_vehicles_map["l1_ew_in"] = received_data->num_vehicles;
  _vehicle_ids_map["l1_ew_in"] = received_data->vehicle_ids;
}
//# End l1_ew_in_OnOneData Marker
// Subscriber Callback - l1_ew_out
//# Start l1_ew_out_OnOneData Marker
void controller::l1_ew_out_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for l1_ew_out Subscriber
  _num_vehicles_map["l1_ew_out"] = received_data->num_vehicles;
  _vehicle_ids_map["l1_ew_out"] = received_data->vehicle_ids;
}
//# End l1_ew_out_OnOneData Marker
// Subscriber Callback - l2_ew_in
//# Start l2_ew_in_OnOneData Marker
void controller::l2_ew_in_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for l2_ew_in Subscriber
  _num_vehicles_map["l2_ew_in"] = received_data->num_vehicles;
  _vehicle_ids_map["l2_ew_in"] = received_data->vehicle_ids;
}
//# End l2_ew_in_OnOneData Marker
// Subscriber Callback - l2_ew_out
//# Start l2_ew_out_OnOneData Marker
void controller::l2_ew_out_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for l2_ew_out Subscriber
  _num_vehicles_map["l2_ew_out"] = received_data->num_vehicles;
  _vehicle_ids_map["l2_ew_out"] = received_data->vehicle_ids;
}
//# End l2_ew_out_OnOneData Marker
// Subscriber Callback - l1_ns_in
//# Start l1_ns_in_OnOneData Marker
void controller::l1_ns_in_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for l1_ns_in Subscriber
  _num_vehicles_map["l1_ns_in"] = received_data->num_vehicles;
  _vehicle_ids_map["l1_ns_in"] = received_data->vehicle_ids;
}
//# End l1_ns_in_OnOneData Marker
// Subscriber Callback - l1_ns_out
//# Start l1_ns_out_OnOneData Marker
void controller::l1_ns_out_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for l1_ns_out Subscriber
  _num_vehicles_map["l1_ns_out"] = received_data->num_vehicles;
  _vehicle_ids_map["l1_ns_out"] = received_data->vehicle_ids;
}
//# End l1_ns_out_OnOneData Marker

// Timer Callback - controller_timer
//# Start controller_timerCallback Marker
void controller::controller_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for controller_timer Timer

  //First we compute the queue length of West-East direction
  int queue_l1, queue_l2;
  vehicle_number( "l1_ew_in", "l1_ew_out", queue_l1 );
  vehicle_number( "l2_ew_in", "l2_ew_out", queue_l2 );
  _queue[0] = queue_l1 + queue_l2;
  LOGGER.DEBUG("EW Q :: %d",_queue[0]);
  //Then we compute the length in North-South direction
  vehicle_number( "l1_ns_in", "l1_ns_out", _queue[1] );
  LOGGER.DEBUG("NS Q :: %d",_queue[1]);
  //Now we compute the clock value of the traffic lights(value k in the paper)
  clock_value ( NSGREEN, _clock[0], _clock[1], _state );
  //Now we need to design the traffic light control logic
  controller_main( _state, WEGREEN, NSGREEN, _queue[0], _queue[1], _clock[0], _clock[1] );
}
//# End controller_timerCallback Marker


// Destructor - Cleanup Ports & Timers
controller::~controller()
{
  controller_timer.stop();
  ryg_control_pub.shutdown();
  ryg_state_sub.shutdown();
  l1_ew_in.shutdown();
  l1_ew_out.shutdown();
  l2_ew_in.shutdown();
  l2_ew_out.shutdown();
  l1_ns_in.shutdown();
  l1_ns_out.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void controller::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO

  // Component Subscriber - ryg_state_sub
  advertiseName = "ryg_state";
  if (portGroupMap.find("ryg_state_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["ryg_state_sub"];
  ros::SubscribeOptions ryg_state_sub_options;
  ryg_state_sub_options = ros::SubscribeOptions::create<dsc::ryg_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller::ryg_state_sub_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->ryg_state_sub = nh.subscribe(ryg_state_sub_options);  
  // Component Subscriber - l1_ew_in
  advertiseName = "sensor_state";
  if (portGroupMap.find("l1_ew_in") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["l1_ew_in"];
  ros::SubscribeOptions l1_ew_in_options;
  l1_ew_in_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller::l1_ew_in_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->l1_ew_in = nh.subscribe(l1_ew_in_options);  
  // Component Subscriber - l1_ew_out
  advertiseName = "sensor_state";
  if (portGroupMap.find("l1_ew_out") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["l1_ew_out"];
  ros::SubscribeOptions l1_ew_out_options;
  l1_ew_out_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller::l1_ew_out_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->l1_ew_out = nh.subscribe(l1_ew_out_options);  
  // Component Subscriber - l2_ew_in
  advertiseName = "sensor_state";
  if (portGroupMap.find("l2_ew_in") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["l2_ew_in"];
  ros::SubscribeOptions l2_ew_in_options;
  l2_ew_in_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller::l2_ew_in_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->l2_ew_in = nh.subscribe(l2_ew_in_options);  
  // Component Subscriber - l2_ew_out
  advertiseName = "sensor_state";
  if (portGroupMap.find("l2_ew_out") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["l2_ew_out"];
  ros::SubscribeOptions l2_ew_out_options;
  l2_ew_out_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller::l2_ew_out_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->l2_ew_out = nh.subscribe(l2_ew_out_options);  
  // Component Subscriber - l1_ns_in
  advertiseName = "sensor_state";
  if (portGroupMap.find("l1_ns_in") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["l1_ns_in"];
  ros::SubscribeOptions l1_ns_in_options;
  l1_ns_in_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller::l1_ns_in_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->l1_ns_in = nh.subscribe(l1_ns_in_options);  
  // Component Subscriber - l1_ns_out
  advertiseName = "sensor_state";
  if (portGroupMap.find("l1_ns_out") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["l1_ns_out"];
  ros::SubscribeOptions l1_ns_out_options;
  l1_ns_out_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller::l1_ns_out_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->l1_ns_out = nh.subscribe(l1_ns_out_options);  

  // Component Publisher - ryg_control_pub
  advertiseName = "ryg_control";
  if (portGroupMap.find("ryg_control_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["ryg_control_pub"];
  this->ryg_control_pub = nh.advertise<dsc::ryg_control>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&controller::Init, this, _1),
     &this->compQueue,
     true,
     false);
  this->initOneShotTimer = nh.createTimer(timer_options);
  this->initOneShotTimer.stop();
  // Component Timer - controller_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&controller::controller_timerCallback, this, _1),
     &this->compQueue, false, false);
  this->controller_timer = nh.createTimer(timer_options);

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
     boost::bind(&controller::component_synchronization_OnOneData, this, _1),
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
  this->controller_timer.start();
  
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new controller(config,argc,argv);
  }
}
