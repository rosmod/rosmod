/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2009, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ROSMOD_CALLBACK_QUEUE_H
#define ROSMOD_CALLBACK_QUEUE_H

#include "rosmod/rosmod_callback_queue_interface.h"
#include "ros/time.h"
#include "rosmod/rosmod_common.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/tss.hpp>

#include <list>
#include <deque>

namespace rosmod
{

/**
 * \brief This is the default implementation of the ros::CallbackQueueInterface
 */
class ROSCPP_DECL CallbackQueue : public CallbackQueueInterface
{
public:
  CallbackQueue(bool enabled = true);
  virtual ~CallbackQueue();

  virtual void addCallback(const CallbackInterfacePtr& callback, 
			   uint64_t removal_id = 0,
			   ROSMOD_Callback_Options callback_options = ROSMOD_Callback_Options());
  virtual void removeByID(uint64_t removal_id);

  enum CallOneResult
  {
    Called,
    TryAgain,
    Disabled,
    Empty,
  };

  std::string scheduling_scheme;

  // Logs callback enqueue, dequeue and deadline violations
  std::unique_ptr<ROSMOD_Logger> ROSMOD_LOGGER;
  //ROSMOD_Logger * ROSMOD_LOGGER;
  
  /**
   * \brief Pop a single callback off the front of the queue and invoke it.  If the callback was not ready to be called,
   * pushes it back onto the queue.
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
   * \brief Invoke all callbacks currently in the queue.  If a callback was not ready to be called, pushes it back onto the queue.
   */
  void callAvailable()
  {
    callAvailable(ros::WallDuration());
  }
  /**
   * \brief Invoke all callbacks currently in the queue.  If a callback was not ready to be called, pushes it back onto the queue.  This version
   * includes a timeout which lets you specify the amount of time to wait for a callback to be available before returning.
   *
   * \param timeout The amount of time to wait for at least one callback to be available.  If there is already at least one callback available,
   * this parameter does nothing.
   */
  void callAvailable(ros::WallDuration timeout);

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

  int getWaitingCallbackPriority();

  void yieldCallback(int current_priority);

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

  struct CallbackInfo
  {
    CallbackInfo()
    : removal_id(0)
    , marked_for_removal(false)
    {}
    CallbackInterfacePtr callback;
    ROSMOD_Callback_Options callback_options;
    uint64_t removal_id;
    bool marked_for_removal;
  };
  typedef std::list<CallbackInfo> L_CallbackInfo;
  typedef std::deque<CallbackInfo> D_CallbackInfo;
  D_CallbackInfo callbacks_;
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
    D_CallbackInfo callbacks;
    D_CallbackInfo::iterator cb_it;
  };
  boost::thread_specific_ptr<TLS> tls_;

  bool enabled_;

 public:
  /**
   * \brief Characteristics of a Deadline Violation
   */
  struct ROSMOD_Deadline_Violation
  {
    std::string alias;
    Duration deadline;
    Time enqueue_time;
    Time dequeue_time;
    Time completion_time;
    Duration exec_time;
  
    ROSMOD_Deadline_Violation() {
      alias = "";
      deadline.sec = -1;
      deadline.nsec = -1;
      enqueue_time.sec = -1;
      enqueue_time.nsec = -1;
      dequeue_time.sec = -1;
      dequeue_time.nsec = -1;
      completion_time.sec = -1;
      completion_time.nsec = -1;
      exec_time.sec = -1;
      exec_time.nsec = -1;
    }
  };
  typedef std::map<std::string, std::vector<ROSMOD_Deadline_Violation>> DL_Map;
  DL_Map deadline_violation_map;

  /**
   * \brief Save Deadline Violations in Completed Operations
   */
  void saveDeadlineViolation(ROSMOD_Deadline_Violation new_violation);

  /**
   * \brief Get a map of all Deadline Violations in Component
   */
  DL_Map getAllDeadlineViolations();

};
typedef boost::shared_ptr<CallbackQueue> CallbackQueuePtr;

}

#endif
