#include "pkg/link_profile_enforcer.hpp"


//# Start User Globals Marker
void link_profile_enforcer::profile_timerCallback(const ros::TimerEvent& event)
{
  std::string tc_binary = "/sbin/tc";
  unsigned long long bandwidth;
  unsigned long long latency;
  profile.getCurrentInterval( bandwidth, latency );

  LOGGER.DEBUG("Setting link bw to %llu",bandwidth);

  std::string tc_args = "qdisc replace dev " + intf_name + " parent 111:1 handle 1111: tbf rate ";
  if (bandwidth == 0)
    bandwidth = 1;
  char bandwidth_str[100];
  sprintf(bandwidth_str,"%llu",bandwidth);
  tc_args += bandwidth_str;
  tc_args += "bit peakrate ";
  sprintf(bandwidth_str,"%llu",(unsigned long long)((double)bandwidth * 1.001f));
  tc_args += bandwidth_str;
  tc_args += "bit mtu 8192 latency 100s burst 1540000000"; // latency here is the maximum time in the tbf

  // FORK
  pid_t parent = getpid();
  pid_t pid = fork();
  if ( pid == -1 )
    {
      LOGGER.DEBUG("ERROR: COULDNT FORK");
    }
  else if ( pid > 0 ) // parent
    {
      timespec start;
      profile.getNextInterval( start, bandwidth, latency );
      
      double dStart = (double)start.tv_sec + (double)start.tv_nsec / (double)1000000000.0;
      ros::Time tStart(dStart);
      ros::Duration nextTime = tStart - ros::Time::now();
      
      ros::NodeHandle nh;
      ros::TimerOptions timer_options;
      timer_options = 
	ros::TimerOptions
	(nextTime,
	 boost::bind(&link_profile_enforcer::profile_timerCallback, this, _1),
	 &this->compQueue,
	 true);
      profile_timer = nh.createTimer(timer_options);
    }
  else  // child
    {
      std::vector<std::string> string_args;
      string_args.push_back(tc_binary);
      string s;
      istringstream f(tc_args);
      while ( getline(f, s, ' ') )
	{
	  string_args.push_back(s);
	}
      // build args
      char *args[string_args.size() + 1]; // must be NULL terminated
      args[string_args.size()] = NULL;
      for (int i=0; i < string_args.size(); i++)
	{
	  args[i] = new char[string_args[i].length()];
	  sprintf(args[i], "%s", string_args[i].c_str());
	}
      // EXECV
      execvp(args[0], args);
      LOGGER.DEBUG("ERROR: EXEC COULDN'T COMPLETE");
    }
}
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void link_profile_enforcer::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  srand (time(NULL));
  double tg_duration = -1;
  std::string fName = "link_profile.csv";
  intf_name = "eth0";
  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--tg_time"))
	{
	  tg_duration = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i], "--profile_name"))
	{
	  fName = node_argv[i+1];
	}
      if (!strcmp(node_argv[i], "--intf_name"))
	{
	  intf_name = node_argv[i+1];
	}
    }

  if ( !profile.initializeFromFile( fName.c_str() ) )
    {
      ros::NodeHandle nh;
      ros::TimerOptions timer_options;
      timer_options = 
	ros::TimerOptions
	(ros::Duration(-1),
	 boost::bind(&link_profile_enforcer::profile_timerCallback, this, _1),
	 &this->compQueue,
	 true);
      profile_timer = nh.createTimer(timer_options);
    }
  
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker



// Destructor - Cleanup Ports & Timers
link_profile_enforcer::~link_profile_enforcer()
{
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void link_profile_enforcer::startUp()
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

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&link_profile_enforcer::Init, this, _1),
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
     boost::bind(&link_profile_enforcer::component_synchronization_OnOneData, this, _1),
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
    return new link_profile_enforcer(config,argc,argv);
  }
}
