#pragma once
#include <stdint.h>

class LCD1602 {
  public:
    // commands
    static const uint8_t CLEAR_DISPLAY = 0x01;
    static const uint8_t RETURN_HOME = 0x02;
    static const uint8_t ENTRY_MODE_SET = 0x04;
    static const uint8_t DISPLAY_CONTROL = 0x08;
    static const uint8_t CURSOR_SHIFT = 0x10;
    static const uint8_t FUNCTION_SET = 0x20;
    static const uint8_t SET_CGRAM_ADDR = 0x40;
    static const uint8_t SET_DDRAM_ADDR = 0x80;

    // flags for ENTRY_MODE_SET
    const uint8_t ENTRY_SHIFT_INCREMENT = 0x01;
    const uint8_t ENTRY_LEFT = 0x02;
    const uint8_t ENTRY_RIGHT = 0x00;

    // flags for DISPLAY_CONTROL
    const uint8_t BLINK_ON = 0x01;
    const uint8_t BLINK_OFF = 0x00;
    const uint8_t CURSOR_ON = 0x02;
    const uint8_t CURSOR_OFF = 0x00;
    const uint8_t DISPLAY_ON = 0x04;
    const uint8_t DISPLAY_OFF = 0x00;

    // flags for CURSOR_SHIFT
    const uint8_t MOVE_RIGHT = 0x04;
    const uint8_t DISPLAY_MOVE = 0x08;

    // flags for FUNCTION_SET
    static const uint8_t DATA_LENGTH_8BITS = 0x10; // everything is datalength 4
    static const uint8_t DATA_LENGTH_4BITS = 0X00;
    static const uint8_t TWO_LINES = 0x08; //  Two Lines
    static const uint8_t ONE_LINE = 0x00;
    static const uint8_t FONT_5X11 = 0x04; // 5x10 Dots
    static const uint8_t FONT_5X7 = 0X00;

    // flag for COMMAND control
    const uint8_t RS_BIT = 0x01; // Register Select
    const uint8_t RW_BIT = 0x02; // Read/Write Select
    const uint8_t EN_BIT = 0x04; // Operation Enable
    const uint8_t BL_BIT = 0x08; // Backlight Bit

    // By default these LCD display drivers are on bus address 0x27
    uint8_t ADDR = 0x27;

    // Modes for send_byte
    const uint8_t CHARACTER = 1;
    const uint8_t COMMAND = 0;

    const uint8_t MAX_LINES = 2;
    const uint8_t MAX_CHARS = 16;

    void scan_i2c_bus();
    void init();
    int send_command(const uint8_t command, uint32_t delay = 120);
    int send_data(uint8_t command, uint32_t delay = 120);
    int write(int row, int col, char data[]);

    uint8_t backlight = 0;
    uint8_t display = 0;
    uint8_t cursor = 0;
    uint8_t blink = 0;

    void clear();
    void home();
    void backlight_on();
    void backlight_off();

  private:
    int send_byte(uint8_t data, uint8_t modifiers, uint32_t delay = 120);
};
