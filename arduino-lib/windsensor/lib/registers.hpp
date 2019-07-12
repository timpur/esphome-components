#pragma once

// REGISTERS
#define REGISTER_SIZE (0x13)
#define REGISTER_IDENTIFICATION (0x00)
#define REGISTER_ADDRESS (0x01)
#define REGISTER_CONFIG (0x02)
#define REGISTER_CONTROL (0x03)
#define REGISTER_FREQUENCY (0x04)
#define REGISTER_PWM (0x08)
#define REGISTER_MAX_FREQUENCY (0x09)
#define REGISTER_MAX_PWM (0x0D)
#define REGISTER_MIN_FREQUENCY (0x0E)
#define REGISTER_MIN_PWM (0x12)

// SHIFTS
#define REGISTER_CONFIG_INPUT_PULLUP_SHIFT (0)
#define REGISTER_CONFIG_PULSE_WIDTH_FILTER_SHIFT (1)
#define REGISTER_CONFIG_PIN_SHIFT (4)

// MASKS
#define REGISTER_CONFIG_INPUT_PULLUP_MASK                                      \
  (0b1 << REGISTER_CONFIG_INPUT_PULLUP_SHIFT)
#define REGISTER_CONFIG_PULSE_WIDTH_FILTER_MASK                                \
  (0b111 << REGISTER_CONFIG_PULSE_WIDTH_FILTER_SHIFT)
#define REGISTER_CONFIG_PIN_MASK (0b11 << REGISTER_CONFIG_PIN_SHIFT)

// VALUES AND DEFAULTS
#define REGISTER_IDENTIFICATION_VALUE ((0b10101 << 3) | (0b001))
#define REGISTER_ADDRESS_DFAULT (0x40)
#define REGISTER_CONFIG_DEFAULT                                                \
  ((0b01 << REGISTER_CONFIG_PIN_SHIFT) |                                       \
   (0b010 << REGISTER_CONFIG_PULSE_WIDTH_FILTER_SHIFT) |                       \
   (0b1 << REGISTER_CONFIG_INPUT_PULLUP_SHIFT))
#define REGISTER_CONTROL_RESET_VALUE (0b1 << 0)
#define REGISTER_CONTROL_SAVE_VALUE (0b1 << 1)
#define REGISTER_CONTROL_CALCULATE_VALUE (0b1 << 2)
