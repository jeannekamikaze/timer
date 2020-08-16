#include "timer.h"

#include <stdio.h>

#include "test.h"

// Sleep puts the calling thread to sleep for a time >= than the given time.
TEST_CASE(sleep) {
  const double sleep_time_sec = 0.1;

  const time_point start = time_now();
  time_sleep(sec_to_time_delta(sleep_time_sec));
  const time_point end = time_now();

  TEST_TRUE(time_delta_to_sec(time_diff(start, end)) >= sleep_time_sec);
}

// The timer starts as soon as it is created.
TEST_CASE(start_on_new) {
  const time_point test_start_time = time_now();
  Timer timer = timer_new();
  TEST_TRUE(time_delta_to_sec(time_diff(test_start_time, timer.start_time)) >=
            0.0);
}

// Tick updates the timer's last tick time.
TEST_CASE(tick_updates_last_tick_time) {
  const double sleep_time_sec = 0.1;

  Timer timer = timer_new();
  time_sleep(sec_to_time_delta(sleep_time_sec));
  timer_tick(&timer);

  TEST_TRUE(time_delta_to_sec(time_diff(timer.start_time, timer.last_tick)) >=
            sleep_time_sec);
}

// Tick updates the timer's delta time.
TEST_CASE(tick_updates_delta_time) {
  const double sleep_time_sec = 0.1;

  Timer timer = timer_new();
  time_sleep(sec_to_time_delta(sleep_time_sec));
  timer_tick(&timer);

  TEST_TRUE(time_delta_to_sec(timer.delta_time) >= sleep_time_sec);
}

// Tick leaves the timer's start time unchanged.
TEST_CASE(tick_does_not_change_start_time) {
  Timer timer = timer_new();
  const time_point start_time = timer.start_time;
  time_sleep(sec_to_time_delta(0.1));
  timer_tick(&timer);
  TEST_TRUE(time_delta_to_sec(time_diff(start_time, timer.start_time)) == 0.0);
}

// Start starts/restarts the timer and updates the timer's start time.
TEST_CASE(start_restarts_start_time) {
  const double sleep_time_seconds = 0.1;
  Timer timer = timer_new();
  const time_point start_time = timer.start_time;
  time_sleep(sec_to_time_delta(sleep_time_seconds));
  timer_start(&timer);
  TEST_TRUE(time_delta_to_sec(time_diff(start_time, timer.start_time)) >=
            sleep_time_seconds);
}

// Count the number of hundred-seconds in a second.
TEST_CASE(count) {
  Timer timer = timer_new();

  int hundred_seconds = 0;
  const time_point start = timer.start_time;
  {
    while (time_delta_to_sec(timer.running_time) <= 1.0) {
      hundred_seconds++;
      time_sleep(sec_to_time_delta(0.1));
      timer_tick(&timer);
      TEST_TRUE(time_delta_to_sec(timer.delta_time) >= 0.1);
    }
  }
  const time_point end = time_now();

  const double time_elapsed = time_delta_to_sec(time_diff(start, end));

  TEST_EQUAL(hundred_seconds, 10);
  TEST_TRUE(time_elapsed >= 1.0);
}

int main(int argc, const char** argv) {
  (void)argc; // Unused.
  printf("Usage: %s --unittest\n", argv[0]);
  return 0;
}
