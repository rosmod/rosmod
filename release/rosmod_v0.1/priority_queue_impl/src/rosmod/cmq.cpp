#include "rosmod/cmq.h"
#include "ros/assert.h"

namespace ros
{

CMQ::CMQ(bool enabled)
: calling_(0)
, enabled_(enabled)
{
}

CMQ::~CMQ()
{
  disable();
}

void CMQ::enable()
{
  boost::mutex::scoped_lock lock(mutex_);
  enabled_ = true;

  condition_.notify_all();
}

void CMQ::disable()
{
  boost::mutex::scoped_lock lock(mutex_);
  enabled_ = false;

  condition_.notify_all();
}

void CMQ::clear()
{
  boost::mutex::scoped_lock lock(mutex_);

  callbacks_.clear();
}

bool CMQ::isEmpty()
{
  boost::mutex::scoped_lock lock(mutex_);

  return callbacks_.empty() && calling_ == 0;
}

bool CMQ::isEnabled()
{
  boost::mutex::scoped_lock lock(mutex_);

  return enabled_;
}

void CMQ::setupTLS()
{
  if (!tls_.get())
  {
    tls_.reset(new TLS);
  }
}

void CMQ::addCallback(const CallbackInterfacePtr& callback, uint64_t removal_id)
{
  ROS_INFO("Inside Hell");
  CMQ_Object info;
  info.callback = callback;
  info.removal_id = removal_id;

  {
    boost::mutex::scoped_lock lock(mutex_);

    if (!enabled_)
    {
      return;
    }

    // Enqueue based on priority here...
    callbacks_.push_back(info);
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
  ROS_INFO("QUEUE::Added callback!");
}

void CMQ::addCallback(const CallbackInterfacePtr& callback, int priority = 0, uint64_t removal_id)
{
  CMQ_Object info;
  info.callback = callback;
  info.removal_id = removal_id;
  info.priority = priority;

  ROS_INFO("Priority - %d", priority);

  {
    boost::mutex::scoped_lock lock(mutex_);

    if (!enabled_)
    {
      return;
    }
  
    bool enqueue = false;

    std::cout << "Callback Queue size - " << callbacks_.size() << std::endl;

    // If there are no objects in CMQ, just push_back new callback object.
    if (callbacks_.size() == 0) { 
        callbacks_.push_back(info);
        enqueue = true;
    }
    // Else, iterate through callbacks in queue to find right spot to insert at
    else {

       D_CMQ_Object::iterator it = callbacks_.begin();
       for (; it != callbacks_.end();)
       { 
           CMQ_Object& this_info = *it;
           if (info.priority > this_info.priority && enqueue == false) {
               callbacks_.insert(it, info);
               enqueue = true;
               break;
           }
           
           ++it;
       }

       if (enqueue == false) {
           callbacks_.push_back(info);
       }
    }

    std::cout << "Callback Queue size - " << callbacks_.size() << std::endl;

    std::cout << "Callback Queue -> [";

    //ROS_INFO("Queue Length - ");
      // Print CMQ
      D_CMQ_Object::iterator it = callbacks_.begin();
      for (; it != callbacks_.end();)
      {
        CMQ_Object& info = *it;
        std::cout << info.priority << " ";
        ++it;
      }
      std::cout << "]" << std::endl;
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
  ROS_INFO("QUEUE::Added callback!");
}

CMQ::IDInfoPtr CMQ::getIDInfo(uint64_t id)
{
  boost::mutex::scoped_lock lock(id_info_mutex_);
  M_IDInfo::iterator it = id_info_.find(id);
  if (it != id_info_.end())
  {
    return it->second;
  }

  return IDInfoPtr();
}

void CMQ::removeByID(uint64_t removal_id)
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
      D_CMQ_Object::iterator it = callbacks_.begin();
      for (; it != callbacks_.end();)
      {
        CMQ_Object& info = *it;
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
    D_CMQ_Object::iterator it = tls_->callbacks.begin();
    D_CMQ_Object::iterator end = tls_->callbacks.end();
    for (; it != end; ++it)
    {
      CMQ_Object& info = *it;
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
  ROS_INFO("Removed callback from queue!");
}

CMQ::CallOneResult CMQ::callOne(ros::WallDuration timeout)
{
  setupTLS();
  TLS* tls = tls_.get();

  CMQ_Object cb_info;

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

    D_CMQ_Object::iterator it = callbacks_.begin();
    for (; it != callbacks_.end();)
    {
      CMQ_Object& info = *it;

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


CMQ::CallOneResult CMQ::callOneCB(TLS* tls)
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

  CMQ_Object info = *tls->cb_it;
  CallbackInterfacePtr& cb = info.callback;

  IDInfoPtr id_info = getIDInfo(info.removal_id);
  if (id_info)
  {
    boost::shared_lock<boost::shared_mutex> rw_lock(id_info->calling_rw_mutex);

    uint64_t last_calling = tls->calling_in_this_thread;
    tls->calling_in_this_thread = id_info->id;

    CallbackInterface::CallResult result = CallbackInterface::Invalid;
    try {
      if (info.marked_for_removal)
      {
        tls->cb_it = tls->callbacks.erase(tls->cb_it);
      }
      else
      {
        tls->cb_it = tls->callbacks.erase(tls->cb_it);
        result = cb->call();
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

    return Called;
  }
  else
  {
    tls->cb_it = tls->callbacks.erase(tls->cb_it);
  }

  return Called;
}



}

