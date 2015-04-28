//
// krpc_client.cpp
// ~~~~~~~~~~~~~~~
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "krpc/KRPC.pb.h"

using boost::asio::ip::tcp;

typedef std::deque<krpc::Request> krpc_request_queue;

class krpc_client
{
public:
  krpc_client(boost::asio::io_service& io_service,
      tcp::resolver::iterator endpoint_iterator)
    : io_service_(io_service),
      socket_(io_service)
  {
    tcp::endpoint endpoint = *endpoint_iterator;
    socket_.async_connect(endpoint,
        boost::bind(&krpc_client::handle_connect, this,
          boost::asio::placeholders::error, ++endpoint_iterator));
  }

  void write(const krpc::Request& req)
  {
    io_service_.post(boost::bind(&krpc_client::do_write, this, req));
  }

  void close()
  {
    io_service_.post(boost::bind(&krpc_client::do_close, this));
  }

private:

  void handle_connect(const boost::system::error_code& error,
      tcp::resolver::iterator endpoint_iterator)
  {
    if (!error)
    {
      boost::asio::async_read(socket_,
          boost::asio::buffer(msg, 10),
          boost::bind(&krpc_client::handle_read_header, this,
            boost::asio::placeholders::error));
    }
    else if (endpoint_iterator != tcp::resolver::iterator())
    {
      socket_.close();
      tcp::endpoint endpoint = *endpoint_iterator;
      socket_.async_connect(endpoint,
          boost::bind(&krpc_client::handle_connect, this,
            boost::asio::placeholders::error, ++endpoint_iterator));
    }
  }

  void handle_read_header(const boost::system::error_code& error)
  {
    if (!error)
    {
      int msgLen = 10;
      //CodedInputStream coded_input = new CodedInputStream(msg);
      //coded_input->ReadVarint(&msgLen);
      boost::asio::async_read(socket_,
          boost::asio::buffer(msg, msgLen),
          boost::bind(&krpc_client::handle_read_body, this,
            boost::asio::placeholders::error));
    }
    else
    {
      do_close();
    }
  }

  void handle_read_body(const boost::system::error_code& error)
  {
    if (!error)
    {
      //std::cout.write(read_res_.body(), read_res_.body_length());
      //std::cout << "\n";
      boost::asio::async_read(socket_,
			      boost::asio::buffer(msg, 10),
			      boost::bind(&krpc_client::handle_read_header, this,
					  boost::asio::placeholders::error));
    }
    else
    {
      do_close();
    }
  }

  void do_write(krpc::Request req)
  {
    bool write_in_progress = !write_reqs_.empty();
    write_reqs_.push_back(req);
    if (!write_in_progress)
    {
      string reqStr;
      write_reqs_.front().SerializeToString(&reqStr);
      boost::asio::async_write(socket_,
          boost::asio::buffer(reqStr,
            reqStr.length()),
          boost::bind(&krpc_client::handle_write, this,
            boost::asio::placeholders::error));
    }
  }

  void handle_write(const boost::system::error_code& error)
  {
    if (!error)
    {
      write_reqs_.pop_front();
      if (!write_reqs_.empty())
      {
	string reqStr;
	write_reqs_.front().SerializeToString(&reqStr);
        boost::asio::async_write(socket_,
				 boost::asio::buffer(reqStr,
						     reqStr.length()),
				 boost::bind(&krpc_client::handle_write, this,
					     boost::asio::placeholders::error));
      }
    }
    else
    {
      do_close();
    }
  }

  void do_close()
  {
    socket_.close();
  }

private:
  boost::asio::io_service& io_service_;
  tcp::socket socket_;
  char msg[1024];
  krpc::Response read_res_;
  krpc_request_queue write_reqs_;
};
