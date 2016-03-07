/*
 * SerialPort.cpp
 *
 *  Created on: Jun 20, 2011
 *      Author: jose
 */

#include <string.h>
#include <sys/ioctl.h>

#include "agse_package/SerialPort.h"

SerialPort::SerialPort() { // : timer(io) {
  port = new boost::asio::serial_port(io);
  //  timeout = this_timeout;
}
/*

void SerialPort::read_complete(const boost::system::error_code& error,
			       size_t bytes_transferred) {
 
  //  read_error = (error || bytes_transferred == 0);
  
  timer.cancel();
}

void SerialPort::time_out(const boost::system::error_code& error) {

  if (error) {
    std::cout << "Some error" << std::endl;
    return;
  }
  std::cout << "Timer timed out!!" << std::endl;
  port->cancel();
}
*/
int SerialPort::connect() {
  port->open("/dev/ttyO5");
  port->set_option(boost::asio::serial_port_base::baud_rate(9600));
  return 1;
}

int SerialPort::connect(char *device, int baud) {
  port->open(device);
  port->set_option(boost::asio::serial_port_base::baud_rate(baud));
  return 1;
}

void SerialPort::disconnect(void)
{
  port->close();
}

int SerialPort::sendArray(unsigned char *buffer, int len) {
  int n = boost::asio::write( *port,
			      boost::asio::buffer(buffer,len));
  return n;
}

int SerialPort::getArray (unsigned char *buffer, int len)
{
  char rcvChar;
  int i = 0;
  //  std::cout << "Inside getArray" << "; Timeout: " << timeout << std::endl;
  /*
  boost::asio::async_read(*port, boost::asio::buffer(&rcvChar, 1), 
			  boost::bind(&SerialPort::read_complete, 
				      this, 
				      boost::asio::placeholders::error, 
				      boost::asio::placeholders::bytes_transferred)); 

  timer.expires_from_now(boost::posix_time::milliseconds(timeout));
  timer.async_wait(boost::bind(&SerialPort::time_out,
			       this, boost::asio::placeholders::error));
  */
  while ( i < len && boost::asio::read( *port, boost::asio::buffer(&rcvChar,1) ) == 1 )
    buffer[i++] = rcvChar;
  return i;
}

