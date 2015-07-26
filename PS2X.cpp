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

uint8_t config_data[] = { 0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t config_join[] = { 0x01, 0x43, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t config_mode[] = { 0x01, 0x44, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00 };
uint8_t config_exit[] = { 0x01, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void PS2X::config_gamepad(uint8_t dat_pin, uint8_t cmd_pin, uint8_t att_pin, uint8_t clk_pin) {
  _dat_pin = dat_pin;
  _cmd_pin = cmd_pin;
  _att_pin = att_pin;
  _clk_pin = clk_pin;

  pinMode(_dat_pin, INPUT_PULLUP);
  pinMode(_cmd_pin, OUTPUT);
  pinMode(_att_pin, OUTPUT);
  pinMode(_clk_pin, OUTPUT);

  digitalWrite(_cmd_pin, HIGH);
  digitalWrite(_att_pin, HIGH);
  digitalWrite(_clk_pin, HIGH);

  init_gamepad();
}

bool PS2X::read_gamepad() {
  for (uint8_t i = 0; i <= 10; i++) {
    send_command(config_data, sizeof(config_data));

    if (_data[1] == 0x73) {
      break;
    }
    else {
      init_gamepad();
    }
  }

  _last_buttons = _buttons;
  _buttons = *(uint16_t*)(_data + 3);

  return (_data[1] == 0x73);
}

uint8_t PS2X::Analog(uint8_t button) {
  return _data[button];
}

bool PS2X::Button(uint16_t button) {
  return ((~_buttons & button) > 0);
}

bool PS2X::ButtonPressed(uint16_t button) {
  return (NewButtonState(button) & Button(button));
}

bool PS2X::ButtonReleased(uint16_t button) {
  return ((NewButtonState(button)) & ((~_last_buttons & button) > 0));
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
  send_command(config_data, sizeof(config_data));
  send_command(config_join, sizeof(config_join));
  send_command(config_mode, sizeof(config_mode));
  send_command(config_exit, sizeof(config_exit));
}

void PS2X::send_command(uint8_t *send_data, uint8_t size) {
  digitalWrite(_att_pin, LOW);
  delayMicroseconds(BYTE_DELAY);

  for (uint8_t i = 0; i < size; i++) {
    _data[i] = shift_gamepad(send_data[i]);
    delayMicroseconds(BYTE_DELAY);
  }

  digitalWrite(_att_pin, HIGH);
  delayMicroseconds(ATT_DELAY);
}

uint8_t PS2X::shift_gamepad(uint8_t transmit_byte) {
  uint8_t received_byte = 0;

  for (uint8_t i = 0; i < 8; i++) {
    if (transmit_byte & _BV(i)) {
      digitalWrite(_cmd_pin, HIGH);
    }
    else {
      digitalWrite(_cmd_pin, LOW);
    }

    digitalWrite(_clk_pin, LOW);
    delayMicroseconds(1);

    if (digitalRead(_dat_pin)) {
      received_byte |= _BV(i);
    }

    digitalWrite(_clk_pin, HIGH);
  }
  digitalWrite(_cmd_pin, HIGH);

  return received_byte;
}
