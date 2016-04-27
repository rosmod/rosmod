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

#include "rosmod/rosmod_callback_queue.h"
#include "ros/assert.h"
#include "rosmod/rosmod_logger.h"

namespace rosmod
{

CallbackQueue::CallbackQueue(bool enabled)
: calling_(0)
, enabled_(enabled)
, scheduling_scheme("FIFO")
{
  ROSMOD_LOGGER.reset(new ROSMOD_Logger());
}

CallbackQueue::~CallbackQueue()
{
  disable();
  ROSMOD_LOGGER.reset();
}

void CallbackQueue::enable()
{
  boost::mutex::scoped_lock lock(mutex_);
  enabled_ = true;

  condition_.notify_all();
}

void CallbackQueue::disable()
{
  boost::mutex::scoped_lock lock(mutex_);
  enabled_ = false;

  condition_.notify_all();
}

void CallbackQueue::clear()
{
  boost::mutex::scoped_lock lock(mutex_);

  callbacks_.clear();
}

bool CallbackQueue::isEmpty()
{
  boost::mutex::scoped_lock lock(mutex_);

  return callbacks_.empty() && calling_ == 0;
}

bool CallbackQueue::isEnabled()
{
  boost::mutex::scoped_lock lock(mutex_);

  return enabled_;
}

void CallbackQueue::setupTLS()
{
  if (!tls_.get())
  {
    tls_.reset(new TLS);
  }
}

int CallbackQueue::getWaitingCallbackPriority() {
  if (callbacks_.size() == 0)
    return -1;
  else
    return callbacks_.front().callback_options.priority;
}

void CallbackQueue::yieldCallback(int current_priority) {
  while (callbacks_.size() != 0) {
    if (callbacks_.front().callback_options.priority > current_priority)
      callOne(ros::WallDuration(0.01));
    else
      break;
  }
}

void CallbackQueue::addCallback(const CallbackInterfacePtr& callback, 
				uint64_t removal_id,
				ROSMOD_Callback_Options callback_options)
{
  CallbackInfo info;
  info.callback = callback;
  info.removal_id = removal_id;
  info.callback_options = callback_options;
  
  {
    boost::mutex::scoped_lock lock(mutex_);

    if (!enabled_)
    {
      return;
    }

    info.callback_options.enqueue_time = ros::Time::now();
    std::string scheme_string;
    if (scheduling_scheme == "FIFO")
      scheme_string = "FIFO";
    else if (scheduling_scheme == "PFIFO")
      scheme_string = "PFIFO";
    else if (scheduling_scheme == "EDF")
      scheme_string = "EDF";
    /*
    ROSMOD_LOGGER->log("DEBUG", 
		       "CALLBACK %s ENQUEUE::Alias=%s; Priority=%d; Deadline sec=%d, nsec=%d; Enqueue Time sec=%d, nsec=%d",
			scheme_string.c_str(),
			info.callback_options.alias.c_str(),
			info.callback_options.priority,
			info.callback_options.deadline.sec,
			info.callback_options.deadline.nsec,
			info.callback_options.enqueue_time.sec,
			info.callback_options.enqueue_time.nsec); 
    */
    std::deque<CallbackInfo>::iterator it;

    // Check scheduling scheme and enqueue based on choice
    if(scheme_string == "FIFO") {
      callbacks_.push_back(info);     
    }
    else if(scheme_string == "PFIFO") {
      if (callbacks_.size() == 0)
	callbacks_.push_back(info);
      else {
	for (it=callbacks_.begin(); it!=callbacks_.end(); ++it) {
	  if(it->callback_options.priority < info.callback_options.priority) {
	    callbacks_.insert(it, info);
	    break;
	  }
	}
      }
    }
    else if(scheme_string == "EDF") {
      if (callbacks_.size() == 0)
	callbacks_.push_back(info);
      else {
	for (it=callbacks_.begin(); it!=callbacks_.end(); ++it) {
	  if(it->callback_options.deadline > info.callback_options.deadline) {
	    callbacks_.insert(it, info);
	    break;
	  }
	}
      }
    }

  }

  {
    boost::mutex::scoped_lock lock(id_info_mutex_);

    M_IDInfo::iterator it = id_info_.find(removal_id);
    if (it == id_info_.end())
    {
      IDInfoPtr id_info(new IDInfo);
      id_info->id = removal_id;
      id_info_.insert(std::make_pair(removal_id, id_info));
    }
  }

  condition_.notify_one();
}

CallbackQueue::IDInfoPtr CallbackQueue::getIDInfo(uint64_t id)
{
  boost::mutex::scoped_lock lock(id_info_mutex_);
  M_IDInfo::iterator it = id_info_.find(id);
  if (it != id_info_.end())
  {
    return it->second;
  }

  return IDInfoPtr();
}

void CallbackQueue::removeByID(uint64_t removal_id)
{
  setupTLS();

  {
    IDInfoPtr id_info;
    {
      boost::mutex::scoped_lock lock(id_info_mutex_);
      M_IDInfo::iterator it = id_info_.find(removal_id);
      if (it != id_info_.end())
      {
        id_info = it->second;
      }
      else
      {
        return;
      }
    }

    // If we're being called from within a callback from our queue, we must unlock the shared lock we already own
    // here so that we can take a unique lock.  We'll re-lock it later.
    if (tls_->calling_in_this_thread == id_info->id)
    {
      id_info->calling_rw_mutex.unlock_shared();
    }

    {
      boost::unique_lock<boost::shared_mutex> rw_lock(id_info->calling_rw_mutex);
      boost::mutex::scoped_lock lock(mutex_);
      D_CallbackInfo::iterator it = callbacks_.begin();
      for (; it != callbacks_.end();)
      {
        CallbackInfo& info = *it;
        if (info.removal_id == removal_id)
        {
          it = callbacks_.erase(it);
        }
        else
        {
          ++it;
        }
      }
    }

    if (tls_->calling_in_this_thread == id_info->id)
    {
      id_info->calling_rw_mutex.lock_shared();
    }
  }

  // If we're being called from within a callback, we need to remove the callbacks that match the id that have already been
  // popped off the queue
  {
    D_CallbackInfo::iterator it = tls_->callbacks.begin();
    D_CallbackInfo::iterator end = tls_->callbacks.end();
    for (; it != end; ++it)
    {
      CallbackInfo& info = *it;
      if (info.removal_id == removal_id)
      {
        info.marked_for_removal = true;
      }
    }
  }

  {
    boost::mutex::scoped_lock lock(id_info_mutex_);
    id_info_.erase(removal_id);
  }
}

CallbackQueue::CallOneResult CallbackQueue::callOne(ros::WallDuration timeout)
{
  setupTLS();
  TLS* tls = tls_.get();

  CallbackInfo cb_info;

  {
    boost::mutex::scoped_lock lock(mutex_);

    if (!enabled_)
    {
      return Disabled;
    }

    if (callbacks_.empty())
    {
      if (!timeout.isZero())
      {
        condition_.timed_wait(lock, boost::posix_time::microseconds(timeout.toSec() * 1000000.0f));
      }

      if (callbacks_.empty())
      {
        return Empty;
      }

      if (!enabled_)
      {
        return Disabled;
      }
    }

    D_CallbackInfo::iterator it = callbacks_.begin();
    for (; it != callbacks_.end();)
    {
      CallbackInfo& info = *it;

      if (info.marked_for_removal)
      {
        it = callbacks_.erase(it);
        continue;
      }

      if (info.callback->ready())
      {
        cb_info = info;
        it = callbacks_.erase(it);
        break;
      }

      ++it;
    }

    if (!cb_info.callback)
    {
      return TryAgain;
    }

    ++calling_;
  }

  bool was_empty = tls->callbacks.empty();
  tls->callbacks.push_back(cb_info);
  if (was_empty)
  {
    tls->cb_it = tls->callbacks.begin();
  }

  CallOneResult res = callOneCB(tls);
  if (res != Empty)
  {
    boost::mutex::scoped_lock lock(mutex_);
    --calling_;
  }
  return res;
}

void CallbackQueue::callAvailable(ros::WallDuration timeout)
{
  setupTLS();
  TLS* tls = tls_.get();

  {
    boost::mutex::scoped_lock lock(mutex_);

    if (!enabled_)
    {
      return;
    }

    if (callbacks_.empty())
    {
      if (!timeout.isZero())
      {
        condition_.timed_wait(lock, boost::posix_time::microseconds(timeout.toSec() * 1000000.0f));
      }

      if (callbacks_.empty() || !enabled_)
      {
        return;
      }
    }

    bool was_empty = tls->callbacks.empty();

    tls->callbacks.insert(tls->callbacks.end(), callbacks_.begin(), callbacks_.end());
    callbacks_.clear();

    calling_ += tls->callbacks.size();

    if (was_empty)
    {
      tls->cb_it = tls->callbacks.begin();
    }
  }

  size_t called = 0;

  while (!tls->callbacks.empty())
  {
    if (callOneCB(tls) != Empty)
    {
      ++called;
    }
  }

  {
    boost::mutex::scoped_lock lock(mutex_);
    calling_ -= called;
  }
}

CallbackQueue::CallOneResult CallbackQueue::callOneCB(TLS* tls)
{
  // Check for a recursive call.  If recursive, increment the current iterator.  Otherwise
  // set the iterator it the beginning of the thread-local callbacks
  if (tls->calling_in_this_thread == 0xffffffffffffffffULL)
  {
    tls->cb_it = tls->callbacks.begin();
  }

  if (tls->cb_it == tls->callbacks.end())
  {
    return Empty;
  }

  ROS_ASSERT(!tls->callbacks.empty());
  ROS_ASSERT(tls->cb_it != tls->callbacks.end());

  CallbackInfo info = *tls->cb_it;
  CallbackInterfacePtr& cb = info.callback;

  IDInfoPtr id_info = getIDInfo(info.removal_id);
  if (id_info)
  {
    boost::shared_lock<boost::shared_mutex> rw_lock(id_info->calling_rw_mutex);

    uint64_t last_calling = tls->calling_in_this_thread;
    tls->calling_in_this_thread = id_info->id;
    ros::Duration execution_time;

    CallbackInterface::CallResult result = CallbackInterface::Invalid;
    try {
      if (info.marked_for_removal)
      {
        tls->cb_it = tls->callbacks.erase(tls->cb_it);
      }
      else
      {
        tls->cb_it = tls->callbacks.erase(tls->cb_it);
	info.callback_options.dequeue_time = ros::Time::now();
        result = cb->call();
	info.callback_options.completion_time = ros::Time::now();
	execution_time = info.callback_options.completion_time
	  - info.callback_options.enqueue_time;
      }
    }
    catch (std::exception&)
    {
      // ensure that thread id gets restored, even in case of an exception
      tls->calling_in_this_thread = last_calling;
      throw;
    }

    tls->calling_in_this_thread = last_calling;

    // Push TryAgain callbacks to the back of the shared queue
    if (result == CallbackInterface::TryAgain && !info.marked_for_removal)
    {
      boost::mutex::scoped_lock lock(mutex_);
      callbacks_.push_back(info);

      return TryAgain;
    }

    ROSMOD_LOGGER->log("DEBUG", "%s, %d.%09d, %d.%09d, %d.%09d, %d.%09d, %d.%09d",
		       info.callback_options.alias.c_str(), 
		       info.callback_options.enqueue_time.sec, info.callback_options.enqueue_time.nsec,
		       info.callback_options.dequeue_time.sec, info.callback_options.dequeue_time.nsec,
		       info.callback_options.completion_time.sec, info.callback_options.completion_time.nsec,
		       execution_time.sec, execution_time.nsec,
		       info.callback_options.deadline.sec, info.callback_options.deadline.nsec);

    if (execution_time > info.callback_options.deadline) {
      ROSMOD_Deadline_Violation new_violation;
      new_violation.alias = info.callback_options.alias;
      new_violation.deadline = info.callback_options.deadline;
      new_violation.enqueue_time = info.callback_options.enqueue_time;
      new_violation.dequeue_time = info.callback_options.dequeue_time;
      new_violation.completion_time = info.callback_options.completion_time;
      new_violation.exec_time = execution_time;
      saveDeadlineViolation(new_violation);
    }

    return Called;
  }
  else
  {
    tls->cb_it = tls->callbacks.erase(tls->cb_it);
  }

  info.callback_options.completion_time = ros::Time::now();
  ros::Duration execution_time = info.callback_options.completion_time
    - info.callback_options.enqueue_time;

  ROSMOD_LOGGER->log("DEBUG", "%s, %d.%09d, %d.%09d, %d.%09d, %d.%09d, %d.%09d",
		     info.callback_options.alias.c_str(), 
		     info.callback_options.enqueue_time.sec, info.callback_options.enqueue_time.nsec,
		     info.callback_options.dequeue_time.sec, info.callback_options.dequeue_time.nsec,
		     info.callback_options.completion_time.sec, info.callback_options.completion_time.nsec,
		     info.callback_options.enqueue_time.sec, info.callback_options.enqueue_time.nsec,
		     info.callback_options.deadline.sec, info.callback_options.deadline.nsec);

  
  if (execution_time > info.callback_options.deadline) {
      ROSMOD_Deadline_Violation new_violation;
      new_violation.alias = info.callback_options.alias;
      new_violation.deadline = info.callback_options.deadline;
      new_violation.enqueue_time = info.callback_options.enqueue_time;
      new_violation.dequeue_time = info.callback_options.dequeue_time;
      new_violation.completion_time = info.callback_options.completion_time;
      new_violation.exec_time = execution_time;
      saveDeadlineViolation(new_violation);
  }

  return Called;
}

  void CallbackQueue::saveDeadlineViolation(ROSMOD_Deadline_Violation new_violation) {
    deadline_violation_map[new_violation.alias].push_back(new_violation);
  }

  CallbackQueue::DL_Map CallbackQueue::getAllDeadlineViolations() {
    return deadline_violation_map;
  }

}
