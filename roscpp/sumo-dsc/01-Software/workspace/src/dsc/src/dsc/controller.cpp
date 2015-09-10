#include "dsc/controller.hpp"


//# Start User Globals Marker
int Light_Min = 30; 
int Light_Max = 120; //the constrains of the length of traffic lights for intersection

int s_NS = 4; 
int s_WE = 15; //the threshold of North-South and West-East direction for intersection

const std::string NSGREEN = "Grr";
const std::string NSYELLOW = "yrr";
const std::string WEGREEN = "rGG";
const std::string WEYELLOW = "ryy";

const std::string NSGREEN1 = "GGrr";
const std::string NSYELLOW1 = "yyrr";
const std::string WEGREEN1 = "rrGG";
const std::string WEYELLOW1 = "rryy";

int _clock[2] = {0,0};
int _queue[2] = {0,0};

std::string id_S[2][2];
std::string _state;

int step = 0;

int total_latency=0;
int car_number=0;
int car_latency=0;
int truck_number=0;
int truck_latency=0;


void vehicle_number(std::string sensor1,
		    std::string sensor2,
		    std::string& id_T1,
		    std::string& id_S1,
		    int& sum_sensor1,
		    int& sum_sensor2,
		    int& queue_length)
{
  int numVehicles = 0;
  //client.getLastStepInductionLoopVehicleNumber(sensor1, numVehicles);
  if (numVehicles == 0)
    id_T1="";
  else
    {
      std::vector<std::string> list_T1;
      //client.getLastStepInductionLoopVehicleIDs(sensor1, list_T1);
      for ( std::vector<std::string>::iterator it = list_T1.begin(); it != list_T1.end(); ++it)
	{
	  if ( *it != id_T1 )
	    {
	      id_T1 = *it;
	      sum_sensor1 += 1;
	    }
	}
    }
  //client.getLastStepInductionLoopVehicleNumber(sensor2, numVehicles);
  if (numVehicles == 0)
    id_S1="";
  else
    {
      std::vector<std::string> list_S1;
      //client.getLastStepInductionLoopVehicleIDs(sensor2, list_S1);
      for ( std::vector<std::string>::iterator it = list_S1.begin(); it != list_S1.end(); ++it)
	{
	  if ( *it != id_S1 )
	    {
	      id_S1 = *it;
	      sum_sensor2 += 1;
	    }
	}
    }
  queue_length = sum_sensor1-sum_sensor2 + 1 ;
}

