#include "krpcWrapper.hpp"

using namespace std;

int main(int argc, char** argv)
{
  KRPC_Client client("wrapperTest");
  if ( client.Connect() )
    {
      int numVessels;
      std::vector<int> vesselIDs;
      client.GetVessels(vesselIDs);
      std::cout << "There are " << vesselIDs.size() << " vessel(s):" << endl;
      for (int i=0;i<vesselIDs.size();i++)
	{
	  std::cout << "\tID #" << i+1 << " = " << vesselIDs[i] << endl;
	}
      int vesselID;
      client.GetActiveVessel(vesselID);
      std::cout << "Active vessel ID: " << vesselID << std::endl;
      std::string vesselName;
      client.GetVesselName(vesselID,vesselName);
      std::cout << "Active vessel Name: " << vesselName << std::endl;
      int orbitalRefFrame;
      client.GetVesselOrbitalReferenceFrame(vesselID, orbitalRefFrame);
      std::cout << "Active vessel Orbital Reference Frame ID: " << orbitalRefFrame << std::endl;
      double position[3];
      client.GetVesselPosition(vesselID, orbitalRefFrame, position);
      //client.SetControlSAS(vesselID,false);
      //client.SetControlRCS(vesselID,false);
      //client.SetThrottle(vesselID,1.0);
    }
  client.Close();
}
