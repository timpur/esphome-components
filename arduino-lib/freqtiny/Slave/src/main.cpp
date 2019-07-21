#include "Arduino.h"
#include "EEPROM.h"
#include "Ticker.h"
#include "Wire.h"
#include "freq_sensor.hpp"
#include "registers.hpp"

// ---- Header ----
void checkRegs();
void onReceive(int size);
void onRequest();
void loadData();
void saveData();
void resetData();
// ---- End ----

Ticker timer1(checkRegs, 1);

void setup() {
  // put your setup code here, to run once:
  noInterrupts();
  loadData();
  Wire.begin(REGISTERS.GET(REGISTER_ADDRESS));
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
  timer1.start();
  interrupts();
}

void loop() {
  // put your main code here, to run repeatedly:
  timer1.update();
}

void checkRegs() {
  if (REGISTERS.GET(REGISTER_CONTROL) != 0x0) {
    noInterrupts();

    switch (REGISTERS.GET(REGISTER_CONTROL)) {
    case REGISTER_CONTROL_RESET_VALUE:
      resetData();
      break;
    case REGISTER_CONTROL_SAVE_VALUE:
      saveData();
    case REGISTER_CONTROL_COMMIT_VALUE:
      FREQ_SENSOR.setup(REGISTERS.GET_PIN(), REGISTERS.GET_INPUT_PULLUP(),
                        REGISTERS.GET_PULSEWIDTH_FILTER_FACTOR());
      break;
    case REGISTER_CONTROL_CALCULATE_VALUE:
      auto data = FREQ_SENSOR.getData();
      *(float *)REGISTERS.REF(REGISTER_FREQUENCY) = data.frequency;
      *REGISTERS.REF(REGISTER_PWM) = data.pwm;
      *(float *)REGISTERS.REF(REGISTER_MAX_FREQUENCY) = data.max_frequency;
      *REGISTERS.REF(REGISTER_MAX_PWM) = data.max_pwm;
      *(float *)REGISTERS.REF(REGISTER_MIN_FREQUENCY) = data.min_frequency;
      *REGISTERS.REF(REGISTER_MIN_PWM) = data.min_pwm;
      break;
    }

    REGISTERS.SET(REGISTER_CONTROL, 0x0);

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