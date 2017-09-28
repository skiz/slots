#ifndef TIMER_H
#define TIMER_H

#include "SDL.h"

class Timer {
 public:
  Timer();
  void Start();
  void Stop();
  void Pause();
  void Unpause();
  Uint32 GetSeconds();
  Uint32 GetTicks();
  bool IsStarted();
  bool IsPaused();
 private:
  Uint32 startTicks;
  Uint32 pauseTicks;
  bool paused;
  bool started;
};
#endif


