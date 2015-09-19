#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <math.h>
#include <algorithm>    // std::swap
#include <memory>
#include <vector>
#include <iomanip>
#include <streambuf>
#include <fstream>

namespace Network {
  class Message {
  public:  
    std::string buffer;
    long bits;
    long id;
    uint64_t connection_id;
    std::vector<timespec> times;
    Message ()
      : bits(0),
	id(0),
	connection_id(0)
    {
    }

    Message ( long len, long i, uint64_t conn_id )
      : bits (len),
	id(i),
	connection_id(conn_id)
    {
      buffer = std::string(this->Bytes()+2,'A');
      long templen = 256;
      char temp[templen];
      memset(temp,0,templen);
      sprintf(temp,"%lu",id);
      templen = strlen(temp);
      if (templen <= this->Bytes())
	std::copy(temp,temp+templen, buffer.begin());
      else
	std::copy(temp,temp+this->Bytes(),buffer.begin());
    }

    Message (const Message &s)
      : bits(s.bits),
	id(s.id),
	buffer(s.buffer),
	times (s.times)
    {
    }

    Message & operator= (const Message &s)
    {    
      if (&s != this)
	{
	  Message tmp (s);
	  swap (tmp);
	}
      return  *this;
    }

    void swap (Message &s)
    {
      std::swap (bits, s.bits);
      std::swap (id, s.id);
      std::swap (buffer, s.buffer);
      std::swap (times, s.times);
    }

    void TimeStamp() { 
      timespec time;
      clock_gettime(CLOCK_REALTIME, &time);
      times.push_back(time);
    }

    std::string Buffer() const {
      return buffer;
    }

    std::vector<double> DoubleTimes() {
      std::vector<double> retTimes;
      double time = 0;
      for (int i=0;i<times.size();i++) {
	time = (double)times[i].tv_sec + ((double)times[i].tv_nsec)/(double)1000000000.0;
	retTimes.push_back( time );
      }
      return retTimes;
    }

    double FirstDoubleTime() const {
      double retTime = 
	(double)times.front().tv_sec + 
	((double)times.front().tv_nsec)/(double)1000000000.0;
      return retTime;
    }

    double LastDoubleTime() const {
      double retTime = 
	(double)times.back().tv_sec + 
	((double)times.back().tv_nsec)/(double)1000000000.0;
      return retTime;
    }

    std::vector<timespec> EpochTimes() {
      std::vector<timespec> retTimes(times);
      return retTimes;
    }

    timespec FirstEpochTime() const {
      timespec retTime = times.front();
      return retTime;
    }

    timespec LastEpochTime() const {
      timespec retTime = times.back();
      return retTime;
    }

    void Clear() { buffer.clear(); }

    long Id() const { return id; }
    void Id(long i) { id = i; }

    long Bits() const { return bits; }
    void Bits(long b) { bits = b; }

    long Bytes() const { return ceil((double)bits/8.0f); }
    void Bytes(long B) { bits = B*8; }
  };
};

#endif
