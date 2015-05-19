#include "krpci.hpp"
using namespace std;

bool KRPCI::ClearTarget()
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("SpaceCenter");
  request.set_procedure("ClearTarget");

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::WarpTo(double UT, float maxRate)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("WarpTo");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->set_value((const char*)(&UT), sizeof(UT));  

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&maxRate), sizeof(maxRate));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::TransformPosition(double position_x, double position_y, double position_z, uint64_t from, uint64_t to, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("TransformPosition");

  argument = request.add_arguments();
  argument->set_position(0);
  krpc::Tuple position;
  KRPCI::EncodeTuple(position_x, 
		     position_y, 
		     position_z, 
		     position); 
  position.SerializeToString(argument->mutable_value());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(from, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(2);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(to, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::TransformDirection(double direction_x, double direction_y, double direction_z, uint64_t from, uint64_t to, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("TransformDirection");

  argument = request.add_arguments();
  argument->set_position(0);
  krpc::Tuple direction;
  KRPCI::EncodeTuple(direction_x, 
		     direction_y, 
		     direction_z, 
		     direction); 
  direction.SerializeToString(argument->mutable_value());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(from, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(2);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(to, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::TransformRotation(double rotation_x, double rotation_y, double rotation_z, uint64_t from, uint64_t to, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("TransformRotation");

  argument = request.add_arguments();
  argument->set_position(0);
  krpc::Tuple rotation;
  KRPCI::EncodeTuple(rotation_x, 
		     rotation_y, 
		     rotation_z, 
		     rotation); 
  rotation.SerializeToString(argument->mutable_value());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(from, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(2);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(to, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::TransformVelocity(double position_x, double position_y, double position_z, double velocity_x, double velocity_y, double velocity_z, uint64_t from, uint64_t to, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("TransformVelocity");

  argument = request.add_arguments();
  argument->set_position(0);
  krpc::Tuple position;
  KRPCI::EncodeTuple(position_x, 
		     position_y, 
		     position_z, 
		     position); 
  position.SerializeToString(argument->mutable_value());

  argument = request.add_arguments();
  argument->set_position(1);
  krpc::Tuple velocity;
  KRPCI::EncodeTuple(velocity_x, 
		     velocity_y, 
		     velocity_z, 
		     velocity); 
  velocity.SerializeToString(argument->mutable_value());

  argument = request.add_arguments();
  argument->set_position(2);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(from, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(3);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(to, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::DrawDirection(double direction_x, double direction_y, double direction_z, uint64_t referenceFrame, double color_x, double color_y, double color_z, float length)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("DrawDirection");

  argument = request.add_arguments();
  argument->set_position(0);
  krpc::Tuple direction;
  KRPCI::EncodeTuple(direction_x, 
		     direction_y, 
		     direction_z, 
		     direction); 
  direction.SerializeToString(argument->mutable_value());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(2);
  krpc::Tuple color;
  KRPCI::EncodeTuple(color_x, 
		     color_y, 
		     color_z, 
		     color); 
  color.SerializeToString(argument->mutable_value());

  argument = request.add_arguments();
  argument->set_position(3);
  argument->set_value((const char*)(&length), sizeof(length));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::ClearDirections()
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("SpaceCenter");
  request.set_procedure("ClearDirections");

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::get_ActiveVessel(uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("SpaceCenter");
  request.set_procedure("get_ActiveVessel");

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::get_Vessels(std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("SpaceCenter");
  request.set_procedure("get_Vessels");

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::get_Bodies(krpc::Dictionary& return_dict)
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("SpaceCenter");
  request.set_procedure("get_Bodies");

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::get_TargetBody(uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("SpaceCenter");
  request.set_procedure("get_TargetBody");

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::set_TargetBody(uint64_t value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("set_TargetBody");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::get_TargetVessel(uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("SpaceCenter");
  request.set_procedure("get_TargetVessel");

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::set_TargetVessel(uint64_t value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("set_TargetVessel");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::get_TargetDockingPort(uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("SpaceCenter");
  request.set_procedure("get_TargetDockingPort");

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::set_TargetDockingPort(uint64_t value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("set_TargetDockingPort");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::get_UT(double& return_value)
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("SpaceCenter");
  request.set_procedure("get_UT");

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::get_G(float& return_value)
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("SpaceCenter");
  request.set_procedure("get_G");

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::get_FARAvailable(bool& return_value)
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("SpaceCenter");
  request.set_procedure("get_FARAvailable");

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::get_RemoteTechAvailable(bool& return_value)
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("SpaceCenter");
  request.set_procedure("get_RemoteTechAvailable");

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::AutoPilot_SetRotation(uint64_t AutoPilot_ID, float pitch, float heading, float roll, uint64_t referenceFrame, bool wait)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_SetRotation");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&pitch), sizeof(pitch));

  argument = request.add_arguments();
  argument->set_position(2);
  argument->set_value((const char*)(&heading), sizeof(heading));

  argument = request.add_arguments();
  argument->set_position(3);
  argument->set_value((const char*)(&roll), sizeof(roll));

  argument = request.add_arguments();
  argument->set_position(4);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(5);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(wait, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::AutoPilot_SetDirection(uint64_t AutoPilot_ID, double direction_x, double direction_y, double direction_z, float roll, uint64_t referenceFrame, bool wait)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_SetDirection");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  krpc::Tuple direction;
  KRPCI::EncodeTuple(direction_x, 
		     direction_y, 
		     direction_z, 
		     direction); 
  direction.SerializeToString(argument->mutable_value());

  argument = request.add_arguments();
  argument->set_position(2);
  argument->set_value((const char*)(&roll), sizeof(roll));

  argument = request.add_arguments();
  argument->set_position(3);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(4);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(wait, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::AutoPilot_Disengage(uint64_t AutoPilot_ID)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_Disengage");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::AutoPilot_get_SAS(uint64_t AutoPilot_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_get_SAS");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::AutoPilot_set_SAS(uint64_t AutoPilot_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_set_SAS");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::AutoPilot_get_SASMode(uint64_t AutoPilot_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_get_SASMode");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::AutoPilot_set_SASMode(uint64_t AutoPilot_ID, int32_t value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_set_SASMode");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::AutoPilot_get_SpeedMode(uint64_t AutoPilot_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_get_SpeedMode");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::AutoPilot_set_SpeedMode(uint64_t AutoPilot_ID, int32_t value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_set_SpeedMode");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::AutoPilot_get_Error(uint64_t AutoPilot_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_get_Error");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::AutoPilot_get_RollError(uint64_t AutoPilot_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_get_RollError");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::CelestialBody_Position(uint64_t CelestialBody_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_Position");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::CelestialBody_Velocity(uint64_t CelestialBody_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_Velocity");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::CelestialBody_Rotation(uint64_t CelestialBody_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_Rotation");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::CelestialBody_Direction(uint64_t CelestialBody_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_Direction");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::CelestialBody_AngularVelocity(uint64_t CelestialBody_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_AngularVelocity");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::CelestialBody_get_Name(uint64_t CelestialBody_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_Name");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::CelestialBody_get_Satellites(uint64_t CelestialBody_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_Satellites");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::CelestialBody_get_Mass(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_Mass");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::CelestialBody_get_GravitationalParameter(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_GravitationalParameter");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::CelestialBody_get_SurfaceGravity(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_SurfaceGravity");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::CelestialBody_get_RotationalPeriod(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_RotationalPeriod");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::CelestialBody_get_RotationalSpeed(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_RotationalSpeed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::CelestialBody_get_EquatorialRadius(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_EquatorialRadius");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::CelestialBody_get_SphereOfInfluence(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_SphereOfInfluence");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::CelestialBody_get_Orbit(uint64_t CelestialBody_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_Orbit");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::CelestialBody_get_HasAtmosphere(uint64_t CelestialBody_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_HasAtmosphere");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::CelestialBody_get_AtmosphereDepth(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_AtmosphereDepth");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::CelestialBody_get_HasAtmosphericOxygen(uint64_t CelestialBody_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_HasAtmosphericOxygen");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::CelestialBody_get_ReferenceFrame(uint64_t CelestialBody_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_ReferenceFrame");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::CelestialBody_get_NonRotatingReferenceFrame(uint64_t CelestialBody_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_NonRotatingReferenceFrame");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::CelestialBody_get_OrbitalReferenceFrame(uint64_t CelestialBody_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_OrbitalReferenceFrame");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Comms_SignalDelayToVessel(uint64_t Comms_ID, uint64_t other, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Comms_SignalDelayToVessel");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Comms_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(other, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Comms_get_HasFlightComputer(uint64_t Comms_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Comms_get_HasFlightComputer");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Comms_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Comms_get_HasConnection(uint64_t Comms_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Comms_get_HasConnection");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Comms_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Comms_get_HasConnectionToGroundStation(uint64_t Comms_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Comms_get_HasConnectionToGroundStation");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Comms_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Comms_get_SignalDelay(uint64_t Comms_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Comms_get_SignalDelay");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Comms_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Comms_get_SignalDelayToGroundStation(uint64_t Comms_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Comms_get_SignalDelayToGroundStation");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Comms_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Control_ActivateNextStage(uint64_t Control_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_ActivateNextStage");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Control_GetActionGroup(uint64_t Control_ID, uint32_t group, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_GetActionGroup");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_SetActionGroup(uint64_t Control_ID, uint32_t group, bool state)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_SetActionGroup");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument = request.add_arguments();
  argument->set_position(2);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(state, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_ToggleActionGroup(uint64_t Control_ID, uint32_t group)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_ToggleActionGroup");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_AddNode(uint64_t Control_ID, double UT, float prograde, float normal, float radial, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_AddNode");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&UT), sizeof(UT));  

  argument = request.add_arguments();
  argument->set_position(2);
  argument->set_value((const char*)(&prograde), sizeof(prograde));

  argument = request.add_arguments();
  argument->set_position(3);
  argument->set_value((const char*)(&normal), sizeof(normal));

  argument = request.add_arguments();
  argument->set_position(4);
  argument->set_value((const char*)(&radial), sizeof(radial));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Control_RemoveNodes(uint64_t Control_ID)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_RemoveNodes");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_SAS(uint64_t Control_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_SAS");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_set_SAS(uint64_t Control_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_SAS");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_RCS(uint64_t Control_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_RCS");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_set_RCS(uint64_t Control_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_RCS");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_Gear(uint64_t Control_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Gear");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_set_Gear(uint64_t Control_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Gear");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_Lights(uint64_t Control_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Lights");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_set_Lights(uint64_t Control_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Lights");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_Brakes(uint64_t Control_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Brakes");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_set_Brakes(uint64_t Control_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Brakes");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_Abort(uint64_t Control_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Abort");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_set_Abort(uint64_t Control_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Abort");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_Throttle(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Throttle");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Control_set_Throttle(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Throttle");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_Pitch(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Pitch");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Control_set_Pitch(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Pitch");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_Yaw(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Yaw");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Control_set_Yaw(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Yaw");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_Roll(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Roll");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Control_set_Roll(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Roll");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_Forward(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Forward");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Control_set_Forward(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Forward");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_Up(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Up");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Control_set_Up(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Up");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_Right(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Right");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Control_set_Right(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Right");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_WheelThrottle(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_WheelThrottle");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Control_set_WheelThrottle(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_WheelThrottle");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_WheelSteering(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_WheelSteering");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Control_set_WheelSteering(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_WheelSteering");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_CurrentStage(uint64_t Control_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_CurrentStage");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Control_get_Nodes(uint64_t Control_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Nodes");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Flight_get_GForce(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_GForce");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_MeanAltitude(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_MeanAltitude");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_SurfaceAltitude(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_SurfaceAltitude");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_BedrockAltitude(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_BedrockAltitude");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_Elevation(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Elevation");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_Latitude(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Latitude");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_Longitude(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Longitude");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_Velocity(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Velocity");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Flight_get_Speed(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Speed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_HorizontalSpeed(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_HorizontalSpeed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_VerticalSpeed(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_VerticalSpeed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_CenterOfMass(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_CenterOfMass");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Flight_get_Rotation(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Rotation");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Flight_get_Direction(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Direction");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Flight_get_Pitch(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Pitch");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_Heading(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Heading");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_Roll(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Roll");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_Prograde(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Prograde");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Flight_get_Retrograde(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Retrograde");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Flight_get_Normal(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Normal");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Flight_get_AntiNormal(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_AntiNormal");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Flight_get_Radial(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Radial");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Flight_get_AntiRadial(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_AntiRadial");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Flight_get_AtmosphereDensity(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_AtmosphereDensity");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_DynamicPressure(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_DynamicPressure");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_StaticPressure(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_StaticPressure");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_AerodynamicForce(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_AerodynamicForce");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Flight_get_Lift(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Lift");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Flight_get_Drag(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Drag");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Flight_get_SpeedOfSound(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_SpeedOfSound");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_Mach(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Mach");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_EquivalentAirSpeed(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_EquivalentAirSpeed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_TerminalVelocity(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_TerminalVelocity");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_AngleOfAttack(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_AngleOfAttack");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_SideslipAngle(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_SideslipAngle");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_TotalAirTemperature(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_TotalAirTemperature");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_StaticAirTemperature(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_StaticAirTemperature");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_StallFraction(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_StallFraction");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_DragCoefficient(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_DragCoefficient");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_LiftCoefficient(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_LiftCoefficient");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_PitchingMomentCoefficient(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_PitchingMomentCoefficient");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_BallisticCoefficient(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_BallisticCoefficient");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_ThrustSpecificFuelConsumption(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_ThrustSpecificFuelConsumption");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Flight_get_FARStatus(uint64_t Flight_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_FARStatus");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Node_BurnVector(uint64_t Node_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_BurnVector");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Node_RemainingBurnVector(uint64_t Node_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_RemainingBurnVector");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Node_Remove(uint64_t Node_ID)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_Remove");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Node_Position(uint64_t Node_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_Position");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Node_Direction(uint64_t Node_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_Direction");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Node_get_Prograde(uint64_t Node_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_Prograde");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Node_set_Prograde(uint64_t Node_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_set_Prograde");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Node_get_Normal(uint64_t Node_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_Normal");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Node_set_Normal(uint64_t Node_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_set_Normal");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Node_get_Radial(uint64_t Node_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_Radial");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Node_set_Radial(uint64_t Node_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_set_Radial");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Node_get_DeltaV(uint64_t Node_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_DeltaV");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Node_set_DeltaV(uint64_t Node_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_set_DeltaV");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Node_get_RemainingDeltaV(uint64_t Node_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_RemainingDeltaV");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Node_get_UT(uint64_t Node_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_UT");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Node_set_UT(uint64_t Node_ID, double value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_set_UT");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));  

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Node_get_TimeTo(uint64_t Node_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_TimeTo");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Node_get_Orbit(uint64_t Node_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_Orbit");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Node_get_ReferenceFrame(uint64_t Node_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_ReferenceFrame");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Node_get_OrbitalReferenceFrame(uint64_t Node_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_OrbitalReferenceFrame");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_ReferencePlaneNormal(uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_ReferencePlaneNormal");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Orbit_ReferencePlaneDirection(uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_ReferencePlaneDirection");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Orbit_get_Body(uint64_t Orbit_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Body");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_Apoapsis(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Apoapsis");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_Periapsis(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Periapsis");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_ApoapsisAltitude(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_ApoapsisAltitude");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_PeriapsisAltitude(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_PeriapsisAltitude");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_SemiMajorAxis(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_SemiMajorAxis");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_SemiMinorAxis(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_SemiMinorAxis");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_Radius(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Radius");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_Speed(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Speed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_Period(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Period");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_TimeToApoapsis(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_TimeToApoapsis");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_TimeToPeriapsis(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_TimeToPeriapsis");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_Eccentricity(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Eccentricity");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_Inclination(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Inclination");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_LongitudeOfAscendingNode(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_LongitudeOfAscendingNode");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_ArgumentOfPeriapsis(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_ArgumentOfPeriapsis");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_MeanAnomalyAtEpoch(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_MeanAnomalyAtEpoch");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_Epoch(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Epoch");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_MeanAnomaly(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_MeanAnomaly");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_EccentricAnomaly(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_EccentricAnomaly");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_NextOrbit(uint64_t Orbit_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_NextOrbit");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Orbit_get_TimeToSOIChange(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_TimeToSOIChange");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Decoupler_Decouple(uint64_t Decoupler_ID)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Decoupler_Decouple");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Decoupler_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Decoupler_get_Part(uint64_t Decoupler_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Decoupler_get_Part");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Decoupler_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Decoupler_get_Decoupled(uint64_t Decoupler_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Decoupler_get_Decoupled");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Decoupler_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Decoupler_get_Impulse(uint64_t Decoupler_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Decoupler_get_Impulse");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Decoupler_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::DockingPort_Undock(uint64_t DockingPort_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_Undock");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::DockingPort_Position(uint64_t DockingPort_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_Position");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::DockingPort_Direction(uint64_t DockingPort_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_Direction");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::DockingPort_Rotation(uint64_t DockingPort_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_Rotation");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::DockingPort_get_Part(uint64_t DockingPort_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_Part");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::DockingPort_get_Name(uint64_t DockingPort_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_Name");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::DockingPort_set_Name(uint64_t DockingPort_ID, std::string value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_set_Name");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::DockingPort_get_State(uint64_t DockingPort_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_State");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::DockingPort_get_DockedPart(uint64_t DockingPort_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_DockedPart");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::DockingPort_get_ReengageDistance(uint64_t DockingPort_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_ReengageDistance");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::DockingPort_get_HasShield(uint64_t DockingPort_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_HasShield");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::DockingPort_get_Shielded(uint64_t DockingPort_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_Shielded");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::DockingPort_set_Shielded(uint64_t DockingPort_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_set_Shielded");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::DockingPort_get_ReferenceFrame(uint64_t DockingPort_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_ReferenceFrame");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Engine_get_Part(uint64_t Engine_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_Part");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Engine_get_Active(uint64_t Engine_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_Active");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Engine_set_Active(uint64_t Engine_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_set_Active");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Engine_get_Thrust(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_Thrust");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Engine_get_AvailableThrust(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_AvailableThrust");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Engine_get_MaxThrust(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_MaxThrust");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Engine_get_MaxVacuumThrust(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_MaxVacuumThrust");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Engine_get_ThrustLimit(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_ThrustLimit");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Engine_set_ThrustLimit(uint64_t Engine_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_set_ThrustLimit");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Engine_get_SpecificImpulse(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_SpecificImpulse");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Engine_get_VacuumSpecificImpulse(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_VacuumSpecificImpulse");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Engine_get_KerbinSeaLevelSpecificImpulse(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_KerbinSeaLevelSpecificImpulse");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Engine_get_Propellants(uint64_t Engine_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_Propellants");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Engine_get_HasFuel(uint64_t Engine_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_HasFuel");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Engine_get_Throttle(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_Throttle");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Engine_get_ThrottleLocked(uint64_t Engine_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_ThrottleLocked");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Engine_get_CanRestart(uint64_t Engine_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_CanRestart");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Engine_get_CanShutdown(uint64_t Engine_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_CanShutdown");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Engine_get_Gimballed(uint64_t Engine_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_Gimballed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Engine_get_GimbalRange(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_GimbalRange");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Engine_get_GimbalLocked(uint64_t Engine_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_GimbalLocked");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Engine_set_GimbalLocked(uint64_t Engine_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_set_GimbalLocked");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Engine_get_GimbalLimit(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_GimbalLimit");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Engine_set_GimbalLimit(uint64_t Engine_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Engine_set_GimbalLimit");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::LandingGear_get_Part(uint64_t LandingGear_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("LandingGear_get_Part");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LandingGear_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::LandingGear_get_State(uint64_t LandingGear_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("LandingGear_get_State");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LandingGear_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::LandingGear_get_Deployed(uint64_t LandingGear_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("LandingGear_get_Deployed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LandingGear_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::LandingGear_set_Deployed(uint64_t LandingGear_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("LandingGear_set_Deployed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LandingGear_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::LandingLeg_get_Part(uint64_t LandingLeg_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("LandingLeg_get_Part");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LandingLeg_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::LandingLeg_get_State(uint64_t LandingLeg_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("LandingLeg_get_State");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LandingLeg_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::LandingLeg_get_Deployed(uint64_t LandingLeg_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("LandingLeg_get_Deployed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LandingLeg_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::LandingLeg_set_Deployed(uint64_t LandingLeg_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("LandingLeg_set_Deployed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LandingLeg_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::LaunchClamp_Release(uint64_t LaunchClamp_ID)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("LaunchClamp_Release");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LaunchClamp_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::LaunchClamp_get_Part(uint64_t LaunchClamp_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("LaunchClamp_get_Part");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LaunchClamp_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Light_get_Part(uint64_t Light_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Light_get_Part");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Light_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Light_get_Active(uint64_t Light_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Light_get_Active");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Light_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Light_set_Active(uint64_t Light_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Light_set_Active");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Light_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Light_get_PowerUsage(uint64_t Light_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Light_get_PowerUsage");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Light_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Module_HasField(uint64_t Module_ID, std::string name, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Module_HasField");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Module_GetField(uint64_t Module_ID, std::string name, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Module_GetField");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Module_HasEvent(uint64_t Module_ID, std::string name, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Module_HasEvent");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Module_TriggerEvent(uint64_t Module_ID, std::string name)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Module_TriggerEvent");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Module_HasAction(uint64_t Module_ID, std::string name, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Module_HasAction");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Module_SetAction(uint64_t Module_ID, std::string name, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Module_SetAction");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument = request.add_arguments();
  argument->set_position(2);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Module_get_Name(uint64_t Module_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Module_get_Name");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Module_get_Part(uint64_t Module_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Module_get_Part");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Module_get_Fields(uint64_t Module_ID, krpc::Dictionary& return_dict)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Module_get_Fields");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Module_get_Events(uint64_t Module_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Module_get_Events");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Module_get_Actions(uint64_t Module_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Module_get_Actions");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parachute_Deploy(uint64_t Parachute_ID)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_Deploy");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Parachute_get_Part(uint64_t Parachute_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_get_Part");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Parachute_get_Deployed(uint64_t Parachute_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_get_Deployed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Parachute_get_State(uint64_t Parachute_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_get_State");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Parachute_get_DeployAltitude(uint64_t Parachute_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_get_DeployAltitude");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Parachute_set_DeployAltitude(uint64_t Parachute_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_set_DeployAltitude");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Parachute_get_DeployMinPressure(uint64_t Parachute_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_get_DeployMinPressure");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Parachute_set_DeployMinPressure(uint64_t Parachute_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_set_DeployMinPressure");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Part_Position(uint64_t Part_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_Position");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Part_Direction(uint64_t Part_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_Direction");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Part_Velocity(uint64_t Part_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_Velocity");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Part_Rotation(uint64_t Part_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_Rotation");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Part_get_Name(uint64_t Part_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Name");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Part_get_Title(uint64_t Part_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Title");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Part_get_Cost(uint64_t Part_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Cost");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_Vessel(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Vessel");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_Parent(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Parent");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_Children(uint64_t Part_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Children");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Part_get_AxiallyAttached(uint64_t Part_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_AxiallyAttached");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Part_get_RadiallyAttached(uint64_t Part_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_RadiallyAttached");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Part_get_Stage(uint64_t Part_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Stage");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Part_get_DecoupleStage(uint64_t Part_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_DecoupleStage");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Part_get_Massless(uint64_t Part_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Massless");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Part_get_Mass(uint64_t Part_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Mass");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_DryMass(uint64_t Part_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_DryMass");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_ImpactTolerance(uint64_t Part_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_ImpactTolerance");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_Temperature(uint64_t Part_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Temperature");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_MaxTemperature(uint64_t Part_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_MaxTemperature");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_Resources(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Resources");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_Crossfeed(uint64_t Part_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Crossfeed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Part_get_FuelLinesFrom(uint64_t Part_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_FuelLinesFrom");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Part_get_FuelLinesTo(uint64_t Part_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_FuelLinesTo");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Part_get_Modules(uint64_t Part_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Modules");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Part_get_Decoupler(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Decoupler");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_DockingPort(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_DockingPort");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_Engine(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Engine");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_LandingGear(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_LandingGear");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_LandingLeg(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_LandingLeg");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_LaunchClamp(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_LaunchClamp");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_Light(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Light");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_Parachute(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Parachute");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_ReactionWheel(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_ReactionWheel");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_Sensor(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Sensor");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_SolarPanel(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_SolarPanel");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Part_get_ReferenceFrame(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_ReferenceFrame");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Parts_WithName(uint64_t Parts_ID, std::string name, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_WithName");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_WithTitle(uint64_t Parts_ID, std::string title, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_WithTitle");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_WithModule(uint64_t Parts_ID, std::string moduleName, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_WithModule");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_InStage(uint64_t Parts_ID, int32_t stage, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_InStage");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_InDecoupleStage(uint64_t Parts_ID, int32_t stage, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_InDecoupleStage");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_ModulesWithName(uint64_t Parts_ID, std::string moduleName, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_ModulesWithName");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_DockingPortWithName(uint64_t Parts_ID, std::string name, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_DockingPortWithName");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Parts_get_All(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_All");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_get_Root(uint64_t Parts_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_Root");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Parts_get_Controlling(uint64_t Parts_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_Controlling");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Parts_set_Controlling(uint64_t Parts_ID, uint64_t value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_set_Controlling");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Parts_get_Decouplers(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_Decouplers");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_get_DockingPorts(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_DockingPorts");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_get_Engines(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_Engines");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_get_LandingGear(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_LandingGear");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_get_LandingLegs(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_LandingLegs");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_get_LaunchClamps(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_LaunchClamps");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_get_Lights(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_Lights");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_get_Parachutes(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_Parachutes");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_get_ReactionWheels(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_ReactionWheels");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_get_Sensors(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_Sensors");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::Parts_get_SolarPanels(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_SolarPanels");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::PartResources_HasResource(uint64_t PartResources_ID, std::string name, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("PartResources_HasResource");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(PartResources_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::PartResources_Max(uint64_t PartResources_ID, std::string name, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("PartResources_Max");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(PartResources_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::PartResources_Amount(uint64_t PartResources_ID, std::string name, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("PartResources_Amount");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(PartResources_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::PartResources_get_Names(uint64_t PartResources_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("PartResources_get_Names");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(PartResources_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::ReactionWheel_get_Part(uint64_t ReactionWheel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("ReactionWheel_get_Part");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ReactionWheel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::ReactionWheel_get_Active(uint64_t ReactionWheel_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("ReactionWheel_get_Active");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ReactionWheel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::ReactionWheel_set_Active(uint64_t ReactionWheel_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("ReactionWheel_set_Active");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ReactionWheel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::ReactionWheel_get_Broken(uint64_t ReactionWheel_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("ReactionWheel_get_Broken");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ReactionWheel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::ReactionWheel_get_PitchTorque(uint64_t ReactionWheel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("ReactionWheel_get_PitchTorque");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ReactionWheel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::ReactionWheel_get_YawTorque(uint64_t ReactionWheel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("ReactionWheel_get_YawTorque");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ReactionWheel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::ReactionWheel_get_RollTorque(uint64_t ReactionWheel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("ReactionWheel_get_RollTorque");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ReactionWheel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Sensor_get_Part(uint64_t Sensor_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Sensor_get_Part");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Sensor_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Sensor_get_Active(uint64_t Sensor_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Sensor_get_Active");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Sensor_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Sensor_set_Active(uint64_t Sensor_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Sensor_set_Active");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Sensor_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Sensor_get_Value(uint64_t Sensor_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Sensor_get_Value");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Sensor_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Sensor_get_PowerUsage(uint64_t Sensor_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Sensor_get_PowerUsage");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Sensor_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::SolarPanel_get_Part(uint64_t SolarPanel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("SolarPanel_get_Part");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(SolarPanel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::SolarPanel_get_Deployed(uint64_t SolarPanel_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("SolarPanel_get_Deployed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(SolarPanel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::SolarPanel_set_Deployed(uint64_t SolarPanel_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("SolarPanel_set_Deployed");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(SolarPanel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::SolarPanel_get_State(uint64_t SolarPanel_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("SolarPanel_get_State");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(SolarPanel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::SolarPanel_get_EnergyFlow(uint64_t SolarPanel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("SolarPanel_get_EnergyFlow");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(SolarPanel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::SolarPanel_get_SunExposure(uint64_t SolarPanel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("SolarPanel_get_SunExposure");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(SolarPanel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_Flight(uint64_t Vessel_ID, uint64_t referenceFrame, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_Flight");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_Position(uint64_t Vessel_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_Position");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Vessel_Velocity(uint64_t Vessel_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_Velocity");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Vessel_Rotation(uint64_t Vessel_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_Rotation");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Vessel_Direction(uint64_t Vessel_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_Direction");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Vessel_AngularVelocity(uint64_t Vessel_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_AngularVelocity");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::Tuple tuple;
      tuple.ParseFromString(response.return_value());
      KRPCI::DecodeTuple(tuple, x, y, z);
    }
  return true;
}

bool KRPCI::Vessel_get_Name(uint64_t Vessel_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Name");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Vessel_set_Name(uint64_t Vessel_ID, std::string value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_set_Name");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Vessel_get_Type(uint64_t Vessel_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Type");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Vessel_set_Type(uint64_t Vessel_ID, int32_t value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_set_Type");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Vessel_get_Situation(uint64_t Vessel_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Situation");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Vessel_get_MET(uint64_t Vessel_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_MET");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_Target(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Target");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_set_Target(uint64_t Vessel_ID, uint64_t value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_set_Target");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::Vessel_get_Orbit(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Orbit");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_Control(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Control");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_AutoPilot(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_AutoPilot");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_Resources(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Resources");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_Parts(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Parts");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_Comms(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Comms");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_Mass(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Mass");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_DryMass(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_DryMass");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_Thrust(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Thrust");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_AvailableThrust(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_AvailableThrust");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_MaxThrust(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_MaxThrust");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_MaxVacuumThrust(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_MaxVacuumThrust");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_SpecificImpulse(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_SpecificImpulse");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_VacuumSpecificImpulse(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_VacuumSpecificImpulse");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_KerbinSeaLevelSpecificImpulse(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_KerbinSeaLevelSpecificImpulse");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_ReferenceFrame(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_ReferenceFrame");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_OrbitalReferenceFrame(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_OrbitalReferenceFrame");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_SurfaceReferenceFrame(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_SurfaceReferenceFrame");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::Vessel_get_SurfaceVelocityReferenceFrame(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_SurfaceVelocityReferenceFrame");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      KRPCI::DecodeVarint(return_value, 
			  (char *)response.return_value().data(), 
			  response.return_value().size());
    }
  return true;
}

bool KRPCI::VesselResources_HasResource(uint64_t VesselResources_ID, std::string name, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("VesselResources_HasResource");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(VesselResources_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::VesselResources_Max(uint64_t VesselResources_ID, std::string name, int32_t stage, bool cumulative, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("VesselResources_Max");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(VesselResources_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument = request.add_arguments();
  argument->set_position(2);
  argument = request.add_arguments();
  argument->set_position(3);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(cumulative, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::VesselResources_Amount(uint64_t VesselResources_ID, std::string name, int32_t stage, bool cumulative, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("VesselResources_Amount");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(VesselResources_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument = request.add_arguments();
  argument->set_position(2);
  argument = request.add_arguments();
  argument->set_position(3);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(cumulative, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      return_value = 0.0;
      memcpy(&return_value, response.return_value().data(), response.return_value().size());
    }
  return true;
}

bool KRPCI::VesselResources_get_Names(uint64_t VesselResources_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("SpaceCenter");
  request.set_procedure("VesselResources_get_Names");

  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(VesselResources_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
      krpc::List output_list;
      output_list.ParseFromString(response.return_value());
      for(int i=0; i< output_list.items_size(); i++)
	{
	  uint64_t return_value;
	  KRPCI::DecodeVarint(return_value,
			      (char *)output_list.items(i).data(),
			      output_list.items(i).size());
	  return_vector.push_back(return_value);
	}
    }
  return true;
}

bool KRPCI::GetStatus(krpc::Status& return_value)
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("KRPC");
  request.set_procedure("GetStatus");

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::GetServices(krpc::Services& return_value)
{
  krpc::Request request;
  krpc::Response response;

  request.set_service("KRPC");
  request.set_procedure("GetServices");

  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::AddStream(krpc::Request input_request, uint32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("KRPC");
  request.set_procedure("AddStream");

  argument = request.add_arguments();
  argument->set_position(0);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

bool KRPCI::RemoveStream(uint32_t id)
{
  krpc::Request request;
  krpc::Response response;
  krpc::Argument* argument;

  request.set_service("KRPC");
  request.set_procedure("RemoveStream");

  argument = request.add_arguments();
  argument->set_position(0);
  if (getResponseFromRequest(request,response))
    {
      if (response.has_error())
	{
	  std::cout << "Response error: " << response.error() << endl;
	  return false;
	}
    }
  return true;
}

