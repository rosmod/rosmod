#include "krpci/krpci.hpp"
using namespace std;

bool KRPCI::ClearTarget_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("ClearTarget");
  return true;
}

bool KRPCI::ClearTarget()
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ClearTarget_createRequest(request);

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

bool KRPCI::WarpTo_createRequest(double UT, float maxRate, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("WarpTo");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->set_value((const char*)(&UT), sizeof(UT));  

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&maxRate), sizeof(maxRate));

  return true;
}

bool KRPCI::WarpTo(double UT, float maxRate)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::WarpTo_createRequest(UT, maxRate, request);

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

bool KRPCI::TransformPosition_createRequest(double position_x, double position_y, double position_z, uint64_t from, uint64_t to, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("TransformPosition");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::TransformPosition(double position_x, double position_y, double position_z, uint64_t from, uint64_t to, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::TransformPosition_createRequest(position_x, position_y, position_z, from, to, request);

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

bool KRPCI::TransformDirection_createRequest(double direction_x, double direction_y, double direction_z, uint64_t from, uint64_t to, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("TransformDirection");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::TransformDirection(double direction_x, double direction_y, double direction_z, uint64_t from, uint64_t to, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::TransformDirection_createRequest(direction_x, direction_y, direction_z, from, to, request);

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

bool KRPCI::TransformRotation_createRequest(double rotation_x, double rotation_y, double rotation_z, uint64_t from, uint64_t to, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("TransformRotation");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::TransformRotation(double rotation_x, double rotation_y, double rotation_z, uint64_t from, uint64_t to, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::TransformRotation_createRequest(rotation_x, rotation_y, rotation_z, from, to, request);

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

bool KRPCI::TransformVelocity_createRequest(double position_x, double position_y, double position_z, double velocity_x, double velocity_y, double velocity_z, uint64_t from, uint64_t to, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("TransformVelocity");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::TransformVelocity(double position_x, double position_y, double position_z, double velocity_x, double velocity_y, double velocity_z, uint64_t from, uint64_t to, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::TransformVelocity_createRequest(position_x, position_y, position_z, velocity_x, velocity_y, velocity_z, from, to, request);

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

bool KRPCI::DrawDirection_createRequest(double direction_x, double direction_y, double direction_z, uint64_t referenceFrame, double color_x, double color_y, double color_z, float length, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("DrawDirection");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::DrawDirection(double direction_x, double direction_y, double direction_z, uint64_t referenceFrame, double color_x, double color_y, double color_z, float length)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::DrawDirection_createRequest(direction_x, direction_y, direction_z, referenceFrame, color_x, color_y, color_z, length, request);

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

bool KRPCI::ClearDirections_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("ClearDirections");
  return true;
}

bool KRPCI::ClearDirections()
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ClearDirections_createRequest(request);

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

bool KRPCI::get_ActiveVessel_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("get_ActiveVessel");
  return true;
}

bool KRPCI::get_ActiveVessel(uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_ActiveVessel_createRequest(request);

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

bool KRPCI::get_Vessels_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("get_Vessels");
  return true;
}

bool KRPCI::get_Vessels(std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_Vessels_createRequest(request);

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

bool KRPCI::get_Bodies_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("get_Bodies");
  return true;
}

bool KRPCI::get_Bodies(krpc::Dictionary& return_dict)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_Bodies_createRequest(request);

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

bool KRPCI::get_TargetBody_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("get_TargetBody");
  return true;
}

bool KRPCI::get_TargetBody(uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_TargetBody_createRequest(request);

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

bool KRPCI::set_TargetBody_createRequest(uint64_t value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("set_TargetBody");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::set_TargetBody(uint64_t value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::set_TargetBody_createRequest(value, request);

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

bool KRPCI::get_TargetVessel_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("get_TargetVessel");
  return true;
}

bool KRPCI::get_TargetVessel(uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_TargetVessel_createRequest(request);

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

bool KRPCI::set_TargetVessel_createRequest(uint64_t value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("set_TargetVessel");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::set_TargetVessel(uint64_t value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::set_TargetVessel_createRequest(value, request);

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

bool KRPCI::get_TargetDockingPort_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("get_TargetDockingPort");
  return true;
}

bool KRPCI::get_TargetDockingPort(uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_TargetDockingPort_createRequest(request);

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

bool KRPCI::set_TargetDockingPort_createRequest(uint64_t value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("set_TargetDockingPort");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::set_TargetDockingPort(uint64_t value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::set_TargetDockingPort_createRequest(value, request);

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

bool KRPCI::get_UT_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("get_UT");
  return true;
}

bool KRPCI::get_UT(double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_UT_createRequest(request);

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

bool KRPCI::get_G_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("get_G");
  return true;
}

bool KRPCI::get_G(float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_G_createRequest(request);

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

bool KRPCI::get_RailsWarpFactor_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("get_RailsWarpFactor");
  return true;
}

bool KRPCI::get_RailsWarpFactor(int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_RailsWarpFactor_createRequest(request);

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

bool KRPCI::set_RailsWarpFactor_createRequest(int32_t value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("set_RailsWarpFactor");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  return true;
}

bool KRPCI::set_RailsWarpFactor(int32_t value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::set_RailsWarpFactor_createRequest(value, request);

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

bool KRPCI::get_PhysicsWarpFactor_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("get_PhysicsWarpFactor");
  return true;
}

bool KRPCI::get_PhysicsWarpFactor(int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_PhysicsWarpFactor_createRequest(request);

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

bool KRPCI::set_PhysicsWarpFactor_createRequest(int32_t value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("set_PhysicsWarpFactor");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  return true;
}

bool KRPCI::set_PhysicsWarpFactor(int32_t value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::set_PhysicsWarpFactor_createRequest(value, request);

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

bool KRPCI::get_WarpMode_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("get_WarpMode");
  return true;
}

bool KRPCI::get_WarpMode(int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_WarpMode_createRequest(request);

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

bool KRPCI::get_WarpRate_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("get_WarpRate");
  return true;
}

bool KRPCI::get_WarpRate(float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_WarpRate_createRequest(request);

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

bool KRPCI::get_FARAvailable_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("get_FARAvailable");
  return true;
}

bool KRPCI::get_FARAvailable(bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_FARAvailable_createRequest(request);

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

bool KRPCI::get_RemoteTechAvailable_createRequest(krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("get_RemoteTechAvailable");
  return true;
}

bool KRPCI::get_RemoteTechAvailable(bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_RemoteTechAvailable_createRequest(request);

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

bool KRPCI::AutoPilot_SetRotation_createRequest(uint64_t AutoPilot_ID, float pitch, float heading, float roll, uint64_t referenceFrame, bool wait, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_SetRotation");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::AutoPilot_SetRotation(uint64_t AutoPilot_ID, float pitch, float heading, float roll, uint64_t referenceFrame, bool wait)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::AutoPilot_SetRotation_createRequest(AutoPilot_ID, pitch, heading, roll, referenceFrame, wait, request);

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

bool KRPCI::AutoPilot_SetDirection_createRequest(uint64_t AutoPilot_ID, double direction_x, double direction_y, double direction_z, float roll, uint64_t referenceFrame, bool wait, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_SetDirection");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::AutoPilot_SetDirection(uint64_t AutoPilot_ID, double direction_x, double direction_y, double direction_z, float roll, uint64_t referenceFrame, bool wait)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::AutoPilot_SetDirection_createRequest(AutoPilot_ID, direction_x, direction_y, direction_z, roll, referenceFrame, wait, request);

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

bool KRPCI::AutoPilot_Disengage_createRequest(uint64_t AutoPilot_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_Disengage");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::AutoPilot_Disengage(uint64_t AutoPilot_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::AutoPilot_Disengage_createRequest(AutoPilot_ID, request);

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

bool KRPCI::AutoPilot_get_SAS_createRequest(uint64_t AutoPilot_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_get_SAS");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::AutoPilot_get_SAS(uint64_t AutoPilot_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::AutoPilot_get_SAS_createRequest(AutoPilot_ID, request);

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

bool KRPCI::AutoPilot_set_SAS_createRequest(uint64_t AutoPilot_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_set_SAS");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::AutoPilot_set_SAS(uint64_t AutoPilot_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::AutoPilot_set_SAS_createRequest(AutoPilot_ID, value, request);

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

bool KRPCI::AutoPilot_get_SASMode_createRequest(uint64_t AutoPilot_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_get_SASMode");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::AutoPilot_get_SASMode(uint64_t AutoPilot_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::AutoPilot_get_SASMode_createRequest(AutoPilot_ID, request);

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

bool KRPCI::AutoPilot_set_SASMode_createRequest(uint64_t AutoPilot_ID, int32_t value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_set_SASMode");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::AutoPilot_set_SASMode(uint64_t AutoPilot_ID, int32_t value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::AutoPilot_set_SASMode_createRequest(AutoPilot_ID, value, request);

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

bool KRPCI::AutoPilot_get_SpeedMode_createRequest(uint64_t AutoPilot_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_get_SpeedMode");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::AutoPilot_get_SpeedMode(uint64_t AutoPilot_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::AutoPilot_get_SpeedMode_createRequest(AutoPilot_ID, request);

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

bool KRPCI::AutoPilot_set_SpeedMode_createRequest(uint64_t AutoPilot_ID, int32_t value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_set_SpeedMode");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::AutoPilot_set_SpeedMode(uint64_t AutoPilot_ID, int32_t value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::AutoPilot_set_SpeedMode_createRequest(AutoPilot_ID, value, request);

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

bool KRPCI::AutoPilot_get_Error_createRequest(uint64_t AutoPilot_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_get_Error");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::AutoPilot_get_Error(uint64_t AutoPilot_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::AutoPilot_get_Error_createRequest(AutoPilot_ID, request);

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

bool KRPCI::AutoPilot_get_RollError_createRequest(uint64_t AutoPilot_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("AutoPilot_get_RollError");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(AutoPilot_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::AutoPilot_get_RollError(uint64_t AutoPilot_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::AutoPilot_get_RollError_createRequest(AutoPilot_ID, request);

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

bool KRPCI::CelestialBody_Position_createRequest(uint64_t CelestialBody_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_Position");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::CelestialBody_Position(uint64_t CelestialBody_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_Position_createRequest(CelestialBody_ID, referenceFrame, request);

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

bool KRPCI::CelestialBody_Velocity_createRequest(uint64_t CelestialBody_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_Velocity");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::CelestialBody_Velocity(uint64_t CelestialBody_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_Velocity_createRequest(CelestialBody_ID, referenceFrame, request);

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

bool KRPCI::CelestialBody_Rotation_createRequest(uint64_t CelestialBody_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_Rotation");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::CelestialBody_Rotation(uint64_t CelestialBody_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_Rotation_createRequest(CelestialBody_ID, referenceFrame, request);

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

bool KRPCI::CelestialBody_Direction_createRequest(uint64_t CelestialBody_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_Direction");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::CelestialBody_Direction(uint64_t CelestialBody_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_Direction_createRequest(CelestialBody_ID, referenceFrame, request);

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

bool KRPCI::CelestialBody_AngularVelocity_createRequest(uint64_t CelestialBody_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_AngularVelocity");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::CelestialBody_AngularVelocity(uint64_t CelestialBody_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_AngularVelocity_createRequest(CelestialBody_ID, referenceFrame, request);

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

bool KRPCI::CelestialBody_get_Name_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_Name");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_Name(uint64_t CelestialBody_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_Name_createRequest(CelestialBody_ID, request);

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

bool KRPCI::CelestialBody_get_Satellites_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_Satellites");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_Satellites(uint64_t CelestialBody_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_Satellites_createRequest(CelestialBody_ID, request);

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

bool KRPCI::CelestialBody_get_Mass_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_Mass");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_Mass(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_Mass_createRequest(CelestialBody_ID, request);

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

bool KRPCI::CelestialBody_get_GravitationalParameter_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_GravitationalParameter");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_GravitationalParameter(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_GravitationalParameter_createRequest(CelestialBody_ID, request);

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

bool KRPCI::CelestialBody_get_SurfaceGravity_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_SurfaceGravity");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_SurfaceGravity(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_SurfaceGravity_createRequest(CelestialBody_ID, request);

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

bool KRPCI::CelestialBody_get_RotationalPeriod_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_RotationalPeriod");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_RotationalPeriod(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_RotationalPeriod_createRequest(CelestialBody_ID, request);

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

bool KRPCI::CelestialBody_get_RotationalSpeed_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_RotationalSpeed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_RotationalSpeed(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_RotationalSpeed_createRequest(CelestialBody_ID, request);

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

bool KRPCI::CelestialBody_get_EquatorialRadius_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_EquatorialRadius");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_EquatorialRadius(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_EquatorialRadius_createRequest(CelestialBody_ID, request);

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

bool KRPCI::CelestialBody_get_SphereOfInfluence_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_SphereOfInfluence");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_SphereOfInfluence(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_SphereOfInfluence_createRequest(CelestialBody_ID, request);

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

bool KRPCI::CelestialBody_get_Orbit_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_Orbit");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_Orbit(uint64_t CelestialBody_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_Orbit_createRequest(CelestialBody_ID, request);

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

bool KRPCI::CelestialBody_get_HasAtmosphere_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_HasAtmosphere");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_HasAtmosphere(uint64_t CelestialBody_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_HasAtmosphere_createRequest(CelestialBody_ID, request);

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

bool KRPCI::CelestialBody_get_AtmosphereDepth_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_AtmosphereDepth");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_AtmosphereDepth(uint64_t CelestialBody_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_AtmosphereDepth_createRequest(CelestialBody_ID, request);

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

bool KRPCI::CelestialBody_get_HasAtmosphericOxygen_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_HasAtmosphericOxygen");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_HasAtmosphericOxygen(uint64_t CelestialBody_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_HasAtmosphericOxygen_createRequest(CelestialBody_ID, request);

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

bool KRPCI::CelestialBody_get_ReferenceFrame_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_ReferenceFrame");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_ReferenceFrame(uint64_t CelestialBody_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_ReferenceFrame_createRequest(CelestialBody_ID, request);

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

bool KRPCI::CelestialBody_get_NonRotatingReferenceFrame_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_NonRotatingReferenceFrame");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_NonRotatingReferenceFrame(uint64_t CelestialBody_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_NonRotatingReferenceFrame_createRequest(CelestialBody_ID, request);

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

bool KRPCI::CelestialBody_get_OrbitalReferenceFrame_createRequest(uint64_t CelestialBody_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("CelestialBody_get_OrbitalReferenceFrame");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(CelestialBody_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::CelestialBody_get_OrbitalReferenceFrame(uint64_t CelestialBody_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CelestialBody_get_OrbitalReferenceFrame_createRequest(CelestialBody_ID, request);

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

bool KRPCI::Comms_SignalDelayToVessel_createRequest(uint64_t Comms_ID, uint64_t other, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Comms_SignalDelayToVessel");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Comms_SignalDelayToVessel(uint64_t Comms_ID, uint64_t other, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Comms_SignalDelayToVessel_createRequest(Comms_ID, other, request);

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

bool KRPCI::Comms_get_HasFlightComputer_createRequest(uint64_t Comms_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Comms_get_HasFlightComputer");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Comms_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Comms_get_HasFlightComputer(uint64_t Comms_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Comms_get_HasFlightComputer_createRequest(Comms_ID, request);

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

bool KRPCI::Comms_get_HasConnection_createRequest(uint64_t Comms_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Comms_get_HasConnection");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Comms_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Comms_get_HasConnection(uint64_t Comms_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Comms_get_HasConnection_createRequest(Comms_ID, request);

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

bool KRPCI::Comms_get_HasConnectionToGroundStation_createRequest(uint64_t Comms_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Comms_get_HasConnectionToGroundStation");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Comms_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Comms_get_HasConnectionToGroundStation(uint64_t Comms_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Comms_get_HasConnectionToGroundStation_createRequest(Comms_ID, request);

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

bool KRPCI::Comms_get_SignalDelay_createRequest(uint64_t Comms_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Comms_get_SignalDelay");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Comms_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Comms_get_SignalDelay(uint64_t Comms_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Comms_get_SignalDelay_createRequest(Comms_ID, request);

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

bool KRPCI::Comms_get_SignalDelayToGroundStation_createRequest(uint64_t Comms_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Comms_get_SignalDelayToGroundStation");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Comms_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Comms_get_SignalDelayToGroundStation(uint64_t Comms_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Comms_get_SignalDelayToGroundStation_createRequest(Comms_ID, request);

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

bool KRPCI::Control_ActivateNextStage_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_ActivateNextStage");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_ActivateNextStage(uint64_t Control_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_ActivateNextStage_createRequest(Control_ID, request);

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

bool KRPCI::Control_GetActionGroup_createRequest(uint64_t Control_ID, uint32_t group, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_GetActionGroup");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Control_GetActionGroup(uint64_t Control_ID, uint32_t group, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_GetActionGroup_createRequest(Control_ID, group, request);

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

bool KRPCI::Control_SetActionGroup_createRequest(uint64_t Control_ID, uint32_t group, bool state, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_SetActionGroup");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Control_SetActionGroup(uint64_t Control_ID, uint32_t group, bool state)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_SetActionGroup_createRequest(Control_ID, group, state, request);

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

bool KRPCI::Control_ToggleActionGroup_createRequest(uint64_t Control_ID, uint32_t group, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_ToggleActionGroup");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Control_ToggleActionGroup(uint64_t Control_ID, uint32_t group)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_ToggleActionGroup_createRequest(Control_ID, group, request);

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

bool KRPCI::Control_AddNode_createRequest(uint64_t Control_ID, double UT, float prograde, float normal, float radial, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_AddNode");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Control_AddNode(uint64_t Control_ID, double UT, float prograde, float normal, float radial, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_AddNode_createRequest(Control_ID, UT, prograde, normal, radial, request);

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

bool KRPCI::Control_RemoveNodes_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_RemoveNodes");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_RemoveNodes(uint64_t Control_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_RemoveNodes_createRequest(Control_ID, request);

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

bool KRPCI::Control_get_SAS_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_SAS");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_SAS(uint64_t Control_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_SAS_createRequest(Control_ID, request);

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

bool KRPCI::Control_set_SAS_createRequest(uint64_t Control_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_SAS");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Control_set_SAS(uint64_t Control_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_set_SAS_createRequest(Control_ID, value, request);

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

bool KRPCI::Control_get_RCS_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_RCS");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_RCS(uint64_t Control_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_RCS_createRequest(Control_ID, request);

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

bool KRPCI::Control_set_RCS_createRequest(uint64_t Control_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_RCS");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Control_set_RCS(uint64_t Control_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_set_RCS_createRequest(Control_ID, value, request);

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

bool KRPCI::Control_get_Gear_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Gear");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_Gear(uint64_t Control_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_Gear_createRequest(Control_ID, request);

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

bool KRPCI::Control_set_Gear_createRequest(uint64_t Control_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Gear");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Control_set_Gear(uint64_t Control_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_set_Gear_createRequest(Control_ID, value, request);

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

bool KRPCI::Control_get_Lights_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Lights");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_Lights(uint64_t Control_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_Lights_createRequest(Control_ID, request);

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

bool KRPCI::Control_set_Lights_createRequest(uint64_t Control_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Lights");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Control_set_Lights(uint64_t Control_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_set_Lights_createRequest(Control_ID, value, request);

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

bool KRPCI::Control_get_Brakes_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Brakes");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_Brakes(uint64_t Control_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_Brakes_createRequest(Control_ID, request);

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

bool KRPCI::Control_set_Brakes_createRequest(uint64_t Control_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Brakes");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Control_set_Brakes(uint64_t Control_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_set_Brakes_createRequest(Control_ID, value, request);

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

bool KRPCI::Control_get_Abort_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Abort");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_Abort(uint64_t Control_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_Abort_createRequest(Control_ID, request);

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

bool KRPCI::Control_set_Abort_createRequest(uint64_t Control_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Abort");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Control_set_Abort(uint64_t Control_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_set_Abort_createRequest(Control_ID, value, request);

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

bool KRPCI::Control_get_Throttle_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Throttle");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_Throttle(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_Throttle_createRequest(Control_ID, request);

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

bool KRPCI::Control_set_Throttle_createRequest(uint64_t Control_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Throttle");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Control_set_Throttle(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_set_Throttle_createRequest(Control_ID, value, request);

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

bool KRPCI::Control_get_Pitch_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Pitch");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_Pitch(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_Pitch_createRequest(Control_ID, request);

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

bool KRPCI::Control_set_Pitch_createRequest(uint64_t Control_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Pitch");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Control_set_Pitch(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_set_Pitch_createRequest(Control_ID, value, request);

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

bool KRPCI::Control_get_Yaw_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Yaw");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_Yaw(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_Yaw_createRequest(Control_ID, request);

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

bool KRPCI::Control_set_Yaw_createRequest(uint64_t Control_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Yaw");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Control_set_Yaw(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_set_Yaw_createRequest(Control_ID, value, request);

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

bool KRPCI::Control_get_Roll_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Roll");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_Roll(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_Roll_createRequest(Control_ID, request);

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

bool KRPCI::Control_set_Roll_createRequest(uint64_t Control_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Roll");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Control_set_Roll(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_set_Roll_createRequest(Control_ID, value, request);

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

bool KRPCI::Control_get_Forward_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Forward");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_Forward(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_Forward_createRequest(Control_ID, request);

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

bool KRPCI::Control_set_Forward_createRequest(uint64_t Control_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Forward");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Control_set_Forward(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_set_Forward_createRequest(Control_ID, value, request);

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

bool KRPCI::Control_get_Up_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Up");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_Up(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_Up_createRequest(Control_ID, request);

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

bool KRPCI::Control_set_Up_createRequest(uint64_t Control_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Up");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Control_set_Up(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_set_Up_createRequest(Control_ID, value, request);

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

bool KRPCI::Control_get_Right_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Right");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_Right(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_Right_createRequest(Control_ID, request);

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

bool KRPCI::Control_set_Right_createRequest(uint64_t Control_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_Right");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Control_set_Right(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_set_Right_createRequest(Control_ID, value, request);

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

bool KRPCI::Control_get_WheelThrottle_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_WheelThrottle");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_WheelThrottle(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_WheelThrottle_createRequest(Control_ID, request);

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

bool KRPCI::Control_set_WheelThrottle_createRequest(uint64_t Control_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_WheelThrottle");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Control_set_WheelThrottle(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_set_WheelThrottle_createRequest(Control_ID, value, request);

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

bool KRPCI::Control_get_WheelSteering_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_WheelSteering");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_WheelSteering(uint64_t Control_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_WheelSteering_createRequest(Control_ID, request);

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

bool KRPCI::Control_set_WheelSteering_createRequest(uint64_t Control_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_set_WheelSteering");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Control_set_WheelSteering(uint64_t Control_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_set_WheelSteering_createRequest(Control_ID, value, request);

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

bool KRPCI::Control_get_CurrentStage_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_CurrentStage");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_CurrentStage(uint64_t Control_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_CurrentStage_createRequest(Control_ID, request);

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

bool KRPCI::Control_get_Nodes_createRequest(uint64_t Control_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Control_get_Nodes");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Control_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Control_get_Nodes(uint64_t Control_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Control_get_Nodes_createRequest(Control_ID, request);

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

bool KRPCI::Flight_get_GForce_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_GForce");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_GForce(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_GForce_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_MeanAltitude_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_MeanAltitude");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_MeanAltitude(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_MeanAltitude_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_SurfaceAltitude_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_SurfaceAltitude");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_SurfaceAltitude(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_SurfaceAltitude_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_BedrockAltitude_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_BedrockAltitude");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_BedrockAltitude(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_BedrockAltitude_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Elevation_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Elevation");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Elevation(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Elevation_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Latitude_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Latitude");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Latitude(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Latitude_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Longitude_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Longitude");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Longitude(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Longitude_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Velocity_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Velocity");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Velocity(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Velocity_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Speed_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Speed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Speed(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Speed_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_HorizontalSpeed_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_HorizontalSpeed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_HorizontalSpeed(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_HorizontalSpeed_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_VerticalSpeed_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_VerticalSpeed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_VerticalSpeed(uint64_t Flight_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_VerticalSpeed_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_CenterOfMass_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_CenterOfMass");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_CenterOfMass(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_CenterOfMass_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Rotation_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Rotation");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Rotation(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Rotation_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Direction_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Direction");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Direction(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Direction_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Pitch_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Pitch");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Pitch(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Pitch_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Heading_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Heading");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Heading(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Heading_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Roll_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Roll");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Roll(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Roll_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Prograde_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Prograde");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Prograde(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Prograde_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Retrograde_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Retrograde");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Retrograde(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Retrograde_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Normal_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Normal");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Normal(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Normal_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_AntiNormal_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_AntiNormal");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_AntiNormal(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_AntiNormal_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Radial_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Radial");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Radial(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Radial_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_AntiRadial_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_AntiRadial");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_AntiRadial(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_AntiRadial_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_AtmosphereDensity_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_AtmosphereDensity");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_AtmosphereDensity(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_AtmosphereDensity_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_DynamicPressure_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_DynamicPressure");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_DynamicPressure(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_DynamicPressure_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_StaticPressure_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_StaticPressure");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_StaticPressure(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_StaticPressure_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_AerodynamicForce_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_AerodynamicForce");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_AerodynamicForce(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_AerodynamicForce_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Lift_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Lift");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Lift(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Lift_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Drag_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Drag");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Drag(uint64_t Flight_ID, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Drag_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_SpeedOfSound_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_SpeedOfSound");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_SpeedOfSound(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_SpeedOfSound_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_Mach_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_Mach");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_Mach(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_Mach_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_EquivalentAirSpeed_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_EquivalentAirSpeed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_EquivalentAirSpeed(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_EquivalentAirSpeed_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_TerminalVelocity_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_TerminalVelocity");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_TerminalVelocity(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_TerminalVelocity_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_AngleOfAttack_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_AngleOfAttack");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_AngleOfAttack(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_AngleOfAttack_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_SideslipAngle_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_SideslipAngle");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_SideslipAngle(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_SideslipAngle_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_TotalAirTemperature_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_TotalAirTemperature");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_TotalAirTemperature(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_TotalAirTemperature_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_StaticAirTemperature_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_StaticAirTemperature");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_StaticAirTemperature(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_StaticAirTemperature_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_StallFraction_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_StallFraction");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_StallFraction(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_StallFraction_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_DragCoefficient_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_DragCoefficient");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_DragCoefficient(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_DragCoefficient_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_LiftCoefficient_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_LiftCoefficient");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_LiftCoefficient(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_LiftCoefficient_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_PitchingMomentCoefficient_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_PitchingMomentCoefficient");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_PitchingMomentCoefficient(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_PitchingMomentCoefficient_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_BallisticCoefficient_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_BallisticCoefficient");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_BallisticCoefficient(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_BallisticCoefficient_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_ThrustSpecificFuelConsumption_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_ThrustSpecificFuelConsumption");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_ThrustSpecificFuelConsumption(uint64_t Flight_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_ThrustSpecificFuelConsumption_createRequest(Flight_ID, request);

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

bool KRPCI::Flight_get_FARStatus_createRequest(uint64_t Flight_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Flight_get_FARStatus");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Flight_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Flight_get_FARStatus(uint64_t Flight_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Flight_get_FARStatus_createRequest(Flight_ID, request);

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

bool KRPCI::Node_BurnVector_createRequest(uint64_t Node_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_BurnVector");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Node_BurnVector(uint64_t Node_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_BurnVector_createRequest(Node_ID, referenceFrame, request);

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

bool KRPCI::Node_RemainingBurnVector_createRequest(uint64_t Node_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_RemainingBurnVector");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Node_RemainingBurnVector(uint64_t Node_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_RemainingBurnVector_createRequest(Node_ID, referenceFrame, request);

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

bool KRPCI::Node_Remove_createRequest(uint64_t Node_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_Remove");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Node_Remove(uint64_t Node_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_Remove_createRequest(Node_ID, request);

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

bool KRPCI::Node_Position_createRequest(uint64_t Node_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_Position");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Node_Position(uint64_t Node_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_Position_createRequest(Node_ID, referenceFrame, request);

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

bool KRPCI::Node_Direction_createRequest(uint64_t Node_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_Direction");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Node_Direction(uint64_t Node_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_Direction_createRequest(Node_ID, referenceFrame, request);

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

bool KRPCI::Node_get_Prograde_createRequest(uint64_t Node_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_Prograde");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Node_get_Prograde(uint64_t Node_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_get_Prograde_createRequest(Node_ID, request);

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

bool KRPCI::Node_set_Prograde_createRequest(uint64_t Node_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_set_Prograde");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Node_set_Prograde(uint64_t Node_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_set_Prograde_createRequest(Node_ID, value, request);

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

bool KRPCI::Node_get_Normal_createRequest(uint64_t Node_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_Normal");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Node_get_Normal(uint64_t Node_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_get_Normal_createRequest(Node_ID, request);

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

bool KRPCI::Node_set_Normal_createRequest(uint64_t Node_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_set_Normal");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Node_set_Normal(uint64_t Node_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_set_Normal_createRequest(Node_ID, value, request);

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

bool KRPCI::Node_get_Radial_createRequest(uint64_t Node_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_Radial");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Node_get_Radial(uint64_t Node_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_get_Radial_createRequest(Node_ID, request);

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

bool KRPCI::Node_set_Radial_createRequest(uint64_t Node_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_set_Radial");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Node_set_Radial(uint64_t Node_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_set_Radial_createRequest(Node_ID, value, request);

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

bool KRPCI::Node_get_DeltaV_createRequest(uint64_t Node_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_DeltaV");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Node_get_DeltaV(uint64_t Node_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_get_DeltaV_createRequest(Node_ID, request);

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

bool KRPCI::Node_set_DeltaV_createRequest(uint64_t Node_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_set_DeltaV");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Node_set_DeltaV(uint64_t Node_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_set_DeltaV_createRequest(Node_ID, value, request);

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

bool KRPCI::Node_get_RemainingDeltaV_createRequest(uint64_t Node_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_RemainingDeltaV");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Node_get_RemainingDeltaV(uint64_t Node_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_get_RemainingDeltaV_createRequest(Node_ID, request);

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

bool KRPCI::Node_get_UT_createRequest(uint64_t Node_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_UT");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Node_get_UT(uint64_t Node_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_get_UT_createRequest(Node_ID, request);

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

bool KRPCI::Node_set_UT_createRequest(uint64_t Node_ID, double value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_set_UT");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));  

  return true;
}

bool KRPCI::Node_set_UT(uint64_t Node_ID, double value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_set_UT_createRequest(Node_ID, value, request);

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

bool KRPCI::Node_get_TimeTo_createRequest(uint64_t Node_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_TimeTo");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Node_get_TimeTo(uint64_t Node_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_get_TimeTo_createRequest(Node_ID, request);

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

bool KRPCI::Node_get_Orbit_createRequest(uint64_t Node_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_Orbit");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Node_get_Orbit(uint64_t Node_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_get_Orbit_createRequest(Node_ID, request);

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

bool KRPCI::Node_get_ReferenceFrame_createRequest(uint64_t Node_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_ReferenceFrame");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Node_get_ReferenceFrame(uint64_t Node_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_get_ReferenceFrame_createRequest(Node_ID, request);

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

bool KRPCI::Node_get_OrbitalReferenceFrame_createRequest(uint64_t Node_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Node_get_OrbitalReferenceFrame");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Node_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Node_get_OrbitalReferenceFrame(uint64_t Node_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Node_get_OrbitalReferenceFrame_createRequest(Node_ID, request);

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

bool KRPCI::Orbit_ReferencePlaneNormal_createRequest(uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_ReferencePlaneNormal");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_ReferencePlaneNormal(uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_ReferencePlaneNormal_createRequest(referenceFrame, request);

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

bool KRPCI::Orbit_ReferencePlaneDirection_createRequest(uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_ReferencePlaneDirection");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(referenceFrame, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_ReferencePlaneDirection(uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_ReferencePlaneDirection_createRequest(referenceFrame, request);

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

bool KRPCI::Orbit_get_Body_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Body");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_Body(uint64_t Orbit_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_Body_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_Apoapsis_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Apoapsis");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_Apoapsis(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_Apoapsis_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_Periapsis_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Periapsis");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_Periapsis(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_Periapsis_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_ApoapsisAltitude_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_ApoapsisAltitude");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_ApoapsisAltitude(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_ApoapsisAltitude_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_PeriapsisAltitude_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_PeriapsisAltitude");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_PeriapsisAltitude(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_PeriapsisAltitude_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_SemiMajorAxis_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_SemiMajorAxis");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_SemiMajorAxis(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_SemiMajorAxis_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_SemiMinorAxis_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_SemiMinorAxis");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_SemiMinorAxis(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_SemiMinorAxis_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_Radius_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Radius");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_Radius(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_Radius_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_Speed_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Speed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_Speed(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_Speed_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_Period_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Period");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_Period(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_Period_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_TimeToApoapsis_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_TimeToApoapsis");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_TimeToApoapsis(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_TimeToApoapsis_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_TimeToPeriapsis_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_TimeToPeriapsis");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_TimeToPeriapsis(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_TimeToPeriapsis_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_Eccentricity_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Eccentricity");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_Eccentricity(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_Eccentricity_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_Inclination_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Inclination");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_Inclination(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_Inclination_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_LongitudeOfAscendingNode_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_LongitudeOfAscendingNode");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_LongitudeOfAscendingNode(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_LongitudeOfAscendingNode_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_ArgumentOfPeriapsis_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_ArgumentOfPeriapsis");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_ArgumentOfPeriapsis(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_ArgumentOfPeriapsis_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_MeanAnomalyAtEpoch_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_MeanAnomalyAtEpoch");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_MeanAnomalyAtEpoch(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_MeanAnomalyAtEpoch_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_Epoch_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_Epoch");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_Epoch(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_Epoch_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_MeanAnomaly_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_MeanAnomaly");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_MeanAnomaly(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_MeanAnomaly_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_EccentricAnomaly_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_EccentricAnomaly");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_EccentricAnomaly(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_EccentricAnomaly_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_NextOrbit_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_NextOrbit");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_NextOrbit(uint64_t Orbit_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_NextOrbit_createRequest(Orbit_ID, request);

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

bool KRPCI::Orbit_get_TimeToSOIChange_createRequest(uint64_t Orbit_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Orbit_get_TimeToSOIChange");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Orbit_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Orbit_get_TimeToSOIChange(uint64_t Orbit_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Orbit_get_TimeToSOIChange_createRequest(Orbit_ID, request);

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

bool KRPCI::Decoupler_Decouple_createRequest(uint64_t Decoupler_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Decoupler_Decouple");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Decoupler_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Decoupler_Decouple(uint64_t Decoupler_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Decoupler_Decouple_createRequest(Decoupler_ID, request);

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

bool KRPCI::Decoupler_get_Part_createRequest(uint64_t Decoupler_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Decoupler_get_Part");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Decoupler_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Decoupler_get_Part(uint64_t Decoupler_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Decoupler_get_Part_createRequest(Decoupler_ID, request);

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

bool KRPCI::Decoupler_get_Decoupled_createRequest(uint64_t Decoupler_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Decoupler_get_Decoupled");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Decoupler_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Decoupler_get_Decoupled(uint64_t Decoupler_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Decoupler_get_Decoupled_createRequest(Decoupler_ID, request);

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

bool KRPCI::Decoupler_get_Impulse_createRequest(uint64_t Decoupler_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Decoupler_get_Impulse");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Decoupler_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Decoupler_get_Impulse(uint64_t Decoupler_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Decoupler_get_Impulse_createRequest(Decoupler_ID, request);

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

bool KRPCI::DockingPort_Undock_createRequest(uint64_t DockingPort_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_Undock");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::DockingPort_Undock(uint64_t DockingPort_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::DockingPort_Undock_createRequest(DockingPort_ID, request);

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

bool KRPCI::DockingPort_Position_createRequest(uint64_t DockingPort_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_Position");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::DockingPort_Position(uint64_t DockingPort_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::DockingPort_Position_createRequest(DockingPort_ID, referenceFrame, request);

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

bool KRPCI::DockingPort_Direction_createRequest(uint64_t DockingPort_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_Direction");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::DockingPort_Direction(uint64_t DockingPort_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::DockingPort_Direction_createRequest(DockingPort_ID, referenceFrame, request);

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

bool KRPCI::DockingPort_Rotation_createRequest(uint64_t DockingPort_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_Rotation");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::DockingPort_Rotation(uint64_t DockingPort_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::DockingPort_Rotation_createRequest(DockingPort_ID, referenceFrame, request);

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

bool KRPCI::DockingPort_get_Part_createRequest(uint64_t DockingPort_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_Part");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::DockingPort_get_Part(uint64_t DockingPort_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::DockingPort_get_Part_createRequest(DockingPort_ID, request);

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

bool KRPCI::DockingPort_get_Name_createRequest(uint64_t DockingPort_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_Name");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::DockingPort_get_Name(uint64_t DockingPort_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::DockingPort_get_Name_createRequest(DockingPort_ID, request);

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

bool KRPCI::DockingPort_set_Name_createRequest(uint64_t DockingPort_ID, std::string value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_set_Name");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::DockingPort_set_Name(uint64_t DockingPort_ID, std::string value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::DockingPort_set_Name_createRequest(DockingPort_ID, value, request);

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

bool KRPCI::DockingPort_get_State_createRequest(uint64_t DockingPort_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_State");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::DockingPort_get_State(uint64_t DockingPort_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::DockingPort_get_State_createRequest(DockingPort_ID, request);

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

bool KRPCI::DockingPort_get_DockedPart_createRequest(uint64_t DockingPort_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_DockedPart");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::DockingPort_get_DockedPart(uint64_t DockingPort_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::DockingPort_get_DockedPart_createRequest(DockingPort_ID, request);

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

bool KRPCI::DockingPort_get_ReengageDistance_createRequest(uint64_t DockingPort_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_ReengageDistance");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::DockingPort_get_ReengageDistance(uint64_t DockingPort_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::DockingPort_get_ReengageDistance_createRequest(DockingPort_ID, request);

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

bool KRPCI::DockingPort_get_HasShield_createRequest(uint64_t DockingPort_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_HasShield");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::DockingPort_get_HasShield(uint64_t DockingPort_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::DockingPort_get_HasShield_createRequest(DockingPort_ID, request);

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

bool KRPCI::DockingPort_get_Shielded_createRequest(uint64_t DockingPort_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_Shielded");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::DockingPort_get_Shielded(uint64_t DockingPort_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::DockingPort_get_Shielded_createRequest(DockingPort_ID, request);

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

bool KRPCI::DockingPort_set_Shielded_createRequest(uint64_t DockingPort_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_set_Shielded");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::DockingPort_set_Shielded(uint64_t DockingPort_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::DockingPort_set_Shielded_createRequest(DockingPort_ID, value, request);

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

bool KRPCI::DockingPort_get_ReferenceFrame_createRequest(uint64_t DockingPort_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("DockingPort_get_ReferenceFrame");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(DockingPort_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::DockingPort_get_ReferenceFrame(uint64_t DockingPort_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::DockingPort_get_ReferenceFrame_createRequest(DockingPort_ID, request);

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

bool KRPCI::Engine_get_Part_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_Part");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_Part(uint64_t Engine_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_Part_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_Active_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_Active");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_Active(uint64_t Engine_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_Active_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_set_Active_createRequest(uint64_t Engine_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_set_Active");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Engine_set_Active(uint64_t Engine_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_set_Active_createRequest(Engine_ID, value, request);

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

bool KRPCI::Engine_get_Thrust_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_Thrust");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_Thrust(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_Thrust_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_AvailableThrust_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_AvailableThrust");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_AvailableThrust(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_AvailableThrust_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_MaxThrust_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_MaxThrust");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_MaxThrust(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_MaxThrust_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_MaxVacuumThrust_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_MaxVacuumThrust");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_MaxVacuumThrust(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_MaxVacuumThrust_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_ThrustLimit_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_ThrustLimit");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_ThrustLimit(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_ThrustLimit_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_set_ThrustLimit_createRequest(uint64_t Engine_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_set_ThrustLimit");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Engine_set_ThrustLimit(uint64_t Engine_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_set_ThrustLimit_createRequest(Engine_ID, value, request);

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

bool KRPCI::Engine_get_SpecificImpulse_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_SpecificImpulse");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_SpecificImpulse(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_SpecificImpulse_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_VacuumSpecificImpulse_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_VacuumSpecificImpulse");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_VacuumSpecificImpulse(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_VacuumSpecificImpulse_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_KerbinSeaLevelSpecificImpulse_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_KerbinSeaLevelSpecificImpulse");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_KerbinSeaLevelSpecificImpulse(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_KerbinSeaLevelSpecificImpulse_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_Propellants_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_Propellants");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_Propellants(uint64_t Engine_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_Propellants_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_PropellantRatios_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_PropellantRatios");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_PropellantRatios(uint64_t Engine_ID, krpc::Dictionary& return_dict)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_PropellantRatios_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_HasFuel_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_HasFuel");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_HasFuel(uint64_t Engine_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_HasFuel_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_Throttle_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_Throttle");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_Throttle(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_Throttle_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_ThrottleLocked_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_ThrottleLocked");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_ThrottleLocked(uint64_t Engine_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_ThrottleLocked_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_CanRestart_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_CanRestart");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_CanRestart(uint64_t Engine_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_CanRestart_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_CanShutdown_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_CanShutdown");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_CanShutdown(uint64_t Engine_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_CanShutdown_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_Gimballed_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_Gimballed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_Gimballed(uint64_t Engine_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_Gimballed_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_GimbalRange_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_GimbalRange");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_GimbalRange(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_GimbalRange_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_get_GimbalLocked_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_GimbalLocked");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_GimbalLocked(uint64_t Engine_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_GimbalLocked_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_set_GimbalLocked_createRequest(uint64_t Engine_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_set_GimbalLocked");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Engine_set_GimbalLocked(uint64_t Engine_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_set_GimbalLocked_createRequest(Engine_ID, value, request);

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

bool KRPCI::Engine_get_GimbalLimit_createRequest(uint64_t Engine_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_get_GimbalLimit");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Engine_get_GimbalLimit(uint64_t Engine_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_get_GimbalLimit_createRequest(Engine_ID, request);

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

bool KRPCI::Engine_set_GimbalLimit_createRequest(uint64_t Engine_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Engine_set_GimbalLimit");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Engine_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Engine_set_GimbalLimit(uint64_t Engine_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Engine_set_GimbalLimit_createRequest(Engine_ID, value, request);

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

bool KRPCI::LandingGear_get_Part_createRequest(uint64_t LandingGear_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("LandingGear_get_Part");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LandingGear_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::LandingGear_get_Part(uint64_t LandingGear_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::LandingGear_get_Part_createRequest(LandingGear_ID, request);

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

bool KRPCI::LandingGear_get_State_createRequest(uint64_t LandingGear_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("LandingGear_get_State");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LandingGear_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::LandingGear_get_State(uint64_t LandingGear_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::LandingGear_get_State_createRequest(LandingGear_ID, request);

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

bool KRPCI::LandingGear_get_Deployed_createRequest(uint64_t LandingGear_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("LandingGear_get_Deployed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LandingGear_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::LandingGear_get_Deployed(uint64_t LandingGear_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::LandingGear_get_Deployed_createRequest(LandingGear_ID, request);

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

bool KRPCI::LandingGear_set_Deployed_createRequest(uint64_t LandingGear_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("LandingGear_set_Deployed");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::LandingGear_set_Deployed(uint64_t LandingGear_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::LandingGear_set_Deployed_createRequest(LandingGear_ID, value, request);

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

bool KRPCI::LandingLeg_get_Part_createRequest(uint64_t LandingLeg_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("LandingLeg_get_Part");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LandingLeg_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::LandingLeg_get_Part(uint64_t LandingLeg_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::LandingLeg_get_Part_createRequest(LandingLeg_ID, request);

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

bool KRPCI::LandingLeg_get_State_createRequest(uint64_t LandingLeg_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("LandingLeg_get_State");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LandingLeg_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::LandingLeg_get_State(uint64_t LandingLeg_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::LandingLeg_get_State_createRequest(LandingLeg_ID, request);

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

bool KRPCI::LandingLeg_get_Deployed_createRequest(uint64_t LandingLeg_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("LandingLeg_get_Deployed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LandingLeg_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::LandingLeg_get_Deployed(uint64_t LandingLeg_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::LandingLeg_get_Deployed_createRequest(LandingLeg_ID, request);

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

bool KRPCI::LandingLeg_set_Deployed_createRequest(uint64_t LandingLeg_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("LandingLeg_set_Deployed");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::LandingLeg_set_Deployed(uint64_t LandingLeg_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::LandingLeg_set_Deployed_createRequest(LandingLeg_ID, value, request);

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

bool KRPCI::LaunchClamp_Release_createRequest(uint64_t LaunchClamp_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("LaunchClamp_Release");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LaunchClamp_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::LaunchClamp_Release(uint64_t LaunchClamp_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::LaunchClamp_Release_createRequest(LaunchClamp_ID, request);

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

bool KRPCI::LaunchClamp_get_Part_createRequest(uint64_t LaunchClamp_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("LaunchClamp_get_Part");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(LaunchClamp_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::LaunchClamp_get_Part(uint64_t LaunchClamp_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::LaunchClamp_get_Part_createRequest(LaunchClamp_ID, request);

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

bool KRPCI::Light_get_Part_createRequest(uint64_t Light_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Light_get_Part");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Light_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Light_get_Part(uint64_t Light_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Light_get_Part_createRequest(Light_ID, request);

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

bool KRPCI::Light_get_Active_createRequest(uint64_t Light_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Light_get_Active");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Light_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Light_get_Active(uint64_t Light_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Light_get_Active_createRequest(Light_ID, request);

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

bool KRPCI::Light_set_Active_createRequest(uint64_t Light_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Light_set_Active");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Light_set_Active(uint64_t Light_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Light_set_Active_createRequest(Light_ID, value, request);

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

bool KRPCI::Light_get_PowerUsage_createRequest(uint64_t Light_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Light_get_PowerUsage");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Light_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Light_get_PowerUsage(uint64_t Light_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Light_get_PowerUsage_createRequest(Light_ID, request);

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

bool KRPCI::Module_HasField_createRequest(uint64_t Module_ID, std::string name, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Module_HasField");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Module_HasField(uint64_t Module_ID, std::string name, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Module_HasField_createRequest(Module_ID, name, request);

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

bool KRPCI::Module_GetField_createRequest(uint64_t Module_ID, std::string name, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Module_GetField");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Module_GetField(uint64_t Module_ID, std::string name, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Module_GetField_createRequest(Module_ID, name, request);

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

bool KRPCI::Module_HasEvent_createRequest(uint64_t Module_ID, std::string name, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Module_HasEvent");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Module_HasEvent(uint64_t Module_ID, std::string name, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Module_HasEvent_createRequest(Module_ID, name, request);

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

bool KRPCI::Module_TriggerEvent_createRequest(uint64_t Module_ID, std::string name, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Module_TriggerEvent");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Module_TriggerEvent(uint64_t Module_ID, std::string name)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Module_TriggerEvent_createRequest(Module_ID, name, request);

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

bool KRPCI::Module_HasAction_createRequest(uint64_t Module_ID, std::string name, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Module_HasAction");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Module_HasAction(uint64_t Module_ID, std::string name, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Module_HasAction_createRequest(Module_ID, name, request);

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

bool KRPCI::Module_SetAction_createRequest(uint64_t Module_ID, std::string name, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Module_SetAction");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Module_SetAction(uint64_t Module_ID, std::string name, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Module_SetAction_createRequest(Module_ID, name, value, request);

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

bool KRPCI::Module_get_Name_createRequest(uint64_t Module_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Module_get_Name");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Module_get_Name(uint64_t Module_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Module_get_Name_createRequest(Module_ID, request);

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

bool KRPCI::Module_get_Part_createRequest(uint64_t Module_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Module_get_Part");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Module_get_Part(uint64_t Module_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Module_get_Part_createRequest(Module_ID, request);

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

bool KRPCI::Module_get_Fields_createRequest(uint64_t Module_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Module_get_Fields");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Module_get_Fields(uint64_t Module_ID, krpc::Dictionary& return_dict)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Module_get_Fields_createRequest(Module_ID, request);

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

bool KRPCI::Module_get_Events_createRequest(uint64_t Module_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Module_get_Events");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Module_get_Events(uint64_t Module_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Module_get_Events_createRequest(Module_ID, request);

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

bool KRPCI::Module_get_Actions_createRequest(uint64_t Module_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Module_get_Actions");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Module_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Module_get_Actions(uint64_t Module_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Module_get_Actions_createRequest(Module_ID, request);

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

bool KRPCI::Parachute_Deploy_createRequest(uint64_t Parachute_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_Deploy");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parachute_Deploy(uint64_t Parachute_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parachute_Deploy_createRequest(Parachute_ID, request);

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

bool KRPCI::Parachute_get_Part_createRequest(uint64_t Parachute_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_get_Part");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parachute_get_Part(uint64_t Parachute_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parachute_get_Part_createRequest(Parachute_ID, request);

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

bool KRPCI::Parachute_get_Deployed_createRequest(uint64_t Parachute_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_get_Deployed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parachute_get_Deployed(uint64_t Parachute_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parachute_get_Deployed_createRequest(Parachute_ID, request);

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

bool KRPCI::Parachute_get_State_createRequest(uint64_t Parachute_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_get_State");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parachute_get_State(uint64_t Parachute_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parachute_get_State_createRequest(Parachute_ID, request);

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

bool KRPCI::Parachute_get_DeployAltitude_createRequest(uint64_t Parachute_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_get_DeployAltitude");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parachute_get_DeployAltitude(uint64_t Parachute_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parachute_get_DeployAltitude_createRequest(Parachute_ID, request);

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

bool KRPCI::Parachute_set_DeployAltitude_createRequest(uint64_t Parachute_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_set_DeployAltitude");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Parachute_set_DeployAltitude(uint64_t Parachute_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parachute_set_DeployAltitude_createRequest(Parachute_ID, value, request);

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

bool KRPCI::Parachute_get_DeployMinPressure_createRequest(uint64_t Parachute_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_get_DeployMinPressure");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parachute_get_DeployMinPressure(uint64_t Parachute_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parachute_get_DeployMinPressure_createRequest(Parachute_ID, request);

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

bool KRPCI::Parachute_set_DeployMinPressure_createRequest(uint64_t Parachute_ID, float value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parachute_set_DeployMinPressure");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parachute_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Parachute_set_DeployMinPressure(uint64_t Parachute_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parachute_set_DeployMinPressure_createRequest(Parachute_ID, value, request);

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

bool KRPCI::Part_Position_createRequest(uint64_t Part_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_Position");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Part_Position(uint64_t Part_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_Position_createRequest(Part_ID, referenceFrame, request);

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

bool KRPCI::Part_Direction_createRequest(uint64_t Part_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_Direction");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Part_Direction(uint64_t Part_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_Direction_createRequest(Part_ID, referenceFrame, request);

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

bool KRPCI::Part_Velocity_createRequest(uint64_t Part_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_Velocity");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Part_Velocity(uint64_t Part_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_Velocity_createRequest(Part_ID, referenceFrame, request);

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

bool KRPCI::Part_Rotation_createRequest(uint64_t Part_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_Rotation");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Part_Rotation(uint64_t Part_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_Rotation_createRequest(Part_ID, referenceFrame, request);

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

bool KRPCI::Part_get_Name_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Name");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Name(uint64_t Part_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Name_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Title_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Title");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Title(uint64_t Part_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Title_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Cost_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Cost");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Cost(uint64_t Part_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Cost_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Vessel_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Vessel");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Vessel(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Vessel_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Parent_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Parent");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Parent(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Parent_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Children_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Children");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Children(uint64_t Part_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Children_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_AxiallyAttached_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_AxiallyAttached");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_AxiallyAttached(uint64_t Part_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_AxiallyAttached_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_RadiallyAttached_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_RadiallyAttached");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_RadiallyAttached(uint64_t Part_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_RadiallyAttached_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Stage_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Stage");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Stage(uint64_t Part_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Stage_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_DecoupleStage_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_DecoupleStage");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_DecoupleStage(uint64_t Part_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_DecoupleStage_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Massless_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Massless");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Massless(uint64_t Part_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Massless_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Mass_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Mass");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Mass(uint64_t Part_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Mass_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_DryMass_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_DryMass");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_DryMass(uint64_t Part_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_DryMass_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_ImpactTolerance_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_ImpactTolerance");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_ImpactTolerance(uint64_t Part_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_ImpactTolerance_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Temperature_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Temperature");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Temperature(uint64_t Part_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Temperature_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_MaxTemperature_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_MaxTemperature");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_MaxTemperature(uint64_t Part_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_MaxTemperature_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Resources_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Resources");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Resources(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Resources_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Crossfeed_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Crossfeed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Crossfeed(uint64_t Part_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Crossfeed_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_FuelLinesFrom_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_FuelLinesFrom");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_FuelLinesFrom(uint64_t Part_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_FuelLinesFrom_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_FuelLinesTo_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_FuelLinesTo");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_FuelLinesTo(uint64_t Part_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_FuelLinesTo_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Modules_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Modules");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Modules(uint64_t Part_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Modules_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Decoupler_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Decoupler");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Decoupler(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Decoupler_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_DockingPort_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_DockingPort");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_DockingPort(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_DockingPort_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Engine_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Engine");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Engine(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Engine_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_LandingGear_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_LandingGear");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_LandingGear(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_LandingGear_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_LandingLeg_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_LandingLeg");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_LandingLeg(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_LandingLeg_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_LaunchClamp_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_LaunchClamp");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_LaunchClamp(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_LaunchClamp_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Light_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Light");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Light(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Light_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Parachute_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Parachute");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Parachute(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Parachute_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_ReactionWheel_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_ReactionWheel");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_ReactionWheel(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_ReactionWheel_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_Sensor_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_Sensor");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_Sensor(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_Sensor_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_SolarPanel_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_SolarPanel");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_SolarPanel(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_SolarPanel_createRequest(Part_ID, request);

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

bool KRPCI::Part_get_ReferenceFrame_createRequest(uint64_t Part_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Part_get_ReferenceFrame");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Part_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Part_get_ReferenceFrame(uint64_t Part_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Part_get_ReferenceFrame_createRequest(Part_ID, request);

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

bool KRPCI::Parts_WithName_createRequest(uint64_t Parts_ID, std::string name, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_WithName");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Parts_WithName(uint64_t Parts_ID, std::string name, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_WithName_createRequest(Parts_ID, name, request);

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

bool KRPCI::Parts_WithTitle_createRequest(uint64_t Parts_ID, std::string title, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_WithTitle");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Parts_WithTitle(uint64_t Parts_ID, std::string title, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_WithTitle_createRequest(Parts_ID, title, request);

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

bool KRPCI::Parts_WithModule_createRequest(uint64_t Parts_ID, std::string moduleName, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_WithModule");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Parts_WithModule(uint64_t Parts_ID, std::string moduleName, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_WithModule_createRequest(Parts_ID, moduleName, request);

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

bool KRPCI::Parts_InStage_createRequest(uint64_t Parts_ID, int32_t stage, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_InStage");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Parts_InStage(uint64_t Parts_ID, int32_t stage, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_InStage_createRequest(Parts_ID, stage, request);

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

bool KRPCI::Parts_InDecoupleStage_createRequest(uint64_t Parts_ID, int32_t stage, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_InDecoupleStage");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Parts_InDecoupleStage(uint64_t Parts_ID, int32_t stage, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_InDecoupleStage_createRequest(Parts_ID, stage, request);

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

bool KRPCI::Parts_ModulesWithName_createRequest(uint64_t Parts_ID, std::string moduleName, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_ModulesWithName");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Parts_ModulesWithName(uint64_t Parts_ID, std::string moduleName, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_ModulesWithName_createRequest(Parts_ID, moduleName, request);

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

bool KRPCI::Parts_DockingPortWithName_createRequest(uint64_t Parts_ID, std::string name, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_DockingPortWithName");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Parts_DockingPortWithName(uint64_t Parts_ID, std::string name, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_DockingPortWithName_createRequest(Parts_ID, name, request);

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

bool KRPCI::Parts_get_All_createRequest(uint64_t Parts_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_All");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parts_get_All(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_get_All_createRequest(Parts_ID, request);

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

bool KRPCI::Parts_get_Root_createRequest(uint64_t Parts_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_Root");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parts_get_Root(uint64_t Parts_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_get_Root_createRequest(Parts_ID, request);

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

bool KRPCI::Parts_get_Controlling_createRequest(uint64_t Parts_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_Controlling");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parts_get_Controlling(uint64_t Parts_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_get_Controlling_createRequest(Parts_ID, request);

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

bool KRPCI::Parts_set_Controlling_createRequest(uint64_t Parts_ID, uint64_t value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_set_Controlling");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Parts_set_Controlling(uint64_t Parts_ID, uint64_t value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_set_Controlling_createRequest(Parts_ID, value, request);

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

bool KRPCI::Parts_get_Decouplers_createRequest(uint64_t Parts_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_Decouplers");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parts_get_Decouplers(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_get_Decouplers_createRequest(Parts_ID, request);

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

bool KRPCI::Parts_get_DockingPorts_createRequest(uint64_t Parts_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_DockingPorts");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parts_get_DockingPorts(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_get_DockingPorts_createRequest(Parts_ID, request);

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

bool KRPCI::Parts_get_Engines_createRequest(uint64_t Parts_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_Engines");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parts_get_Engines(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_get_Engines_createRequest(Parts_ID, request);

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

bool KRPCI::Parts_get_LandingGear_createRequest(uint64_t Parts_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_LandingGear");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parts_get_LandingGear(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_get_LandingGear_createRequest(Parts_ID, request);

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

bool KRPCI::Parts_get_LandingLegs_createRequest(uint64_t Parts_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_LandingLegs");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parts_get_LandingLegs(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_get_LandingLegs_createRequest(Parts_ID, request);

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

bool KRPCI::Parts_get_LaunchClamps_createRequest(uint64_t Parts_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_LaunchClamps");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parts_get_LaunchClamps(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_get_LaunchClamps_createRequest(Parts_ID, request);

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

bool KRPCI::Parts_get_Lights_createRequest(uint64_t Parts_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_Lights");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parts_get_Lights(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_get_Lights_createRequest(Parts_ID, request);

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

bool KRPCI::Parts_get_Parachutes_createRequest(uint64_t Parts_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_Parachutes");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parts_get_Parachutes(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_get_Parachutes_createRequest(Parts_ID, request);

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

bool KRPCI::Parts_get_ReactionWheels_createRequest(uint64_t Parts_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_ReactionWheels");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parts_get_ReactionWheels(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_get_ReactionWheels_createRequest(Parts_ID, request);

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

bool KRPCI::Parts_get_Sensors_createRequest(uint64_t Parts_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_Sensors");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parts_get_Sensors(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_get_Sensors_createRequest(Parts_ID, request);

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

bool KRPCI::Parts_get_SolarPanels_createRequest(uint64_t Parts_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Parts_get_SolarPanels");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Parts_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Parts_get_SolarPanels(uint64_t Parts_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Parts_get_SolarPanels_createRequest(Parts_ID, request);

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

bool KRPCI::ReactionWheel_get_Part_createRequest(uint64_t ReactionWheel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("ReactionWheel_get_Part");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ReactionWheel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ReactionWheel_get_Part(uint64_t ReactionWheel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ReactionWheel_get_Part_createRequest(ReactionWheel_ID, request);

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

bool KRPCI::ReactionWheel_get_Active_createRequest(uint64_t ReactionWheel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("ReactionWheel_get_Active");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ReactionWheel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ReactionWheel_get_Active(uint64_t ReactionWheel_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ReactionWheel_get_Active_createRequest(ReactionWheel_ID, request);

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

bool KRPCI::ReactionWheel_set_Active_createRequest(uint64_t ReactionWheel_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("ReactionWheel_set_Active");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::ReactionWheel_set_Active(uint64_t ReactionWheel_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ReactionWheel_set_Active_createRequest(ReactionWheel_ID, value, request);

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

bool KRPCI::ReactionWheel_get_Broken_createRequest(uint64_t ReactionWheel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("ReactionWheel_get_Broken");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ReactionWheel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ReactionWheel_get_Broken(uint64_t ReactionWheel_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ReactionWheel_get_Broken_createRequest(ReactionWheel_ID, request);

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

bool KRPCI::ReactionWheel_get_PitchTorque_createRequest(uint64_t ReactionWheel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("ReactionWheel_get_PitchTorque");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ReactionWheel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ReactionWheel_get_PitchTorque(uint64_t ReactionWheel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ReactionWheel_get_PitchTorque_createRequest(ReactionWheel_ID, request);

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

bool KRPCI::ReactionWheel_get_YawTorque_createRequest(uint64_t ReactionWheel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("ReactionWheel_get_YawTorque");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ReactionWheel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ReactionWheel_get_YawTorque(uint64_t ReactionWheel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ReactionWheel_get_YawTorque_createRequest(ReactionWheel_ID, request);

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

bool KRPCI::ReactionWheel_get_RollTorque_createRequest(uint64_t ReactionWheel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("ReactionWheel_get_RollTorque");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ReactionWheel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ReactionWheel_get_RollTorque(uint64_t ReactionWheel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ReactionWheel_get_RollTorque_createRequest(ReactionWheel_ID, request);

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

bool KRPCI::Sensor_get_Part_createRequest(uint64_t Sensor_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Sensor_get_Part");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Sensor_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Sensor_get_Part(uint64_t Sensor_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Sensor_get_Part_createRequest(Sensor_ID, request);

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

bool KRPCI::Sensor_get_Active_createRequest(uint64_t Sensor_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Sensor_get_Active");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Sensor_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Sensor_get_Active(uint64_t Sensor_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Sensor_get_Active_createRequest(Sensor_ID, request);

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

bool KRPCI::Sensor_set_Active_createRequest(uint64_t Sensor_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Sensor_set_Active");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Sensor_set_Active(uint64_t Sensor_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Sensor_set_Active_createRequest(Sensor_ID, value, request);

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

bool KRPCI::Sensor_get_Value_createRequest(uint64_t Sensor_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Sensor_get_Value");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Sensor_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Sensor_get_Value(uint64_t Sensor_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Sensor_get_Value_createRequest(Sensor_ID, request);

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

bool KRPCI::Sensor_get_PowerUsage_createRequest(uint64_t Sensor_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Sensor_get_PowerUsage");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Sensor_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Sensor_get_PowerUsage(uint64_t Sensor_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Sensor_get_PowerUsage_createRequest(Sensor_ID, request);

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

bool KRPCI::SolarPanel_get_Part_createRequest(uint64_t SolarPanel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("SolarPanel_get_Part");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(SolarPanel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::SolarPanel_get_Part(uint64_t SolarPanel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::SolarPanel_get_Part_createRequest(SolarPanel_ID, request);

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

bool KRPCI::SolarPanel_get_Deployed_createRequest(uint64_t SolarPanel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("SolarPanel_get_Deployed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(SolarPanel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::SolarPanel_get_Deployed(uint64_t SolarPanel_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::SolarPanel_get_Deployed_createRequest(SolarPanel_ID, request);

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

bool KRPCI::SolarPanel_set_Deployed_createRequest(uint64_t SolarPanel_ID, bool value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("SolarPanel_set_Deployed");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::SolarPanel_set_Deployed(uint64_t SolarPanel_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::SolarPanel_set_Deployed_createRequest(SolarPanel_ID, value, request);

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

bool KRPCI::SolarPanel_get_State_createRequest(uint64_t SolarPanel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("SolarPanel_get_State");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(SolarPanel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::SolarPanel_get_State(uint64_t SolarPanel_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::SolarPanel_get_State_createRequest(SolarPanel_ID, request);

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

bool KRPCI::SolarPanel_get_EnergyFlow_createRequest(uint64_t SolarPanel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("SolarPanel_get_EnergyFlow");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(SolarPanel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::SolarPanel_get_EnergyFlow(uint64_t SolarPanel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::SolarPanel_get_EnergyFlow_createRequest(SolarPanel_ID, request);

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

bool KRPCI::SolarPanel_get_SunExposure_createRequest(uint64_t SolarPanel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("SolarPanel_get_SunExposure");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(SolarPanel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::SolarPanel_get_SunExposure(uint64_t SolarPanel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::SolarPanel_get_SunExposure_createRequest(SolarPanel_ID, request);

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

bool KRPCI::Resources_HasResource_createRequest(uint64_t Resources_ID, std::string name, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Resources_HasResource");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Resources_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Resources_HasResource(uint64_t Resources_ID, std::string name, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Resources_HasResource_createRequest(Resources_ID, name, request);

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

bool KRPCI::Resources_Max_createRequest(uint64_t Resources_ID, std::string name, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Resources_Max");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Resources_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Resources_Max(uint64_t Resources_ID, std::string name, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Resources_Max_createRequest(Resources_ID, name, request);

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

bool KRPCI::Resources_Amount_createRequest(uint64_t Resources_ID, std::string name, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Resources_Amount");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Resources_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Resources_Amount(uint64_t Resources_ID, std::string name, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Resources_Amount_createRequest(Resources_ID, name, request);

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

bool KRPCI::Resources_Density_createRequest(std::string name, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Resources_Density");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  return true;
}

bool KRPCI::Resources_Density(std::string name, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Resources_Density_createRequest(name, request);

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

bool KRPCI::Resources_get_Names_createRequest(uint64_t Resources_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Resources_get_Names");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Resources_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Resources_get_Names(uint64_t Resources_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Resources_get_Names_createRequest(Resources_ID, request);

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

bool KRPCI::Vessel_Flight_createRequest(uint64_t Vessel_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_Flight");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Vessel_Flight(uint64_t Vessel_ID, uint64_t referenceFrame, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_Flight_createRequest(Vessel_ID, referenceFrame, request);

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

bool KRPCI::Vessel_ResourcesInDecoupleStage_createRequest(uint64_t Vessel_ID, int32_t stage, bool cumulative, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_ResourcesInDecoupleStage");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument = request.add_arguments();
  argument->set_position(2);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(cumulative, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_ResourcesInDecoupleStage(uint64_t Vessel_ID, int32_t stage, bool cumulative, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_ResourcesInDecoupleStage_createRequest(Vessel_ID, stage, cumulative, request);

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

bool KRPCI::Vessel_Position_createRequest(uint64_t Vessel_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_Position");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Vessel_Position(uint64_t Vessel_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_Position_createRequest(Vessel_ID, referenceFrame, request);

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

bool KRPCI::Vessel_Velocity_createRequest(uint64_t Vessel_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_Velocity");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Vessel_Velocity(uint64_t Vessel_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_Velocity_createRequest(Vessel_ID, referenceFrame, request);

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

bool KRPCI::Vessel_Rotation_createRequest(uint64_t Vessel_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_Rotation");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Vessel_Rotation(uint64_t Vessel_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_Rotation_createRequest(Vessel_ID, referenceFrame, request);

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

bool KRPCI::Vessel_Direction_createRequest(uint64_t Vessel_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_Direction");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Vessel_Direction(uint64_t Vessel_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_Direction_createRequest(Vessel_ID, referenceFrame, request);

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

bool KRPCI::Vessel_AngularVelocity_createRequest(uint64_t Vessel_ID, uint64_t referenceFrame, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_AngularVelocity");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Vessel_AngularVelocity(uint64_t Vessel_ID, uint64_t referenceFrame, double& x, double& y, double& z)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_AngularVelocity_createRequest(Vessel_ID, referenceFrame, request);

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

bool KRPCI::Vessel_get_Name_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Name");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_Name(uint64_t Vessel_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_Name_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_set_Name_createRequest(uint64_t Vessel_ID, std::string value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_set_Name");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Vessel_set_Name(uint64_t Vessel_ID, std::string value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_set_Name_createRequest(Vessel_ID, value, request);

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

bool KRPCI::Vessel_get_Type_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Type");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_Type(uint64_t Vessel_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_Type_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_set_Type_createRequest(uint64_t Vessel_ID, int32_t value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_set_Type");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Vessel_set_Type(uint64_t Vessel_ID, int32_t value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_set_Type_createRequest(Vessel_ID, value, request);

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

bool KRPCI::Vessel_get_Situation_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Situation");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_Situation(uint64_t Vessel_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_Situation_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_MET_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_MET");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_MET(uint64_t Vessel_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_MET_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_Target_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Target");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_Target(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_Target_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_set_Target_createRequest(uint64_t Vessel_ID, uint64_t value, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_set_Target");
  krpc::Argument* argument;
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

  return true;
}

bool KRPCI::Vessel_set_Target(uint64_t Vessel_ID, uint64_t value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_set_Target_createRequest(Vessel_ID, value, request);

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

bool KRPCI::Vessel_get_Orbit_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Orbit");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_Orbit(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_Orbit_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_Control_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Control");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_Control(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_Control_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_AutoPilot_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_AutoPilot");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_AutoPilot(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_AutoPilot_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_Resources_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Resources");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_Resources(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_Resources_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_Parts_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Parts");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_Parts(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_Parts_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_Comms_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Comms");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_Comms(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_Comms_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_Mass_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Mass");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_Mass(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_Mass_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_DryMass_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_DryMass");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_DryMass(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_DryMass_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_Thrust_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_Thrust");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_Thrust(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_Thrust_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_AvailableThrust_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_AvailableThrust");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_AvailableThrust(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_AvailableThrust_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_MaxThrust_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_MaxThrust");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_MaxThrust(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_MaxThrust_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_MaxVacuumThrust_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_MaxVacuumThrust");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_MaxVacuumThrust(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_MaxVacuumThrust_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_SpecificImpulse_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_SpecificImpulse");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_SpecificImpulse(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_SpecificImpulse_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_VacuumSpecificImpulse_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_VacuumSpecificImpulse");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_VacuumSpecificImpulse(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_VacuumSpecificImpulse_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_KerbinSeaLevelSpecificImpulse_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_KerbinSeaLevelSpecificImpulse");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_KerbinSeaLevelSpecificImpulse(uint64_t Vessel_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_KerbinSeaLevelSpecificImpulse_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_ReferenceFrame_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_ReferenceFrame");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_ReferenceFrame(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_ReferenceFrame_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_OrbitalReferenceFrame_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_OrbitalReferenceFrame");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_OrbitalReferenceFrame(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_OrbitalReferenceFrame_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_SurfaceReferenceFrame_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_SurfaceReferenceFrame");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_SurfaceReferenceFrame(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_SurfaceReferenceFrame_createRequest(Vessel_ID, request);

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

bool KRPCI::Vessel_get_SurfaceVelocityReferenceFrame_createRequest(uint64_t Vessel_ID, krpc::Request& request)
{
  request.set_service("SpaceCenter");
  request.set_procedure("Vessel_get_SurfaceVelocityReferenceFrame");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Vessel_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Vessel_get_SurfaceVelocityReferenceFrame(uint64_t Vessel_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Vessel_get_SurfaceVelocityReferenceFrame_createRequest(Vessel_ID, request);

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

bool KRPCI::AlarmWithName_createRequest(std::string name, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("AlarmWithName");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  return true;
}

bool KRPCI::AlarmWithName(std::string name, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::AlarmWithName_createRequest(name, request);

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

bool KRPCI::AlarmsWithType_createRequest(int32_t type, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("AlarmsWithType");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  return true;
}

bool KRPCI::AlarmsWithType(int32_t type, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::AlarmsWithType_createRequest(type, request);

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

bool KRPCI::CreateAlarm_createRequest(int32_t type, std::string name, double ut, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("CreateAlarm");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument = request.add_arguments();
  argument->set_position(1);
  argument = request.add_arguments();
  argument->set_position(2);
  argument->set_value((const char*)(&ut), sizeof(ut));  

  return true;
}

bool KRPCI::CreateAlarm(int32_t type, std::string name, double ut, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::CreateAlarm_createRequest(type, name, ut, request);

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

bool KRPCI::get_Alarms_createRequest(krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("get_Alarms");
  return true;
}

bool KRPCI::get_Alarms(std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_Alarms_createRequest(request);

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

bool KRPCI::Alarm_Delete_createRequest(uint64_t Alarm_ID, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_Delete");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_Delete(uint64_t Alarm_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_Delete_createRequest(Alarm_ID, request);

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

bool KRPCI::Alarm_get_Action_createRequest(uint64_t Alarm_ID, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_get_Action");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_get_Action(uint64_t Alarm_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_get_Action_createRequest(Alarm_ID, request);

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

bool KRPCI::Alarm_set_Action_createRequest(uint64_t Alarm_ID, int32_t value, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_set_Action");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Alarm_set_Action(uint64_t Alarm_ID, int32_t value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_set_Action_createRequest(Alarm_ID, value, request);

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

bool KRPCI::Alarm_get_Margin_createRequest(uint64_t Alarm_ID, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_get_Margin");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_get_Margin(uint64_t Alarm_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_get_Margin_createRequest(Alarm_ID, request);

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

bool KRPCI::Alarm_set_Margin_createRequest(uint64_t Alarm_ID, double value, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_set_Margin");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));  

  return true;
}

bool KRPCI::Alarm_set_Margin(uint64_t Alarm_ID, double value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_set_Margin_createRequest(Alarm_ID, value, request);

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

bool KRPCI::Alarm_get_Time_createRequest(uint64_t Alarm_ID, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_get_Time");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_get_Time(uint64_t Alarm_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_get_Time_createRequest(Alarm_ID, request);

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

bool KRPCI::Alarm_set_Time_createRequest(uint64_t Alarm_ID, double value, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_set_Time");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));  

  return true;
}

bool KRPCI::Alarm_set_Time(uint64_t Alarm_ID, double value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_set_Time_createRequest(Alarm_ID, value, request);

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

bool KRPCI::Alarm_get_Type_createRequest(uint64_t Alarm_ID, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_get_Type");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_get_Type(uint64_t Alarm_ID, int32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_get_Type_createRequest(Alarm_ID, request);

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

bool KRPCI::Alarm_get_ID_createRequest(uint64_t Alarm_ID, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_get_ID");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_get_ID(uint64_t Alarm_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_get_ID_createRequest(Alarm_ID, request);

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

bool KRPCI::Alarm_get_Name_createRequest(uint64_t Alarm_ID, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_get_Name");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_get_Name(uint64_t Alarm_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_get_Name_createRequest(Alarm_ID, request);

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

bool KRPCI::Alarm_set_Name_createRequest(uint64_t Alarm_ID, std::string value, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_set_Name");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Alarm_set_Name(uint64_t Alarm_ID, std::string value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_set_Name_createRequest(Alarm_ID, value, request);

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

bool KRPCI::Alarm_get_Notes_createRequest(uint64_t Alarm_ID, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_get_Notes");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_get_Notes(uint64_t Alarm_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_get_Notes_createRequest(Alarm_ID, request);

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

bool KRPCI::Alarm_set_Notes_createRequest(uint64_t Alarm_ID, std::string value, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_set_Notes");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Alarm_set_Notes(uint64_t Alarm_ID, std::string value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_set_Notes_createRequest(Alarm_ID, value, request);

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

bool KRPCI::Alarm_get_Remaining_createRequest(uint64_t Alarm_ID, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_get_Remaining");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_get_Remaining(uint64_t Alarm_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_get_Remaining_createRequest(Alarm_ID, request);

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

bool KRPCI::Alarm_get_Repeat_createRequest(uint64_t Alarm_ID, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_get_Repeat");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_get_Repeat(uint64_t Alarm_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_get_Repeat_createRequest(Alarm_ID, request);

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

bool KRPCI::Alarm_set_Repeat_createRequest(uint64_t Alarm_ID, bool value, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_set_Repeat");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_set_Repeat(uint64_t Alarm_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_set_Repeat_createRequest(Alarm_ID, value, request);

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

bool KRPCI::Alarm_get_RepeatPeriod_createRequest(uint64_t Alarm_ID, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_get_RepeatPeriod");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_get_RepeatPeriod(uint64_t Alarm_ID, double& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_get_RepeatPeriod_createRequest(Alarm_ID, request);

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

bool KRPCI::Alarm_set_RepeatPeriod_createRequest(uint64_t Alarm_ID, double value, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_set_RepeatPeriod");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));  

  return true;
}

bool KRPCI::Alarm_set_RepeatPeriod(uint64_t Alarm_ID, double value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_set_RepeatPeriod_createRequest(Alarm_ID, value, request);

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

bool KRPCI::Alarm_get_Vessel_createRequest(uint64_t Alarm_ID, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_get_Vessel");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_get_Vessel(uint64_t Alarm_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_get_Vessel_createRequest(Alarm_ID, request);

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

bool KRPCI::Alarm_set_Vessel_createRequest(uint64_t Alarm_ID, uint64_t value, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_set_Vessel");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_set_Vessel(uint64_t Alarm_ID, uint64_t value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_set_Vessel_createRequest(Alarm_ID, value, request);

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

bool KRPCI::Alarm_get_XferOriginBody_createRequest(uint64_t Alarm_ID, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_get_XferOriginBody");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_get_XferOriginBody(uint64_t Alarm_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_get_XferOriginBody_createRequest(Alarm_ID, request);

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

bool KRPCI::Alarm_set_XferOriginBody_createRequest(uint64_t Alarm_ID, uint64_t value, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_set_XferOriginBody");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_set_XferOriginBody(uint64_t Alarm_ID, uint64_t value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_set_XferOriginBody_createRequest(Alarm_ID, value, request);

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

bool KRPCI::Alarm_get_XferTargetBody_createRequest(uint64_t Alarm_ID, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_get_XferTargetBody");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_get_XferTargetBody(uint64_t Alarm_ID, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_get_XferTargetBody_createRequest(Alarm_ID, request);

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

bool KRPCI::Alarm_set_XferTargetBody_createRequest(uint64_t Alarm_ID, uint64_t value, krpc::Request& request)
{
  request.set_service("KerbalAlarmClock");
  request.set_procedure("Alarm_set_XferTargetBody");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Alarm_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Alarm_set_XferTargetBody(uint64_t Alarm_ID, uint64_t value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Alarm_set_XferTargetBody_createRequest(Alarm_ID, value, request);

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

bool KRPCI::ServoGroupWithName_createRequest(std::string name, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ServoGroupWithName");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  return true;
}

bool KRPCI::ServoGroupWithName(std::string name, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ServoGroupWithName_createRequest(name, request);

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

bool KRPCI::ServoWithName_createRequest(std::string name, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ServoWithName");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  return true;
}

bool KRPCI::ServoWithName(std::string name, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ServoWithName_createRequest(name, request);

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

bool KRPCI::get_ServoGroups_createRequest(krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("get_ServoGroups");
  return true;
}

bool KRPCI::get_ServoGroups(std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::get_ServoGroups_createRequest(request);

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

bool KRPCI::ControlGroup_ServoWithName_createRequest(uint64_t ControlGroup_ID, std::string name, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_ServoWithName");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::ControlGroup_ServoWithName(uint64_t ControlGroup_ID, std::string name, uint64_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_ServoWithName_createRequest(ControlGroup_ID, name, request);

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

bool KRPCI::ControlGroup_MoveRight_createRequest(uint64_t ControlGroup_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_MoveRight");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ControlGroup_MoveRight(uint64_t ControlGroup_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_MoveRight_createRequest(ControlGroup_ID, request);

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

bool KRPCI::ControlGroup_MoveLeft_createRequest(uint64_t ControlGroup_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_MoveLeft");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ControlGroup_MoveLeft(uint64_t ControlGroup_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_MoveLeft_createRequest(ControlGroup_ID, request);

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

bool KRPCI::ControlGroup_MoveCenter_createRequest(uint64_t ControlGroup_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_MoveCenter");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ControlGroup_MoveCenter(uint64_t ControlGroup_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_MoveCenter_createRequest(ControlGroup_ID, request);

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

bool KRPCI::ControlGroup_MoveNextPreset_createRequest(uint64_t ControlGroup_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_MoveNextPreset");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ControlGroup_MoveNextPreset(uint64_t ControlGroup_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_MoveNextPreset_createRequest(ControlGroup_ID, request);

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

bool KRPCI::ControlGroup_MovePrevPreset_createRequest(uint64_t ControlGroup_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_MovePrevPreset");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ControlGroup_MovePrevPreset(uint64_t ControlGroup_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_MovePrevPreset_createRequest(ControlGroup_ID, request);

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

bool KRPCI::ControlGroup_Stop_createRequest(uint64_t ControlGroup_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_Stop");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ControlGroup_Stop(uint64_t ControlGroup_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_Stop_createRequest(ControlGroup_ID, request);

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

bool KRPCI::ControlGroup_get_Name_createRequest(uint64_t ControlGroup_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_get_Name");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ControlGroup_get_Name(uint64_t ControlGroup_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_get_Name_createRequest(ControlGroup_ID, request);

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

bool KRPCI::ControlGroup_set_Name_createRequest(uint64_t ControlGroup_ID, std::string value, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_set_Name");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::ControlGroup_set_Name(uint64_t ControlGroup_ID, std::string value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_set_Name_createRequest(ControlGroup_ID, value, request);

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

bool KRPCI::ControlGroup_get_ForwardKey_createRequest(uint64_t ControlGroup_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_get_ForwardKey");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ControlGroup_get_ForwardKey(uint64_t ControlGroup_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_get_ForwardKey_createRequest(ControlGroup_ID, request);

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

bool KRPCI::ControlGroup_set_ForwardKey_createRequest(uint64_t ControlGroup_ID, std::string value, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_set_ForwardKey");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::ControlGroup_set_ForwardKey(uint64_t ControlGroup_ID, std::string value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_set_ForwardKey_createRequest(ControlGroup_ID, value, request);

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

bool KRPCI::ControlGroup_get_ReverseKey_createRequest(uint64_t ControlGroup_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_get_ReverseKey");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ControlGroup_get_ReverseKey(uint64_t ControlGroup_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_get_ReverseKey_createRequest(ControlGroup_ID, request);

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

bool KRPCI::ControlGroup_set_ReverseKey_createRequest(uint64_t ControlGroup_ID, std::string value, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_set_ReverseKey");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::ControlGroup_set_ReverseKey(uint64_t ControlGroup_ID, std::string value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_set_ReverseKey_createRequest(ControlGroup_ID, value, request);

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

bool KRPCI::ControlGroup_get_Speed_createRequest(uint64_t ControlGroup_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_get_Speed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ControlGroup_get_Speed(uint64_t ControlGroup_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_get_Speed_createRequest(ControlGroup_ID, request);

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

bool KRPCI::ControlGroup_set_Speed_createRequest(uint64_t ControlGroup_ID, float value, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_set_Speed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::ControlGroup_set_Speed(uint64_t ControlGroup_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_set_Speed_createRequest(ControlGroup_ID, value, request);

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

bool KRPCI::ControlGroup_get_Expanded_createRequest(uint64_t ControlGroup_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_get_Expanded");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ControlGroup_get_Expanded(uint64_t ControlGroup_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_get_Expanded_createRequest(ControlGroup_ID, request);

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

bool KRPCI::ControlGroup_set_Expanded_createRequest(uint64_t ControlGroup_ID, bool value, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_set_Expanded");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ControlGroup_set_Expanded(uint64_t ControlGroup_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_set_Expanded_createRequest(ControlGroup_ID, value, request);

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

bool KRPCI::ControlGroup_get_Servos_createRequest(uint64_t ControlGroup_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("ControlGroup_get_Servos");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(ControlGroup_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::ControlGroup_get_Servos(uint64_t ControlGroup_ID, std::vector<uint64_t>& return_vector)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::ControlGroup_get_Servos_createRequest(ControlGroup_ID, request);

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

bool KRPCI::Servo_MoveRight_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_MoveRight");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_MoveRight(uint64_t Servo_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_MoveRight_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_MoveLeft_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_MoveLeft");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_MoveLeft(uint64_t Servo_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_MoveLeft_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_MoveCenter_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_MoveCenter");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_MoveCenter(uint64_t Servo_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_MoveCenter_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_MoveNextPreset_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_MoveNextPreset");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_MoveNextPreset(uint64_t Servo_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_MoveNextPreset_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_MovePrevPreset_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_MovePrevPreset");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_MovePrevPreset(uint64_t Servo_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_MovePrevPreset_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_MoveTo_createRequest(uint64_t Servo_ID, float position, float speed, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_MoveTo");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&position), sizeof(position));

  argument = request.add_arguments();
  argument->set_position(2);
  argument->set_value((const char*)(&speed), sizeof(speed));

  return true;
}

bool KRPCI::Servo_MoveTo(uint64_t Servo_ID, float position, float speed)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_MoveTo_createRequest(Servo_ID, position, speed, request);

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

bool KRPCI::Servo_Stop_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_Stop");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_Stop(uint64_t Servo_ID)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_Stop_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_get_Name_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_get_Name");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_get_Name(uint64_t Servo_ID, std::string& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_get_Name_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_set_Name_createRequest(uint64_t Servo_ID, std::string value, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_set_Name");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  return true;
}

bool KRPCI::Servo_set_Name(uint64_t Servo_ID, std::string value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_set_Name_createRequest(Servo_ID, value, request);

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

bool KRPCI::Servo_set_Highlight_createRequest(uint64_t Servo_ID, bool value, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_set_Highlight");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_set_Highlight(uint64_t Servo_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_set_Highlight_createRequest(Servo_ID, value, request);

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

bool KRPCI::Servo_get_Position_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_get_Position");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_get_Position(uint64_t Servo_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_get_Position_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_get_MinConfigPosition_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_get_MinConfigPosition");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_get_MinConfigPosition(uint64_t Servo_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_get_MinConfigPosition_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_get_MaxConfigPosition_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_get_MaxConfigPosition");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_get_MaxConfigPosition(uint64_t Servo_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_get_MaxConfigPosition_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_get_MinPosition_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_get_MinPosition");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_get_MinPosition(uint64_t Servo_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_get_MinPosition_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_set_MinPosition_createRequest(uint64_t Servo_ID, float value, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_set_MinPosition");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Servo_set_MinPosition(uint64_t Servo_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_set_MinPosition_createRequest(Servo_ID, value, request);

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

bool KRPCI::Servo_get_MaxPosition_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_get_MaxPosition");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_get_MaxPosition(uint64_t Servo_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_get_MaxPosition_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_set_MaxPosition_createRequest(uint64_t Servo_ID, float value, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_set_MaxPosition");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Servo_set_MaxPosition(uint64_t Servo_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_set_MaxPosition_createRequest(Servo_ID, value, request);

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

bool KRPCI::Servo_get_ConfigSpeed_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_get_ConfigSpeed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_get_ConfigSpeed(uint64_t Servo_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_get_ConfigSpeed_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_get_Speed_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_get_Speed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_get_Speed(uint64_t Servo_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_get_Speed_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_set_Speed_createRequest(uint64_t Servo_ID, float value, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_set_Speed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Servo_set_Speed(uint64_t Servo_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_set_Speed_createRequest(Servo_ID, value, request);

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

bool KRPCI::Servo_get_CurrentSpeed_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_get_CurrentSpeed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_get_CurrentSpeed(uint64_t Servo_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_get_CurrentSpeed_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_set_CurrentSpeed_createRequest(uint64_t Servo_ID, float value, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_set_CurrentSpeed");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Servo_set_CurrentSpeed(uint64_t Servo_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_set_CurrentSpeed_createRequest(Servo_ID, value, request);

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

bool KRPCI::Servo_get_Acceleration_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_get_Acceleration");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_get_Acceleration(uint64_t Servo_ID, float& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_get_Acceleration_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_set_Acceleration_createRequest(uint64_t Servo_ID, float value, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_set_Acceleration");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->set_value((const char*)(&value), sizeof(value));

  return true;
}

bool KRPCI::Servo_set_Acceleration(uint64_t Servo_ID, float value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_set_Acceleration_createRequest(Servo_ID, value, request);

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

bool KRPCI::Servo_get_IsMoving_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_get_IsMoving");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_get_IsMoving(uint64_t Servo_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_get_IsMoving_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_get_IsFreeMoving_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_get_IsFreeMoving");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_get_IsFreeMoving(uint64_t Servo_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_get_IsFreeMoving_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_get_IsLocked_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_get_IsLocked");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_get_IsLocked(uint64_t Servo_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_get_IsLocked_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_set_IsLocked_createRequest(uint64_t Servo_ID, bool value, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_set_IsLocked");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_set_IsLocked(uint64_t Servo_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_set_IsLocked_createRequest(Servo_ID, value, request);

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

bool KRPCI::Servo_get_IsAxisInverted_createRequest(uint64_t Servo_ID, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_get_IsAxisInverted");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_get_IsAxisInverted(uint64_t Servo_ID, bool& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_get_IsAxisInverted_createRequest(Servo_ID, request);

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

bool KRPCI::Servo_set_IsAxisInverted_createRequest(uint64_t Servo_ID, bool value, krpc::Request& request)
{
  request.set_service("InfernalRobotics");
  request.set_procedure("Servo_set_IsAxisInverted");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint64ToArray(Servo_ID, 
		      (unsigned char *)argument->mutable_value()->data());

  argument = request.add_arguments();
  argument->set_position(1);
  argument->mutable_value()->resize(10);
  CodedOutputStream::WriteVarint32ToArray(value, 
		      (unsigned char *)argument->mutable_value()->data());

  return true;
}

bool KRPCI::Servo_set_IsAxisInverted(uint64_t Servo_ID, bool value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::Servo_set_IsAxisInverted_createRequest(Servo_ID, value, request);

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

bool KRPCI::GetStatus_createRequest(krpc::Request& request)
{
  request.set_service("KRPC");
  request.set_procedure("GetStatus");
  return true;
}

bool KRPCI::GetStatus(krpc::Status& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::GetStatus_createRequest(request);

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

bool KRPCI::GetServices_createRequest(krpc::Request& request)
{
  request.set_service("KRPC");
  request.set_procedure("GetServices");
  return true;
}

bool KRPCI::GetServices(krpc::Services& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::GetServices_createRequest(request);

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

bool KRPCI::AddStream_createRequest(krpc::Request input_request, krpc::Request& request)
{
  request.set_service("KRPC");
  request.set_procedure("AddStream");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  return true;
}

bool KRPCI::AddStream(krpc::Request input_request, uint32_t& return_value)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::AddStream_createRequest(input_request, request);

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

bool KRPCI::RemoveStream_createRequest(uint32_t id, krpc::Request& request)
{
  request.set_service("KRPC");
  request.set_procedure("RemoveStream");
  krpc::Argument* argument;
  argument = request.add_arguments();
  argument->set_position(0);
  return true;
}

bool KRPCI::RemoveStream(uint32_t id)
{
  krpc::Request request;
  krpc::Response response;
  KRPCI::RemoveStream_createRequest(id, request);

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

