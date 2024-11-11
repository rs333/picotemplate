#include "Sequencer.h"
#include <hardware/i2c.h>
#include <inttypes.h>
#include <stdio.h>
#include <string>

void Sequencer::init(const SequencerOptions &options) {
    step_size_us = options.step_size_us;

    last_time = get_absolute_time();
    light = false;
    // Initialize LED pin
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    i2c = i2c_default;

    // Initialize chosen serial port
    stdio_init_all();
    lcd.init();
    lcd.scan_i2c_bus();
    lcd.clear();
    char bullet[] = "o";
    for (int row = 1; row >= 0; row--) {
        for (int col = 15; col >= 0; col--) {
            sleep_ms(40);
            lcd.write(row, col, bullet);
        }
    }
}

void Sequencer::updateDisplayBuffer(uint64_t timestamp_us) {
    uint64_t last = timestamp_us / 1000;
    int32_t ms = last % 1000;
    last = last / 1000;
    int32_t s = last % 60;
    last = last / 60;
    int32_t m = last % 60;
    last = last / 60;
    int32_t h = last % 24;
    int32_t d = last / 24;

    snprintf(display_buffer, MAX_DIGITS + 1,
             "Day-HH:MM:SS.sss%03d-%02d:%02d:%02d.%03d", d, h, m, s, ms);
}

bool Sequencer::step(struct repeating_timer *t) {
    curr_time = get_absolute_time();
    delta_time = absolute_time_diff_us(last_time, curr_time);
    updateDisplayBuffer(to_us_since_boot(curr_time));

    lcd.write(0, 0, display_buffer);
    if (delta_time > step_size_us) {
        printf("Delta Exceeded: %" PRId64 " > %" PRId64 "\n", delta_time,
               step_size_us);
    };
    int64_t seconds = curr_time / 1000000;
    if (seconds != last_seconds) {
        gpio_put(LED_PIN, light);
        printf("    Delta Time: %" PRId64 " <= %" PRId64 "\n", delta_time,
               step_size_us);

        light = !light;
    }
    last_time = curr_time;
    last_seconds = seconds;
    return true;
}