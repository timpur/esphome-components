#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace freqtiny {

class FreqTinyComponent : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void update() override;

  void set_frequency_sensor(sensor::Sensor *frequency_sensor) { frequency_sensor_ = frequency_sensor; }
  void set_pwm_sensor(sensor::Sensor *pwm_sensor) { pwm_sensor_ = pwm_sensor; }

  void set_max_frequency_sensor(sensor::Sensor *max_frequency_sensor) { max_frequency_sensor_ = max_frequency_sensor; }
  void set_max_pwm_sensor(sensor::Sensor *max_pwm_sensor) { max_pwm_sensor_ = max_pwm_sensor; }

  void set_min_frequency_sensor(sensor::Sensor *min_frequency_sensor) { min_frequency_sensor_ = min_frequency_sensor; }
  void set_min_pwm_sensor(sensor::Sensor *min_pwm_sensor) { min_pwm_sensor_ = min_pwm_sensor; }

 protected:
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