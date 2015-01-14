#ifndef ROSMOD_SUBSCRIBE_OPTIONS_H
#define ROSMOD_SUBSCRIBE_OPTIONS_H

#include "ros/forwards.h"
#include "ros/common.h"
#include "ros/transport_hints.h"
#include "ros/message_traits.h"
#include "ros/subscription_callback_helper.h"
#include "rosmod/cmq.h"

namespace ros
{

/**
 * \brief Encapsulates all options available for creating a Subscriber
 */
struct SubscribeOptions2
{
  /**
   *
   */
  SubscribeOptions2()
  : queue_size(1)
  , callback_queue(0)
  , priority(0)
  , allow_concurrent_callbacks(false)
  {
  }

  /**
   * \brief Constructor
   * \param _topic Topic to subscribe on
   * \param _queue_size Number of incoming messages to queue up for
   *        processing (messages in excess of this queue capacity will be
   *        discarded).
   * \param _md5sum
   * \param _datatype
   */
  SubscribeOptions2(const std::string& _topic, uint32_t _queue_size, const std::string& _md5sum, const std::string& _datatype)
  : topic(_topic)
  , queue_size(_queue_size)
  , md5sum(_md5sum)
  , datatype(_datatype)
  , callback_queue(0)
  , priority(0)
  , allow_concurrent_callbacks(false)
  {}

  /**
   * \brief Templated initialization, templated on callback parameter type.  Supports any callback parameters supported by the SubscriptionCallbackAdapter
   * \param _topic Topic to subscribe on
   * \param _queue_size Number of incoming messages to queue up for
   *        processing (messages in excess of this queue capacity will be
   *        discarded).
   * \param _callback Callback to call when a message arrives on this topic
   */
  template<class P>
  void initByFullCallbackType(const std::string& _topic, uint32_t _queue_size,
       const boost::function<void (P)>& _callback,
       const boost::function<boost::shared_ptr<typename ParameterAdapter<P>::Message>(void)>& factory_fn = DefaultMessageCreator<typename ParameterAdapter<P>::Message>())
  {
    typedef typename ParameterAdapter<P>::Message MessageType;
    topic = _topic;
    queue_size = _queue_size;
    md5sum = message_traits::md5sum<MessageType>();
    datatype = message_traits::datatype<MessageType>();
    helper = SubscriptionCallbackHelperPtr(new SubscriptionCallbackHelperT<P>(_callback, factory_fn));
  }

  /**
   * \brief Templated initialization, templated on message type.  Only supports "const boost::shared_ptr<M const>&" callback types
   * \param _topic Topic to subscribe on
   * \param _queue_size Number of incoming messages to queue up for
   *        processing (messages in excess of this queue capacity will be
   *        discarded).
   * \param _callback Callback to call when a message arrives on this topic
   */
  template<class M>
  void init(const std::string& _topic, uint32_t _queue_size,
       const boost::function<void (const boost::shared_ptr<M const>&)>& _callback,
       const boost::function<boost::shared_ptr<M>(void)>& factory_fn = DefaultMessageCreator<M>())
  {
    typedef typename ParameterAdapter<M>::Message MessageType;
    topic = _topic;
    queue_size = _queue_size;
    md5sum = message_traits::md5sum<MessageType>();
    datatype = message_traits::datatype<MessageType>();
    helper = SubscriptionCallbackHelperPtr(new SubscriptionCallbackHelperT<const boost::shared_ptr<MessageType const>&>(_callback, factory_fn));
  }

  std::string topic;                                                ///< Topic to subscribe to
  uint32_t queue_size;                                              ///< Number of incoming messages to queue up for processing (messages in excess of this queue capacity will be discarded).

  std::string md5sum;                                               ///< MD5 of the message datatype
  std::string datatype;                                             ///< Datatype of the message we'd like to subscribe as

  SubscriptionCallbackHelperPtr helper;                              ///< Helper object used to get create messages and call callbacks

  CMQ* callback_queue;                           ///< Queue to add callbacks to.  If NULL, the global callback queue will be used
  int priority;

  /// By default subscription callbacks are guaranteed to arrive in-order, with only one callback happening for this subscription at any given
  /// time.  Setting this to true allows you to receive multiple messages on the same topic from multiple threads at the same time
  bool allow_concurrent_callbacks;

  /**
   * \brief An object whose destruction will prevent the callback associated with this subscription
   *
   * A shared pointer to an object to track for these callbacks.  If set, the a weak_ptr will be created to this object,
   * and if the reference count goes to 0 the subscriber callbacks will not get called.
   *
   * \note Note that setting this will cause a new reference to be added to the object before the
   * callback, and for it to go out of scope (and potentially be deleted) in the code path (and therefore
   * thread) that the callback is invoked from.
   */
  VoidConstPtr tracked_object;

  TransportHints transport_hints;                                   ///< Hints for transport type and options

  /**
   * \brief Templated helper function for creating an AdvertiseServiceOptions with most of its options
   * \param topic Topic name to subscribe to
   * \param queue_size Number of incoming messages to queue up for
   *        processing (messages in excess of this queue capacity will be
   *        discarded).
   * \param callback The callback to invoke when a message is received on this topic
   * \param tracked_object The tracked object to use (see SubscribeOptions::tracked_object)
   * \param queue The callback queue to use (see SubscribeOptions::callback_queue)
   */
  template<class M>
  static SubscribeOptions2 create(const std::string& topic, uint32_t queue_size,
                                 const boost::function<void (const boost::shared_ptr<M const>&)>& callback,
                                 const VoidConstPtr& tracked_object, CMQ* queue, int priority)
  {
    SubscribeOptions2 ops;
    ops.init<M>(topic, queue_size, callback);
    ops.tracked_object = tracked_object;
    ops.callback_queue = queue;
    ops.priority = priority;
    return ops;
  }
};

}

#endif
