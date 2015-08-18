#ifndef NETWORK_RECEIVER_HPP
#define NETWORK_RECEIVER_HPP

#include "ros/ros.h"

#include <deque>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

#include "network/NetworkProfile.hpp"
#include <std_msgs/UInt64MultiArray.h>

namespace Network
{

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

  class receiver
  {
  public:
    receiver();
    void add_sender(uint64_t uuid, std::string profileName);
    void set_duration(double dur);
    void set_output_filename(std::string filename);
    void oob_send(std::vector<uint64_t>& send_uuids, bool val);
    void update_sender_stream(uint64_t uuid, ros::Time t, uint64_t new_size);
    void buffer_receive_thread(void);
    void unlimit_ddos(void);
    void limit_ddos(ros::Time now, double timeWindow);

  public:
    std::vector<Network::Message> messages;
    
    message_buffer<Network::Message> buffer;
    Network::NetworkProfile profile;
    
    ros::Duration duration;
    ros::Time endTime;
    bool received_data;

  private:
    ros::Publisher oob_pub;

    std::string output_filename;

    std::vector<uint64_t> uuids;
    std::vector<uint64_t> disabled_uuids;
    std::map<uint64_t, Network::NetworkProfile> profile_map;
    std::map<uint64_t, std::map<ros::Time, uint64_t>> receive_map; // uuid - > <time, data>
  };
};

#endif
