#include "satellite_flight_application/SatelliteBusInterface_def.hpp"

//# Start User Globals Marker

//# End User Globals Marker

// -------------------------------------------------------
// BUSINESS LOGIC OF THESE FUNCTIONS SUPPLIED BY DEVELOPER
// -------------------------------------------------------

// Init Function
//# Start Init Marker
void SatelliteBusInterface_def::Init(const ros::TimerEvent& event)
{
    // Initialize Component

    // Stop Init Timer
    initOneShotTimer.stop();
}
//# End Init Marker

// Component Service Callback
//# Start SatelliteStateCallback Marker
bool SatelliteBusInterface_def::SatelliteStateCallback(satellite_flight_application::SatelliteState::Request  &req,
    satellite_flight_application::SatelliteState::Response &res)
{
    int sockfd;
    u_short server_portno;
    char* server_ip_str = "191.168.127.100";
    server_portno = 37777;

    /* socket: create the socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) <0) {
        perror("ERROR opening socket");
	return false;
    }

    /* build the server's Internet address */
    struct sockaddr_in server_addr;    

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_portno);

    if (inet_pton(AF_INET, server_ip_str, &(server_addr.sin_addr)) !=1) {
       perror("inet_pton");
       return false;
    }
    
    memset(server_addr.sin_zero, 0, sizeof server_addr.sin_zero);

    /* connect: create a connection with the server */
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
      perror("ERROR connecting");
      return false;
    }


    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
      LOGGER.INFO("Couldn't set sockopts!");
      close(sockfd);
      return false;
    }

    /* send the message line to the server */

    int sentbytes=0,numbytes=0;
    char msgbuf[100];
    memset(msgbuf,0,100);
    sprintf(msgbuf,"Ship:%s:Status2:1:1:1\r\n",hostName.c_str());
    if ( numbytes = send(sockfd, msgbuf, strlen(msgbuf),0) == -1) {
      perror("send");
      return false;
    }
    LOGGER.INFO("Sent %s, #bytes = %d, return code = %d\n",
	     msgbuf,strlen(msgbuf),numbytes);
	
    char recvbuf[1024];
    memset(recvbuf,0,1024);
    int bytesreceived =0;
    if ( (bytesreceived=recv(sockfd,recvbuf,1023,0)) <= 0) {
      perror("recv");
      return false;
    }
    LOGGER.INFO("Received %s, #bytes = %d\n",
	     recvbuf,bytesreceived);

    close(sockfd);

    char * pch;
    pch = strtok (recvbuf,",");
    int count = 0;

    satellite_flight_application::SatState state;

    state.sat_id = hostName;

    while (pch != NULL)
    {
        count++;
        printf ("%s\n",pch);
        switch (count-7) {
        case 0:     // rpos_x
            state.rpos_x = atof(pch);
            break;
        case 1:     // rpos_y
            state.rpos_y = atof(pch);
            break;
        case 2:     // rpos_z
            state.rpos_z = atof(pch);
            break;
        case 3:     // rvel_x
            state.rvel_x = atof(pch);
            break;
        case 4:     // rvel_y
            state.rvel_y = atof(pch);
            break;
        case 5:     // rvel_z
            state.rvel_z = atof(pch);
            break;
        case 6:     // vrot_x
            state.vrot_x = atof(pch);
            break;
        case 7:     // vrot_y
            state.vrot_y = atof(pch);
            break;
        case 8:     // vrot_z
            state.vrot_z = atof(pch);
            break;
        default:
            break;
        }
        pch = strtok (NULL, ",");
    }

    // Business Logic for SatelliteState_server Server providing SatelliteState Service
  LOGGER.INFO("Returning the current satellite state vector for satellite %s.",hostName.c_str());
  res.state = state;
    return true;
}
//# End SatelliteStateCallback Marker
// Component Service Callback
//# Start ThrusterCommCallback Marker
bool SatelliteBusInterface_def::ThrusterCommCallback(satellite_flight_application::ThrusterComm::Request  &req,
    satellite_flight_application::ThrusterComm::Response &res)
{
    // Business Logic for ThrusterComm_server Server providing ThrusterComm Service
  int thruster_id = req.thruster_id;
  float amount = req.amount;
  float duration = req.duration;
  int retVal = 0;

	int sockfd;
	u_short server_portno;
	char* server_ip_str = "191.168.127.100";
	server_portno = 37777;

	/* socket: create the socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) <0) {
		perror("ERROR opening socket");
		return false;
	}

	/* build the server's Internet address */
	struct sockaddr_in server_addr;


	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_portno);

	if (inet_pton(AF_INET, server_ip_str, &(server_addr.sin_addr)) !=1) {
		perror("inet_pton");
		return false;
	}

	memset(server_addr.sin_zero, 0, sizeof server_addr.sin_zero);

	/* connect: create a connection with the server */
	if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
	  perror("ERROR connecting");
	  return false;
	}

	/* send the message line to the server */

	int sentbytes=0,numbytes=0;
	char msgbuf[100];
	memset(msgbuf,0,100);
	sprintf(msgbuf,"Ship:%s:SetEngineGrpLevel:0:%f\r\n",hostName.c_str(),amount);
	if ( numbytes = send(sockfd, msgbuf, strlen(msgbuf),0) == -1) {
		perror("send");
		return false;
	}
	LOGGER.INFO("Sent %s, #bytes = %d, return code = %d\n",
		 msgbuf,strlen(msgbuf),numbytes);

	char recvbuf[1024];
	memset(recvbuf,0,1024);
	int bytesreceived =0;
	if ( (bytesreceived=recv(sockfd,recvbuf,1023,0)) <= 0) {
		perror("recv");
		return false;
	}
	LOGGER.INFO("Received %s, #bytes = %d\n",
		 recvbuf,bytesreceived);


	int seconds = (int)duration;
	int nanoseconds = (int)((duration-seconds)*1000000000);
	LOGGER.INFO("Activating engine for %f = %d seconds and %d nanoseconds\r\n",duration,seconds,nanoseconds);
	ros::Duration(duration).sleep();
	LOGGER.INFO("Turning off engine now.");

	memset(msgbuf,0,100);
	sprintf(msgbuf,"Ship:%s:SetEngineGrpLevel:0:0\r\n",hostName.c_str());
	if ( numbytes = send(sockfd, msgbuf, strlen(msgbuf),0) == -1) {
		perror("send");
		return false;
	}
	LOGGER.INFO("Sent %s, #bytes = %d, return code = %d\n",
		 msgbuf,strlen(msgbuf),numbytes);

	memset(recvbuf,0,1024);
	bytesreceived =0;
	if ( (bytesreceived=recv(sockfd,recvbuf,1023,0)) <= 0) {
		perror("recv");
		return false;
	}
	LOGGER.INFO("Received %s, #bytes = %d\n",
		 recvbuf,bytesreceived);

	close(sockfd);

  LOGGER.INFO("Activated the satellite thruster %d for satellite %s.",thruster_id,hostName.c_str());
  res.retVal = retVal;
    return true;
}
//# End ThrusterCommCallback Marker

