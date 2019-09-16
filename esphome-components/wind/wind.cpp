#include "wind.h"
#include "esphome/core/log.h"

namespace esphome {
namespace wind {

static const char* TAG = "wind";

static const char* DEG_TO_COMPASS[] = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE",
                                       "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};

void WindComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Wind Sensor...");
  if (this->dep_heading_sensor_ == nullptr || this->dep_frequency_sensor_ == nullptr ||
      this->dep_max_frequency_sensor_ == nullptr) {
    this->error_code_ = DEPENDENCIES_ERROR;
    this->mark_failed();
    return;
  }

  this->dep_heading_sensor_->add_on_state_callback([this](float value) { this->update_heading_sensor(value); });
  this->dep_frequency_sensor_->add_on_state_callback([this](float value) { this->update_speed_sensor(value); });
  this->dep_max_frequency_sensor_->add_on_state_callback([this](float value) { this->update_max_speed_sensor(value); });
  this->heading_sensor_->add_on_state_callback([this](float value) { this->update_heading_compass_sensor(value); });
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

void WindComponent::update_heading_compass_sensor(float value) {
  std::string heading = this->heading_to_heading_compass(value);
  this->heading_compass_sensor_->publish_state(heading);
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
  float heading = value * -1.0;
  return heading;
}

std::string WindComponent::heading_to_heading_compass(float value) {
  int val = int(value) % 360;
  val = int((val / 22.5) + 0.5);
  return DEG_TO_COMPASS[(val % 16)];
}

float WindComponent::frequency_to_speed(float value) {
  float speed = value * 1.0;  // TODO:
  return speed;
}

}  // namespace wind
}  // namespace esphome