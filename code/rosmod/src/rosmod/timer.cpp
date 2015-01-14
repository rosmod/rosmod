#include "rosmod/timer.h"
#include "rosmod/timer_manager.h"

namespace ros
{

Timer2::Impl::Impl()
  : started_(false)
  , timer_handle_(-1)
{ }

Timer2::Impl::~Impl()
{
  ROS_DEBUG("Timer deregistering callbacks.");
  stop();
}

bool Timer2::Impl::isValid()
{
  return !period_.isZero();
}

void Timer2::Impl::start()
{
  if (!started_)
  {
    VoidConstPtr tracked_object;
    if (has_tracked_object_)
    {
      tracked_object = tracked_object_.lock();
    }

    timer_handle_ = TimerManager2<Time, Duration, TimerEvent>::global().add(period_, callback_, callback_queue_, priority_, tracked_object, oneshot_);
    started_ = true;
  }
}

void Timer2::Impl::stop()
{
  if (started_)
  {
    started_ = false;
    TimerManager2<Time, Duration, TimerEvent>::global().remove(timer_handle_);
    timer_handle_ = -1;
  }
}

bool Timer2::Impl::hasPending()
{
  if (!isValid() || timer_handle_ == -1)
  {
    return false;
  }

  return TimerManager2<Time, Duration, TimerEvent>::global().hasPending(timer_handle_);
}

void Timer2::Impl::setPeriod(const Duration& period)
{
  period_ = period;
  TimerManager2<Time, Duration, TimerEvent>::global().setPeriod(timer_handle_, period);
}

Timer2::Timer2(const TimerOptions& ops)
: impl_(new Impl)
{
  impl_->period_ = ops.period;
  impl_->callback_ = ops.callback;
  impl_->callback_queue_ = (ros::CMQ *)ops.callback_queue;
  impl_->tracked_object_ = ops.tracked_object;
  impl_->has_tracked_object_ = ops.tracked_object;
  impl_->oneshot_ = ops.oneshot;
}

Timer2::Timer2(const TimerOptions2& ops)
: impl_(new Impl)
{
  impl_->period_ = ops.period;
  impl_->callback_ = ops.callback;
  impl_->callback_queue_ = ops.callback_queue;
  impl_->priority_ = ops.priority;
  impl_->tracked_object_ = ops.tracked_object;
  impl_->has_tracked_object_ = ops.tracked_object;
  impl_->oneshot_ = ops.oneshot;
}

Timer2::Timer2(const Timer2& rhs)
{
  impl_ = rhs.impl_;
}

Timer2::~Timer2()
{
}

void Timer2::start()
{
  if (impl_)
  {
    impl_->start();
  }
}

void Timer2::stop()
{
  if (impl_)
  {
    impl_->stop();
  }
}

bool Timer2::hasPending()
{
  if (impl_)
  {
    return impl_->hasPending();
  }

  return false;
}

void Timer2::setPeriod(const Duration& period)
{
  if (impl_)
  {
    impl_->setPeriod(period);
  }
}

}
