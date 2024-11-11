#include "Runner.h"
#include "pico/stdlib.h"
#include <functional>

const uint64_t CHECK_KEEP_RUNNING_INTERVAL = 1000000; // us

Sequencer Runner::sequencer;

bool Runner::callback(struct repeating_timer *t)
{
  return sequencer.step(t);
}

void Runner::init(const RunnerOptions &options)
{
  keep_running = true;

  step_size_us = options.step_size_us;

  sequencer.init({.step_size_us = step_size_us});
}

int Runner::run()
{
  add_repeating_timer_us(-step_size_us, callback, nullptr, &timer);
  while (keep_running)
  {
    sleep_us(CHECK_KEEP_RUNNING_INTERVAL);
  }
  return 0;
}