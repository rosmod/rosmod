/*
 * ROSMOD Logger
 * Author: Pranav Srinivar Kumar
 * Date: 2015.03.21
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <stdarg.h>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

class Logger 
{

public:

  // Writes log_content to log file & empties log_content
  ~Logger();

  // Create log file
  bool CREATE_FILE(const char * target_log_path);

  // Write log contents to file
  bool WRITE_TO_FILE();

  // Check size of log_contents
  bool CHECK_LOG_SIZE();

  // Create a debug log entry
  bool DEBUG(const char * format, ...);

  // Create an error log entry
  bool ERROR(const char * format, ...);

  // Get log entry size
  int SIZE_OF_LOG();

  // Get Current Clock Value
  string CLOCK_VALUE();

private:
  // String representing the contents of log
  string log_content;

  // Max size of log_content before contents are written to file
  const int max_log_unit = 1000;

  // Log file stream
  ofstream log_stream;

  // Absolute path of log file
  const char * log_path;

  // High Resolution Clock
  chrono::high_resolution_clock clock;
};

#endif
