/**
 * Project:     PS2X Library
 * Description: Playstation 2 controller library for Arduino
 * Version:     v2.6
 * Author:      Bill Porter
 *              Kompanets Konstantin (aka I2M)
 */

#ifndef PS2X_H
#define PS2X_H

#include "Arduino.h"

#define BYTE_DELAY       16
#define ATT_DELAY        16000

#define PSB_SELECT       0x0001
#define PSB_L3           0x0002
#define PSB_R3           0x0004
#define PSB_START        0x0008
#define PSB_PAD_UP       0x0010
#define PSB_PAD_RIGHT    0x0020
#define PSB_PAD_DOWN     0x0040
#define PSB_PAD_LEFT     0x0080
#define PSB_L2           0x0100
#define PSB_R2           0x0200
#define PSB_L1           0x0400
#define PSB_R1           0x0800
#define PSB_GREEN        0x1000
#define PSB_RED          0x2000
#define PSB_BLUE         0x4000
#define PSB_PINK         0x8000
#define PSB_TRIANGLE     0x1000
#define PSB_CIRCLE       0x2000
#define PSB_CROSS        0x4000
#define PSB_SQUARE       0x8000

#define PSS_RX           5
#define PSS_RY           6
#define PSS_LX           7
#define PSS_LY           8

#define PSAB_PAD_RIGHT   9
#define PSAB_PAD_LEFT    10
#define PSAB_PAD_UP      11
#define PSAB_PAD_DOWN    12
#define PSAB_GREEN       13
#define PSAB_RED         14
#define PSAB_BLUE        15
#define PSAB_PINK        16
#define PSAB_TRIANGLE    13
#define PSAB_CIRCLE      14
#define PSAB_CROSS       15
#define PSAB_SQUARE      16
#define PSAB_L1          17
#define PSAB_R1          18
#define PSAB_L2          19
#define PSAB_R2          20

class PS2X {
public:
  void ConfigGamepad(uint8_t dat_pin, uint8_t cmd_pin, uint8_t att_pin, uint8_t clk_pin, bool rumble = false, bool native = false);
  bool ReadGamepad(bool small_motor = false, uint8_t large_motor = 0);
  bool Button(uint16_t button);
  bool ButtonPressed(uint16_t button);
  bool ButtonReleased(uint16_t button);
  bool ButtonNewState();
  bool ButtonNewState(uint16_t button);
  uint8_t Analog(uint8_t button);
private:
  void InitGamepad();
  void SendCommand(const uint8_t *command, uint8_t size);
  uint8_t ShiftGamepad(uint8_t transmit_byte);
  uint8_t _clk_pin;
  uint8_t _cmd_pin;
  uint8_t _att_pin;
  uint8_t _dat_pin;
  uint8_t _data[21];
  uint16_t _last_buttons;
  uint16_t _buttons;
  bool _rumble;
  bool _native;
};

#endif
