/**
 * Project:     PS2X Library
 * Description: Playstation 2 controller library for Arduino
 * Version:     v2.6
 * Author:      Bill Porter
 *              Kompanets Konstantin (aka I2M)
 */

#include "PS2X.h"

uint8_t query_hello[] = { 0x01, 0x42, 0x00, 0x00, 0x00 };
uint8_t config_mode[] = { 0x01, 0x43, 0x00, 0x01, 0x00 };
uint8_t analog_mode[] = { 0x01, 0x44, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00 };
uint8_t rumble_mode[] = { 0x01, 0x4D, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t native_mode[] = { 0x01, 0x4F, 0x00, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00 };
uint8_t config_exit[] = { 0x01, 0x43, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A };

void PS2X::ConfigGamepad(uint8_t dat_pin, uint8_t cmd_pin, uint8_t att_pin, uint8_t clk_pin, bool rumble, bool native) {
  _dat_pin = dat_pin;
  _cmd_pin = cmd_pin;
  _att_pin = att_pin;
  _clk_pin = clk_pin;

  _rumble = rumble;
  _native = native;

  pinMode(_dat_pin, INPUT_PULLUP);
  pinMode(_cmd_pin, OUTPUT);
  pinMode(_att_pin, OUTPUT);
  pinMode(_clk_pin, OUTPUT);

  digitalWrite(_cmd_pin, HIGH);
  digitalWrite(_att_pin, HIGH);
  digitalWrite(_clk_pin, HIGH);

  InitGamepad();
}

bool PS2X::ReadGamepad(bool small_motor, uint8_t large_motor) {
  uint8_t query_data[21] = { 0x01, 0x42, 0x00, small_motor, large_motor };

  for (uint8_t i = 0; i < 2; i++) {
    if (_native) {
      SendCommand(query_data, sizeof(query_data));
    }
    else {
      SendCommand(query_data, 9);
    }

    if ((_data[1] & 0xF0) == 0x70) {
      break;
    }
    else {
      InitGamepad();
    }
  }

  _last_buttons = _buttons;
  _buttons = *(uint16_t *)(_data + 3);

  return ((_data[1] & 0xF0) == 0x70);
}

bool PS2X::Button(uint16_t button) {
  return ((~_buttons & button) > 0);
}

bool PS2X::ButtonPressed(uint16_t button) {
  return ((ButtonNewState(button)) & (Button(button)));
}

bool PS2X::ButtonReleased(uint16_t button) {
  return ((ButtonNewState(button)) & ((~_last_buttons & button) > 0));
}

bool PS2X::ButtonNewState() {
  return ((_last_buttons ^ _buttons) > 0);
}

bool PS2X::ButtonNewState(uint16_t button) {
  return (((_last_buttons ^ _buttons) & button) > 0);
}

uint8_t PS2X::Analog(uint8_t button) {
  return _data[button];
}

void PS2X::InitGamepad() {
  SendCommand(query_hello, sizeof(query_hello));
  SendCommand(config_mode, sizeof(config_mode));
  SendCommand(analog_mode, sizeof(analog_mode));

  if (_rumble) {
    SendCommand(rumble_mode, sizeof(rumble_mode));
  }
  if (_native) {
    SendCommand(native_mode, sizeof(native_mode));
  }

  SendCommand(config_exit, sizeof(config_exit));
}

void PS2X::SendCommand(const uint8_t *send_data, uint8_t size) {
  digitalWrite(_att_pin, LOW);
  delayMicroseconds(BYTE_DELAY);

  for (uint8_t i = 0; i < size; i++) {
    _data[i] = ShiftGamepad(send_data[i]);
    delayMicroseconds(BYTE_DELAY);
  }

  digitalWrite(_att_pin, HIGH);
  delayMicroseconds(ATT_DELAY);
}

uint8_t PS2X::ShiftGamepad(uint8_t transmit_byte) {
  uint8_t received_byte = 0;

  for (uint8_t i = 0; i < 8; i++) {
    if (transmit_byte & _BV(i)) {
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
  digitalWrite(_cmd_pin, HIGH);

  return received_byte;
}
