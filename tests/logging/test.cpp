#include <stdio.h>
#include <iostream>
#include <string>
#include <memory>
#include "StdCapture.hpp"
#include "Logger.hpp"

int main() {
  std::unique_ptr<Logger> logger(new Logger());
  logger->set_is_periodic(true);
  logger->create_file("somefile.log");
  logger->log("DEBUG", "Something");
  logger->log("DEBUG", "Something_Else");
  logger->log("WARNING", "Something_Metal");

  std::unique_ptr<Logger> stdout_logger(new Logger());
  stdout_logger->set_is_periodic(false);
  stdout_logger->log("INFO", "Something New");

  StdCapture capture_obj;
  capture_obj.Init();
  stdout_logger->log("DEBUG", "Initialized capture object!");
  stdout_logger->log("DEBUG", "Beginning capture!");
  capture_obj.BeginCapture();
  std::cout << "Test1\n";
  std::cout << "Test2\n";
  try {
    throw 0;
    printf("Test3\n");
    perror("Test4\n");
    stdout_logger->log("WARNING", "Ended capture!");
  }
  catch (int e) {
    capture_obj.EndCapture();
    stdout_logger->log("INFO", "Captured Text: %s", capture_obj.GetCapture().c_str());
    //delete stdout_logger.release(); 
  }
  return 0;
}
