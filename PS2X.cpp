/**
 * Project PS2 Library
 * Description: PS2 controller library for Arduino
 * Version: v2.1
 * Programmer: Bill Porter
 *             Kompanets Konstantin (aka I2M)
 *
 * Hardware setup: Playstation 2 gamepad
 */

#include "PS2X.h"

static uint8_t config_enter[] = { 0x01, 0x43, 0x00, 0x01, 0x00 };
static uint8_t config_mode[] = { 0x01, 0x44, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00 };
static uint8_t config_exit[] = { 0x01, 0x43, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A };

void PS2X::config_gamepad(uint8_t dat_pin, uint8_t cmd_pin, uint8_t att_pin, uint8_t clk_pin) {
  _dat_pin = dat_pin;
  _cmd_pin = cmd_pin;
  _att_pin = att_pin;
  _clk_pin = clk_pin;

  pinMode(_dat_pin, INPUT_PULLUP);
  pinMode(_cmd_pin, OUTPUT);
  pinMode(_att_pin, OUTPUT);
  pinMode(_clk_pin, OUTPUT);

  init_gamepad();
}

bool PS2X::read_gamepad() {
  for (uint8_t i = 0; i <= 10; i++) {
    _data[0] = 0x01;
    _data[1] = 0x42;

    digitalWrite(_att_pin, LOW);

    for (uint8_t j = 0; j < 9; j++) {
      _data[j] = shift_gamepad(_data[j]);
    }

    digitalWrite(_att_pin, HIGH);

    if ((_data[1] & 0xf0) != 0x70) {
      init_gamepad();
    }
    else {
      break;
    }
  }

  _last_buttons = _buttons;
  _buttons = *(uint16_t*)(_data + 3);

  return ((_data[1] & 0xf0) == 0x70);
}

uint8_t PS2X::Analog(uint8_t button) {
  return _data[button];
}

bool PS2X::Button(uint16_t button) {
  return ((~_buttons & button) > 0);
}

bool PS2X::ButtonPressed(uint16_t button) {
  return(NewButtonState(button) & Button(button));
}

bool PS2X::ButtonReleased(uint16_t button) {
  return((NewButtonState(button)) & ((~_last_buttons & button) > 0));
}

bool PS2X::NewButtonState() {
  return ((_last_buttons ^ _buttons) > 0);
}

bool PS2X::NewButtonState(uint16_t button) {
  return (((_last_buttons ^ _buttons) & button) > 0);
}

uint16_t PS2X::ButtonDataByte() {
  return ~_buttons;
}

void PS2X::init_gamepad() {
  send_command(config_enter, sizeof(config_enter));
  send_command(config_mode, sizeof(config_mode));
  send_command(config_exit, sizeof(config_exit));
}

void PS2X::send_command(uint8_t *send_data, uint8_t size) {
  digitalWrite(_att_pin, LOW);

  for (uint8_t i = 0; i < size; i++) {
    shift_gamepad(send_data[i]);
  }

  digitalWrite(_att_pin, HIGH);
}

uint8_t PS2X::shift_gamepad(uint8_t transmit_byte) {
  uint8_t received_byte = 0;

  for(uint8_t i = 0; i < 8; i++) {
    if(transmit_byte & _BV(i)) {
      digitalWrite(_cmd_pin, HIGH);
    }
    else {
      digitalWrite(_cmd_pin, LOW);
    }

    digitalWrite(_clk_pin, LOW);

    if (digitalRead(_dat_pin)) {
      received_byte |= _BV(i);
    }

    digitalWrite(_clk_pin, HIGH);
  }

  return received_byte;
}
