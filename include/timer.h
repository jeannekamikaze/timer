#pragma once

#ifndef _WIN32
#include <time.h>
#endif

namespace kx {

#ifdef _WIN32
    #ifdef _MSC_VER
        using time_point = __int64;
    #else
        using time_point = __UINT64_TYPE__;
    #endif
#else
    using time_point = timespec;
#endif

// to-do: consider using a new type and enforcing rules.
// example: TimePoint - TimePoint = TimeDelta
using time_delta = double;

/// A high resolution timer.
class Timer
{
    time_point start_time;   // The instant the timer was last started.
    time_point last_tick;    // The instant the timer was last ticked.
    time_delta running_time; // Time elapsed since the timer was last started.
    time_delta delta_time;   // Time elapsed since the last tick.

public:

    /// Construct a new timer.
    Timer ();

    /// Update the timer's running and delta times.
    void tick ();

    /// Start the timer.
    /// This sets the time point from which time deltas are measured.
    /// Calling this multilple times effectively resets the timer.
    void start ();

    /// Return the total running from the last tick to the last time the timer was started.
    time_delta time () const { return running_time; }

    /// Return the time elapsed since the last tick.
    time_delta delta () const { return delta_time; }
};

/// Get a timestamp of the current time.
time_point now ();

/// Return the time elapsed between two timestamps.
time_delta time_diff (time_point start, time_point end);

/// Put the caller thread to sleep for the given number of seconds.
void sleep (time_delta seconds);

/// Return time point 0.
extern const time_point time_zero;

} // namespace kx
