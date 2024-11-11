#pragma once
#include "Sequencer.h"
#include "pico/stdlib.h"
#include <atomic>

class Runner
{
public:
  struct RunnerOptions
  {
    int64_t step_size_us;
  };

private:
  Runner() = default;;

public:
  Runner(Runner const &) = delete;
  Runner(Runner &&) = delete;
  void operator=(Runner const &) = delete;
  Runner &operator=(Runner &&) = delete;

  static Runner &getInstance()
  {
    static Runner instance;
    return instance;
  }

  void init(const RunnerOptions &options);

  int run();

private:
  static bool callback(struct repeating_timer *t);
  std::atomic<bool> keep_running;
  struct repeating_timer timer;
  int64_t step_size_us;
  static Sequencer sequencer;
};