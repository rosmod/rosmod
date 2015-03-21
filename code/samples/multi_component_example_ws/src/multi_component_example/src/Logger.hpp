/*
 * ROSMOD Logger
 * Author: Pranav Srinivar Kumar
 * Date: 2015.03.21
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

class Logger 
{

public:

  // Default Constructor
  Logger();

  // Initializes Logger
  Logger(const char * target_log_path);

  // Writes log_content to log file & empties log_content
  ~Logger();

  // Create log file
  bool CREATE_FILE(const char * target_log_path);
  // Write log contents to file
  bool WRITE_TO_FILE();
  // Create a debug log entry
  bool DEBUG(string text);
  // Create an error log entry
  bool ERROR(string text);

  // Get Current Clock Value
  string CLOCK_VALUE() {
    // Clock Value String
    stringstream clock_string;
    clock_string << clock.now().time_since_epoch().count();
    return clock_string.str();
  }

private:
  // String representing the contents of log
  string log_content;
  // Log file stream
  ofstream log_stream;
  // Absolute path of log file
  const char * log_path;
  // High Resolution Clock
  chrono::high_resolution_clock clock;
};

#endif
