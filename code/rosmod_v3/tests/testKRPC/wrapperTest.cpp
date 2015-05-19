#include "krpcWrapper.hpp"

using namespace std;

void myStreamFunc(krpc::Response& response)
{
  if ( response.has_error() )
    {
      std::cout << "Response error: " << response.error() << endl;
      return;
    }
  krpc::Tuple tuple;
  double x,y,z;
  tuple.ParseFromString(response.return_value());
  KRPCI::DecodeTuple(tuple,x,y,z);
  printf("(x,y,z) = (%f,%f,%f)\n",x,y,z);
}

int main(int argc, char** argv)
{
  KRPCI client("wrapperTest");
  if ( client.Connect() )
    {
      int numVessels;
      std::vector<uint64_t> vesselIDs;
      client.GetVessels(vesselIDs);
      std::cout << "There are " << vesselIDs.size() << " vessel(s):" << endl;
      for (int i=0;i<vesselIDs.size();i++)
	{
	  std::cout << "\tID #" << i+1 << " = " << vesselIDs[i] << endl;
	}
      uint64_t vesselID;
      client.GetActiveVessel(vesselID);
      std::cout << "Active vessel ID: " << vesselID << std::endl;
      std::string vesselName;
      client.GetVesselName(vesselID,vesselName);
      std::cout << "Active vessel Name: " << vesselName << std::endl;
      uint64_t orbitID;
      client.GetVesselOrbit(vesselID, orbitID);
      std::cout << "Active vessel orbit: " << orbitID << endl;
      double apoapsis;
      client.GetOrbitApoapsisAltitude(orbitID,apoapsis);
      std::cout << "Active vessel orbit has apoapsis altitude: " << apoapsis << endl;
      double time;
      client.GetOrbitTimeToApoapsis(orbitID,time);
      std::cout << "Active vessel reaches orbit apoapsis in: " << time << endl;
      uint64_t bodyID;
      client.GetOrbitBody(orbitID, bodyID);
      std::cout << "Active vessel orbiting body: " << bodyID << std::endl;
      uint64_t orbitalRefFrame;
      client.GetBodyReferenceFrame(bodyID, orbitalRefFrame);
      std::cout << "Orbiting body's Reference Frame ID: " << orbitalRefFrame << std::endl;
      double position[3];
      client.GetVesselPosition(vesselID, orbitalRefFrame, position[0], position[1], position[2]);
      std::cout << "Active vessel Position: "<< position[0]<<","<<position[1]<<","<<position[2]<<endl;
      uint64_t controlID;
      client.GetVesselControl(vesselID, controlID);
      std::cout << "Active vessel has control ID: " << controlID << endl;
      client.SetControlSAS(controlID,true);
      client.SetControlRCS(controlID,true);
      client.SetControlThrottle(controlID,1.0);
      client.SetControlPitch(controlID,45.0);
      client.SetControlRoll(controlID,20.0);
      client.SetControlYaw(controlID,30.0);

      std::string streamName = "testStream";
      krpc::Request request;
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
      CodedOutputStream::WriteVarint64ToArray(orbitalRefFrame, (unsigned char *)argument->mutable_value()->data());
      client.CreateStream(streamName,request, myStreamFunc);

      sleep(10);

      client.RemoveStream(streamName);

      sleep(10);
    }
  client.Close();
}
