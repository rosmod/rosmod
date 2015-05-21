#ifndef ROSMOD_INTRAPROCESS_PUBLISHER_LINK_H
#define ROSMOD_INTRAPROCESS_PUBLISHER_LINK_H

#include "rosmod/publisher_link.h"
#include "ros/common.h"

#include <boost/thread/recursive_mutex.hpp>

namespace ros
{
class Subscription2;
typedef boost::shared_ptr<Subscription2> SubscriptionPtr2;
typedef boost::weak_ptr<Subscription2> SubscriptionWPtr2;

class IntraProcessSubscriberLink2;
typedef boost::shared_ptr<IntraProcessSubscriberLink2> IntraProcessSubscriberLinkPtr2;

/**
 * \brief Handles a connection to a single publisher on a given topic.  Receives messages from a publisher
 * and hands them off to its parent Subscription
 */
class IntraProcessPublisherLink2 : public PublisherLink2
{
public:
  IntraProcessPublisherLink2(const SubscriptionPtr2& parent, const std::string& xmlrpc_uri, const TransportHints& transport_hints);
  virtual ~IntraProcessPublisherLink2();

  void setPublisher(const IntraProcessSubscriberLinkPtr2& publisher);

  virtual std::string getTransportType();
  virtual std::string getTransportInfo();
  virtual void drop();

  /**
   * \brief Handles handing off a received message to the subscription, where it will be deserialized and called back
   */
  virtual void handleMessage(const SerializedMessage& m, bool ser, bool nocopy);

  void getPublishTypes(bool& ser, bool& nocopy, const std::type_info& ti);

private:
  IntraProcessSubscriberLinkPtr2 publisher_;
  bool dropped_;
  boost::recursive_mutex drop_mutex_;
};
typedef boost::shared_ptr<IntraProcessPublisherLink2> IntraProcessPublisherLinkPtr2;

} // namespace ros

#endif // ROSCPP_INTRAPROCESS_PUBLISHER_LINK_H



