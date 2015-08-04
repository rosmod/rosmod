#include "pub_sub_tg/receiver.hpp"


//# Start User Globals Marker
#include <deque>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

// Thread safe circular buffer 
class Buffer_Empty {};

template <typename T>
class message_buffer : private boost::noncopyable
{
public:
  typedef boost::mutex::scoped_lock lock;
  message_buffer() : _size(0), _maxSize(0), _capacity(0) {}
  message_buffer(int n) : message_buffer() { _capacity = n; }
  void send (T data, uint64_t len) {
    lock lk(monitor);
    if (!_capacity ||
	(_capacity && len <= (_capacity - _size)) ) {
      _size += len;
      _maxSize = max(_size,_maxSize);
      sizes.push_back(len);
      q.push_back(data);
      buffer_not_empty.notify_one();
    }
  }
  T receive() {
    lock lk(monitor);
    while (q.empty())
      buffer_not_empty.wait(lk);
    T data = q.front();
    q.pop_front();
    uint64_t len = sizes.front();
    sizes.pop_front();
    _size = _size - len;
    return data;
  }
  T non_blocking_receive() {
    lock lk(monitor);
    if (q.empty()) {
      lk.unlock();
      throw Buffer_Empty();
    }
    else {
      lk.unlock();
      return receive();
    }
  }
  void clear() {
    lock lk(monitor);
    q.clear();
    sizes.clear();
    _size = _maxSize = 0;
  }
  uint64_t size() {
    lock lk(monitor);
    return _size;
  }
  uint64_t maxSize() {
    lock lk(monitor);
    return _maxSize;
  }
  void set_capacity(uint64_t capacity) {
    lock lk(monitor);
    _capacity = capacity;
  }
private:
  uint64_t _size;
  uint64_t _maxSize;
  uint64_t _capacity;
  boost::condition buffer_not_empty;
  boost::mutex monitor;
  std::deque<uint64_t> sizes;
  std::deque<T> q;
};

message_buffer<Network::Message> buffer;

/*
  Need to implement:
  * configuration of oob channels
  * monitoring code to check the buffer space and send back to publishers
 */

void receiver::TrafficGeneratorTimer(const ros::TimerEvent& event)
{
  // READ FROM THE BUFFER
  double timerDelay = -1;
  try
    {
      Network::Message msg = buffer.non_blocking_receive();
      msg.TimeStamp();
      messages.push_back(msg);
      // CHECK AGAINST RECEIVER PROFILE: LOOK UP WHEN I CAN READ NEXT
      timerDelay = profile.Delay(msg.Bits(), msg.LastEpochTime());
    }
  catch ( Buffer_Empty& ex )
    {
    }
  if ( ros::Time::now() >= endTime )
    {
      printf("WRITING LOG; max Size was: %lu; got %lu messages\n",
	     buffer.maxSize(), messages.size());
      std::string fName = nodeName + "." + compName + ".network.csv";
      Network::write_data(fName.c_str(),messages);
    }
  else
    {
      // RESTART TIMER FOR THAT TIME
      ros::TimerOptions timer_options;
      timer_options = 
	ros::TimerOptions
	(ros::Duration(timerDelay),
	 boost::bind(&receiver::TrafficGeneratorTimer, this, _1),
	 &this->compQueue,
	 true);
      ros::NodeHandle nh;
      tgTimer = nh.createTimer(timer_options);
    }
}

//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void receiver::Init(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering receiver::Init");
  // Initialize Here
  
  // INITIALIZE OUR PROFILE
  printf("Initializing MW\n");
  // INITIALIZE N/W MIDDLEWARE HERE
  ros::Time now = ros::Time::now();
  endTime = now + ros::Duration(config.tg_time);
  // LOAD NETWORK PROFILE HERE
  profile.initializeFromFile(this->config.profileName.c_str());
  printf("Initialized Profile\n");
  printf("%s\n",profile.toString().c_str());
