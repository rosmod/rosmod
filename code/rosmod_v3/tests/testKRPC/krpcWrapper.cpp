#include "krpcWrapper.hpp"

using namespace std;

KRPCI::KRPCI(string name, string ip, int port, int streamPort) 
  : name_(name),
    ip_(ip),
    port_(port),
    streamPort_(streamPort)
{
  name_.resize(32);
  id_.resize(16);
  timeout_ = 1;
}

KRPCI::~KRPCI()
{
  Close();
}

bool KRPCI::Connect()
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

bool KRPCI::Close()
{
  close(socket_);
  close(streamSocket_);
}

bool KRPCI::CreateStream(std::string streamName, krpc::Request req)
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

bool KRPCI::GetActiveVessel(uint64_t& id)
{
  krpc::Request request;
  krpc::Response response;
  uint64_t vesselID = 0;

  request.set_service("SpaceCenter");
  request.set_procedure("get_ActiveVessel");

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(id, (char *)response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::GetVessels(std::vector<uint64_t>& ids)
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
      krpc::List vesselList;
      vesselList.ParseFromString(response.return_value());
      for (int i=0;i<vesselList.items_size();i++)
	{
	  uint64_t id;
	  const string item = vesselList.items(i);
	  CodedInputStream::ReadLittleEndian64FromArray((const unsigned char*)item.data(),&id);
	  ids.push_back(id);
	}
    }
  return true;
}

bool KRPCI::GetVesselName(uint64_t vesselID, string& name)
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
      KRPCI::DecodeString(name, (char *)response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::GetVesselPosition(uint64_t vesselID, uint64_t refFrame, double &x, double &y, double &z)
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
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple,x,y,z);
    }
  return true;
}

bool KRPCI::GetVesselVelocity(uint64_t vesselID, uint64_t refFrame, double &x, double &y, double &z)
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
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple,x,y,z);
    }
  return true;
}

bool KRPCI::GetVesselOrbitalReferenceFrame(uint64_t vesselID, uint64_t& refFrame)
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
      KRPCI::DecodeVarint(refFrame, (char *)response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::GetVesselSurfaceReferenceFrame(uint64_t vesselID, uint64_t& refFrame)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_SurfaceReferenceFrame");

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
      KRPCI::DecodeVarint(refFrame, (char *)response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::GetVesselRotation(uint64_t vesselID, uint64_t refFrame, double &x, double &y, double &z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_Rotation");

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
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple,x,y,z);
    }
  return true;
}

