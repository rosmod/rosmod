#ifndef ROSMOD_PUBLISHER_HANDLE_H
#define ROSMOD_PUBLISHER_HANDLE_H

#include "ros/forwards.h"
#include "ros/common.h"
#include "ros/message.h"
#include "ros/serialization.h"
#include <boost/bind.hpp>

#include "rosmod/subscriber.h"
#include "rosmod/topic_manager.h"
#include "rosmod/node_handle.h"

namespace ros
{

class NodeHandle2;
typedef boost::shared_ptr<NodeHandle2> NodeHandlePtr2;

  /**
   * \brief Manages an advertisement on a specific topic.
   *
   * A Publisher should always be created through a call to NodeHandle::advertise(), or copied from one
   * that was. Once all copies of a specific
   * Publisher go out of scope, any subscriber status callbacks associated with that handle will stop
   * being called.  Once all Publishers for a given topic go out of scope the topic will be unadvertised.
   */
  class Publisher2
  {
  public:
    Publisher2() {}
    Publisher2(const Publisher2& rhs);
    ~Publisher2();

    /**
     * \brief Publish a message on the topic associated with this Publisher.
     *
     * This version of publish will allow fast intra-process message-passing in the future,
     * so you may not mutate the message after it has been passed in here (since it will be
     * passed directly into a callback function)
     *
     */
    template <typename M>
      void publish(const boost::shared_ptr<M>& message) const
    {
      using namespace serialization;

      if (!impl_)
        {
          ROS_ASSERT_MSG(false, "Call to publish() on an invalid Publisher");
          return;
        }

      if (!impl_->isValid())
        {
          ROS_ASSERT_MSG(false, "Call to publish() on an invalid Publisher (topic [%s])", impl_->topic_.c_str());
          return;
        }

      ROS_ASSERT_MSG(impl_->md5sum_ == "*" || std::string(mt::md5sum<M>(*message)) == "*" || impl_->md5sum_ == mt::md5sum<M>(*message),
                     "Trying to publish message of type [%s/%s] on a publisher with type [%s/%s]",
                     mt::datatype<M>(*message), mt::md5sum<M>(*message),
                     impl_->datatype_.c_str(), impl_->md5sum_.c_str());

      SerializedMessage m;
      m.type_info = &typeid(M);
      m.message = message;

      publish(boost::bind(serializeMessage<M>, boost::ref(*message)), m);
    }

    /**
     * \brief Publish a message on the topic associated with this Publisher.
     */
    template <typename M>
      void publish(const M& message) const
    {
      using namespace serialization;
      namespace mt = ros::message_traits;

      if (!impl_)
        {
          ROS_ASSERT_MSG(false, "Call to publish() on an invalid Publisher");
          return;
        }

      if (!impl_->isValid())
        {
          ROS_ASSERT_MSG(false, "Call to publish() on an invalid Publisher (topic [%s])", impl_->topic_.c_str());
          return;
        }

      ROS_ASSERT_MSG(impl_->md5sum_ == "*" || std::string(mt::md5sum<M>(message)) == "*" || impl_->md5sum_ == mt::md5sum<M>(message),
                     "Trying to publish message of type [%s/%s] on a publisher with type [%s/%s]",
                     mt::datatype<M>(message), mt::md5sum<M>(message),
                     impl_->datatype_.c_str(), impl_->md5sum_.c_str());

      SerializedMessage m;
      publish(boost::bind(serializeMessage<M>, boost::ref(message)), m);
    }

    /**
     * \brief Shutdown the advertisement associated with this Publisher
     *
     * This method usually does not need to be explicitly called, as automatic shutdown happens when
     * all copies of this Publisher go out of scope
     *
     * This method overrides the automatic reference counted unadvertise, and does so immediately.
     * \note Note that if multiple advertisements were made through NodeHandle::advertise(), this will
     * only remove the one associated with this Publisher
     */
    void shutdown();

    /**
     * \brief Returns the topic that this Publisher will publish on.
     */
    std::string getTopic() const;

    /**
     * \brief Returns the number of subscribers that are currently connected to this Publisher
     */
    uint32_t getNumSubscribers() const;

    /**
     * \brief Returns whether or not this topic is latched
     */
    bool isLatched() const;

    operator void*() const { return (impl_ && impl_->isValid()) ? (void*)1 : (void*)0; }

    bool operator<(const Publisher2& rhs) const
    {
      return impl_ < rhs.impl_;
    }

    bool operator==(const Publisher2& rhs) const
    {
      return impl_ == rhs.impl_;
    }

    bool operator!=(const Publisher2& rhs) const
    {
      return impl_ != rhs.impl_;
    }

    Publisher2(const std::string& topic, const std::string& md5sum, 
              const std::string& datatype, const NodeHandle2& node_handle, 
              const SubscriberCallbacksPtr2& callbacks);

  private:

    void publish(const boost::function<SerializedMessage(void)>& serfunc, SerializedMessage& m) const;
    void incrementSequence() const;

    class Impl
    {
    public:
      Impl();
      ~Impl();

      void unadvertise();
      bool isValid() const;

      std::string topic_;
      std::string md5sum_;
      std::string datatype_;
      NodeHandlePtr2 node_handle_;
      SubscriberCallbacksPtr2 callbacks_;
      bool unadvertised_;
    };
    typedef boost::shared_ptr<Impl> ImplPtr;
    typedef boost::weak_ptr<Impl> ImplWPtr;

    ImplPtr impl_;

    friend class NodeHandle2;
    friend class NodeHandleBackingCollection2;
  };

  typedef std::vector<Publisher2> V_Publisher2;
}

#endif // ROSCPP_PUBLISHER_HANDLE_H
