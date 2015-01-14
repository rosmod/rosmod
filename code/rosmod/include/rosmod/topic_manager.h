#ifndef ROSMOD_TOPIC_MANAGER_H
#define ROSMOD_TOPIC_MANAGER_H

#include "ros/common.h"
#include "ros/serialization.h"
#include "ros/rosout_appender.h"
#include "rosmod/subscription.h"
#include "rosmod/publication.h"
#include "rosmod/advertise_options.h"

#include "XmlRpcValue.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace ros
{

class Message;
struct SubscribeOptions2;
struct AdvertiseOptions2;

class TopicManager2;
typedef boost::shared_ptr<TopicManager2> TopicManagerPtr2;

class PollManager;
typedef boost::shared_ptr<PollManager> PollManagerPtr;

class XMLRPCManager;
typedef boost::shared_ptr<XMLRPCManager> XMLRPCManagerPtr;

class ConnectionManager;
typedef boost::shared_ptr<ConnectionManager> ConnectionManagerPtr;

class SubscriptionCallbackHelper;
typedef boost::shared_ptr<SubscriptionCallbackHelper> SubscriptionCallbackHelperPtr;

typedef std::vector<PublicationPtr2> V_Publication2;
typedef boost::shared_ptr<Subscription2> SubscriptionPtr2;

struct SubscriberCallbacks2;
typedef boost::shared_ptr<SubscriberCallbacks2> SubscriberCallbacksPtr2;

class TopicManager2
{
public:
  static const TopicManagerPtr2& instance();

  TopicManager2();
  ~TopicManager2();

  void start();
  void shutdown();

  bool subscribe(const SubscribeOptions2& ops);
  bool unsubscribe(const std::string &_topic, const SubscriptionCallbackHelperPtr& helper);

  bool advertise(const AdvertiseOptions2& ops, const SubscriberCallbacksPtr2& callbacks);
  bool unadvertise(const std::string &topic, const SubscriberCallbacksPtr2& callbacks);

  /** @brief Get the list of topics advertised by this node
   *
   * @param[out] topics The advertised topics
   */
  void getAdvertisedTopics(V_string& topics);

  /** @brief Get the list of topics subscribed to by this node
   *
   * @param[out] The subscribed topics
   */
  void getSubscribedTopics(V_string& topics);

  /** @brief Lookup an advertised topic.
   *
   * This method iterates over advertised_topics, looking for one with name
   * matching the given topic name.  The advertised_topics_mutex is locked
   * during this search.  This method is only used internally.
   *
   * @param topic The topic name to look for.
   *
   * @returns Pointer to the matching Publication, NULL if none is found.
   */
  PublicationPtr2 lookupPublication(const std::string& topic);

  /** @brief Return the number of subscribers a node has for a particular topic:
   *
   * @param _topic The topic name to check
   *
   * @return number of subscribers
   */
  size_t getNumSubscribers(const std::string &_topic);
  size_t getNumSubscriptions();

  /**
   * \brief Return the number of publishers connected to this node on a particular topic
   *
   * \param _topic the topic name to check
   * \return the number of subscribers
   */
  size_t getNumPublishers(const std::string &_topic);

  template<typename M>
  void publish(const std::string& topic, const M& message)
  {
    using namespace serialization;

    SerializedMessage m;
    publish(topic, boost::bind(serializeMessage<M>, boost::ref(message)), m);
  }

  void publish(const std::string &_topic, const boost::function<SerializedMessage(void)>& serfunc, SerializedMessage& m);

  void incrementSequence(const std::string &_topic);
  bool isLatched(const std::string& topic);

private:
  /** if it finds a pre-existing subscription to the same topic and of the
   *  same message type, it appends the Functor to the callback vector for
   *  that subscription. otherwise, it returns false, indicating that a new
   *  subscription needs to be created.
   */
  bool addSubCallback(const SubscribeOptions2& ops);

  /** @brief Request a topic
   *
   * Negotiate a subscriber connection on a topic.
   *
   * @param topic The topic of interest.
   * @param protos List of transport protocols, in preference order
   * @param ret Return value
   *
   * @return true on success, false otherwise
   *
   * @todo Consider making this private
   */
  bool requestTopic(const std::string &topic, XmlRpc::XmlRpcValue &protos, XmlRpc::XmlRpcValue &ret);

  // Must lock the advertised topics mutex before calling this function
  bool isTopicAdvertised(const std::string& topic);

  bool registerSubscriber(const SubscriptionPtr2& s, const std::string& datatype);
  bool unregisterSubscriber(const std::string& topic);
  bool unregisterPublisher(const std::string& topic);

  PublicationPtr2 lookupPublicationWithoutLock(const std::string &topic);

  void processPublishQueues();

  /** @brief Compute the statistics for the node's connectivity
   *
   * This is the implementation of the xml-rpc getBusStats function;
   * it populates the XmlRpcValue object sent to it with various statistics
   * about the node's connectivity, bandwidth utilization, etc.
   */
  void getBusStats(XmlRpc::XmlRpcValue &stats);

  /** @brief Compute the info for the node's connectivity
   *
   * This is the implementation of the xml-rpc getBusInfo function;
   * it populates the XmlRpcValue object sent to it with various info
   * about the node's connectivity.
   */
  void getBusInfo(XmlRpc::XmlRpcValue &info);

  /** @brief Return the list of subcriptions for the node
   *
   * This is the implementation of the xml-rpc getSubscriptions
   * function; it populates the XmlRpcValue object sent to it with the
   * list of subscribed topics and their datatypes.
   */
  void getSubscriptions(XmlRpc::XmlRpcValue &subscriptions);

  /** @brief Return the list of advertised topics for the node
   *
   * This is the implementation of the xml-rpc getPublications
   * function; it populates the XmlRpcValue object sent to it with the
   * list of advertised topics and their datatypes.
   */
  void getPublications(XmlRpc::XmlRpcValue &publications);

  /** @brief Update local publisher lists.
   *
   * Use this method to update address information for publishers on a
   * given topic.
   *
   * @param topic The topic of interest
   * @param pubs The list of publishers to update.
   *
   * @return true on success, false otherwise.
   */
  bool pubUpdate(const std::string &topic, const std::vector<std::string> &pubs);

  void pubUpdateCallback(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
  void requestTopicCallback(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
  void getBusStatsCallback(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
  void getBusInfoCallback(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
  void getSubscriptionsCallback(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
  void getPublicationsCallback(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);

  bool isShuttingDown() { return shutting_down_; }

  boost::mutex subs_mutex_;
  std::list<SubscriptionPtr2> subscriptions_;

  boost::recursive_mutex advertised_topics_mutex_;
  V_Publication2 advertised_topics_;
  std::list<std::string> advertised_topic_names_;
  boost::mutex advertised_topic_names_mutex_;

  volatile bool shutting_down_;
  boost::mutex shutting_down_mutex_;

  PollManagerPtr poll_manager_;
  ConnectionManagerPtr connection_manager_;
  XMLRPCManagerPtr xmlrpc_manager_;
};

} // namespace ros

#endif // ROSCPP_TOPIC_MANAGER_H
