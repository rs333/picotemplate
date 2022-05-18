#include "Sequencer.h"
#include <stdio.h>
#include <inttypes.h>
#include <string>
// #include <hardware/i2c.h"?


const uint sda_pin = 6;
const uint scl_pin = 7;
const uint8_t LCD_ADDR = 0x27;
void Sequencer::init(const SequencerOptions &options)
{
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
  lcd.scan();
  // lcd.scan_memory();
  char cat[] = "X";
  lcd.write(0,0,cat);
  lcd.write(1,5,cat);  
  lcd.scan_memory();
  // lcd.clear();
}

void Sequencer::updateDigits(uint64_t timestamp_us){
  size_t digit = 0;
  digits[1] = 0;
  digits[0] = 0;
  uint64_t last_ms = timestamp_us / 1000;
  while( last_ms > 0 && digit < MAX_DIGITS -1  ){
    // Shift digits right
    // for( int i=digit; i>0; --i){
    //   digits[i] = digits[i-1];
    // }
    digits[digit] = ( last_ms % 10 ) + 48 ;
    last_ms = last_ms / 10;
    digit++;
  }
  digits[digit] = 0;
}
bool Sequencer::step(struct repeating_timer *t)
{
  curr_time = get_absolute_time();
  delta_time = absolute_time_diff_us(last_time, curr_time);
  updateDigits(curr_time._private_us_since_boot);
  
  // printf("Current Time: %s\n",digits );
  lcd.write(0,14,digits);
  if (delta_time > step_size_us)
  {
    printf("Delta Exceeded: %" PRId64 "\n", delta_time);
  };
 
  gpio_put(LED_PIN, light);
  light ^= true;
  last_time = curr_time;
  return true;
}