#ifndef ROSMOD_TIMER_OPTIONS_H
#define ROSMOD_TIMER_OPTIONS_H

#include "ros/common.h"
#include "ros/forwards.h"
#include "rosmod/cmq.h"

namespace ros
{

/**
 * \brief Encapsulates all options available for starting a timer
 */
struct TimerOptions2
{
  TimerOptions2()
    : period(0.1)
    , callback_queue(0)
    , priority(0)
    , oneshot(false)
    , autostart(true)
  { }

  /*
   * \brief Constructor
   * \param
   */
  TimerOptions2(Duration _period, const TimerCallback& _callback, 
               CMQ* _queue, int _priority, bool oneshot = false, bool autostart = true)
    : period(_period)
    , callback(_callback)
    , callback_queue(_queue)
    , priority(_priority)
    , oneshot(oneshot)
    , autostart(autostart)
  { }

  Duration period;                                                  ///< The period to call the callback at
  TimerCallback callback;                                           ///< The callback to call

  CMQ* callback_queue; ///< Queue to add callbacks to.  If NULL, the global callback queue will be used
  int priority;
  /**
   * A shared pointer to an object to track for these callbacks.  If set, the a weak_ptr will be created to this object,
   * and if the reference count goes to 0 the subscriber callbacks will not get called.
   *
   * \note Note that setting this will cause a new reference to be added to the object before the
   * callback, and for it to go out of scope (and potentially be deleted) in the code path (and therefore
   * thread) that the callback is invoked from.
   */
  VoidConstPtr tracked_object;

  bool oneshot;
  bool autostart;
};


}

#endif
