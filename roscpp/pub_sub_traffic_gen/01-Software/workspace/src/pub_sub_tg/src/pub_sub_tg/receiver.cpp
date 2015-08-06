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
  message_buffer() : _bits(0), _maxSize(0), _capacity(0) {}
  message_buffer(int bits) : message_buffer() { _capacity = bits; }
  void send (T data, uint64_t bits) {
    lock lk(monitor);
    if (!_capacity ||
	(_capacity && bits <= (_capacity - _bits)) ) {
      _bits += bits;
      _maxSize = max(_bits,_maxSize);
      sizes.push_back(bits);
      q.push_back(data);
      buffer_not_empty.notify_one();
    }
  }
  T receive(uint64_t timeout_ms = 0) {
    boost::system_time const timeout =
      boost::get_system_time()+ boost::posix_time::milliseconds(timeout_ms);
    lock lk(monitor);
    while (q.empty()) {
      if (!buffer_not_empty.timed_wait(lk, timeout)) {
	lk.unlock();
	throw Buffer_Empty();
      }
    }
    T data = q.front();
    q.pop_front();
    uint64_t bits = sizes.front();
    sizes.pop_front();
    _bits = _bits - bits;
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
    _bits = _maxSize = 0;
  }
  uint64_t bits() {
    lock lk(monitor);
    return _bits;
  }
  uint64_t bytes() {
    lock lk(monitor);
    return _bits / 8;
  }
  uint64_t maxBits() {
    lock lk(monitor);
    return _maxSize;
  }
  uint64_t maxBytes() {
    lock lk(monitor);
    return _maxSize / 8;
  }
  uint64_t capacityBits() {
    lock lk(monitor);
    return _capacity;
  }
  uint64_t capacityBytes() {
    lock lk(monitor);
    return _capacity / 8;
  }
  void set_capacityBits(uint64_t capacityBits) {
    lock lk(monitor);
    _capacity = capacityBits;
  }
  void set_capacityBytes(uint64_t capacityBytes) {
    lock lk(monitor);
    _capacity = capacityBytes * 8;
  }
private:
  uint64_t _bits;
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
  * monitoring code to check the buffer space and send back to publishers
 */

bool receivedData = false;
void receiver::bufferReceiveThread(void)
{
  while ( true )
    {
      // UPDATE SENDERS IF BUFFER IS CLEARING:
      uint64_t currentSize = buffer.bits();
      uint64_t currentCapacity = buffer.capacityBits();
      if ( currentCapacity > 0 )
	{
	  double utilization = (double)currentSize/(double)currentCapacity;
	  if ( utilization < 0.75 )
	    unlimitDDoS();
	}
      // READ FROM THE BUFFER
      double timerDelay = -1;
      try
	{
	  Network::Message msg = buffer.receive(1000);
	  if (!receivedData)
	    {
	      ros::Time now = ros::Time::now();
	      endTime = now + ros::Duration(config.tg_time);
	      receivedData = true;
	    }
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
	  LOGGER.DEBUG("WRITING LOG; max Size was: %lu bits; got %lu messages",
		       buffer.maxBits(), messages.size());
	  std::string fName = nodeName + "." + compName + ".network.csv";
	  Network::write_data(fName.c_str(),messages);
	  LOGGER.DEBUG("EXITING BUFFER RECEIVE THREAD!");
	  break;
	}
      else
	{
	  // SLEEP UNTIL NEXT TIME BUFFER CAN BE SERVICED
	  if (timerDelay > 0)
	    ros::Duration(timerDelay).sleep();
	}
    }
}

void receiver::unlimitDDoS(void)
{
  for (auto uuid_it = disabled_uuids.begin();
       uuid_it != disabled_uuids.end(); ++uuid_it)
    {
      ros::ServiceClient* sender = oob_map[*uuid_it];
      pub_sub_tg::oob_comm oob;
      oob.request.deactivateSender = false;
      sender->call(oob);
    }
  disabled_uuids.clear();
}

void receiver::limitDDoS(ros::Time now, double timeWindow)
{
  std::vector<uint64_t> bad_uuids;
  ros::Time prevTime = now - ros::Duration(timeWindow);
  for (auto uuid_it = uuids.begin();
       uuid_it != uuids.end(); ++uuid_it)
    {
      uint64_t d1 = receive_map[*uuid_it].lower_bound(prevTime)->second;
      ros::Time t1 = receive_map[*uuid_it].lower_bound(prevTime)->first;
      uint64_t d2 = receive_map[*uuid_it].end()->second;
      ros::Time t2 = receive_map[*uuid_it].end()->first;
      // get data @ t1 from profile, get data @ t2 from profile
      uint64_t pd1 = profile_map[*uuid_it].getDataAtTime({t1.sec, t1.nsec});
      uint64_t pd2 = profile_map[*uuid_it].getDataAtTime({t2.sec, t2.nsec});
      // if profile difference < (d2-d1) : they are sending too much
      uint64_t pDiff = pd2 - pd1;
      uint64_t diff = d2 - d1;
      if (diff > pDiff) {
	bad_uuids.push_back(*uuid_it);
	if ( std::find(disabled_uuids.begin(),
		       disabled_uuids.end(),
		       *uuid_it) == disabled_uuids.end() )
	  disabled_uuids.push_back(*uuid_it);
      }
    }
  for (auto uuid_it = bad_uuids.begin();
       uuid_it != bad_uuids.end(); ++uuid_it)
    {
      ros::ServiceClient* sender = oob_map[*uuid_it];
      pub_sub_tg::oob_comm oob;
      oob.request.deactivateSender = true;
      sender->call(oob);
    }
}