void clock_value(std::string intersection,
		 int& clock_WE,
		 int& clock_NS,
		 std::string& tl_state)
{
  //client.getRedYellowGreenState(intersection, tl_state);
  if (!tl_state.compare(NSGREEN))
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

void clock_value1(std::string intersection,
		  int& clock_WE,
		  int& clock_NS,
		  std::string& tl_state)
{
  //client.getRedYellowGreenState(intersection, tl_state);
  if (!tl_state.compare(NSGREEN1))
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

void controller_main(std::string intersection,
		     std::string& tl_state,
		     int queue_WE,
		     int queue_NS,
		     int& clock_WE,
		     int& clock_NS,
		     int Light_Min,
		     int Light_Max,
		     int s_WE,
		     int s_NS)
{
  if ((queue_WE < s_WE && queue_NS < s_NS) || (queue_WE >= s_WE && queue_NS >= s_NS))
    {
      if ( !tl_state.compare(WEGREEN) && clock_WE > Light_Max )
	{
	  //client.setRedYellowGreenState(intersection, NSGREEN);
	  clock_WE = 0;
	}
      if ( !tl_state.compare(NSGREEN) && clock_NS > Light_Max )
	{
	  //client.setRedYellowGreenState(intersection, WEGREEN);
	  clock_NS = 0;
	}
    }
  else if (queue_WE >= s_WE && queue_NS <s_NS)
    {
      if ( !tl_state.compare(NSGREEN) && clock_NS > Light_Min )
	{
	  //client.setRedYellowGreenState(intersection, WEGREEN);
	  clock_NS = 0;
	}
      if ( !tl_state.compare(WEGREEN) && clock_WE > Light_Max )
	{
	  //client.setRedYellowGreenState(intersection, NSGREEN);
	  clock_WE = 0;
	}
    }
  else
    {
      if ( !tl_state.compare(WEGREEN) && clock_WE > Light_Min )
	{
	  //client.setRedYellowGreenState(intersection, NSGREEN);
	  clock_WE = 0;
	}
      if ( !tl_state.compare(NSGREEN) && clock_NS > Light_Max )
	{
	  //client.setRedYellowGreenState(intersection, WEGREEN);
	  clock_NS = 0;
	}
    }
}

void controller1_main(std::string intersection,
		      std::string& tl_state,
		      int queue_WE,
		      int queue_NS,
		      int& clock_WE,
		      int& clock_NS,
		      int Light_Min,
		      int Light_Max,
		      int s_WE,
		      int s_NS)
{
  if ((queue_WE < s_WE && queue_NS < s_NS) || (queue_WE >= s_WE && queue_NS >= s_NS))
    {
      if ( !tl_state.compare(WEGREEN1) && clock_WE > Light_Max )
	{
	  //client.setRedYellowGreenState(intersection, NSGREEN1);
	  clock_WE = 0;
	}
      if ( !tl_state.compare(NSGREEN1) && clock_NS > Light_Max )
	{
	  //client.setRedYellowGreenState(intersection, WEGREEN1);
	  clock_NS = 0;
	}
    }
  else if (queue_WE >= s_WE && queue_NS <s_NS)
    {
      if ( !tl_state.compare(NSGREEN1) && clock_NS > Light_Min )
	{
	  //client.setRedYellowGreenState(intersection, WEGREEN1);
	  clock_NS = 0;
	}
      if ( !tl_state.compare(WEGREEN1) && clock_WE > Light_Max )
	{
	  //client.setRedYellowGreenState(intersection, NSGREEN1);
	  clock_WE = 0;
	}
    }
  else
    {
      if ( !tl_state.compare(WEGREEN1) && clock_WE > Light_Min )
	{
	  //client.setRedYellowGreenState(intersection, NSGREEN1);
	  clock_WE = 0;
	}
      if ( !tl_state.compare(NSGREEN1) && clock_NS > Light_Max )
	{
	  //client.setRedYellowGreenState(intersection, WEGREEN1);
	  clock_NS = 0;
	}
    }
}
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void controller::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  _id = "";
  for (int i=0;i<node_argc;i++)
    {
      if (!strcmp(node_argv[i],"--id"))
	{
	  _id = node_argv[i+1];
	}
      if (!strcmp(node_argv[i],"--light_min"))
	{
	  Light_Min = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--light_max"))
	{
	  Light_Max = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--s_NS"))
	{
	  s_NS = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--s_WE"))
	{
	  s_WE = atoi(node_argv[i+1]);
	}
    }

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker



// Subscriber Callback - sensor_state_sub
//# Start sensor_state_sub_OnOneData Marker
void controller::sensor_state_sub_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for sensor_state_sub Subscriber
  
}
//# End sensor_state_sub_OnOneData Marker
// Subscriber Callback - ryg_state_sub
//# Start ryg_state_sub_OnOneData Marker
void controller::ryg_state_sub_OnOneData(const dsc::ryg_state::ConstPtr& received_data)
{
  // Business Logic for ryg_state_sub Subscriber

}
//# End ryg_state_sub_OnOneData Marker

// Timer Callback - controller_timer
//# Start controller_timerCallback Marker
void controller::controller_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for controller_timer Timer

}
//# End controller_timerCallback Marker


// Destructor - Cleanup Ports & Timers
controller::~controller()
{
  controller_timer.stop();
  ryg_control_pub.shutdown();
  sensor_state_sub.shutdown();
  ryg_state_sub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void controller::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO

  // Component Subscriber - sensor_state_sub
  advertiseName = "sensor_state";
  if (portGroupMap.find("sensor_state_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["sensor_state_sub"];
  ros::SubscribeOptions sensor_state_sub_options;
  sensor_state_sub_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller::sensor_state_sub_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->sensor_state_sub = nh.subscribe(sensor_state_sub_options);  
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
