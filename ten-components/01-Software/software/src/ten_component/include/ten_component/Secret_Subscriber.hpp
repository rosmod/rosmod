#ifndef SECRET_SUBSCRIBER_HPP
#define SECRET_SUBSCRIBER_HPP
#include "node/Component.hpp"
#include "ten_component/secret_message.h"
#include "ten_component/secret_service.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif



//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Secret_Subscriber : public Component
{
public:
  // Constructor
  Secret_Subscriber(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - secret_message_subscriber
  void secret_message_subscriber_operation(const ten_component::secret_message::ConstPtr& received_data); 
 
  // Server Operation - secret_server
  bool secret_service_operation(ten_component::secret_service::Request &req, 
    ten_component::secret_service::Response &res);

  // Start up
  void startUp();

  // Destructor
  ~Secret_Subscriber();

private:

  // Subscriber
  NAMESPACE::Subscriber secret_message_subscriber;

  // Server 
  NAMESPACE::ServiceServer secret_server;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
