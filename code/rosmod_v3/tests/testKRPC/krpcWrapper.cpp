#include "krpcWrapper.hpp"

using namespace std;

KRPC_Client::KRPC_Client(string name, string ip, int port) 
  : name_(name),
    ip_(ip),
    port_(port)
{
  name_.reserve(32);
  timeout_ = 1;
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
  char connID[32] = "testKRPC_program";
  if ( numbytes = send(socket_, connID, 32,0) == -1) {
    perror("send");
    return false;
  }

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
  return true;
}

bool KRPC_Client::Close()
{
  close(socket_);
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
}

bool KRPC_Client::GetVesselName(int vesselID, string& name)
{
  krpc::Request request;
  krpc::Response response;
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Name");
  krpc::Argument* argument = request.add_arguments();
  argument->set_position(0);
  unsigned char varint[10];
  CodedOutputStream::WriteVarint64ToArray(vesselID, varint);
  argument->set_value((const char*)varint);
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

bool KRPC_Client::GetVesselPosition(int vesselID, int refFrame, krpc::Tuple& pos)
{
}

bool KRPC_Client::GetVesselVelocity(int vesselID, int refFrame, krpc::Tuple& vel)
{
}

bool KRPC_Client::GetVesselRotation(int vesselID, int refFrame, krpc::Tuple& rot)
{
}

bool KRPC_Client::SetActiveVessel(int vesselID)
{
}

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
      }
      ZeroCopyInputStream* raw_input = new ArrayInputStream(buf,maxBufferSize);
      CodedInputStream* coded_input = new CodedInputStream(raw_input);
      uint64_t size;
      coded_input->ReadVarint64(&size);
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
