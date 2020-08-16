#pragma once

#include <stdint.h>

/// A particular point in time.
#ifdef _WIN32
typedef uint64_t time_point;
#else
#include <time.h>
typedef struct timespec time_point;
#endif

/// Time elapsed between two time points.
typedef uint64_t time_delta;

/// A high resolution timer.
typedef struct {
  time_point start_time;   // The instant the timer was last started.
  time_point last_tick;    // The instant the timer was last ticked.
  time_delta running_time; // Time elapsed since the timer was last started.
  time_delta delta_time;   // Time elapsed since the last tick.
} Timer;

/// Construct a new timer.
Timer timer_new(void);

/// Start the timer.
/// This sets the time point from which time deltas are measured.
/// Calling this multilple times resets the timer.
void timer_start(Timer*);

/// Update the timer's running and delta times.
void timer_tick(Timer*);

/// Get the current time.
time_point time_now(void);

/// Return the time elapsed between two timestamps.
time_delta time_diff(time_point start, time_point end);

/// Return the time elapsed in seconds.
double time_delta_to_sec(time_delta dt);

/// Convert the time elapsed in seconds to a time delta.
time_delta sec_to_time_delta(double seconds);

/// Put the caller thread to sleep for the given amount of time.
void time_sleep(time_delta dt);

/// The time point 0.
#ifdef _WIN32
static const time_point time_zero = 0;
#else
static const time_point time_zero = {0, 0};
#endif
