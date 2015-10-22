#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <stdarg.h>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <typeinfo>

class Logger 
{

public:

  Logger() {
    logs_to_file_ = false;
    is_periodic_ = true;
    max_log_unit_ = 1;
    log_content_ = "=================================================================\n";
  }

  void set_is_periodic(bool is_periodic) {
    is_periodic_ = is_periodic;
  }

  void set_max_log_unit(int max_log_unit) {
    max_log_unit_ = max_log_unit;
  }

  void set_logs_to_file(bool logs_to_file) {
    logs_to_file_ = logs_to_file;
  }

  ~Logger() {
    write();
    log_stream_.close();
  }

  bool create_file(std::string log_path) {
    log_path_ = log_path;
    log_stream_.open(log_path_, std::ios::out | std::ios::app);
    logs_to_file_ = true;
    return true;
  }
  
  bool write() {
    if (logs_to_file_) {
      log_stream_ << log_content_;
      log_stream_.flush();
    }
    else
      printf("%s", log_content_.c_str());
    return true;
  }

  bool flush() {
    if (is_periodic_ && size() > max_log_unit_) {
      write();
      log_content_ = "";
      return true;
    }
    return false;
  }

  bool log(std::string log_level, const char * format, ...) {
      va_list args;
      va_start (args, format);
      char log_entry[1024];
      vsprintf (log_entry, format, args);
      std::string log_entry_string(log_entry);
      va_end (args);
      log_content_ += "ROSMOD::" + log_level  + "::" + clock() + 
	"::" + log_entry_string + "\n";
      flush();
  }

  int size() {
    return log_content_.size();
  }

  std::string clock() {
    std::stringstream clock_string;
    clock_string << clock_.now().time_since_epoch().count();
    return clock_string.str();
  }

private:
  std::ofstream log_stream_;
  std::string log_content_;
  std::string log_path_;
  bool is_periodic_;
  bool logs_to_file_;
  int max_log_unit_;
  std::chrono::high_resolution_clock clock_;
};

#endif
