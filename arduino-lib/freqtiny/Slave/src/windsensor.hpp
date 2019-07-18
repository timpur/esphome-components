#pragma once

#include "Arduino.h"
#include "registers.hpp"

void enablePCIPin(uint8_t pin);

struct WindSensorData {
  float frequency = 0;
  uint8_t pwm = 0;
  float max_frequency = 0;
  uint8_t max_pwm = 0;
  float min_frequency = 0;
  uint8_t min_pwm = 0;
};

class WindSensor {
public:
  void setup(uint8_t pin, bool inputPullup, uint8_t pulse_width_filter_factor) {
    PIN = pin;
    PULSE_WIDTH_FILTER = pulse_width_filter_factor * 50;
    pinMode(PIN, inputPullup ? INPUT_PULLUP : INPUT);
    enablePCIPin(PIN);
  }

  WindSensorData getData() {
    WindSensorData data;

    float HIGH_AVG = (float)HIGH_TOTAL / (float)HIGH_COUNT;
    float LOW_AVG = (float)LOW_TOTAL / (float)LOW_COUNT;

    data.frequency = getFrequency(HIGH_AVG, LOW_AVG);
    data.pwm = getPWM(HIGH_AVG, LOW_AVG);
    data.max_frequency = getFrequency(HIGH_MIN, LOW_MIN);
    data.max_pwm = getPWM(HIGH_MIN, LOW_MIN);
    data.min_frequency = getFrequency(HIGH_MAX, LOW_MAX);
    data.min_pwm = getPWM(HIGH_MAX, LOW_MAX);

    HIGH_TOTAL = 0;
    HIGH_COUNT = 0;
    HIGH_MAX = 0;
    HIGH_MIN = 0;
    LOW_TOTAL = 0;
    LOW_COUNT = 0;
    LOW_MAX = 0;
    LOW_MIN = 0;
    LAST_PULSE_TIME = 0;

    return data;
  }

  void onChange() {
    const uint32_t now = micros();
    const bool side = !digitalRead(PIN);

    bool abort = false;

    // Start of the pulses
    if (LAST_PULSE_TIME == 0)
      abort = true;

    // Handel micros overflow
    else if (LAST_PULSE_TIME > now)
      abort = true;

    const uint32_t pulse_width = now - LAST_PULSE_TIME;
    LAST_PULSE_TIME = now;

    if (pulse_width <= PULSE_WIDTH_FILTER)
      abort = true;

    if (abort)
      return;

    volatile uint32_t &total = side ? HIGH_TOTAL : LOW_TOTAL;
    volatile uint32_t &count = side ? HIGH_COUNT : LOW_COUNT;
    volatile uint32_t &max = side ? HIGH_MAX : LOW_MAX;
    volatile uint32_t &min = side ? HIGH_MIN : LOW_MIN;

    if (count == 0) {
      total = pulse_width;
      max = pulse_width;
      min = pulse_width;
    } else {
      total += pulse_width;
      if (pulse_width > max)
        max = pulse_width;
      if (pulse_width < min)
        min = pulse_width;
    }

    count++;
  }

private:
  uint8_t PIN;
  uint16_t PULSE_WIDTH_FILTER = 0;

  volatile uint32_t LAST_PULSE_TIME = 0;

  volatile uint32_t HIGH_TOTAL = 0;
  volatile uint32_t HIGH_COUNT = 0;
  volatile uint32_t HIGH_MAX = 0;
  volatile uint32_t HIGH_MIN = 0;

  volatile uint32_t LOW_TOTAL = 0;
  volatile uint32_t LOW_COUNT = 0;
  volatile uint32_t LOW_MAX = 0;
  volatile uint32_t LOW_MIN = 0;

  static float getFrequency(float high, float low) {
    if (isnan(high) || isnan(low) || high == 0.0 || low == 0.0)
      return 0.0;

    return (1000000.0 / (high + low));
  }

  static uint8_t getPWM(float high, float low) {
    if (isnan(high) || isnan(low) || high == 0.0 || low == 0.0)
      return 0;

    return (uint8_t)((high / (high + low)) * 255.0);
  }
} WIND_SENSOR;

void enablePCIPin(uint8_t pin) {
  // enable interrupt for the group
  bitSet(*digitalPinToPCICR(pin), digitalPinToPCICRbit(pin));
  // enable pin
  bitSet(*digitalPinToPCMSK(pin), digitalPinToPCMSKbit(pin));
}

ISR(PCINT0_vect) { WIND_SENSOR.onChange(); }
