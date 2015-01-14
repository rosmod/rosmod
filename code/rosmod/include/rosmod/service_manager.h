#ifndef ROSMOD_SERVICE_MANAGER_H
#define ROSMOD_SERVICE_MANAGER_H

#include "ros/forwards.h"
#include "ros/common.h"
#include "rosmod/advertise_service_options.h"
#include "ros/service_manager.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace ros
{

class ServiceManager2;
typedef boost::shared_ptr<ServiceManager2> ServiceManager2Ptr;

class ServiceManager2 // : ServiceManager
{
public:
  static const ServiceManager2Ptr& instance();
  bool advertiseService(const AdvertiseServiceOptions2& ops);
  bool unadvertiseService(const std::string& serv_name);

private:
  bool isServiceAdvertised(const std::string& serv_name);
  bool unregisterService(const std::string& service);

  bool isShuttingDown() { return shutting_down_; }

  L_ServicePublication service_publications_;
  boost::mutex service_publications_mutex_;

  L_ServiceServerLink service_server_links_;
  boost::mutex service_server_links_mutex_;

  volatile bool shutting_down_;
  boost::recursive_mutex shutting_down_mutex_;

  PollManagerPtr poll_manager_;
  ConnectionManagerPtr connection_manager_;
  XMLRPCManagerPtr xmlrpc_manager_;
};

}

#endif
