#ifndef ROSMOD_CMQ_H
#define ROSMOD_CMQ_H

#include "ros/callback_queue.h"
#include "ros/callback_queue_interface.h"
#include "ros/time.h"
#include "ros/common.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/tss.hpp>

#include <list>
#include <deque>
#include <queue>

namespace ros
{
  
  /**
   * \brief This is the implementation of rosmod::CMQ
   */
class CMQ : public CallbackQueueInterface

{
  public:

    CMQ(bool enabled = true);
    virtual ~CMQ();

    virtual void addCallback(const CallbackInterfacePtr& callback, 
			       			 uint64_t removal_id = 0);
    virtual void addCallback(const CallbackInterfacePtr& callback, int priority,  
			       			 uint64_t removal_id = 0);

    virtual void removeByID(uint64_t removal_id);    

    enum CallOneResult
    {
      Called,
      TryAgain,
      Disabled,
      Empty,
    };

    /**
     * \brief Pop a single callback off the front of the queue and invoke it. If the callback was not ready to be called,
     * pushes it back onto the queue
     */
    CallOneResult callOne()
    {
      return callOne(ros::WallDuration());
    }

  /**
   * \brief Pop a single callback off the front of the queue and invoke it.  If the callback was not ready to be called,
   * pushes it back onto the queue.  This version includes a timeout which lets you specify the amount of time to wait for
   * a callback to be available before returning.
   *
   * \param timeout The amount of time to wait for a callback to be available.  If there is already a callback available,
   * this parameter does nothing.
   */
  CallOneResult callOne(ros::WallDuration timeout);

  /**
   * \brief returns whether or not the queue is empty
   */
  bool empty() { return isEmpty(); }
  /**
   * \brief returns whether or not the queue is empty
   */
  bool isEmpty();

  /**
   * \brief Removes all callbacks from the queue.  Does \b not wait for calls currently in progress to finish.
   */
  void clear();  

  /**
   * \brief Enable the queue (queue is enabled by default)
   */
  void enable();
  /**
   * \brief Disable the queue, meaning any calls to addCallback() will have no effect
   */
  void disable();
  /**
   * \brief Returns whether or not this queue is enabled
   */
  bool isEnabled();



 protected:
  void setupTLS();

  struct TLS;
  CallOneResult callOneCB(TLS* tls);

  struct IDInfo
  {
    uint64_t id;
    boost::shared_mutex calling_rw_mutex;
  };
  typedef boost::shared_ptr<IDInfo> IDInfoPtr;
  typedef std::map<uint64_t, IDInfoPtr> M_IDInfo;

  IDInfoPtr getIDInfo(uint64_t id);
  
  struct CMQ_Object
  {
    CMQ_Object()
    : removal_id(0)
    , marked_for_removal(false)
    {}
    CallbackInterfacePtr callback;
    uint64_t priority;
    uint64_t removal_id;
    bool marked_for_removal;
  };

  typedef std::list<CMQ_Object> L_CMQ_Object;
  typedef std::deque<CMQ_Object> D_CMQ_Object;

  D_CMQ_Object callbacks_;
  size_t calling_;
  boost::mutex mutex_;
  boost::condition_variable condition_;

  boost::mutex id_info_mutex_;
  M_IDInfo id_info_;

  struct TLS
  {
    TLS()
    : calling_in_this_thread(0xffffffffffffffffULL)
    , cb_it(callbacks.end())
    {}
    uint64_t calling_in_this_thread;
    D_CMQ_Object callbacks;
    D_CMQ_Object::iterator cb_it;
  };
  boost::thread_specific_ptr<TLS> tls_;

  bool enabled_;  

  };

typedef boost::shared_ptr<CMQ> CMQPtr;

}

#endif
