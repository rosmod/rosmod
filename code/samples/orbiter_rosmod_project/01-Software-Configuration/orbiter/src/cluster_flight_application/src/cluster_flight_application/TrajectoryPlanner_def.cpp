#include "cluster_flight_application/TrajectoryPlanner_def.hpp"

//# Start User Globals Marker

//# End User Globals Marker

// -------------------------------------------------------
// BUSINESS LOGIC OF THESE FUNCTIONS SUPPLIED BY DEVELOPER
// -------------------------------------------------------

// Init Function
//# Start Init Marker
void TrajectoryPlanner_def::Init(const ros::TimerEvent& event)
{
    // Initialize Component
    ROS_INFO("Ready to receive commands and determine new orbits; running on satellite %s", hostName.c_str());
    // Stop Init Timer
    initOneShotTimer.stop();
}
//# End Init Marker

// OnOneData Subscription handler for satStateSub subscriber
//# Start satStateSub_OnOneData Marker
void TrajectoryPlanner_def::satStateSub_OnOneData(const satellite_flight_application::SatState::ConstPtr& received_data)
{
    // Business Logic for satStateSub subscriber subscribing to topic SatState callback
  ROS_INFO("I got a satellite state from satellite %s",received_data->sat_id.c_str()); 
}
//# End satStateSub_OnOneData Marker
// OnOneData Subscription handler for satCommandSub subscriber
//# Start satCommandSub_OnOneData Marker
void TrajectoryPlanner_def::satCommandSub_OnOneData(const satellite_flight_application::GroundCommand::ConstPtr& received_data)
{
    // Business Logic for satCommandSub subscriber subscribing to topic GroundCommand callback 
  ROS_INFO("Calculating new orbit");
  cluster_flight_application::TargetOrbit orbit;
  ROS_INFO("Publishing new target orbit");
  targetOrbitPub.publish(orbit);
}
//# End satCommandSub_OnOneData Marker

// ---------------------------------------------
// EVERYTHING BELOW HERE IS COMPLETELY GENERATED
// ---------------------------------------------

// Destructor - required for clean shutdown when process is killed
TrajectoryPlanner_def::~TrajectoryPlanner_def()
{
    targetOrbitPub.shutdown();
    satStateSub.shutdown();
    satCommandSub.shutdown();
//# Start Destructor Marker

//# End Destructor Marker
}

void TrajectoryPlanner_def::startUp()
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

    // Configure all subscribers associated with this component
    // subscriber: satStateSub
    advertiseName = "SatState";
    if ( portGroupMap != NULL && portGroupMap->find("satStateSub") != portGroupMap->end() )
        advertiseName += "_" + (*portGroupMap)["satStateSub"];
    ros::SubscribeOptions satStateSub_options;
    satStateSub_options = 
	ros::SubscribeOptions::create<satellite_flight_application::SatState>
	    (advertiseName.c_str(),
	     1000,
	     boost::bind(&TrajectoryPlanner_def::satStateSub_OnOneData, this, _1),
	     ros::VoidPtr(),
             &this->compQueue);
    this->satStateSub = nh.subscribe(satStateSub_options);
    // subscriber: satCommandSub
    advertiseName = "GroundCommand";
    if ( portGroupMap != NULL && portGroupMap->find("satCommandSub") != portGroupMap->end() )
        advertiseName += "_" + (*portGroupMap)["satCommandSub"];
    ros::SubscribeOptions satCommandSub_options;
    satCommandSub_options = 
	ros::SubscribeOptions::create<satellite_flight_application::GroundCommand>
	    (advertiseName.c_str(),
	     1000,
	     boost::bind(&TrajectoryPlanner_def::satCommandSub_OnOneData, this, _1),
	     ros::VoidPtr(),
             &this->compQueue);
    this->satCommandSub = nh.subscribe(satCommandSub_options);

    // Configure all publishers associated with this component
    // publisher: targetOrbitPub
    advertiseName = "TargetOrbit";
    if ( portGroupMap != NULL && portGroupMap->find("targetOrbitPub") != portGroupMap->end() )
        advertiseName += "_" + (*portGroupMap)["targetOrbitPub"];
    this->targetOrbitPub = nh.advertise<cluster_flight_application::TargetOrbit>
	(advertiseName.c_str(), 1000);	

    // Create Init Timer
    ros::TimerOptions timer_options;
    timer_options = 
	ros::TimerOptions
	    (ros::Duration(-1),
	     boost::bind(&TrajectoryPlanner_def::Init, this, _1),
	     &this->compQueue,
             true);
    this->initOneShotTimer = nh.createTimer(timer_options);  
  
}
