#include "timer.h"

#include <stdlib.h>

#ifdef _WIN32
static const int64_t microseconds = 1000000;
#endif
static const int64_t nanoseconds = 1000000000;

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifdef _WIN32
static double seconds_per_count;
#endif

static void timer_initialise() {
#ifdef _WIN32
  __int64 counts_per_sec;
  QueryPerformanceFrequency((LARGE_INTEGER*)&counts_per_sec);
  seconds_per_count = 1.0 / (double)counts_per_sec;
#endif
}

Timer timer_new(void) {
  timer_initialise();
  Timer timer = {0};
  timer_start(&timer);
  return timer;
}

void timer_start(Timer* timer) {
  timer->start_time = time_now();
  timer->last_tick = timer->start_time;
  timer->running_time = 0;
  timer->delta_time = 0;
}

void timer_tick(Timer* timer) {
  const time_point this_tick = time_now();
  timer->running_time = time_diff(timer->start_time, this_tick);
  timer->delta_time = time_diff(timer->last_tick, this_tick);
  timer->last_tick = this_tick;
}

time_point time_now(void) {
  time_point t;
#ifdef _WIN32
  QueryPerformanceCounter((LARGE_INTEGER*)&t);
#else
  clock_gettime(CLOCK_REALTIME, &t);
#endif
  return t;
}

time_delta time_diff(time_point start, time_point end) {
#ifdef _WIN32
  // Force nonnegative. The DXSDK's CDXUTTimer mentions that if the
  // processor goes into a power save mode or we get shuffled to
  // another processor, then the delta time can be negative.
  return std::max(0, end - start);
#else
  return (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
#endif
}

double time_delta_to_sec(time_delta dt) {
#ifdef _WIN32
  return (double)dt * seconds_per_count;
#else
  return (double)dt * 1.0e-9;
#endif
}

time_delta sec_to_time_delta(double seconds) {
#ifdef _WIN32
  return (time_delta)(seconds / seconds_per_count);
#else
  return (time_delta)(seconds * 1.0e9);
#endif
}

void time_sleep(time_delta dt) {
#ifdef _WIN32
  const int64_t ms = dt / microseconds;
  Sleep((DWORD)(ms));
#else
  const int64_t sec = dt / nanoseconds;
  struct timespec ts;
  ts.tv_sec = (long)sec;
  ts.tv_nsec = (long)(dt % nanoseconds);
  nanosleep(&ts, NULL);
#endif
}
