#ifndef ROSMOD_INTERNAL_TIMER_MANAGER_H
#define ROSMOD_INTERNAL_TIMER_MANAGER_H

#include "ros/forwards.h"
#include <ros/time.h>
#include "ros/common.h"

namespace ros
{

template<typename T, typename D, typename E> class TimerManager2;
typedef TimerManager2<WallTime, WallDuration, WallTimerEvent> InternalTimerManager2;
typedef boost::shared_ptr<InternalTimerManager2> InternalTimerManagerPtr2;

ROSCPP_DECL void initInternalTimerManager2();
ROSCPP_DECL InternalTimerManagerPtr2 getInternalTimerManager2();

}

#endif
