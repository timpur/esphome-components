#include "../../lib/registers.hpp"
#include "Arduino.h"
#include "Wire.h"

const uint8_t address = REGISTER_ADDRESS_DFAULT;

uint8_t getByte(uint8_t reg) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(address, (uint8_t)1, true);

  return Wire.read();
}

float getFloat(uint8_t reg) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(address, (uint8_t)4, true);

  uint8_t data[4];
  data[0] = Wire.read();
  data[1] = Wire.read();
  data[2] = Wire.read();
  data[3] = Wire.read();

  return *(float *)&data;
}

void setByte(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void printStat(const char *str, int data) {
  Serial.print(" ");
  Serial.print(str);
  Serial.print(": ");
  Serial.print(data);
}

void printStat(const char *str, float data) {
  Serial.print(" ");
  Serial.print(str);
  Serial.print(": ");
  Serial.print(data);
}

void testPulse(int us) {
  digitalWrite(3, HIGH);
  delayMicroseconds(us);
  digitalWrite(3, LOW);
  delayMicroseconds(us);
}

void testSensor() {
  // tone(3, 5000);
  testPulse(2000);
  testPulse(2000);
  testPulse(1000);
  testPulse(4000);
  testPulse(2000);
  testPulse(500);
  testPulse(50);
  testPulse(2000);
  delay(10);
}

void printSensorData() {
  setByte(REGISTER_CONTROL, REGISTER_CONTROL_CALCULATE_VALUE);
  delay(50);

  printStat("ID", getByte(REGISTER_IDENTIFICATION));
  printStat("Address", getByte(REGISTER_ADDRESS));
  printStat("Config", getByte(REGISTER_CONFIG));
  printStat("Control", getByte(REGISTER_CONTROL));
  printStat("Frequency", getFloat(REGISTER_FREQUENCY));
  printStat("PWM", getByte(REGISTER_PWM));
  printStat("Max Frequency", getFloat(REGISTER_MAX_FREQUENCY));
  printStat("Max PWM", getByte(REGISTER_MAX_PWM));
  printStat("Min Frequency", getFloat(REGISTER_MIN_FREQUENCY));
  printStat("Min PWM", getByte(REGISTER_MIN_PWM));

  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Wire.begin();

  pinMode(3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  testSensor();
  printSensorData();
  delay(5000);
}