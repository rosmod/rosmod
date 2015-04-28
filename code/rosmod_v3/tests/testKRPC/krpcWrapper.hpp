#include <iostream>
#include <fstream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "KRPC.pb.h"

#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>
using namespace google::protobuf::io;

#include <boost/thread.hpp>

#include <stdio.h>
#include <stdlib.h>

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

const int maxBufferSize = 65535;
const char helloMessage[] = { 0x48, 0x45, 0x4C, 0x4C, 0x4F, 0x2D, 0x52, 0x50, 0x43, 0x00, 0x00, 0x00 };
const char helloStreamMessage[] = { 0x48, 0x45, 0x4C, 0x4C, 0x4F, 0x2D, 0x53, 0x54, 0x52, 0x45, 0x41, 0x4D };
const char streamAck[] = { 0x4F, 0x4B };

class KRPC_Client
{
public:
  KRPC_Client(std::string name, std::string ip="127.0.0.1", int port=50000, int streamPort=50001);
  ~KRPC_Client();

  bool Connect();
  bool Close();

  bool CreateStream(std::string streamName, krpc::Request req);
  bool GetLatestStreamData(std::string streamName, krpc::Response& res);

  bool GetActiveVessel(int& id);
  bool GetVessels(std::vector<int>& ids);
  bool GetVesselName(int vesselID, std::string& name);
  bool GetVesselPosition(int vesselID, int refFrame, double* pos);
  bool GetVesselVelocity(int vesselID, int refFrame, double* vel);
  bool GetVesselRotation(int vesselID, int refFrame, double* rot);
  bool GetVesselOrbitalReferenceFrame(int vesselID, int& refFrame);

  bool GetApoapsis(int vesselID, double& apo);
  bool GetPeriapsis(int vesselID, double& peri);
  bool GetOrbitalSpeed(int vesselID, double& speed);
  bool GetTimeToApoapsis(int vesselID, double& time);
  bool GetTimeToPeriapsis(int vesselID, double& time);

  bool SetTargetVessel(int vesselID);
  bool SetControlSAS(int vesselID, bool on);
  bool SetControlRCS(int vesselID, bool on);
  bool SetThrottle(int vesselID, float value);
  bool SetPitch(int vesselID, float value);
  bool SetRoll(int vesselID, float value);
  bool SetYaw(int vesselID, float value);
protected:
  bool createRequestString(krpc::Request req, std::string& str);
  bool getResponseFromRequest(krpc::Request req, krpc::Response& res);
  void streamThreadFunc();
private:
  std::map<std::string,krpc::Response> activeStreams_;

  int port_;
  int streamPort_;
  std::string ip_;
  std::string id_;
  std::string name_;
  int socket_;
  int streamSocket_;
  int timeout_;
};
