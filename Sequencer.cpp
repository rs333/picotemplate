#include "Sequencer.h"
#include <stdio.h>
#include <inttypes.h>

void Sequencer::init(const SequencerOptions &options)
{
  step_size_us = options.step_size_us;

  last_time = get_absolute_time();
  light = false;
  // Initialize LED pin
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  // Initialize chosen serial port
  stdio_init_all();
}

bool Sequencer::step(struct repeating_timer *t)
{
  curr_time = get_absolute_time();
  delta_time = absolute_time_diff_us(last_time, curr_time);

  if (delta_time > step_size_us)
  {
    printf("Delta Exceeded: %" PRId64 "\n", delta_time);
  };

  gpio_put(LED_PIN, light);
  light ^= true;
  last_time = curr_time;
  return true;
}