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
      std::cout << "There are " << vesselIDs.size() << " vessels." << endl;
      for (int i=0;i<vesselIDs.size();i++)
	{
	  std::cout << "ID " << i << " = " << vesselIDs[i] << endl;
	}
      int vesselID;
      client.GetActiveVessel(vesselID);
      std::cout << "Active vessel ID: " << vesselID << std::endl;
      std::string vesselName;
      client.GetVesselName(vesselID,vesselName);
      std::cout << "Active vessel Name: " << vesselName << std::endl;
    }
  client.Close();
}
