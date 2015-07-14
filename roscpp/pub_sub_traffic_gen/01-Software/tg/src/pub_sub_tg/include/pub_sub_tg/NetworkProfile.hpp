#ifndef NETWORK_PROFILE_HPP
#define NETWORK_PROFILE_HPP

#include "CSVIterator.hpp"
#include "log_macro.hpp"

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <iomanip>
#include <algorithm>    // std::swap
#include <streambuf>
#include <list>
#include <vector>
#include <memory>

#include <time.h>

#define NETWORK_PROFILE_DEBUG 0

struct profileMemBuf : std::streambuf {
  profileMemBuf(char* begin, char* end) {
    this->setg(begin, begin, end);
  }
};

class ResourceEntry {
public:
  double time;
  unsigned long long bandwidth;
  unsigned long long data;
  unsigned long long latency;
};

class NetworkProfile {
private:
  bool initialized;
public:
  std::vector<ResourceEntry> resources;
  timespec start_time;
  double period;

public:

  NetworkProfile()
  : initialized(false),
  resources (0),
  period (0)
  {
    start_time.tv_sec = 0;
    start_time.tv_nsec = 0;
  }

  NetworkProfile (const NetworkProfile &s)
  : initialized(s.initialized),
  resources(s.resources),
  period(s.period)
  {
    start_time.tv_sec = s.start_time.tv_sec;
    start_time.tv_nsec = s.start_time.tv_nsec;
  }

  ~NetworkProfile()
  {
  }

  NetworkProfile & operator= (const NetworkProfile &s)
  {
    if (&s != this)
    {
      NetworkProfile tmp (s);
      swap (tmp);
    }
    return  *this;
  }

  NetworkProfile* clone() const {
    return new NetworkProfile( *this );
  }

  void swap (NetworkProfile &s) {
    std::swap (initialized, s.initialized);
    std::swap (resources, s.resources);
    std::swap (period, s.period);
    std::swap (start_time.tv_sec, s.start_time.tv_sec);
    std::swap (start_time.tv_nsec, s.start_time.tv_nsec);
  }

  int initializeFromFile(const char* fname) {
    std::ifstream file(fname);
    if ( !file.is_open() ) {
      TG_LOG("ERROR: couldn't open file %s\n",
	     fname);
      return -1;
    }
    TG_LOG("Reading profile %s:\n",fname);
    return initializeFromIStream(file);
  }

  int initializeFromString(char* buffer) {
    profileMemBuf sbuf(buffer,buffer + strlen(buffer));
    std::istream file(&sbuf);
    return initializeFromIStream(file);
  }

  int initializeFromIStream(std::istream& stream)
  {
    std::vector<std::vector<double> > csv;

    for (CSVIterator loop(stream);loop != CSVIterator();++loop) {
      std::vector<double> rowvec;
      if ( (*loop).size() > 0 && (*loop)[0].c_str()[0] != '%' ) {
        for (int i=0;i<(*loop).size();i++) {
          rowvec.push_back( atof( (*loop)[i].c_str() ) );
        }
        csv.push_back(rowvec);
      }
    }
    if ( parse_csv(csv) )
      return -1;
    else {
      initialized = true;
      return 0;
    }
  }

  int parse_csv(std::vector<std::vector<double> > csv) {
    // first csv row contains profile start time and period
    // each other csv row contains time,bandwidth,latency
    double fractpart,intpart;
    fractpart = modf(csv[0][0],&intpart);
    start_time.tv_sec = (unsigned long long)(intpart);
    start_time.tv_nsec = (unsigned long)(fractpart*1000000000.0);
    period = csv[0][1];
    TG_LOG("Got start time and period: %lu.%lu , %f\n",
	   start_time.tv_sec,
	   start_time.tv_nsec,
	   period);
    for (int i=1;i<csv.size();i++) {
      ResourceEntry entry;
      entry.time = csv[i][0];                              // s
      entry.bandwidth = (unsigned long long) (csv[i][1]);  // bps
      entry.latency = (unsigned long long) (csv[i][2]);    // ms

      if ( resources.size() > 0 ) {
        entry.data = resources.back().data +
        resources.back().bandwidth *
        (entry.time - resources.back().time);
      }
      else {
        entry.data = 0;
      }
      TG_LOG("Got interval: [ %f, %llu, %llu, %llu]\n",
	     entry.time,
	     entry.bandwidth,
	     entry.latency,
	     entry.data);
      resources.push_back(entry);
    }
    if (resources.size () && (resources.back().time < period)) {
      ResourceEntry entry;
      entry.time = period;
      entry.bandwidth = 0;
      entry.latency = 0;
      entry.data = resources.back().data +
      resources.back().bandwidth *
      (entry.time - resources.back().time);
      TG_LOG("Got interval: [ %f, %llu, %llu, %llu]\n",
	     entry.time,
	     entry.bandwidth,
	     entry.latency,
	     entry.data);
      resources.push_back(entry);
    }
    return 0;
  }

