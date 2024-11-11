#include "LCD1602.h"
#include <cstring>
#include <hardware/i2c.h>
#include <stdio.h>
// #include <inttypes.h>
#include <pico/stdlib.h>

void errorCheck(int result) {
    if (result < 0) {
        printf("i2c Error");
    } else {
        printf("%02x", result);
    }
    printf((result < 0) ? "i2c error" : "!");
}
void LCD1602::backlight_on() { backlight = BL_BIT; }

void LCD1602::backlight_off() { backlight = 0; }
void LCD1602::clear() { send_command(CLEAR_DISPLAY, 6000); }
void LCD1602::home() { send_command(RETURN_HOME, 6000); }
void LCD1602::init() {
    i2c_init(i2c_default, 1776 * 1000);

    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    backlight_on();
    sleep_ms(30);
    send_command(FUNCTION_SET | DATA_LENGTH_8BITS, 8200);
    send_command(FUNCTION_SET | DATA_LENGTH_8BITS, 200);
    send_command(FUNCTION_SET | DATA_LENGTH_8BITS);
    send_command(FUNCTION_SET | DATA_LENGTH_8BITS);
    send_command(FUNCTION_SET | DATA_LENGTH_4BITS);
    send_command(FUNCTION_SET | DATA_LENGTH_4BITS | TWO_LINES | FONT_5X7);
    send_command(DISPLAY_CONTROL | DISPLAY_OFF | CURSOR_OFF | BLINK_OFF);
    clear();
    // send_command(ENTRY_MODE_SET | ENTRY_RIGHT); // Fill from right to left
    send_command(DISPLAY_CONTROL | DISPLAY_ON | CURSOR_OFF | BLINK_OFF, 200);
}

void LCD1602::scan_i2c_bus() {
    printf("\nI2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
    for (uint8_t addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        int ret;
        uint8_t rxdata;

        ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
}

int LCD1602::send_byte(uint8_t data, uint8_t modifiers, uint32_t delay) {
    uint8_t data_u = (data & 0xf0) | modifiers;
    uint8_t data_l = (data & 0x0f) << 4 | modifiers;
    uint8_t data_u_en = data_u | EN_BIT;
    uint8_t data_l_en = data_l | EN_BIT;

    // Set RS & RW, other values are don't care
    int retVal = i2c_write_blocking(i2c_default, ADDR, &data_u_en, 1, false);
    if (retVal != 1)
        printf("i2c send mismatch\n");
    // Wait at least 125ns
    busy_wait_us(1);

    // Set Enable, high order bits, and modifiers
    retVal += i2c_write_blocking(i2c_default, ADDR, &data_u_en, 1, false);
    if (retVal != 2)
        printf("i2c send mismatch\n");
    // Wait at least 475ns to ensure data is valid when enable is cleared
    busy_wait_us(1);

    // Clear Enable, to trigger write
    retVal += i2c_write_blocking(i2c_default, ADDR, &data_u, 1, false);
    if (retVal != 3)
        printf("i2c send mismatch\n");
    // Wait at least 550 ns  to ensure sufficient cycle time and data hold time.
    busy_wait_us(1);

    // Set Enable, low order bits, and maintain modifiers
    retVal += i2c_write_blocking(i2c_default, ADDR, &data_l_en, 1, false);
    if (retVal != 4)
        printf("i2c send mismatch\n");
    // Wait at least 475ns to ensure data is valid when enable is cleared
    busy_wait_us(1);

    // Wait at least 550 ns  to ensure sufficient cycle time and data hold time.
    retVal += i2c_write_blocking(i2c_default, ADDR, &data_l, 1, false);
    busy_wait_us(1);
    if (retVal != 5)
        printf("i2c send mismatch\n");

    // Wait for command to finish
    busy_wait_us(delay);

    return retVal;
}

int LCD1602::send_command(const uint8_t command, uint32_t delay) {
    return send_byte(command, backlight, delay);
}

int LCD1602::send_data(const uint8_t data, uint32_t delay) {
    return send_byte(data, RS_BIT | backlight, delay);
}

int LCD1602::write(int row, int col, char data[]) {
    size_t data_length = strlen(data);
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

    for (size_t i = 0; i < data_length; i++) {
        uint8_t ddram_address = SET_DDRAM_ADDR + 0x40 * row + col;
        send_command(ddram_address); // send the x y data
        send_data(data[i]);
        col++;
        // Wrap
        if (col > 15) {
            row++;
            col = 0;
        }
        if (row > 1) {
            row = 0;
        }
    } // send the data
    return data_length;
}