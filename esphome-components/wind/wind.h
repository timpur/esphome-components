#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace wind {
class WindComponent : public Component {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;

  void set_heading_sensor(sensor::Sensor *heading_sensor) { heading_sensor_ = heading_sensor; }
  void set_speed_sensor(sensor::Sensor *speed_sensor) { speed_sensor_ = speed_sensor; }
  void set_max_speed_sensor(sensor::Sensor *max_speed_sensor) { max_speed_sensor_ = max_speed_sensor; }

  void set_dependent_sensors(sensor::Sensor *dep_heading_sensor, sensor::Sensor *dep_frequency_sensor,
                             sensor::Sensor *dep_max_frequency_sensor) {
    dep_heading_sensor_ = dep_heading_sensor;
    dep_frequency_sensor_ = dep_frequency_sensor;
    dep_max_frequency_sensor_ = dep_max_frequency_sensor;
  }

 protected:
  sensor::Sensor *heading_sensor_;
  sensor::Sensor *speed_sensor_;
  sensor::Sensor *max_speed_sensor_;
  sensor::Sensor *dep_heading_sensor_;
  sensor::Sensor *dep_frequency_sensor_;
  sensor::Sensor *dep_max_frequency_sensor_;

  enum ErrorCode {
    NONE = 0,
    DEPENDENCIES_ERROR,
  } error_code_;

  void update_heading_sensor(float value);
  void update_speed_sensor(float value);
  void update_max_speed_sensor(float value);

  float heading_to_heading(float value);
  float frequency_to_speed(float value);
};

}  // namespace wind
}  // namespace esphome