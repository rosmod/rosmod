#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <stdarg.h>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

/*
 * Log Levels in ROSMOD
 * DEBUG: 
 */
struct Log_Levels {
  bool DEBUG;
  bool INFO;
  bool WARNING;
  bool ERROR;
  bool CRITICAL;

  Log_Levels()
  {
    DEBUG = false;
    INFO = true;
    WARNING = false;
    ERROR = true;
    CRITICAL = true;
  }
};

/*
 * Logger class
 * An object of this class is part of every Component instance
 */
class Logger 
{

public:

  // Writes log_content to log file & empties log_content
  ~Logger();

  // Create log file
  bool CREATE_FILE(string target_log_path);

  // Write log contents to file
  bool WRITE_TO_FILE();

  // Check size of log_contents
  bool CHECK_LOG_SIZE();

  // Create a DEBUG log entry
  bool DEBUG(const char * format, ...);

  // Create a INFO log entry
  bool INFO(const char * format, ...);

  // Create a WARNINGg log entry
  bool WARNING(const char * format, ...);

  // Create an ERROR log entry
  bool ERROR(const char * format, ...);

  // Create a CRITICAL log entry
  bool CRITICAL(const char * format, ...);

  // Set Log Levels
  bool SET_LOG_LEVELS(Log_Levels target_log_levels);

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
  string log_path;

  // Level of Logging
  Log_Levels log_levels;

  // High Resolution Clock
  chrono::high_resolution_clock clock;
};

#endif
