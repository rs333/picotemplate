#pragma once
#include "pico/stdlib.h"
#include <hardware/i2c.h>
#include "LCD1602.h"
class Sequencer
{
public:
  struct SequencerOptions
  {
    int64_t step_size_us;
  };

  void init(const SequencerOptions &options);
  bool step(struct repeating_timer *t);

private:
  absolute_time_t curr_time;
  absolute_time_t last_time;
  int64_t last_seconds = 0;
  int64_t delta_time;
  int64_t step_size_us;

  const uint LED_PIN = 25;
  bool light;
  i2c_inst_t *i2c;
  LCD1602 lcd;

  static const size_t MAX_DIGITS = 32;
  char display_buffer[MAX_DIGITS + 1];

  void updateDisplayBuffer(uint64_t timestamp);
};