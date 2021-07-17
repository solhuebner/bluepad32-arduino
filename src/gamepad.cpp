// Copyright 2021 - 2021, Ricardo Quesada, http://retro.moe
// SPDX-License-Identifier: Apache-2.0 or LGPL-2.1-or-later

#include "gamepad.h"

#include <inttypes.h>

#include "constants.h"
#include "utility/debug.h"
#include "utility/spi_drv.h"
#include "utility/wl_types.h"

Gamepad::Gamepad() : _connected(false), _state() {}

bool Gamepad::isConnected() const { return _connected; }

void Gamepad::setPlayerLEDs(uint8_t led) const {
  if (!isConnected()) {
    WARN("gamepad not connected");
    return;
  }

  WAIT_FOR_SLAVE_SELECT();
  // Send Command
  SpiDrv::sendCmd(BP32_SET_GAMEPAD_PLAYERS_LED, PARAM_NUMS_2);
  SpiDrv::sendParam((uint8_t*)&_state.idx, 1, NO_LAST_PARAM);
  SpiDrv::sendParam((uint8_t*)&led, 1, LAST_PARAM);

  // pad to multiple of 4
  SpiDrv::readChar();

  SpiDrv::spiSlaveDeselect();
  // Wait the reply elaboration
  SpiDrv::waitForSlaveReady();
  SpiDrv::spiSlaveSelect();

  // Wait for reply
  uint8_t data, dataLen;
  if (!SpiDrv::waitResponseCmd(BP32_SET_GAMEPAD_PLAYERS_LED, PARAM_NUMS_1,
                               &data, &dataLen)) {
    WARN("error waitResponse");
  }
  SpiDrv::spiSlaveDeselect();

  if (data != BP32_RESPONSE_OK || dataLen != 1) {
    WARN("Failed to set players led");
  }
}

void Gamepad::setColorLED(uint8_t red, uint8_t green, uint8_t blue) const {
  if (!isConnected()) {
    WARN("gamepad not connected");
    return;
  }

  WAIT_FOR_SLAVE_SELECT();
  // Send Command
  SpiDrv::sendCmd(BP32_SET_GAMEPAD_COLOR_LED, PARAM_NUMS_4);
  SpiDrv::sendParam((uint8_t*)&_state.idx, 1, NO_LAST_PARAM);
  SpiDrv::sendParam((uint8_t*)&red, 1, NO_LAST_PARAM);
  SpiDrv::sendParam((uint8_t*)&green, 1, NO_LAST_PARAM);
  SpiDrv::sendParam((uint8_t*)&blue, 1, LAST_PARAM);

  // pad to multiple of 4
  SpiDrv::readChar();
  SpiDrv::readChar();
  SpiDrv::readChar();

  SpiDrv::spiSlaveDeselect();
  // Wait the reply elaboration
  SpiDrv::waitForSlaveReady();
  SpiDrv::spiSlaveSelect();

  // Wait for reply
  uint8_t data, dataLen;
  if (!SpiDrv::waitResponseCmd(BP32_SET_GAMEPAD_COLOR_LED, PARAM_NUMS_1, &data,
                               &dataLen)) {
    WARN("error waitResponse");
  }
  SpiDrv::spiSlaveDeselect();

  if (data != BP32_RESPONSE_OK || dataLen != 1) {
    WARN("Failed to set color led");
  }
}

void Gamepad::setRumble(uint8_t force, uint8_t duration) const {
  if (!isConnected()) {
    WARN("gamepad not connected");
    return;
  }

  WAIT_FOR_SLAVE_SELECT();
  // Send Command
  SpiDrv::sendCmd(BP32_SET_GAMEPAD_RUMBLE, PARAM_NUMS_3);
  SpiDrv::sendParam((uint8_t*)&_state.idx, 1, NO_LAST_PARAM);
  SpiDrv::sendParam((uint8_t*)&force, 1, NO_LAST_PARAM);
  SpiDrv::sendParam((uint8_t*)&duration, 1, LAST_PARAM);

  // already padded to multiple of 4

  SpiDrv::spiSlaveDeselect();
  // Wait the reply elaboration
  SpiDrv::waitForSlaveReady();
  SpiDrv::spiSlaveSelect();

  // Wait for reply
  uint8_t data, dataLen;
  if (!SpiDrv::waitResponseCmd(BP32_SET_GAMEPAD_RUMBLE, PARAM_NUMS_1, &data,
                               &dataLen)) {
    WARN("error waitResponse");
  }

  if (data != BP32_RESPONSE_OK || dataLen != 1) {
    WARN("Failed to set rumble");
  }
  SpiDrv::spiSlaveDeselect();
}