#include "node/Logger.hpp"

/*
 * Set Logging Periodicity
 */
void Logger::SET_PERIODICITY(bool is_periodic) {
  periodic_logging = is_periodic;
}

/*
 * Change max log size
 */
void Logger::CHANGE_LOG_SIZE(int new_size) {
  max_log_unit = new_size;
}

/*
 * Write remaining log contents to file
 */
Logger::~Logger() {
  WRITE_TO_FILE();
  log_stream.close();
}

/*
 * Open file outstream @ provided target path
 */
bool Logger::CREATE_FILE(string target_log_path) {
  log_path = target_log_path;
  log_stream.open(log_path, ios::out | ios::app );  
  log_content = "--------------------------------------------------------------------------------\n";
  return true;
}

/*
 * Write Log contents to file & close stream
 */ 
bool Logger::WRITE_TO_FILE() {
  log_stream << log_content;
  log_stream.flush();
  return true;
}

/*
 * Check log content size 
 * If size > max_log_unit, write to file
 */
bool Logger::CHECK_LOG_SIZE() {
  if (SIZE_OF_LOG() > max_log_unit) {
    WRITE_TO_FILE();
    log_content = "";
    return true;
  }
  return false;
}

/*
 * Create a DEBUG log entry
 */
bool Logger::DEBUG(const char * format, ...) {
  if (log_levels.DEBUG == true) {
    va_list args;
    va_start (args, format);
    char log_entry[1024];
    vsprintf (log_entry, format, args);
    std::string log_entry_string(log_entry);
    va_end (args);

    log_content += "ROSMOD::DEBUG::" + CLOCK_VALUE() + "::" + log_entry_string + "\n";
    if (periodic_logging)
      CHECK_LOG_SIZE();
  }
  return true;
}

/*
 * Create a INFORMATION log entry
 */
bool Logger::INFO(const char * format, ...) {
  if (log_levels.INFO == true) {
    va_list args;
    va_start (args, format);
    char log_entry[1024];
    vsprintf (log_entry, format, args);
    std::string log_entry_string(log_entry);
    va_end (args);

    log_content += "ROSMOD::INFO::" + CLOCK_VALUE() + "::" + log_entry_string + "\n"; 
    if (periodic_logging)
      CHECK_LOG_SIZE();
  }
  return true;
}

/*
 * Create a WARNING log entry
 */
bool Logger::WARNING(const char * format, ...) {
  if (log_levels.WARNING == true) {
    va_list args;
    va_start (args, format);
    char log_entry[1024];
    vsprintf (log_entry, format, args);
    std::string log_entry_string(log_entry);
    va_end (args);

    log_content += "ROSMOD::WARNING::" + CLOCK_VALUE() + "::" + log_entry_string + "\n"; 
    if (periodic_logging)
      CHECK_LOG_SIZE();
  }
  return true;
}

/*
 * Create an ERROR log entry
 */
bool Logger::ERROR(const char * format, ...) {
  if (log_levels.ERROR = true) {
    va_list args;
    va_start (args, format);
    char log_entry[1024];
    vsprintf (log_entry, format, args);
    std::string log_entry_string(log_entry);
    va_end (args);

    log_content += "ROSMOD::ERROR::" + CLOCK_VALUE() + "::" + log_entry_string + "\n";
    if (periodic_logging)
      CHECK_LOG_SIZE();
  }
  return true;
}

/*
 * Create a CRITICAL log entry
 */
bool Logger::CRITICAL(const char * format, ...) {
  if (log_levels.CRITICAL == true) {
    va_list args;
    va_start (args, format);
    char log_entry[1024];
    vsprintf (log_entry, format, args);
    std::string log_entry_string(log_entry);
    va_end (args);

    log_content += "ROSMOD::CRITICAL::" + CLOCK_VALUE() + "::" + log_entry_string + "\n"; 
    if (periodic_logging)
      CHECK_LOG_SIZE();
  }
  return true;
}

/*
 * Set Log Levels 
 */
bool Logger::SET_LOG_LEVELS(Log_Levels target_log_levels) {
  log_levels.DEBUG = target_log_levels.DEBUG;
  log_levels.INFO = target_log_levels.INFO;
  log_levels.WARNING = target_log_levels.WARNING;
  log_levels.ERROR = target_log_levels.ERROR;
  log_levels.CRITICAL = target_log_levels.CRITICAL;
}

/*
 * Return size of log_content
 */
int Logger::SIZE_OF_LOG() {
  return log_content.size();
}

/*
 * Get Current Clock Value
 */
string Logger::CLOCK_VALUE() {
  stringstream clock_string;
  clock_string << clock.now().time_since_epoch().count();
  return clock_string.str();  
}
