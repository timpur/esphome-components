#pragma once

#include "../../lib/registers.hpp"
#include "Arduino.h"

class RegisterManager {
public:
  RegisterManager() {
    REGISTERS[REGISTER_IDENTIFICATION] = REGISTER_IDENTIFICATION_VALUE;
    RESET();
  }

  uint8_t inline GET_PTR() {
    // Get reg ptr
    return REGISTER_PTR;
  }
  void inline SET_PTR(uint8_t reg) {
    // Set reg ptr and contrain to size of regs
    REGISTER_PTR = constrain(reg, 0, REGISTER_SIZE - 1);
  }
  void inline INCREMENT() {
    // Increment reg ptr
    SET_PTR(REGISTER_PTR + 1);
  }
  uint8_t *REF() {
    // Get ptr ref of reg at ptr
    return REF(REGISTER_PTR);
  }
  uint8_t *REF(uint8_t reg) {
    // Get ptr ref of reg at ptr
    return REGISTERS + reg;
  }
  uint8_t inline REMAINING() {
    // Remaining size from ptr to regs length
    return REGISTER_SIZE - REGISTER_PTR;
  }
  uint8_t inline GET() {
    // Get value of reg at ptr
    return GET(REGISTER_PTR);
  }
  uint8_t inline GET(uint8_t reg) {
    // Get value of reg at ptr
    return REGISTERS[reg];
  }
  void inline SET(uint8_t val) {
    // Set reg value at ptr and constrain to writable regs
    SET(REGISTER_PTR, val);
  }
  void inline SET(uint8_t reg, uint8_t val) {
    // Set reg value at ptr and constrain to writable regs
    if (reg >= 0x01 && reg <= 0x03) {
      REGISTERS[reg] = val;
    }
  }
  void inline RESET() {
    // Reset regs to defaults
    SET(REGISTER_ADDRESS, REGISTER_ADDRESS_DFAULT);
    SET(REGISTER_CONFIG, REGISTER_CONFIG_DEFAULT);
  }
  uint8_t inline GET_PIN() {
    uint8_t pin = (GET(REGISTER_CONFIG) & REGISTER_CONFIG_PIN_MASK) >>
                  REGISTER_CONFIG_PIN_SHIFT;
    switch (pin) {
    case 0:
      return 5;
    case 1:
      return 3;
    case 2:
      return 4;
    case 3:
      return 1;
    default:
      return 3;
    }
  }
  uint8_t inline GET_INPUT_PULLUP() {
    return (GET(REGISTER_CONFIG) & REGISTER_CONFIG_INPUT_PULLUP_MASK) >>
           REGISTER_CONFIG_INPUT_PULLUP_SHIFT;
  }
  uint8_t inline GET_PULSEWIDTH_FILTER_FACTOR() {
    return (GET(REGISTER_CONFIG) & REGISTER_CONFIG_PULSE_WIDTH_FILTER_MASK) >>
           REGISTER_CONFIG_PULSE_WIDTH_FILTER_SHIFT;
  }

private:
  // Array of bytes for all regs
  uint8_t REGISTERS[REGISTER_SIZE];
  // Reg ptr
  uint8_t REGISTER_PTR = 0x00;

} REGISTERS;
