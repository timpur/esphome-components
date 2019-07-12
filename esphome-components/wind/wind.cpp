#include "wind.h"
#include "esphome/core/log.h"

namespace esphome {
namespace wind {
static const char *TAG = "wind";

void WindComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Wind Sensor...");
  if (this->dep_heading_sensor_ != nullptr && this->dep_frequency_sensor_ != nullptr &&
      this->dep_max_frequency_sensor_ != nullptr) {
    this->error_code_ = DEPENDENCIES_ERROR;
    this->mark_failed();
    return;
  }

  this->dep_heading_sensor_->add_on_state_callback([this](float value) { this->update_heading_sensor(value); });
  this->dep_frequency_sensor_->add_on_state_callback([this](float value) { this->update_speed_sensor(value); });
  this->dep_max_frequency_sensor_->add_on_state_callback([this](float value) { this->update_max_speed_sensor(value); });
}

void WindComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Speed:");
  if (this->error_code_ == DEPENDENCIES_ERROR) {
    ESP_LOGE(TAG, "Required dependency sensors not given.");
  }

  LOG_SENSOR("  ", "Heading", this->heading_sensor_);
  LOG_SENSOR("  ", "Speed", this->speed_sensor_);
  LOG_SENSOR("  ", "Max Speed", this->max_speed_sensor_);
}

float WindComponent::get_setup_priority() const { return setup_priority::PROCESSOR; }

void WindComponent::update_heading_sensor(float value) {
  float heading = this->heading_to_heading(value);
  this->heading_sensor_->publish_state(heading);
}

void WindComponent::update_speed_sensor(float value) {
  float speed = this->frequency_to_speed(value);
  this->speed_sensor_->publish_state(speed);
}

void WindComponent::update_max_speed_sensor(float value) {
  float speed = this->frequency_to_speed(value);
  this->max_speed_sensor_->publish_state(speed);
}

float WindComponent::heading_to_heading(float value) {
  float heading = value;
  return heading;
}

float WindComponent::frequency_to_speed(float value) {
  float speed = value * 10.0;
  return speed;
}

}  // namespace wind
}  // namespace esphome