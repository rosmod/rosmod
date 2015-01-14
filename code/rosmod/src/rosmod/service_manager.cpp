#include <cstdio>
#include "ros/service_manager.h"
#include "ros/xmlrpc_manager.h"
#include "ros/connection_manager.h"
#include "ros/poll_manager.h"
#include "ros/service_publication.h"
#include "ros/service_client_link.h"
#include "ros/service_server_link.h"
#include "ros/this_node.h"
#include "ros/network.h"
#include "ros/master.h"
#include "ros/transport/transport_tcp.h"
#include "ros/transport/transport_udp.h"
#include "ros/init.h"
#include "ros/connection.h"
#include "ros/file_log.h"

#include "XmlRpc.h"

#include "ros/console.h"
#include "rosmod/advertise_service_options.h"
#include "rosmod/service_manager.h"

using namespace XmlRpc; // A battle to be fought later
using namespace std; // sigh

namespace ros
{

ServiceManager2Ptr g_service_manager;
boost::mutex g_service_manager_mutex;
const ServiceManager2Ptr& ServiceManager2::instance()
{
  if (!g_service_manager)
  {
    boost::mutex::scoped_lock lock(g_service_manager_mutex);
    if (!g_service_manager)
    {
      g_service_manager.reset(new ServiceManager2);
    }
  }

  return g_service_manager;
}

bool ServiceManager2::advertiseService(const AdvertiseServiceOptions2& ops)
{
  boost::recursive_mutex::scoped_lock shutdown_lock(shutting_down_mutex_);
  if (shutting_down_)
  {
    return false;
  }

  {
    boost::mutex::scoped_lock lock(service_publications_mutex_);

    if (isServiceAdvertised(ops.service))
    {
      ROS_ERROR("Tried to advertise a service that is already advertised in this node [%s]", ops.service.c_str());
      return false;
    }

    ServicePublicationPtr pub(new ServicePublication(ops.service, ops.md5sum, ops.datatype, ops.req_datatype, ops.res_datatype, ops.helper, ops.callback_queue, ops.tracked_object));
    service_publications_.push_back(pub);
  }

  XmlRpcValue args, result, payload;
  args[0] = this_node::getName();
  args[1] = ops.service;
  char uri_buf[1024];
  snprintf(uri_buf, sizeof(uri_buf), "rosrpc://%s:%d",
           network::getHost().c_str(), connection_manager_->getTCPPort());
  args[2] = string(uri_buf);
  args[3] = xmlrpc_manager_->getServerURI();
  master::execute("registerService", args, result, payload, true);

  return true;
}

bool ServiceManager2::unadvertiseService(const string &serv_name)
{
  boost::recursive_mutex::scoped_lock shutdown_lock(shutting_down_mutex_);
  if (shutting_down_)
  {
    return false;
  }

  ServicePublicationPtr pub;
  {
    boost::mutex::scoped_lock lock(service_publications_mutex_);

    for (L_ServicePublication::iterator i = service_publications_.begin();
         i != service_publications_.end(); ++i)
    {
      if((*i)->getName() == serv_name && !(*i)->isDropped())
      {
        pub = *i;
        service_publications_.erase(i);
        break;
      }
    }
  }

  if (pub)
  {
    unregisterService(pub->getName());
    ROSCPP_LOG_DEBUG( "shutting down service [%s]", pub->getName().c_str());
    pub->drop();
    return true;
  }

  return false;
}

bool ServiceManager2::isServiceAdvertised(const string& serv_name)
{
  for (L_ServicePublication::iterator s = service_publications_.begin(); s != service_publications_.end(); ++s)
  {
    if (((*s)->getName() == serv_name) && !(*s)->isDropped())
    {
      return true;
    }
  }

  return false;
}

bool ServiceManager2::unregisterService(const std::string& service)
{
  XmlRpcValue args, result, payload;
  args[0] = this_node::getName();
  args[1] = service;
  char uri_buf[1024];
  snprintf(uri_buf, sizeof(uri_buf), "rosrpc://%s:%d",
           network::getHost().c_str(), connection_manager_->getTCPPort());
  args[2] = string(uri_buf);

  master::execute("unregisterService", args, result, payload, false);

  return true;
}

}
