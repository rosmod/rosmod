#include "Logger.hpp"

Logger::Logger(){}

Logger::Logger(const char * target_log_path) {
  log_content = "ROSMOD::LOG::" + CLOCK_VALUE();
  log_path = target_log_path;
  log_stream.open(log_path, ios::out | ios::app );
}

Logger::~Logger() {
  WRITE_TO_FILE();
  log_content = "";
}

bool Logger::CREATE_FILE(const char * target_log_path) {
  log_path = target_log_path;
  log_stream.open(log_path, ios::out | ios::app );  
  log_content = "ROSMOD::LOG::" + CLOCK_VALUE();
  return true;
}

bool Logger::WRITE_TO_FILE() {
  log_stream << log_content << endl;
  log_stream.close();
  return true;
}

bool Logger::DEBUG(string text) {
  log_content += "\nROSMOD::DEBUG::" + CLOCK_VALUE() + "::" + text; 
  return true;
}

bool Logger::ERROR(string text) {
  log_content += "\nROSMOD::ERROR::" + CLOCK_VALUE() + "::" + text;
  return true;
}