  inline double getOffset(timespec& t) {
    double _start = (double)(start_time.tv_sec) +
    (double)(start_time.tv_nsec)/1000000000.0;
    double _time = (double)(t.tv_sec) + (double)(t.tv_nsec)/1000000000.0;
    double offset = 0;
    offset += (double)(t.tv_sec - start_time.tv_sec);
    offset += ((double)(t.tv_nsec - start_time.tv_nsec)/1000000000.0);
    offset = fmod(offset,period);
    offset = fabs(offset);
    if ( _time < _start ) {
      offset = period - offset;
    }
    return offset;
  }

  int getNextInterval( timespec& start, unsigned long long& bandwidth, unsigned long long& latency ) {
    if (resources.size () == 0)
      return -1;
    timespec currentTime;
    int returnCode = clock_gettime (CLOCK_REALTIME, &currentTime);
    double offset = getOffset(currentTime);
    double end = period;
    bandwidth = resources[0].bandwidth;
    latency = resources[0].latency;
    for ( int i=0; i < resources.size() - 1; i++ ) {
      if ( resources[i].time > offset ) {
	end = resources[i].time;
	bandwidth = resources[i].bandwidth;
	latency = resources[i].latency;
	break;
      }
    }
    double timeDiff = end - offset;
    double fractpart, intpart;
    fractpart = modf(timeDiff, &intpart);
    start.tv_sec = (long) intpart;
    start.tv_nsec = (long)( fractpart*1000000000.0f );
    start.tv_sec += currentTime.tv_sec;
    start.tv_nsec += currentTime.tv_nsec;
    if ( start.tv_nsec > 999999999 ) {
      start.tv_sec += 1;
      start.tv_nsec = (start.tv_nsec - 1000000000);
    }
    return 0;
  }

  double Delay(unsigned long dataLen, timespec sentTime) {
    if (resources.size () == 0)
      return -1;

    double offset = getOffset(sentTime);

    double start = resources.back().time;
    unsigned long long offsetData = resources.back().data;
    unsigned long long bandwidth = resources.back().bandwidth;
    int res_id = resources.size() - 1;
    for (int i=0;i<resources.size();i++) {
      if ( resources[i].time > offset ) {
        res_id = i;
        start = resources[i-1].time;
        bandwidth = resources[i-1].bandwidth;
        offsetData = resources[i-1].data;
        break;
      }
    }

    offsetData += (unsigned long long)((double)(offset-start)*((double)bandwidth));

    double timeDiff = 0;
    unsigned long long dataInPeriod = resources.back().data;
    unsigned long long dataToEnd = dataInPeriod - offsetData;
    unsigned long long numPeriods = dataLen / dataInPeriod;
    unsigned long long modData = dataLen % dataInPeriod;
  
    if ( numPeriods > 0 ) { // will take more than numPeriods to send data
      timeDiff += (double)numPeriods * period;
    }

    if ( dataToEnd < modData ) { // will have to cycle back to beginning to send data
      timeDiff += period - offset;
      offsetData = 0;
      offset = 0;
      res_id = 0;
      modData = modData - dataToEnd;
    }

    unsigned long long remainder = modData;
    if ( (resources[res_id].data - offsetData) <= modData ) {
      remainder = modData - (resources[res_id].data - offsetData);
      timeDiff += resources[res_id++].time - offset;
      while ( (resources[res_id].data - offsetData) < modData ) {
        remainder = modData - (resources[res_id].data - offsetData);
        timeDiff += resources[res_id].time - resources[res_id-1].time;
        res_id++;
      }
    }
    res_id--;

    timeDiff += (double)remainder / (double)resources[res_id].bandwidth;

    int  return_code;
    timespec current_time;
    return_code = clock_gettime (CLOCK_REALTIME, &current_time);
    double delta = 0;
    delta += (double)(current_time.tv_sec - sentTime.tv_sec);
    delta += ((double)(current_time.tv_nsec - sentTime.tv_nsec)/1000000000.0);
    timeDiff = timeDiff - delta;

    if (timeDiff < 0)
      timeDiff = 0;

    return timeDiff;
  }
  
  bool Initialized() const { return initialized; }
};

#endif