// ---------------------------------------------
// EVERYTHING BELOW HERE IS COMPLETELY GENERATED
// ---------------------------------------------

// Destructor - required for clean shutdown when process is killed
SatelliteBusInterface_def::~SatelliteBusInterface_def()
{
    SatelliteState_server.shutdown();
    ThrusterComm_server.shutdown();
//# Start Destructor Marker

//# End Destructor Marker
}

void SatelliteBusInterface_def::startUp()
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

    // Configure all provided services associated with this component
    // server: SatelliteState_server
    advertiseName = "SatelliteState";
    if ( portGroupMap != NULL && portGroupMap->find("SatelliteState_server") != portGroupMap->end() )
        advertiseName += "_" + (*portGroupMap)["SatelliteState_server"];
    ros::AdvertiseServiceOptions SatelliteState_server_options;
    SatelliteState_server_options = 
	ros::AdvertiseServiceOptions::create<satellite_flight_application::SatelliteState>
	    (advertiseName.c_str(),
             boost::bind(&SatelliteBusInterface_def::SatelliteStateCallback, this, _1, _2),
	     ros::VoidPtr(),
             &this->compQueue);
    this->SatelliteState_server = nh.advertiseService(SatelliteState_server_options);
    // server: ThrusterComm_server
    advertiseName = "ThrusterComm";
    if ( portGroupMap != NULL && portGroupMap->find("ThrusterComm_server") != portGroupMap->end() )
        advertiseName += "_" + (*portGroupMap)["ThrusterComm_server"];
    ros::AdvertiseServiceOptions ThrusterComm_server_options;
    ThrusterComm_server_options = 
	ros::AdvertiseServiceOptions::create<satellite_flight_application::ThrusterComm>
	    (advertiseName.c_str(),
             boost::bind(&SatelliteBusInterface_def::ThrusterCommCallback, this, _1, _2),
	     ros::VoidPtr(),
             &this->compQueue);
    this->ThrusterComm_server = nh.advertiseService(ThrusterComm_server_options);
 
    // Create Init Timer
    ros::TimerOptions timer_options;
    timer_options = 
	ros::TimerOptions
	    (ros::Duration(-1),
	     boost::bind(&SatelliteBusInterface_def::Init, this, _1),
	     &this->compQueue,
             true);
    this->initOneShotTimer = nh.createTimer(timer_options);  
  

    /*
     * Identify present working directory of node executable
     */
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
    // Establish the log file name
    std::string log_file_path = pwd + nodeName + "." + compName + ".log"; 

    // Create the log file & open file stream
    LOGGER.CREATE_FILE(log_file_path);

    // Establish log levels of LOGGER
    Log_Levels target_log_levels = {.DEBUG = false,
				    .INFO = true,
				    .WARNING = false,
				    .ERROR = true,
				    .CRITICAL = true};
    LOGGER.SET_LOG_LEVELS(target_log_levels);
}
