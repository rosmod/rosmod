#include "krpcWrapper.hpp"

using namespace std;

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
      uint64_t orbitalRefFrame;
      client.GetVesselOrbitalReferenceFrame(vesselID, orbitalRefFrame);
      std::cout << "Active vessel Orbital Reference Frame ID: " << orbitalRefFrame << std::endl;
      double position[3];
      client.GetVesselPosition(vesselID, orbitalRefFrame, position[0], position[1], position[2]);
      std::cout << "Active vessel Position: "<< position[0]<<","<<position[1]<<","<<position[2]<<endl;
      uint64_t orbitID;
      client.GetVesselOrbit(vesselID, orbitID);
      std::cout << "Active vessel orbit: " << orbitID << endl;
      double apoapsis;
      client.GetOrbitApoapsisAltitude(orbitID,apoapsis);
      std::cout << "Active vessel orbit has apoapsis altitude: " << apoapsis << endl;
      double time;
      client.GetOrbitTimeToApoapsis(orbitID,time);
      std::cout << "Active vessel reaches orbit apoapsis in: " << time << endl;
      //client.SetControlSAS(vesselID,false);
      //client.SetControlRCS(vesselID,false);
      //client.SetThrottle(vesselID,1.0);
    }
  client.Close();
}
