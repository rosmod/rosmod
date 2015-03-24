#include "satellite_flight_application/GroundInterface_def.hpp"

//# Start User Globals Marker
void *get_in_addr(struct sockaddr *sa){
	if (sa->sa_family == AF_INET){
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
u_short get_in_port(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return ((struct sockaddr_in*)sa)->sin_port;
    }

    return ((struct sockaddr_in6*)sa)->sin6_port;
}

u_long get_my_ip_with_prefix(char* prefix){

    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa ->ifa_addr->sa_family==AF_INET) { 
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("%s IP v4 Address %s\n", ifa->ifa_name, addressBuffer); 

            if (strncmp(addressBuffer,prefix,strnlen(prefix,15)) == 0)
                return ((struct in_addr *)tmpAddrPtr)->s_addr;
        } else if (ifa->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            printf("%s IP v6 Address %s\n ", ifa->ifa_name, addressBuffer); 
            printf("This function does not support IP v6 for now\n"); 
        } 
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
    return 0;
}
//# End User Globals Marker

// -------------------------------------------------------
// BUSINESS LOGIC OF THESE FUNCTIONS SUPPLIED BY DEVELOPER
// -------------------------------------------------------

// Init Function
//# Start Init Marker
void GroundInterface_def::Init(const ros::TimerEvent& event)
{
    // Initialize Component
int portno = 7777;
    char IP_prefix[16] = "191";

    if ((sockfd= socket(AF_INET, SOCK_DGRAM,0))< 0){
      LOGGER.DEBUG("Failed to create socket!");
    }	

    struct sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(portno);
    my_addr.sin_addr.s_addr = get_my_ip_with_prefix(IP_prefix); 
    if (my_addr.sin_addr.s_addr ==0) {
      LOGGER.DEBUG("can not find matching IP!");
      close(sockfd);
      return;
    }

    if (bind(sockfd,(struct sockaddr *)&my_addr,sizeof(my_addr))<0){
      LOGGER.DEBUG("Couldn't bind!");
      close(sockfd);
      return;
    }

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
      LOGGER.DEBUG("Couldn't set sockopts!");
      close(sockfd);
      return;
    }
    // Stop Init Timer
    initOneShotTimer.stop();
}
//# End Init Marker

// Callback for Timer0 timer
//# Start Timer0Callback Marker
void GroundInterface_def::Timer0Callback(const ros::TimerEvent& event)
{
    // Business Logic for Timer0 
  satellite_flight_application::GroundCommand gndCommand;

    char buf[1024];
    memset(buf,0,1024);	// 0 out the recv buffer
    struct sockaddr peer_addr;
    socklen_t peer_addr_len = sizeof peer_addr;

    int numbytes, recvbytes=0;

    if ((numbytes = recvfrom(sockfd, buf, 1023,MSG_DONTWAIT, (struct sockaddr *) &peer_addr, &peer_addr_len)) == -1) {
      LOGGER.DEBUG("Receive Timeout!");
      return;
    }
    else
      LOGGER.DEBUG("Server receives %s : %d bytes",buf,numbytes);

    char * pch = NULL;
    pch = strtok (buf,",");
    int count = 0;

    while (pch != NULL)
    {
      switch (count++) {
      case 0:     // command string, e.g. "START"
    	gndCommand.command = pch;
        break;
      case 1:     // duration
    	gndCommand.params[0] = atof(pch);
        break;
	// SHOULD GET OTHER PARAMS HERE IF WE WANT TO USE THEM
      default:
        pch = NULL;
        break;
      }
      pch = strtok (NULL, ",");
    }

  gndCommandPub.publish(gndCommand);
  LOGGER.DEBUG("Published new GroundCommand");
}
//# End Timer0Callback Marker

// ---------------------------------------------
// EVERYTHING BELOW HERE IS COMPLETELY GENERATED
// ---------------------------------------------

// Destructor - required for clean shutdown when process is killed
GroundInterface_def::~GroundInterface_def()
{
    Timer0.stop();
    gndCommandPub.shutdown();
//# Start Destructor Marker

//# End Destructor Marker
}

void GroundInterface_def::startUp()
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

    // Configure all publishers associated with this component
    // publisher: gndCommandPub
    advertiseName = "GroundCommand";
    if ( portGroupMap != NULL && portGroupMap->find("gndCommandPub") != portGroupMap->end() )
        advertiseName += "_" + (*portGroupMap)["gndCommandPub"];
    this->gndCommandPub = nh.advertise<satellite_flight_application::GroundCommand>
	(advertiseName.c_str(), 1000);	

    // Create Init Timer
    ros::TimerOptions timer_options;
    timer_options = 
	ros::TimerOptions
	    (ros::Duration(-1),
	     boost::bind(&GroundInterface_def::Init, this, _1),
	     &this->compQueue,
             true);
    this->initOneShotTimer = nh.createTimer(timer_options);  
  
    // Create all component timers
    // timer: timer.properties["name"]
    timer_options = 
	ros::TimerOptions
             (ros::Duration(1.0),
	     boost::bind(&GroundInterface_def::Timer0Callback, this, _1),
	     &this->compQueue);
    this->Timer0 = nh.createTimer(timer_options);


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
