#pragma once
#include "pico/stdlib.h"

class Sequencer
{
public:
  struct SequencerOptions
  {
    int64_t step_size_us;
  };

public:
  void init(const SequencerOptions &options);
  bool step(struct repeating_timer *t);

private:
  absolute_time_t curr_time;
  absolute_time_t last_time;
  int64_t delta_time;
  int64_t step_size_us;

  const uint LED_PIN = 25;
  bool light;
};