bool KRPCI::GetVesselOrbit(uint64_t vesselID, uint64_t &orbit)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Orbit");

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
      KRPCI::DecodeVarint(orbit, (char *)response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::GetOrbitBody(uint64_t orbitID, uint64_t &bodyID)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Body");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(orbitID, (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(bodyID, (char *)response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::GetOrbitApoapsis(uint64_t orbitID, double& apo)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Apoapsis");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(orbitID, (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      apo = 0.0;
      memcpy(&apo, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::GetOrbitApoapsisAltitude(uint64_t orbitID, double& alt)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_ApoapsisAltitude");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(orbitID, (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      alt = 0.0;
      memcpy(&alt, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::GetOrbitPeriapsis(uint64_t orbitID, double& peri)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Periapsis");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(orbitID, (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      peri = 0.0;
      memcpy(&peri, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::GetOrbitSpeed(uint64_t orbitID, double& speed)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Speed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(orbitID, (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      speed = 0.0;
      memcpy(&speed, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::GetOrbitTimeToApoapsis(uint64_t orbitID, double& time)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_TimeToApoapsis");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(orbitID, (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if ( response.has_error() )
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      time = 0.0;
      memcpy(&time, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::GetOrbitTimeToPeriapsis(uint64_t orbitID, double& time)
{
}

bool KRPCI::GetBodyReferenceFrame(uint64_t vesselID, uint64_t& refFrame)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_ReferenceFrame");

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
      KRPCI::DecodeVarint(refFrame, (char *)response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::SetTargetVessel(uint64_t vesselID)
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

bool KRPCI::SetControlSAS(uint64_t controlID, bool on)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_SAS");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(controlID, (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(on, (unsigned char *)argument->mutable_value()->data());

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

bool KRPCI::SetControlRCS(uint64_t controlID, bool on)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_RCS");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(controlID, (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(on, (unsigned char *)argument->mutable_value()->data());

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

bool KRPCI::SetThrottle(uint64_t vesselID, float value)
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

bool KRPCI::SetPitch(uint64_t vesselID, float value)
{
}

bool KRPCI::SetRoll(uint64_t vesselID, float value)
{
}

bool KRPCI::SetYaw(uint64_t vesselID, float value)
{
}

// UTILITY FUNCTIONS:

bool KRPCI::createRequestString(krpc::Request req, std::string& str)
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

bool KRPCI::getResponseFromRequest(krpc::Request req, krpc::Response& res)
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

void KRPCI::PrintBytesHex(const char *buf, int size)
{
  for (int i = 0; i < size; i++)
    printf("0x%02X ", (unsigned char)buf[i]);
}

void KRPCI::EncodeVarint(uint32_t value, char *buf, int &size)
{
  size = google::protobuf::io::CodedOutputStream::VarintSize32(value);

  google::protobuf::io::ZeroCopyOutputStream *zcos = new google::protobuf::io::ArrayOutputStream(buf, size);
  google::protobuf::io::CodedOutputStream *cos = new google::protobuf::io::CodedOutputStream(zcos);
  cos->WriteVarint32(value);

  delete cos;
  delete zcos;
}

void KRPCI::EncodeVarint(uint64_t value, char *buf, int &size)
{
  size = google::protobuf::io::CodedOutputStream::VarintSize64(value);

  google::protobuf::io::ZeroCopyOutputStream *zcos = new google::protobuf::io::ArrayOutputStream(buf, size);
  google::protobuf::io::CodedOutputStream *cos = new google::protobuf::io::CodedOutputStream(zcos);
  cos->WriteVarint64(value);

  delete cos;
  delete zcos;
}

void KRPCI::DecodeVarint(uint32_t &value, char *buf, int size)
{
  // init coded stream
  google::protobuf::io::ZeroCopyInputStream *zcos = new google::protobuf::io::ArrayInputStream(buf, size);
  google::protobuf::io::CodedInputStream *cos = new google::protobuf::io::CodedInputStream(zcos);
  cos->ReadVarint32(&value);

  delete cos;
  delete zcos;
}

void KRPCI::DecodeVarint(uint64_t &value, char *buf, int size)
{
  // init coded stream
  google::protobuf::io::ZeroCopyInputStream *zcos = new google::protobuf::io::ArrayInputStream(buf, size);
  google::protobuf::io::CodedInputStream *cos = new google::protobuf::io::CodedInputStream(zcos);
  cos->ReadVarint64(&value);

  delete cos;
  delete zcos;
}

void KRPCI::DecodeString(string& str, char *buf, int size)
{
  google::protobuf::io::ZeroCopyInputStream *zcos = new google::protobuf::io::ArrayInputStream(buf, size);
  google::protobuf::io::CodedInputStream *cos = new google::protobuf::io::CodedInputStream(zcos);
  uint64_t len;
  cos->ReadVarint64(&len);
  cos->ReadString(&str,len);

  delete cos;
  delete zcos;
}

void KRPCI::EncodeTuple(double x, double y, double z, krpc::Tuple &tuple)
{ 
  string *x_str = tuple.add_items();
  string *y_str = tuple.add_items();
  string *z_str = tuple.add_items();

  x_str->assign((char *)&x, sizeof(x));
  y_str->assign((char *)&y, sizeof(y));
  z_str->assign((char *)&z, sizeof(z));
}

void KRPCI::DecodeTuple(krpc::Tuple tuple, double& x, double& y, double& z)
{
  memcpy(&x, tuple.items(0).data(), tuple.items(0).size());
  memcpy(&y, tuple.items(1).data(), tuple.items(0).size());
  memcpy(&z, tuple.items(2).data(), tuple.items(0).size());
}
