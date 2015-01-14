#ifndef ROSMOD_TRANSPORT_SUBSCRIBER_LINK_H
#define ROSMOD_TRANSPORT_SUBSCRIBER_LINK_H
#include "ros/common.h"
#include "rosmod/subscriber_link.h"

#include <boost/signals2/connection.hpp>

namespace ros
{

/**
 * \brief SubscriberLink handles broadcasting messages to a single subscriber on a single topic
 */
class TransportSubscriberLink2 : public SubscriberLink2
{
public:
  TransportSubscriberLink2();
  virtual ~TransportSubscriberLink2();

  //
  bool initialize(const ConnectionPtr& connection);
  bool handleHeader(const Header& header);

  const ConnectionPtr& getConnection() { return connection_; }

  virtual void enqueueMessage(const SerializedMessage& m, bool ser, bool nocopy);
  virtual void drop();
  virtual std::string getTransportType();
  virtual std::string getTransportInfo();

private:
  void onConnectionDropped(const ConnectionPtr& conn);

  void onHeaderWritten(const ConnectionPtr& conn);
  void onMessageWritten(const ConnectionPtr& conn);
  void startMessageWrite(bool immediate_write);

  bool writing_message_;
  bool header_written_;

  ConnectionPtr connection_;
  boost::signals2::connection dropped_conn_;

  std::queue<SerializedMessage> outbox_;
  boost::mutex outbox_mutex_;
  bool queue_full_;
};
typedef boost::shared_ptr<TransportSubscriberLink2> TransportSubscriberLinkPtr2;

}

#endif
