#ifndef NETWORK_BUFFER_HPP
#define NETWORK_BUFFER_HPP

#include <deque>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>

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
	_maxSize = std::max(_bits,_maxSize);
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

};

#endif
