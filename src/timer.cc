#include <timer.h>
#include <stdlib.h>

using namespace kx;

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

namespace {
#ifdef _WIN32
double seconds_per_count;
#endif

void timer_initialise ()
{
#ifdef _WIN32
    __int64 counts_per_sec;
    QueryPerformanceFrequency((LARGE_INTEGER*)&counts_per_sec);
    seconds_per_count = 1.0 / (double)counts_per_sec;
#endif
}
} // namespace

#ifdef _WIN32
time_point time_zero = 0;
#else
time_point time_zero = {0,0};
#endif

time_point kx::now ()
{
    time_point t;
#ifdef _WIN32
    QueryPerformanceCounter((LARGE_INTEGER*)&t);
#else
    clock_gettime(CLOCK_REALTIME, &t);
#endif
    return t;
}

time_delta kx::time_diff (time_point start, time_point end)
{
#ifdef WIN32
    // Force nonnegative. The DXSDK's CDXUTTimer mentions that if the
    // processor goes into a power save mode or we get shuffled to
    // another processor, then the delta time can be negative.
    return std::max(0, (end-start)*seconds_per_count);
#else
    return time_delta(end.tv_sec - start.tv_sec) + time_delta(end.tv_nsec - start.tv_nsec)*1.0e-9;
#endif
}

void kx::sleep (time_delta seconds)
{
#ifdef _WIN32
    Sleep((DWORD)(seconds * 1000));
#else
    struct timespec ts;
    ts.tv_sec  = (int) seconds;
    ts.tv_nsec = (long) ((double)(seconds - (int)seconds) * 1e9);
    nanosleep(&ts, NULL);
#endif
}

Timer::Timer ()
{
    timer_initialise();
    start();
}

void Timer::tick ()
{
    time_point this_tick = now();
    running_time = time_diff(start_time, this_tick);
    delta_time = time_diff(last_tick, this_tick);
    last_tick = this_tick;
}

void Timer::start ()
{
    start_time = now();
    last_tick = start_time;
    running_time = 0;
    delta_time = 0;
}
