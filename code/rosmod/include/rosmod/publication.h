#ifndef ROSMOD_PUBLICATION_H
#define ROSMOD_PUBLICATION_H

#include "rosmod/advertise_options.h"
#include "ros/common.h"
#include "XmlRpc.h"

#include <boost/thread/mutex.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#include <vector>

namespace ros
{

class SubscriberLink2;
typedef boost::shared_ptr<SubscriberLink2> SubscriberLinkPtr2;
typedef std::vector<SubscriberLinkPtr2> V_SubscriberLink2;

class SingleSubscriberPublisher2;
typedef boost::function<void(const SingleSubscriberPublisher2&)> SubscriberStatusCallback2;

struct SubscriberCallbacks2
{
  SubscriberCallbacks2(const SubscriberStatusCallback2& connect = SubscriberStatusCallback2(),
                      const SubscriberStatusCallback2& disconnect = SubscriberStatusCallback2(),
                      const VoidConstPtr& tracked_object = VoidConstPtr(),
                      CMQ* callback_queue = 0, int priority = 0)
  : connect_(connect)
  , disconnect_(disconnect)
  , callback_queue_(callback_queue)
  , priority_(priority)
  {
    has_tracked_object_ = false;
    if (tracked_object)
    {
      has_tracked_object_ = true;
      tracked_object_ = tracked_object;
    }
  }
  SubscriberStatusCallback2 connect_;
  SubscriberStatusCallback2 disconnect_;

  bool has_tracked_object_;
  VoidConstWPtr tracked_object_;
  CMQ* callback_queue_;
  int priority_;
};
typedef boost::shared_ptr<SubscriberCallbacks2> SubscriberCallbacksPtr2;

/**
 * \brief A Publication manages an advertised topic
 */
class Publication2
{
public:
  Publication2(const std::string &name,
            const std::string& datatype,
            const std::string& _md5sum,
            const std::string& message_definition,
            size_t max_queue,
            bool latch,
            bool has_header);

  ~Publication2();

  void addCallbacks(const SubscriberCallbacksPtr2& callbacks);
  void removeCallbacks(const SubscriberCallbacksPtr2& callbacks);

  /**
   * \brief queues an outgoing message into each of the publishers, so that it gets sent to every subscriber
   */
  bool enqueueMessage(const SerializedMessage& m);
  /**
   * \brief returns the max queue size of this publication
   */
  inline size_t getMaxQueue() { return max_queue_; }
  /**
   * \brief Get the accumulated stats for this publication
   */
  XmlRpc::XmlRpcValue getStats();
  /**
   * \brief Get the accumulated info for this publication
   */
  void getInfo(XmlRpc::XmlRpcValue& info);

  /**
   * \brief Returns whether or not this publication has any subscribers
   */
  bool hasSubscribers();
  /**
   * \brief Returns the number of subscribers this publication has
   */
  uint32_t getNumSubscribers();

  void getPublishTypes(bool& serialize, bool& nocopy, const std::type_info& ti);

  /**
   * \brief Returns the name of the topic this publication broadcasts to
   */
  const std::string& getName() const { return name_; }
  /**
   * \brief Returns the data type of the message published by this publication
   */
  const std::string& getDataType() const { return datatype_; }
  /**
   * \brief Returns the md5sum of the message published by this publication
   */
  const std::string& getMD5Sum() const { return md5sum_; }
  /**
   * \brief Returns the full definition of the message published by this publication
   */
  const std::string& getMessageDefinition() const { return message_definition_; }
  /**
   * \brief Returns the sequence number
   */
  uint32_t getSequence() { return seq_; }

  bool isLatched() { return latch_; }

  /**
   * \brief Adds a publisher to our list
   */
  void addSubscriberLink(const SubscriberLinkPtr2& sub_link);
  /**
   * \brief Removes a publisher from our list (deleting it if it's the last reference)
   */
  void removeSubscriberLink(const SubscriberLinkPtr2& sub_link);

  /**
   * \brief Drop this publication.  Disconnects all publishers.
   */
  void drop();
  /**
   * \brief Returns if this publication is valid or not
   */
  bool isDropped() { return dropped_; }

  uint32_t incrementSequence();

  size_t getNumCallbacks();

  bool isLatching() { return latch_; }

  void publish(SerializedMessage& m);
  void processPublishQueue();

  bool validateHeader(const Header& h, std::string& error_msg);

private:
  void dropAllConnections();

  /**
   * \brief Called when a new peer has connected. Calls the connection callback
   */
  void peerConnect(const SubscriberLinkPtr2& sub_link);
  /**
   * \brief Called when a peer has disconnected. Calls the disconnection callback
   */
  void peerDisconnect(const SubscriberLinkPtr2& sub_link);

  std::string name_;
  std::string datatype_;
  std::string md5sum_;
  std::string message_definition_;
  size_t max_queue_;
  uint32_t seq_;
  boost::mutex seq_mutex_;

  typedef std::vector<SubscriberCallbacksPtr2> V_Callback2;
  V_Callback2 callbacks_;
  boost::mutex callbacks_mutex_;

  V_SubscriberLink2 subscriber_links_;
  // We use a recursive mutex here for the rare case that a publish call causes another one (like in the case of a rosconsole call)
  boost::mutex subscriber_links_mutex_;

  bool dropped_;

  bool latch_;
  bool has_header_;
  SerializedMessage last_message_;

  uint32_t intraprocess_subscriber_count_;

  typedef std::vector<SerializedMessage> V_SerializedMessage;
  V_SerializedMessage publish_queue_;
  boost::mutex publish_queue_mutex_;
};

}

#endif // ROSCPP_PUBLICATION_H
