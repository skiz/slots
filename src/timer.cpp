#include "timer.h"
#include "SDL.h"

Timer::Timer() {
  startTicks = 0;
  pauseTicks = 0;
  paused = false;
  started = false;
}

void Timer::Start() {
  started = true;
  paused = false;
  startTicks = SDL_GetTicks();
  pauseTicks = 0;
}

void Timer::Stop() {
  started = false;
  paused = false;
  startTicks = false;
  pauseTicks = false;
}

void Timer::Pause() {
  if (started && !paused) {
    paused = true;
    pauseTicks = SDL_GetTicks() - startTicks;
    startTicks = 0;
  }
}

void Timer::Unpause() {
  if (started && paused) {
    paused = false;
    startTicks = SDL_GetTicks() - pauseTicks;
    pauseTicks = 0;
  }
}

Uint32 Timer::GetSeconds() {
  return GetTicks() / 1000;
}

Uint32 Timer::GetTicks() {
  Uint32 time = 0;
  if (started) {
    if (paused) {
      time = pauseTicks;
    } else {
      time = SDL_GetTicks() - startTicks;
    }
  }
  return time;
}

bool Timer::IsStarted() {
  return started;
}

bool Timer::IsPaused() {
  return paused;
}


