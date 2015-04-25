#include "krpcWrapper.hpp"

using namespace std;

int main(int argc, char** argv)
{
  KRPC_Client client("wrapperTest");
  if ( client.Connect() )
    {
      int vesselID;
      client.GetActiveVessel(vesselID);
      std::cout << "Active vessel ID: " << vesselID << std::endl;
      std::string vesselName;
      client.GetVesselName(vesselID,vesselName);
      std::cout << "Active vessel Name: " << vesselName << std::endl;
    }
  client.Close();
}
