#ifndef ROSMOD_TIMER_H
#define ROSMOD_TIMER_H

#include "ros/common.h"
#include "ros/forwards.h"
#include "rosmod/timer_options.h"
#include "rosmod/cmq.h"
#include "ros/timer_options.h"

namespace ros
{

/**
 * \brief Manages a timer callback
 *
 * A Timer should always be created through a call to NodeHandle::createTimer(), or copied from one
 * that was. Once all copies of a specific
 * Timer go out of scope, the callback associated with that handle will stop
 * being called.
 */
class Timer2
{
public:
  Timer2() {}
  Timer2(const Timer2& rhs);
  Timer2(const TimerOptions& ops);
  Timer2(const TimerOptions2& ops);
  ~Timer2();

  /**
   * \brief Start the timer.  Does nothing if the timer is already started.
   */
  void start();
  /**
   * \brief Stop the timer.  Once this call returns, no more callbacks will be called.  Does
   * nothing if the timer is already stopped.
   */
  void stop();

  /**
   * \brief Returns whether or not the timer has any pending events to call.
   */
  bool hasPending();

  /**
   * \brief Set the period of this timer
   */
  void setPeriod(const Duration& period);

  bool isValid() { return impl_ && impl_->isValid(); }
  operator void*() { return isValid() ? (void*)1 : (void*)0; }

  bool operator<(const Timer2& rhs)
  {
    return impl_ < rhs.impl_;
  }

  bool operator==(const Timer2& rhs)
  {
    return impl_ == rhs.impl_;
  }

  bool operator!=(const Timer2& rhs)
  {
    return impl_ != rhs.impl_;
  }

private:

  class Impl
  {
  public:
    Impl();
    ~Impl();

    bool isValid();
    bool hasPending();
    void setPeriod(const Duration& period);

    void start();
    void stop();

    bool started_;
    int32_t timer_handle_;

    Duration period_;
    TimerCallback callback_;
    CMQ* callback_queue_;
    int priority_;
    VoidConstWPtr tracked_object_;
    bool has_tracked_object_;
    bool oneshot_;
  };
  typedef boost::shared_ptr<Impl> ImplPtr;
  typedef boost::weak_ptr<Impl> ImplWPtr;

  ImplPtr impl_;

  friend class NodeHandle;
};

}

#endif
