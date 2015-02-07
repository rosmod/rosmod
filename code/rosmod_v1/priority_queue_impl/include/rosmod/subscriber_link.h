#ifndef ROSMOD_SUBSCRIBER_LINK_H
#define ROSMOD_SUBSCRIBER_LINK_H

#include "ros/common.h"

#include <boost/thread/mutex.hpp>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <queue>

namespace ros
{
class Header;
class Message;
class Publication2;
typedef boost::shared_ptr<Publication2> PublicationPtr2;
typedef boost::weak_ptr<Publication2> PublicationWPtr2;
class Connection;
typedef boost::shared_ptr<Connection> ConnectionPtr;

class SubscriberLink2 : public boost::enable_shared_from_this<SubscriberLink2>
{
public:
  class Stats
  {
  public:
    uint64_t bytes_sent_, message_data_sent_, messages_sent_;
    Stats()
    : bytes_sent_(0), message_data_sent_(0), messages_sent_(0) { }
  };

  SubscriberLink2();
  virtual ~SubscriberLink2();

  const std::string& getTopic() const { return topic_; }
  const Stats &getStats() { return stats_; }
  const std::string &getDestinationCallerID() const { return destination_caller_id_; }
  int getConnectionID() const { return connection_id_; }

  /**
   * \brief Queue up a message for publication.  Throws out old messages if we've reached our Publication's max queue size
   */
  virtual void enqueueMessage(const SerializedMessage& m, bool nocopy, bool ser) = 0;

  virtual void drop() = 0;

  virtual std::string getTransportType() = 0;
  virtual std::string getTransportInfo() = 0;

  virtual bool isIntraprocess() { return false; }
  virtual void getPublishTypes(bool& ser, bool& nocopy, const std::type_info& ti) { ser = true; nocopy = false; }

  const std::string& getMD5Sum();
  const std::string& getDataType();
  const std::string& getMessageDefinition();

protected:
  bool verifyDatatype(const std::string &datatype);

  PublicationWPtr2 parent_;
  unsigned int connection_id_;
  std::string destination_caller_id_;
  Stats stats_;
  std::string topic_;
};

} // namespace ros

#endif


