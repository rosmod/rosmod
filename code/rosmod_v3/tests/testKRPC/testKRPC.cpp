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
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
      perror("Couldn't set sockopts!");
      close(sockfd);
    }
    /* send the message line to the server */
    int sentbytes=0,numbytes=0;
    char hellobuf[] = {
      0x48, 0x45, 0x4C, 0x4C, 0x4F, 0x2D, 0x52, 0x50, 0x43, 0x00, 0x00, 0x00
    };
    if ( numbytes = send(sockfd, hellobuf, 12,0) == -1) {
      perror("send");
    }
    char connID[32] = "testKRPC_program";
    if ( numbytes = send(sockfd, connID, 32,0) == -1) {
      perror("send");
    }
    bool haveReceivedID = false;
    char clientID[16];
    while (!haveReceivedID)
      {
	memset(clientID,0,1024);
	int bytesreceived =0;
	if ( (bytesreceived=recv(sockfd,clientID,16,0)) <= 0) {
	  perror("recv");
	}
	else {
	  std::cout << "RECEIVED UNIQUE CLIENT ID: " << clientID << endl;
	  haveReceivedID = true;
	}
      }
    string message;
    message.reserve(1024);
    uint64_t size;
    if (request.SerializeToString(&message))
      {
	/* write the length of the serialized message */
	std::cout << "Converting message length " << message.length() << " to Varint64" << endl;
	unsigned char messageLen[10];
	CodedOutputStream::WriteVarint64ToArray(message.length(), messageLen);
	//coded_output->WriteVarint64(message.length());
#if 0
	string messageLen;
	messageLen.reserve(10);
	ZeroCopyOutputStream* raw_output = new ArrayOutputStream(&messageLen[0],10);
	CodedOutputStream* coded_output = new CodedOutputStream(raw_output);
	if ( numbytes = send(sockfd, messageLen.data(), messageLen.length(), 0) == -1) {
	  perror("send");
	}
	std::cout << "Sent message length: " << messageLen << endl;
	/* write the message */
	if ( numbytes = send(sockfd, message.c_str(), message.length(),0) == -1) {
	  perror("send");
	}
	std::cout << "Sent message: " << message << endl;

	delete coded_output;
	delete raw_output;
#else
	std::cout << messageLen << " : " << strlen((char *)messageLen) << endl;
	std::cout << message << " : " << message.length() << endl;
	string msg = string((const char *)messageLen) + message;
	if ( numbytes = send(sockfd, msg.data(), msg.length(), 0) == -1) {
	  perror("send");
	}
	std::cout << "Sent message: " << msg << endl;	
#endif
	/* receive the response from the server */
	char recvbuf[1024];
	memset(recvbuf,0,1024);
	int bytesreceived =0;
	if ( (bytesreceived=recv(sockfd,recvbuf,1023,0)) <= 0) {
	  perror("recv");
	}

	ZeroCopyInputStream* raw_input = new ArrayInputStream(recvbuf,1024);
	CodedInputStream* coded_input = new CodedInputStream(raw_input);
	coded_input->ReadVarint64(&size);
	std::cout << "Received a return message of length: " << size << std::endl;

	krpc::Response response;

	delete coded_input;
	delete raw_input;
      } else
      {
	std::cerr << "Couldn't serialize message!" << endl;
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
