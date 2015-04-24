#include <iostream>
#include <fstream>
#include <string>
#include "KRPC.pb.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>
using namespace google::protobuf::io;

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <ifaddrs.h>

#include <cstring>      // std::memcpy
#include <algorithm>    // std::swap
#include <memory>

using namespace std;

// This function fills in a Argument message based on user input.
void PromptForArgument(krpc::Argument* argument) {
  unsigned int position;
  cout << "Input position for argument: ";
  cin >> position;
  argument->set_position(position);
  cin.ignore(256, '\n');
  
  cout << "Input value for the argument (as a string): ";
  string value;
  getline(cin, value);
  argument->set_value(value);
}

// Main function:  Reads the entire address book from a file,
//   adds one argument based on user input, then writes it back out to the same
//   file.
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    cerr << "Usage:  " << argv[0]  << " REQUEST_FILE" << endl;
    return -1;
  }

  krpc::Request request;

  {
    // Read the existing address book.
    fstream input(argv[1], ios::in | ios::binary);
    if (!input) {
      cout << argv[1] << ": File not found.  Creating a new file." << endl;
      cout << "Enter service name: ";
      string service;
      getline(cin, service);
      request.set_service(service);

      cout << "Enter procedure name: ";
      string procedure;
      getline(cin, procedure);
      request.set_procedure(procedure);
    } else if (!request.ParseFromIstream(&input)) {
      cerr << "Failed to parse request." << endl;
      return -1;
    } else {
      cout << "Service name: " << request.service() << endl;
      cout << "Service procedure name: " << request.procedure() << endl;
      for (int i=0; i < request.arguments_size(); i++)
	{
	  const krpc::Argument& argument = request.arguments(i);
	  cout << "Argument position: " << argument.position() << endl;
	  cout << "Argument value: " << argument.value() << endl;
	}
    }
  }

  // Add an address.

  while (true)
    {
      cout << "Add argument (Y/N)? ";
      char result;
      cin >> result;
      if (result == 'Y')
	{
	  cin.ignore(256, '\n');
	  PromptForArgument(request.add_arguments());
	}
      else
	break;
    }

  /* SEND DATA TO KRPC SERVER */
  {
    int sockfd;
    u_short server_portno = 50000;
    char* server_ip_str = "127.0.0.1";
  
    /* socket: create the socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) <0) {
      perror("ERROR opening socket");
    }
    /* build the server's Internet address */
    struct sockaddr_in server_addr;    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_portno);
    if (inet_pton(AF_INET, server_ip_str, &(server_addr.sin_addr)) !=1) {
      perror("inet_pton");
    }
    /* set the address to zero */
    memset(server_addr.sin_zero, 0, sizeof server_addr.sin_zero);
    /* connect: create a connection with the server */
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
      perror("ERROR connecting");
    }
    /* set the timeout on the socket receive */
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
      perror("Couldn't set sockopts!");
      close(sockfd);
      return false;
    }
    /* send the message line to the server */
    int sentbytes=0,numbytes=0;
    string message;
    if (!request.SerializeToString(&message))
      {
	if ( numbytes = send(sockfd, message.c_str(), message.length(),0) == -1) {
	  perror("send");
	  return false;
	}
	char recvbuf[1024];
	memset(recvbuf,0,1024);
	int bytesreceived =0;
	if ( (bytesreceived=recv(sockfd,recvbuf,1023,0)) <= 0) {
	  perror("recv");
	  return false;
	}

	ZeroCopyInputStream* raw_input = new ArrayInputStream(recvbuf,1024);
	CodedInputStream* coded_input = new CodedInputStream(raw_input);
	uint64_t size;
	coded_input->ReadVarint64(&size);
	std::cout << "Received a return message of length: " << size << std::endl;
	delete coded_input;
	delete raw_input;
      }

    close(sockfd);
  }
  
  {
    // Write the new request back to disk.
    fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    if (!request.SerializeToOstream(&output)) {
      cerr << "Failed to write request." << endl;
      return -1;
    }
  }

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
