#include "krpcWrapper.hpp"

using namespace std;

KRPC_Client::KRPC_Client(string name, string ip, int port, int streamPort) 
  : name_(name),
    ip_(ip),
    port_(port),
    streamPort_(streamPort)
{
  name_.resize(32);
  id_.resize(16);
  timeout_ = 1;
}

KRPC_Client::~KRPC_Client()
{
  Close();
}

bool KRPC_Client::Connect()
{
  /* socket: create the socket */
  if ((socket_ = socket(AF_INET, SOCK_STREAM, 0)) <0) {
    perror("ERROR opening socket");
    return false;
  }
  /* build the server's Internet address */
  struct sockaddr_in server_addr;    
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port_);
  if (inet_pton(AF_INET, ip_.c_str(), &(server_addr.sin_addr)) !=1) {
    perror("inet_pton");
    return false;
  }
  /* set the address to zero */
  memset(server_addr.sin_zero, 0, sizeof server_addr.sin_zero);
  /* connect: create a connection with the server */
  if (connect(socket_, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    perror("ERROR connecting");
    return false;
  }
  /* set the timeout on the socket receive */
  struct timeval tv;
  tv.tv_sec = timeout_;
  tv.tv_usec = 0;
  if (setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
    perror("Couldn't set sockopts!");
    close(socket_);
    return false;
  }
  /* send the message line to the server */
  int sentbytes=0,numbytes=0;
  if ( numbytes = send(socket_, helloMessage, 12,0) == -1) {
    perror("send");
    return false;
  }
  /* send the client's connection name to the server */
  if ( numbytes = send(socket_, name_.c_str(), 32,0) == -1) {
    perror("send");
    return false;
  }
  /* receive the unique ID from server for this client */
  bool haveReceivedID = false;
  while (!haveReceivedID)
    {
      id_.clear();
      int bytesreceived =0;
      if ( (bytesreceived=recv(socket_,(void *)id_.data(),16,0)) <= 0) {
      }
      else {
	haveReceivedID = true;
      }
    }
  // ****************************
  // CONNECT TO THE STREAM SERVER
  // ****************************
  /* Open the stream socket */
  if ((streamSocket_ = socket(AF_INET, SOCK_STREAM, 0)) <0) {
    perror("ERROR opening stream socket");
    return false;
  }
  /* build the server's Internet address */
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(streamPort_);
  if (inet_pton(AF_INET, ip_.c_str(), &(server_addr.sin_addr)) !=1) {
    perror("stream inet_pton");
    return false;
  }
  /* set the address to zero */
  memset(server_addr.sin_zero, 0, sizeof server_addr.sin_zero);
  /* connect: create a connection with the server */
  if (connect(streamSocket_, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    perror("ERROR connecting to stream");
    return false;
  }
  /* set the timeout on the socket receive */
  tv.tv_sec = timeout_;
  tv.tv_usec = 0;
  if (setsockopt(streamSocket_, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
    perror("Couldn't set sockopts!");
    Close();
    return false;
  }
  /* Send the stream Hello message */
  sentbytes=0,numbytes=0;
  if ( numbytes = send(streamSocket_, helloStreamMessage, 12,0) == -1) {
    perror("send");
    return false;
  }
  /* Send the client ID */
  if ( numbytes = send(streamSocket_, id_.c_str(), 16,0) == -1) {
    perror("send");
    return false;
  }
  /* Receive the stream ACK */
  bool haveReceivedAck = false;
  char ack[2];
  while (!haveReceivedAck)
    {
      memset(ack,0,2);
      int bytesreceived =0;
      if ( (bytesreceived=recv(streamSocket_,(void *)ack,2,0)) <= 0) {
      }
      else {
	if ( strncmp(ack,streamAck,2) == 0 )
	  haveReceivedAck = true;
      }
    }
  return true;
}

bool KRPC_Client::Close()
{
  close(socket_);
  close(streamSocket_);
}

bool KRPC_Client::CreateStream(std::string streamName, krpc::Request req)
{
  string message;
  message.reserve(40);
  bool retVal = true;
  if ( createRequestString(req,message) )
    {
      int numBytes;
      if ( (numBytes = send(streamSocket_, message.data(), message.length(), 0)) == -1 )
	{
	  perror("sending request");
	  return false;
	}
      activeStreams_[streamName];
    } else
    {
      std::cerr << "Couldn't serialize request!" << std::endl;
      retVal = false;
    }
  return retVal;
}

bool KRPC_Client::GetActiveVessel(int& id)
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("SpaceCenter");
  request.set_procedure("get_ActiveVessel");

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      ZeroCopyInputStream* input = 
	new ArrayInputStream(response.return_value().data(), response.return_value().length());
      CodedInputStream* codeStream = 
	new CodedInputStream(input);
      uint64_t val;
      codeStream->ReadVarint64(&val);
      id = val;
      delete codeStream;
      delete input;
    }
  return true;
}

bool KRPC_Client::GetVessels(std::vector<int>& ids)
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("SpaceCenter");
  request.set_procedure("get_Vessels");

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      ZeroCopyInputStream* input = 
	new ArrayInputStream(response.return_value().data(), response.return_value().length());
      CodedInputStream* codeStream = 
	new CodedInputStream(input);
      krpc::List vesselList;
      vesselList.ParseFromCodedStream(codeStream);
      for (int i=0;i<vesselList.items_size();i++)
	{
	  uint64_t id;
	  const string item = vesselList.items(i);
	  CodedInputStream::ReadLittleEndian64FromArray((const unsigned char*)item.data(),&id);
	  ids.push_back(id);
	}
      delete codeStream;
      delete input;
    }
  return true;
}

