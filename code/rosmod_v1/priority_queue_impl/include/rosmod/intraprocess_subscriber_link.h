#ifndef ROSMOD_INTRAPROCESS_SUBSCRIBER_LINK_H
#define ROSMOD_INTRAPROCESS_SUBSCRIBER_LINK_H
#include "rosmod/subscriber_link.h"
#include "ros/common.h"

#include <boost/thread/recursive_mutex.hpp>

namespace ros
{

class IntraProcessPublisherLink2;
typedef boost::shared_ptr<IntraProcessPublisherLink2> IntraProcessPublisherLinkPtr2;

/**
 * \brief SubscriberLink handles broadcasting messages to a single subscriber on a single topic
 */
class IntraProcessSubscriberLink2 : public SubscriberLink2
{
public:
  IntraProcessSubscriberLink2(const PublicationPtr2& parent);
  virtual ~IntraProcessSubscriberLink2();

  void setSubscriber(const IntraProcessPublisherLinkPtr2& subscriber);
  bool isLatching();

  virtual void enqueueMessage(const SerializedMessage& m, bool ser, bool nocopy);
  virtual void drop();
  virtual std::string getTransportType();
  virtual std::string getTransportInfo();
  virtual bool isIntraprocess() { return true; }
  virtual void getPublishTypes(bool& ser, bool& nocopy, const std::type_info& ti);

private:
  IntraProcessPublisherLinkPtr2 subscriber_;
  bool dropped_;
  boost::recursive_mutex drop_mutex_;
};
typedef boost::shared_ptr<IntraProcessSubscriberLink2> IntraProcessSubscriberLinkPtr2;

} // namespace ros

#endif
