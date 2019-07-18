#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace freqtiny {

enum FreqTinyPin {
  FREQTINY_PIN_5 = 0b00,
  FREQTINY_PIN_3 = 0b01,
  FREQTINY_PIN_4 = 0b10,
  FREQTINY_PIN_1 = 0b11,

};

enum FreqTinyPulseWidthFilter {
  FREQTINY_PULSE_WIDTH_FILTER_OFF = 0b000,
  FREQTINY_PULSE_WIDTH_FILTER_50_US = 0b001,
  FREQTINY_PULSE_WIDTH_FILTER_100_US = 0b010,
  FREQTINY_PULSE_WIDTH_FILTER_150_US = 0b011,
  FREQTINY_PULSE_WIDTH_FILTER_200_US = 0b100,
  FREQTINY_PULSE_WIDTH_FILTER_250_US = 0b101,
  FREQTINY_PULSE_WIDTH_FILTER_300_US = 0b110,
  FREQTINY_PULSE_WIDTH_FILTER_350_US = 0b111,
};

class FreqTinyComponent : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void update() override;

  void set_pin(FreqTinyPin pin) { pin_ = pin; }
  void set_pin_pullup(bool pin_pullup) { pin_pullup_ = pin_pullup; }
  void set_filter(FreqTinyPulseWidthFilter pulse_width_filter) { pulse_width_filter_ = pulse_width_filter; }
  void set_frequency_sensor(sensor::Sensor *frequency_sensor) { frequency_sensor_ = frequency_sensor; }
  void set_pwm_sensor(sensor::Sensor *pwm_sensor) { pwm_sensor_ = pwm_sensor; }
  void set_max_frequency_sensor(sensor::Sensor *max_frequency_sensor) { max_frequency_sensor_ = max_frequency_sensor; }
  void set_max_pwm_sensor(sensor::Sensor *max_pwm_sensor) { max_pwm_sensor_ = max_pwm_sensor; }
  void set_min_frequency_sensor(sensor::Sensor *min_frequency_sensor) { min_frequency_sensor_ = min_frequency_sensor; }
  void set_min_pwm_sensor(sensor::Sensor *min_pwm_sensor) { min_pwm_sensor_ = min_pwm_sensor; }

 protected:
  FreqTinyPin pin_{FREQTINY_PIN_3};
  bool pin_pullup_;
  FreqTinyPulseWidthFilter pulse_width_filter_{FREQTINY_PULSE_WIDTH_FILTER_100_US};
  sensor::Sensor *frequency_sensor_;
  sensor::Sensor *pwm_sensor_;
  sensor::Sensor *max_frequency_sensor_;
  sensor::Sensor *max_pwm_sensor_;
  sensor::Sensor *min_frequency_sensor_;
  sensor::Sensor *min_pwm_sensor_;
  enum ErrorCode {
    NONE = 0,
    COMMUNICATION_FAILED,
    ID_REGISTERS,
  } error_code_;

 private:
  bool read_float(uint8_t address, float *data);
};
}  // namespace freqtiny
}  // namespace esphome