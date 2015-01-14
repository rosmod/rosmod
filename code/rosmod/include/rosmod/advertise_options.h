#ifndef ROSMOD_ADVERTISE_OPTIONS_H
#define ROSMOD_ADVERTISE_OPTIONS_H

#include "ros/forwards.h"
#include "ros/message_traits.h"
#include "ros/common.h"

#include "rosmod/cmq.h"
#include "ros/advertise_options.h"
#include "rosmod/publication.h"

namespace ros
{

class SingleSubscriberPublisher2;
typedef boost::function<void(const SingleSubscriberPublisher2&)> SubscriberStatusCallback2;

/**
 * \brief Encapsulates all options available for creating a Publisher
 */
struct AdvertiseOptions2
{
  AdvertiseOptions2()
  : callback_queue(0)
  , priority(0)
  , latch(false)
  {
  }

  /*
   * \brief Constructor
   * \param _topic Topic to publish on
   * \param _queue_size Maximum number of outgoing messages to be queued for delivery to subscribers
   * \param _md5sum The md5sum of the message datatype published on this topic
   * \param _datatype Datatype of the message published on this topic (eg. "std_msgs/String")
   * \param _connect_cb Function to call when a subscriber connects to this topic
   * \param _disconnect_cb Function to call when a subscriber disconnects from this topic
   */
  AdvertiseOptions2(const std::string& _topic, uint32_t _queue_size, const std::string& _md5sum,
                   const std::string& _datatype, const std::string& _message_definition,
                   const SubscriberStatusCallback2& _connect_cb = SubscriberStatusCallback2(),
                   const SubscriberStatusCallback2& _disconnect_cb = SubscriberStatusCallback2())
  : topic(_topic)
  , queue_size(_queue_size)
  , md5sum(_md5sum)
  , datatype(_datatype)
  , message_definition(_message_definition)
  , connect_cb(_connect_cb)
  , disconnect_cb(_disconnect_cb)
  , callback_queue(0)
  , priority(0)
  , latch(false)
  , has_header(false)
  {}

  /**
   * \brief templated helper function for automatically filling out md5sum, datatype and message definition
   *
   * \param M [template] Message type
   * \param _topic Topic to publish on
   * \param _queue_size Maximum number of outgoing messages to be queued for delivery to subscribers
   * \param _connect_cb Function to call when a subscriber connects to this topic
   * \param _disconnect_cb Function to call when a subscriber disconnects from this topic
   */
  template <class M>
  void init(const std::string& _topic, uint32_t _queue_size,
            const SubscriberStatusCallback2& _connect_cb = SubscriberStatusCallback2(),
            const SubscriberStatusCallback2& _disconnect_cb = SubscriberStatusCallback2())
  {
    topic = _topic;
    queue_size = _queue_size;
    connect_cb = _connect_cb;
    disconnect_cb = _disconnect_cb;
    md5sum = message_traits::md5sum<M>();
    datatype = message_traits::datatype<M>();
    message_definition = message_traits::definition<M>();
    has_header = message_traits::hasHeader<M>();
  }

  std::string topic;                                                ///< The topic to publish on
  uint32_t queue_size;                                              ///< The maximum number of outgoing messages to be queued for delivery to subscribers

  std::string md5sum;                                               ///< The md5sum of the message datatype published on this topic
  std::string datatype;                                             ///< The datatype of the message published on this topic (eg. "std_msgs/String")
  std::string message_definition;                                   ///< The full definition of the message published on this topic

  SubscriberStatusCallback2 connect_cb;                              ///< The function to call when a subscriber connects to this topic
  SubscriberStatusCallback2 disconnect_cb;                           ///< The function to call when a subscriber disconnects from this topic

  CMQ* callback_queue;                           ///< Queue to add callbacks to.  If NULL, the global callback queue will be used
  int priority;

  /**
   * \brief An object whose destruction will prevent the callbacks associated with this advertisement from being called
   *
   * A shared pointer to an object to track for these callbacks.  If set, the a weak_ptr will be created to this object,
   * and if the reference count goes to 0 the subscriber callbacks will not get called.
   *
   * \note Note that setting this will cause a new reference to be added to the object before the
   * callback, and for it to go out of scope (and potentially be deleted) in the code path (and therefore
   * thread) that the callback is invoked from.
   */
  VoidConstPtr tracked_object;

  /**
   * \brief Whether or not this publication should "latch".  A latching publication will automatically send out the last published message
   * to any new subscribers.
   */
  bool latch;

  /** \brief Tells whether or not the message has a header.  If it does, the sequence number will be written directly into the
   *         serialized bytes after the message has been serialized.
   */
  bool has_header;


  /**
   * \brief Templated helper function for creating an AdvertiseOptions for a message type with most options.
   *
   * \param M [template] Message type
   * \param topic Topic to publish on
   * \param queue_size Maximum number of outgoing messages to be queued for delivery to subscribers
   * \param connect_cb Function to call when a subscriber connects to this topic
   * \param disconnect_cb Function to call when a subscriber disconnects from this topic
   * \param tracked_object tracked object to use (see AdvertiseOptions::tracked_object)
   * \param queue The callback queue to use (see AdvertiseOptions::callback_queue)
   *
   * \return an AdvertiseOptions which embodies the parameters
   */
  template<class M>
  static AdvertiseOptions2 create(const std::string& topic, uint32_t queue_size,
                          const SubscriberStatusCallback2& connect_cb,
                          const SubscriberStatusCallback2& disconnect_cb,
                          const VoidConstPtr& tracked_object,
                          CMQ* queue, int priority)
  {
    AdvertiseOptions2 ops;
    ops.init<M>(topic, queue_size, connect_cb, disconnect_cb);
    ops.tracked_object = tracked_object;
    ops.callback_queue = queue;
    ops.priority = priority;
    return ops;
  }
};


}

#endif
