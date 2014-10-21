/**
 * Project PS2 Library
 * Description: PS2 controller library for Arduino
 * Version: v2.0
 * Programmer: Bill Porter
 *             Kompanets Konstantin (aka I2M)
 *
 * Hardware setup: Playstation 2 gamepad
 */

#ifndef PS2X_H
#define PS2X_H

#include "Arduino.h"

//These are our button constants
#define PSB_SELECT         0x0001
#define PSB_L3             0x0002
#define PSB_R3             0x0004
#define PSB_START          0x0008
#define PSB_PAD_UP         0x0010
#define PSB_PAD_RIGHT      0x0020
#define PSB_PAD_DOWN       0x0040
#define PSB_PAD_LEFT       0x0080
#define PSB_L2             0x0100
#define PSB_R2             0x0200
#define PSB_L1             0x0400
#define PSB_R1             0x0800
#define PSB_GREEN          0x1000
#define PSB_RED            0x2000
#define PSB_BLUE           0x4000
#define PSB_PINK           0x8000
#define PSB_TRIANGLE       0x1000
#define PSB_CIRCLE         0x2000
#define PSB_CROSS          0x4000
#define PSB_SQUARE         0x8000

//These are stick values
#define PSS_RX             5
#define PSS_RY             6
#define PSS_LX             7
#define PSS_LY             8

//These are analog buttons
#define PSAB_PAD_RIGHT     9
#define PSAB_PAD_UP        11
#define PSAB_PAD_DOWN      12
#define PSAB_PAD_LEFT      10
#define PSAB_L2            19
#define PSAB_R2            20
#define PSAB_L1            17
#define PSAB_R1            18
#define PSAB_GREEN         13
#define PSAB_RED           14
#define PSAB_BLUE          15
#define PSAB_PINK          16
#define PSAB_TRIANGLE      13
#define PSAB_CIRCLE        14
#define PSAB_CROSS         15
#define PSAB_SQUARE        16

class PS2X {
public:
  uint16_t ButtonDataByte();
  uint8_t NewButtonState();
  uint8_t NewButtonState(uint16_t);
  uint8_t ButtonPressed(uint16_t);
  uint8_t ButtonReleased(uint16_t);
  uint8_t Button(uint16_t);
  uint8_t Analog(uint8_t);
  uint8_t read_gamepad();
  void config_gamepad(uint8_t, uint8_t, uint8_t, uint8_t);
private:
  uint16_t _last_buttons;
  uint16_t _buttons;
  uint8_t _data[21];
  uint8_t _clk;
  uint8_t _cmd;
  uint8_t _att;
  uint8_t _dat;
  uint8_t shift_gamepad(uint8_t);
  void send_command(uint8_t*, uint8_t);
  void init_gamepad();
};

#endif
