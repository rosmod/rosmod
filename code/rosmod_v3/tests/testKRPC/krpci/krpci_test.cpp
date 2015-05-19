#include "krpci.hpp"

using namespace std;

int main(int argc, char** argv)
{
  KRPCI client("wrapperTest");
  if ( client.Connect() )
    {
      int numVessels;
      std::vector<uint64_t> vesselIDs;
      client.get_Vessels(vesselIDs);
      std::cout << "There are " << vesselIDs.size() << " vessel(s):" << endl;
      for (int i=0;i<vesselIDs.size();i++)
	{
	  std::cout << "\tID #" << i+1 << " = " << vesselIDs[i] << endl;
	}
      uint64_t vesselID;
      client.get_ActiveVessel(vesselID);
      std::cout << "Active vessel ID: " << vesselID << std::endl;
      std::string vesselName;
      client.Vessel_get_Name(vesselID,vesselName);
      std::cout << "Active vessel Name: " << vesselName << std::endl;
      uint64_t orbitID;
      client.Vessel_get_Orbit(vesselID, orbitID);
      std::cout << "Active vessel orbit: " << orbitID << endl;
      double apoapsis;
      client.Orbit_get_ApoapsisAltitude(orbitID,apoapsis);
      std::cout << "Active vessel orbit has apoapsis altitude: " << apoapsis << endl;
      double time;
      client.Orbit_get_TimeToApoapsis(orbitID,time);
      std::cout << "Active vessel reaches orbit apoapsis in: " << time << endl;
      uint64_t bodyID;
      client.Orbit_get_Body(orbitID, bodyID);
      std::cout << "Active vessel orbiting body: " << bodyID << std::endl;
      uint64_t orbitalRefFrame;
      client.CelestialBody_get_ReferenceFrame(bodyID, orbitalRefFrame);
      std::cout << "Orbiting body's Reference Frame ID: " << orbitalRefFrame << std::endl;
      double position[3];
      client.Vessel_Position(vesselID, orbitalRefFrame, position[0], position[1], position[2]);
      std::cout << "Active vessel Position: "<< position[0]<<","<<position[1]<<","<<position[2]<<endl;
      uint64_t controlID;
      client.Vessel_get_Control(vesselID, controlID);
      std::cout << "Active vessel has control ID: " << controlID << endl;
      client.Control_set_SAS(controlID,true);
      client.Control_set_RCS(controlID,true);
      client.Control_set_Throttle(controlID,1.0);
      client.Control_set_Pitch(controlID,45.0);
      client.Control_set_Roll(controlID,20.0);
      client.Control_set_Yaw(controlID,30.0);
    }
  client.Close();
}
