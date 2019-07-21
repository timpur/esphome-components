#include "freqtiny.h"
#include "esphome/core/log.h"

namespace esphome {
namespace freqtiny {
static const char *TAG = "freqtiny";

#define REGISTER_IDENTIFICATION (0x00)
#define REGISTER_CONFIG (0x02)
#define REGISTER_CONTROL (0x03)
#define REGISTER_FREQUENCY (0x04)
#define REGISTER_PWM (0x08)
#define REGISTER_MAX_FREQUENCY (0x09)
#define REGISTER_MAX_PWM (0x0D)
#define REGISTER_MIN_FREQUENCY (0x0E)
#define REGISTER_MIN_PWM (0x12)

#define REGISTER_IDENTIFICATION_VALUE ((0b10101 << 3) | (0b001))
#define REGISTER_CONTROL_COMMIT_VALUE (0b1 << 1)
#define REGISTER_CONTROL_CALCULATE_VALUE (0b1 << 3)

void FreqTinyComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up FreqTiny ...");

  uint8_t id;
  if (!this->read_byte(REGISTER_IDENTIFICATION, &id)) {
    this->error_code_ = COMMUNICATION_FAILED;
    this->mark_failed();
    return;
  }

  if (id != REGISTER_IDENTIFICATION_VALUE) {
    this->error_code_ = ID_REGISTERS;
    this->mark_failed();
    return;
  }

  uint8_t config = 0;
  config |= this->pin_ << 0;                 // Select pin
  config |= this->pin_pullup_ << 2;          // Select Pullup pin
  config |= this->pulse_width_filter_ << 3;  // Select pulse width filter mode;
  if (!this->write_byte(REGISTER_CONFIG, config)) {
    this->error_code_ = COMMUNICATION_FAILED;
    this->mark_failed();
    return;
  }

  // Commit config
  if (!this->write_byte(REGISTER_CONTROL, REGISTER_CONTROL_COMMIT_VALUE)) {
    this->error_code_ = COMMUNICATION_FAILED;
    this->mark_failed();
    return;
  }
}

void FreqTinyComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "FreqTiny:");
  ESP_LOGCONFIG(TAG, "  pin=%u pin_pullup=%u pulse_width_filter=%u", this->pin_, this->pin_pullup_,
           this->pulse_width_filter_);
  LOG_I2C_DEVICE(this);
  if (this->error_code_ == COMMUNICATION_FAILED) {
    ESP_LOGE(TAG, "Communication with Frequency senor failed!");
  } else if (this->error_code_ == ID_REGISTERS) {
    ESP_LOGE(TAG, "The ID registers don't match - Has the version changed?");
  }
  LOG_UPDATE_INTERVAL(this);

  LOG_SENSOR("  ", "Frequency", this->frequency_sensor_);
  LOG_SENSOR("  ", "PWM", this->pwm_sensor_);

  LOG_SENSOR("  ", "MAX Frequency", this->max_frequency_sensor_);
  LOG_SENSOR("  ", "MAX PWM", this->max_pwm_sensor_);

  LOG_SENSOR("  ", "MIN Frequency", this->min_frequency_sensor_);
  LOG_SENSOR("  ", "MIN PWM", this->min_pwm_sensor_);
}

float FreqTinyComponent::get_setup_priority() const { return setup_priority::DATA; }

void FreqTinyComponent::update() {
  this->write_byte(REGISTER_CONTROL, REGISTER_CONTROL_CALCULATE_VALUE);
  delay(10);

  float frequency, max_frequency, min_frequency;
  uint8_t pwm, max_pwm, min_pwm;

  if (!this->read_float(REGISTER_FREQUENCY, &frequency) || !this->read_byte(REGISTER_PWM, &pwm) ||
      !this->read_float(REGISTER_MAX_FREQUENCY, &max_frequency) || !this->read_byte(REGISTER_MAX_PWM, &max_pwm) ||
      !this->read_float(REGISTER_MIN_FREQUENCY, &min_frequency) || !this->read_byte(REGISTER_MIN_PWM, &min_pwm)) {
    this->status_set_warning();
    return;
  }

  this->status_clear_warning();

  ESP_LOGD(TAG, "Got frq=%0.02fHz pwm=%u max_frq=%0.02fHz max_pwm=%u min_frq=%0.02fHz min_pwm=%u", frequency, pwm,
           max_frequency, max_pwm, min_frequency, min_pwm);

  if (this->frequency_sensor_ != nullptr)
    this->frequency_sensor_->publish_state(frequency);
  if (this->pwm_sensor_ != nullptr)
    this->pwm_sensor_->publish_state(pwm);
  if (this->max_frequency_sensor_ != nullptr)
    this->max_frequency_sensor_->publish_state(max_frequency);
  if (this->max_pwm_sensor_ != nullptr)
    this->max_pwm_sensor_->publish_state(max_pwm);
  if (this->min_frequency_sensor_ != nullptr)
    this->min_frequency_sensor_->publish_state(min_frequency);
  if (this->min_pwm_sensor_ != nullptr)
    this->min_pwm_sensor_->publish_state(min_pwm);
}

bool FreqTinyComponent::read_float(uint8_t address, float *data) {
  float raw_data;
  if (!this->read_bytes(address, (uint8_t *) &raw_data, 4))
    return false;

  *data = raw_data;
  return true;
}

}  // namespace freqtiny
}  // namespace esphome