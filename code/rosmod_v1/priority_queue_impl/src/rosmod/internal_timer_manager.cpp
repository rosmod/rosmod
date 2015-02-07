#include "rosmod/internal_timer_manager.h"
#include "rosmod/timer_manager.h"

namespace ros
{

static InternalTimerManagerPtr2 g_timer_manager;

InternalTimerManagerPtr2 getInternalTimerManager2()
{
  return g_timer_manager;
}

void initInternalTimerManager2()
{
  if (!g_timer_manager)
  {
    g_timer_manager.reset(new InternalTimerManager2);
  }
}

} // namespace ros