//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void receiver::Init(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering receiver::Init");
  // Initialize Here

  ros::NodeHandle nh;
  
  // INITIALIZE OUR PROFILE
  LOGGER.DEBUG("Initializing MW");
  // INITIALIZE N/W MIDDLEWARE HERE
  // LOAD NETWORK PROFILE HERE
  profile.initializeFromFile(this->config.profileName.c_str());
  LOGGER.DEBUG("Initialized Profile");
  LOGGER.DEBUG("%s",profile.toString().c_str());

  buffer.set_capacityBits(400000);

  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--buffer_capacity_bits"))
	{
	  buffer.set_capacityBits(atoi(node_argv[i+1]));
	}
      if (!strcmp(node_argv[i], "--buffer_capacity_bytes"))
	{
	  buffer.set_capacityBytes(atoi(node_argv[i+1]));
	}
    }
  
  LOGGER.DEBUG("Set Buffer Capacity to %lu bits", buffer.capacityBits());
  LOGGER.DEBUG("Current Buffer Size is %lu bits", buffer.bits());

  // CONFIGURE ALL OOB CHANNELS
  std::string advertiseName;
  advertiseName = "oob_comm_pub1";
  this->oob_client_pub1 = nh.serviceClient<pub_sub_tg::oob_comm>(advertiseName.c_str());
  advertiseName = "oob_comm_pub2";
  this->oob_client_pub2 = nh.serviceClient<pub_sub_tg::oob_comm>(advertiseName.c_str());
  advertiseName = "oob_comm_pub3";
  this->oob_client_pub3 = nh.serviceClient<pub_sub_tg::oob_comm>(advertiseName.c_str());
  
  // set up uuids for senders
  uint64_t uuid_pub1 = 1;
  uint64_t uuid_pub2 = 2;
  uint64_t uuid_pub3 = 3;
  uuids.push_back(uuid_pub1);
  uuids.push_back(uuid_pub2);
  uuids.push_back(uuid_pub3);

  oob_map[uuid_pub1] = &this->oob_client_pub1;
  oob_map[uuid_pub2] = &this->oob_client_pub2;
  oob_map[uuid_pub3] = &this->oob_client_pub3;

  // LOAD PROFILES
  profile_map[uuid_pub1] = Network::NetworkProfile();
  profile_map[uuid_pub1].initializeFromFile("required1.csv");

  profile_map[uuid_pub2] = Network::NetworkProfile();
  profile_map[uuid_pub2].initializeFromFile("required2.csv");

  profile_map[uuid_pub3] = Network::NetworkProfile();
  profile_map[uuid_pub3].initializeFromFile("required3.csv");

  // prepare receive map
  receive_map[uuid_pub1] = std::map<ros::Time, uint64_t>();
  receive_map[uuid_pub2] = std::map<ros::Time, uint64_t>();
  receive_map[uuid_pub3] = std::map<ros::Time, uint64_t>();

  id = 0;

  // CREATE THREAD HERE FOR RECEIVING DATA
  boost::thread *tmr_thread =
    new boost::thread( boost::bind(&receiver::bufferReceiveThread, this) );
  LOGGER.DEBUG("Created Buffer Recv Thread");

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

  // MEASURE SIZE OF INCOMING MESSAGE
  uint64_t msgBytes =
    ros::serialization::Serializer<pub_sub_tg::message>::serializedLength(*received_data);

  // KEEP TRACK OF EACH SENDER'S INCOMING DATA PROFILE
  ros::Time now = ros::Time::now();
  uint64_t prevData = 0;
  if ( receive_map[uuid].size() )
    prevData = receive_map[uuid].end()->second;
  receive_map[uuid][now] = msgBytes * 8 + prevData;

  // MANAGE AVAILABLE BUFFER SPACE
  uint64_t currentSize = buffer.bits();
  uint64_t currentCapacity = buffer.capacityBits();
  if ( currentCapacity > 0 )
    {
      double utilization = (double)currentSize/(double)currentCapacity;
      if (utilization > 0.95)
	{
	  limitDDoS(now, 1.0); // FOR NOW ONLY LOOKING AT PREVIOUS SECOND
	}
    }
  
  // RECORD MESSAGE
  Network::Message new_msg;
  new_msg.Bytes(msgBytes);
  new_msg.Id(id++);
  new_msg.TimeStamp();

  // PUT MESSAGE INTO A BUFFER
  buffer.send(new_msg, msgBytes * 8);
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
  message_sub_options.transport_hints = ros::TransportHints()
    .udp();

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
  this->comp_sync_sub.shutdown();
  this->comp_sync_pub.shutdown();

  this->initOneShotTimer.start();
  
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new receiver(config,argc,argv);
  }
}
