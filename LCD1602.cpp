#include "LCD1602.h"
#include <hardware/i2c.h>
#include <stdio.h>
#include <cstring>
// #include <inttypes.h>
#include <pico/stdlib.h>

void errorCheck(int result)
{
  if (result < 0)
  {
    printf("i2c Error");
  }
  else
  {
    printf("%02x", result);
  }
  // printf((result < 0) ? "i2c error" : "!");
}
void LCD1602::backlight_on()
{
  backlight = BL_BIT;
  // send_command(0);
}

void LCD1602::backlight_off()
{
  backlight = 0;
  // send_command(0);
}
void LCD1602::clear()
{
  send_command(CLEAR_DISPLAY, 2500);
}
void LCD1602::home()
{
  send_command(RETURN_HOME, 2500);
}
void LCD1602::init()
{
  int result = 0;

  i2c_init(i2c_default, 100 * 1000);

  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  backlight_on();
  send_command(FUNCTION_SET | DATA_LENGTH_8BITS);
  send_command(FUNCTION_SET | DATA_LENGTH_8BITS);
  send_command(FUNCTION_SET | DATA_LENGTH_8BITS);
  send_command(FUNCTION_SET | DATA_LENGTH_4BITS);
  send_command(FUNCTION_SET | DATA_LENGTH_4BITS | TWO_LINES | FONT_5X7);
  send_command(DISPLAY_CONTROL | DISPLAY_OFF | CURSOR_OFF | BLINK_OFF);
  clear();
  send_command(ENTRY_MODE_SET | ENTRY_RIGHT); // Fill from right to left
  send_command(DISPLAY_CONTROL | DISPLAY_ON | CURSOR_OFF | BLINK_OFF);
}

void LCD1602::scan()
{
  printf("\nI2C Bus Scan\n");
  printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
  for (int addr = 0; addr < (1 << 7); ++addr)
  {
    if (addr % 16 == 0)
    {
      printf("%02x ", addr);
    }

    int ret;
    uint8_t rxdata;

    ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);

    printf(ret < 0 ? "." : "@");
    printf(addr % 16 == 15 ? "\n" : "  ");
  }
}
// int LCD1602::get_data(const uint8_t command, uint32_t delay)
// {
//   uint8_t data_t[4];
//   uint8_t data_u = (command & 0xf0);
//   uint8_t data_l = (command & 0x0f) << 4;
//   data_t[0] = data_u | backlight | EN_BIT | RS_BIT;
//   data_t[1] = data_u | backlight | RS_BIT;
//   data_t[2] = data_l | backlight | EN_BIT | RS_BIT;
//   data_t[3] = data_l | backlight | RS_BIT;

//   int retVal = i2c_write_blocking(i2c_default, ADDR, data_t, 4, false);

//   busy_wait_us(delay);
//   return retVal;
// }
void LCD1602::scan_memory()
{
  send_command(SET_DDRAM_ADDR + 0x40 * 0, 2000);
  uint8_t memory[32 + 1];
  int ret = 0;
  for (int i = 0; i <= 32; ++i)
  {
    memory[i] = 0;
    uint8_t data[4];
    data[0] = 0 | RW_BIT | RS_BIT;
    data[1] = 0 | RW_BIT | RS_BIT | EN_BIT;
    data[2] = 0 | RW_BIT | RS_BIT;
    data[3] = 0 | RW_BIT | RS_BIT | EN_BIT;

    ret = i2c_read_blocking(i2c_default, ADDR, data, 4, false);
    printf("%d,", ret);
    for (int j = 0; j < ret; ++j)
    {
      printf("%02x ", data[j]);
    }
    printf("\n");
    busy_wait_ms(1);
  }
  printf("\n");
  if (ret > 0)
  {
    for (int i = 0; i < 32; ++i)
    {
      printf("%02x", memory[i]);
      printf(((i + 1) % 16 == 0 ? "\n" : " "));
    }
  }
}

int LCD1602::send_command(const uint8_t command, uint32_t delay)
{
  uint8_t data_t[4];
  uint8_t data_u = (command & 0xf0);
  uint8_t data_l = (command & 0x0f) << 4;
  data_t[0] = data_u | backlight | EN_BIT;
  data_t[1] = data_u | backlight;
  data_t[2] = data_l | backlight | EN_BIT;
  data_t[3] = data_l | backlight;
  int retVal = i2c_write_blocking(i2c_default, ADDR, data_t, 4, false);

  busy_wait_us(delay); // TODO Find out if LCD sends response when done.
  return retVal;
}
int LCD1602::send_data(const uint8_t command, uint32_t delay)
{
  uint8_t data_t[4];
  uint8_t data_u = (command & 0xf0);
  uint8_t data_l = (command & 0x0f) << 4;
  data_t[0] = data_u | backlight | EN_BIT | RS_BIT;
  data_t[1] = data_u | backlight | RS_BIT;
  data_t[2] = data_l | backlight | EN_BIT | RS_BIT;
  data_t[3] = data_l | backlight | RS_BIT;

  int retVal = i2c_write_blocking(i2c_default, ADDR, data_t, 4, false);

  busy_wait_us(delay);
  return retVal;
}

int LCD1602::write(int row, int col, char data[])
{
  // constrain the input position
  if (col < 0)
    col = 0;
  if (col > 15)
    col = 15;
  if (row < 0)
    row = 0;
  if (row > 1)
    row = 1;

  // Move cursor by setting display data ram address
  uint8_t ddram_address = SET_DDRAM_ADDR + 0x40 * row + col;
  send_command(ddram_address); // send the x y data

  size_t data_length = strlen(data);
  size_t total_sent = 0;
  for (size_t i = 0; i < data_length; i++)
  {
    total_sent += send_data(data[i]);
  } // send the data
  return total_sent;
}