bool KRPC_Client::GetVesselName(int vesselID, string& name)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Name");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(vesselID, (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      ZeroCopyInputStream* input = 
	new ArrayInputStream(response.return_value().data(), response.return_value().length());
      CodedInputStream* codeStream = 
	new CodedInputStream(input);
      uint64_t len;
      codeStream->ReadVarint64(&len);
      codeStream->ReadString(&name,len);
      delete codeStream;
      delete input;
    }
  return true;
}

bool KRPC_Client::GetVesselPosition(int vesselID, int refFrame, double* pos)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_Position");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(vesselID, (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(refFrame, (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      ZeroCopyInputStream* input = 
	new ArrayInputStream(response.return_value().data(), response.return_value().length());
      std::cout << "length = " << response.return_value().length() << endl;
      CodedInputStream* codeStream = 
	new CodedInputStream(input);
      krpc::Tuple tuple;
      tuple.ParseFromCodedStream(codeStream);
      std::cout << "Got a tuple of length: " << tuple.items_size() << endl;

      for (int i=0;i<tuple.items_size();i++)
	{
	  delete codeStream;
	  delete input;
	  input = new ArrayInputStream(tuple.items(i).data(), tuple.items(i).length());
	  codeStream = new CodedInputStream(input);
	  char buf[10];
	  bool test = codeStream->ReadRaw(buf,8);
	  std::cout << test << endl;
	  cout.setf(ios::hex, ios::basefield);
	  for (int j =0;j++;j<8)
	    printf("%d\n",buf[j]);
	  cout.unsetf(ios::hex);

	  double val = *&buf[0];
	  pos[i] = val;
	}
      delete codeStream;
      delete input;
    }
  return true;
}

bool KRPC_Client::GetVesselVelocity(int vesselID, int refFrame, double* vel)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_Velocity");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(vesselID, (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(refFrame, (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      ZeroCopyInputStream* input = 
	new ArrayInputStream(response.return_value().data(), response.return_value().length());
      CodedInputStream* codeStream = 
	new CodedInputStream(input);
      uint64_t len;
      codeStream->ReadVarint64(&len);
      delete codeStream;
      delete input;
    }
  return true;
}

bool KRPC_Client::GetVesselOrbitalReferenceFrame(int vesselID, int& refFrame)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_OrbitalReferenceFrame");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(vesselID, (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      ZeroCopyInputStream* input = 
	new ArrayInputStream(response.return_value().data(), response.return_value().length());
      CodedInputStream* codeStream = 
	new CodedInputStream(input);
      codeStream->ReadVarint64((uint64_t *)&refFrame);
      delete codeStream;
      delete input;
    }
  return true;
}

bool KRPC_Client::GetVesselRotation(int vesselID, int refFrame, double* rot)
{
}

bool KRPC_Client::GetApoapsis(int vesselID, double& apo)
{
}

bool KRPC_Client::GetPeriapsis(int vesselID, double& peri)
{
}

bool KRPC_Client::GetOrbitalSpeed(int vesselID, double& speed)
{
}

bool KRPC_Client::GetTimeToApoapsis(int vesselID, double& time)
{
}

bool KRPC_Client::GetTimeToPeriapsis(int vesselID, double& time)
{
}


bool KRPC_Client::SetTargetVessel(int vesselID)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("set_TargetVessel");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(vesselID, (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPC_Client::SetControlSAS(int vesselID, bool on)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_SAS");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(vesselID, (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(1);
  CodedOutputStream::WriteVarint64ToArray(on, (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPC_Client::SetControlRCS(int vesselID, bool on)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_RCS");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(vesselID, (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(1);
  CodedOutputStream::WriteVarint64ToArray(on, (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPC_Client::SetThrottle(int vesselID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_set_Throttle");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(vesselID, (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(8);
  double v = value;
  CodedOutputStream::WriteRawToArray((char *)&v, 8, (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPC_Client::SetPitch(int vesselID, float value)
{
}

bool KRPC_Client::SetRoll(int vesselID, float value)
{
}

bool KRPC_Client::SetYaw(int vesselID, float value)
{
}

// UTILITY FUNCTIONS:

bool KRPC_Client::createRequestString(krpc::Request req, std::string& str)
{
  uint64_t size;
  string msg;
  if ( req.SerializeToString(&msg) )
    {
      unsigned char msgLen[10] = {0};
      CodedOutputStream::WriteVarint64ToArray(msg.length(), msgLen);
      str = string((const char *)msgLen) + msg;
    } else
    {
      return false;
    }
  return true;
}

bool KRPC_Client::getResponseFromRequest(krpc::Request req, krpc::Response& res)
{
  string message;
  message.reserve(40);
  bool retVal = true;
  if ( createRequestString(req,message) )
    {
      int numBytes;
      if ( (numBytes = send(socket_, message.data(), message.length(), 0)) == -1 )
	{
	  perror("sending request");
	  return false;
	}
      char buf[maxBufferSize];
      memset(buf,0,maxBufferSize);
      int bytesreceived =0;
      if ( (bytesreceived=recv(socket_,buf,maxBufferSize-1,0)) <= 0) {
	perror("recv");
	return false;
      }
      //std::cout << "Socket received # bytes = " << bytesreceived << endl;
      ZeroCopyInputStream* raw_input = new ArrayInputStream(buf,maxBufferSize);
      CodedInputStream* coded_input = new CodedInputStream(raw_input);
      uint64_t size;
      coded_input->ReadVarint64(&size);
      //std::cout << "Received " << size << " bytes." << endl;
      if (!res.ParseFromCodedStream(coded_input))
	{
	  retVal = false;
	}
      delete coded_input;
      delete raw_input;
    } else
    {
      std::cerr << "Couldn't serialize request!" << std::endl;
      retVal = false;
    }
  return retVal;
}
