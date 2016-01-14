/**
 * Project:     PS2X Library
 * Description: Playstation 2 controller library for Arduino
 * Version:     v2.4
 * Author:      Bill Porter
 *              Kompanets Konstantin (aka I2M)
 */

#ifndef PS2X_H
#define PS2X_H

#include "Arduino.h"

#define ANALOG_MODE      0x73

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

class PS2X {
public:
  void ConfigGamepad(uint8_t dat_pin, uint8_t cmd_pin, uint8_t att_pin, uint8_t clk_pin);
  bool ReadGamepad();
  bool Button(uint16_t button);
  bool ButtonPressed(uint16_t button);
  bool ButtonReleased(uint16_t button);
  bool NewButtonState();
  bool NewButtonState(uint16_t button);
  uint8_t Analog(uint8_t button);
  uint16_t ButtonDataByte();
private:
  void InitGamepad();
  void SendCommand(uint8_t *send_data, uint8_t size);
  uint8_t ShiftGamepad(uint8_t transmit_byte);
  uint8_t _clk_pin;
  uint8_t _cmd_pin;
  uint8_t _att_pin;
  uint8_t _dat_pin;
  uint8_t _data[9];
  uint16_t _last_buttons;
  uint16_t _buttons;
};

#endif