#if 0
  // GET ALL OOB SERVER UUIDS FOR USE IN CALLBACK
  // FOR EACH OOB_CLIENT:
  pub_sub_tg::oob_comm oob_get_uuid;
  oob_get_uuid.request.deactivateSender = false;
  oob_get_uuid.request.meterSender = false;
  oob_client.call(oob_get_uuid);
  uint64_t uuid = oob_get_uuid.response.uuid;
  oob_map[uuid] = &oob_client;
  std::string profileName = oob_get_uuid.response.profileName;
  // LOAD PROFILES
  profile_map[uuid] = Network::NetworkProfile();
  profile_map[uuid].initializeFromFile(profileName.c_str());
#endif
  id = 0;

  // CREATE TIMER HERE FOR RECEIVING DATA ACCORDING TO PROFILE
  ros::NodeHandle nh;
  ros::TimerOptions timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&receiver::TrafficGeneratorTimer, this, _1),
     &this->compQueue,
     true);
  tgTimer = nh.createTimer(timer_options);
  printf("Created Timer\n");

  // Stop Init Timer
  initOneShotTimer.stop();
  LOGGER.DEBUG("Exiting receiver::Init");
}
//# End Init Marker

// Subscriber Callback - message_sub
//# Start message_sub_OnOneData Marker
void receiver::message_sub_OnOneData(const pub_sub_tg::message::ConstPtr& received_data)
{
  // GET SENDER ID
  uint64_t uuid = received_data->uuid;
  // CHECK NETWORK PROFILE HERE FOR SENDER
  Network::NetworkProfile* profile = &profile_map[uuid];
  // DO I NEED RECEIVER PROFILE TO DESCRIBE THE RATE AT WHICH THE
  //   RECEIVER PULLS FROM THE QUEUE?
  // IF THE NETWORK PROFILE HAS BEEN EXCEEDED FOR TOO LONG:
  //   I.E. IF OUR REMAINING BUFFER SPACE IS TOO LOW (CALCULABLE BASED ON
  //   KNOWN PEAK RATES OF SENDERS)
  // THEN SEND A REQUEST BACK TO SENDER(S) MIDDLEWARE TO METER OR STOP
#if 0
  ros::ServiceClient* sender = oob_map[uuid];
  pub_sub_tg::oob_comm oob;
  oob.request.deactivateSender = true;
  sender->call(oob);
#endif
  // MEASURE AND RECORD DATA OUTPUT
  uint64_t msgBytes =
    ros::serialization::Serializer<pub_sub_tg::message>::serializedLength(*received_data);

  Network::Message new_msg;
  new_msg.Bytes(msgBytes);
  new_msg.Id(id++);
  new_msg.TimeStamp();

  // PUT MESSAGE INTO A BUFFER
  buffer.send(new_msg, msgBytes);
}
//# End message_sub_OnOneData Marker


// Destructor - Cleanup Ports & Timers
receiver::~receiver()
{
  message_sub.shutdown();
  oob_client.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void receiver::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO

  // Component Subscriber - message_sub
  advertiseName = "message";
  if (portGroupMap.find("message_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["message_sub"];
  ros::SubscribeOptions message_sub_options;
  message_sub_options = ros::SubscribeOptions::create<pub_sub_tg::message>
      (advertiseName.c_str(),
       1000,
       boost::bind(&receiver::message_sub_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->message_sub = nh.subscribe(message_sub_options);  

  // Configure all required services associated with this component
  // Component Client - oob_client
  advertiseName = "oob_comm";
  if (portGroupMap.find("oob_client") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["oob_client"];
      this->oob_client = nh.serviceClient<pub_sub_tg::oob_comm>(advertiseName.c_str()); 

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&receiver::Init, this, _1),
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
     boost::bind(&receiver::component_synchronization_OnOneData, this, _1),
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
    return new receiver(config,argc,argv);
  }
}
