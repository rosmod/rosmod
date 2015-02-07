#ifndef ROSMOD_TRANSPORT_PUBLISHER_LINK_H
#define ROSMOD_TRANSPORT_PUBLISHER_LINK_H

#include "ros/common.h"
#include "rosmod/publisher_link.h"
#include "ros/connection.h"

namespace ros
{
class Header;
class Message;
class Subscription2;
typedef boost::shared_ptr<Subscription2> SubscriptionPtr2;
typedef boost::weak_ptr<Subscription2> SubscriptionWPtr2;
class Connection;
typedef boost::shared_ptr<Connection> ConnectionPtr;

class WallTimerEvent;

/**
 * \brief Handles a connection to a single publisher on a given topic.  Receives messages from a publisher
 * and hands them off to its parent Subscription
 */
class TransportPublisherLink2 : public PublisherLink2
{
public:
  TransportPublisherLink2(const SubscriptionPtr2& parent, const std::string& xmlrpc_uri, const TransportHints& transport_hints);
  virtual ~TransportPublisherLink2();

  //
  bool initialize(const ConnectionPtr& connection);

  const ConnectionPtr& getConnection() { return connection_; }

  virtual std::string getTransportType();
  virtual std::string getTransportInfo();
  virtual void drop();

private:
  void onConnectionDropped(const ConnectionPtr& conn, Connection::DropReason reason);
  bool onHeaderReceived(const ConnectionPtr& conn, const Header& header);

  /**
   * \brief Handles handing off a received message to the subscription, where it will be deserialized and called back
   */
  virtual void handleMessage(const SerializedMessage& m, bool ser, bool nocopy);

  void onHeaderWritten(const ConnectionPtr& conn);
  void onMessageLength(const ConnectionPtr& conn, const boost::shared_array<uint8_t>& buffer, uint32_t size, bool success);
  void onMessage(const ConnectionPtr& conn, const boost::shared_array<uint8_t>& buffer, uint32_t size, bool success);

  void onRetryTimer(const ros::WallTimerEvent&);

  ConnectionPtr connection_;

  int32_t retry_timer_handle_;
  bool needs_retry_;
  WallDuration retry_period_;
  WallTime next_retry_;
  bool dropping_;
};
typedef boost::shared_ptr<TransportPublisherLink2> TransportPublisherLinkPtr2;

} // namespace ros

#endif



