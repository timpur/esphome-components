#include "Arduino.h"
#include "EEPROM.h"
#include "Ticker.h"
#include "Wire.h"
#include "registers.hpp"
#include "windsensor.hpp"

// ---- Header ----
void checkRegs();
void onReceive(int size);
void onRequest();
void loadData();
void saveData();
void resetData();
bool getInputPullup();
uint8_t getPuleWidthFilterFactor();
uint8_t getPin();
// ---- End ----

Ticker timer1(checkRegs, 1);

void setup() {
  // put your setup code here, to run once:
  noInterrupts();

  loadData();

  Wire.begin(REGISTERS.GET(REGISTER_ADDRESS));
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);

  WIND_SENSOR.setup(getPin(), getInputPullup(), getPuleWidthFilterFactor());

  timer1.start();

  interrupts();
}

void loop() {
  // put your main code here, to run repeatedly:
  timer1.update();
}

void checkRegs() {
  if (REGISTERS.GET(REGISTER_CONTROL) != 0) {
    noInterrupts();

    switch (REGISTERS.GET(REGISTER_CONTROL)) {
    case 1:
      resetData();
      break;
    case 2:
      saveData();
      break;
    case 4:
      auto data = WIND_SENSOR.getData();
      *(float *)REGISTERS.REF(REGISTER_FREQUENCY) = data.frequency;
      *REGISTERS.REF(REGISTER_PWM) = data.pwm;
      *(float *)REGISTERS.REF(REGISTER_MAX_FREQUENCY) = data.max_frequency;
      *REGISTERS.REF(REGISTER_MAX_PWM) = data.max_pwm;
      *(float *)REGISTERS.REF(REGISTER_MIN_FREQUENCY) = data.min_frequency;
      *REGISTERS.REF(REGISTER_MIN_PWM) = data.min_pwm;
      break;
    }

    REGISTERS.SET(REGISTER_CONTROL, 0);

    interrupts();
  }
}

void onReceive(int size) {
  if (!size)
    return;

  REGISTERS.SET_PTR(Wire.read());
  --size;

  if (!size)
    return;

  do {
    REGISTERS.SET(Wire.read());
    REGISTERS.INCREMENT();
  } while (--size);
}

void onRequest() { Wire.write(REGISTERS.REF(), REGISTERS.REMAINING()); }

void loadData() {
  // if not set, dont load
  if (EEPROM.read(0x00) != true) {
    return;
  }
  REGISTERS.SET(REGISTER_ADDRESS, EEPROM.read(0x01));
  REGISTERS.SET(REGISTER_CONFIG, EEPROM.read(0x02));
}

void saveData() {
  EEPROM.update(0x00, true);
  EEPROM.update(0x01, REGISTERS.GET(REGISTER_ADDRESS));
  EEPROM.update(0x02, REGISTERS.GET(REGISTER_CONFIG));
}

void resetData() {
  EEPROM.update(0x00, 0xFF);
  REGISTERS.RESET();
}

bool getInputPullup() {
  return !!(REGISTERS.GET(REGISTER_CONFIG) & REGISTER_CONFIG_INPUT_PULLUP_MASK);
}

uint8_t getPuleWidthFilterFactor() {
  return (REGISTERS.GET(REGISTER_CONFIG) &
          REGISTER_CONFIG_PULSE_WIDTH_FILTER_MASK) >>
         REGISTER_CONFIG_PULSE_WIDTH_FILTER_SHIFT;
}

uint8_t getPin() {

  uint8_t pin = (REGISTERS.GET(REGISTER_CONFIG) & REGISTER_CONFIG_PIN_MASK) >>
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
  }
}