/**
 * Project PS2 Library
 * Description: PS2 controller library for Arduino
 * Version: v2.0
 * Programmer: Bill Porter
 *             Kompanets Konstantin (aka I2M)
 *
 * Hardware setup: Playstation 2 gamepad
 */

#include "PS2X.h"

static uint8_t enter_config[] = { 0x01, 0x43, 0x00, 0x01, 0x00 };
static uint8_t analog_mode[] = { 0x01, 0x44, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00 };
static uint8_t exit_config[] = { 0x01, 0x43, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A };

uint16_t PS2X::Buttons() {
  return ~_buttons;
}

uint8_t PS2X::NewButtonState() {
  return ((_last_buttons ^ _buttons) > 0);
}

uint8_t PS2X::NewButtonState(uint16_t button) {
  return (((_last_buttons ^ _buttons) & button) > 0);
}

uint8_t PS2X::ButtonPressed(uint16_t button) {
  return(NewButtonState(button) & Button(button));
}

uint8_t PS2X::ButtonReleased(uint16_t button) {
  return((NewButtonState(button)) & ((~_last_buttons & button) > 0));
}

uint8_t PS2X::Button(uint16_t button) {
  return ((~_buttons & button) > 0);
}

uint8_t PS2X::Analog(uint8_t button) {
  return _data[button];
}

uint8_t PS2X::read_gamepad() {
  while (true) {
    digitalWrite(_cmd, HIGH);
    digitalWrite(_clk, HIGH);
    digitalWrite(_att, LOW);

    _data[0] = 0x01;
    _data[1] = 0x42;

    for (uint8_t i = 0; i < 21; i++) {
      _data[i] = gamepad_shift(_data[i]);
    }

    digitalWrite(_att, HIGH);

    if ((_data[1] & 0xf0) == 0x70) {
      break;
    }

    reconfig_gamepad();
  }

  _last_buttons = _buttons;
  _buttons = *(uint16_t*)(_data + 3);

  return ((_data[1] & 0xf0) == 0x70);
}

void PS2X::config_gamepad(uint8_t clk, uint8_t cmd, uint8_t att, uint8_t dat) {
  _clk = clk;
  _cmd = cmd;
  _att = att;
  _dat = dat;

  pinMode(_clk, OUTPUT);
  pinMode(_att, OUTPUT);
  pinMode(_cmd, OUTPUT);
  pinMode(_dat, INPUT_PULLUP);

  read_gamepad();
}

uint8_t PS2X::gamepad_shift(uint8_t transmit_byte) {
  uint8_t received_byte = 0;
  for(uint8_t i = 0; i < 8; i++) {
    if(transmit_byte & _BV(i)) {
      digitalWrite(_cmd, HIGH);
    }
    else {
      digitalWrite(_cmd, LOW);
    }

    digitalWrite(_clk, LOW);
    delayMicroseconds(1);

    if (digitalRead(_dat)) {
      received_byte |= _BV(i);
    }

    digitalWrite(_clk, HIGH);
  }

  digitalWrite(_cmd, HIGH);
  delayMicroseconds(10);

  return received_byte;
}

void PS2X::send_command(uint8_t send_data[], uint8_t size) {
  digitalWrite(_att, LOW);
  for (uint8_t i = 0; i < size; i++) {
    gamepad_shift(send_data[i]);
  }
  digitalWrite(_att, HIGH);
}

void PS2X::reconfig_gamepad() {
  send_command(enter_config, sizeof(enter_config));
  send_command(analog_mode, sizeof(analog_mode));
  send_command(exit_config, sizeof(exit_config));
}
