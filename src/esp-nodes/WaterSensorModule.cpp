#include "WaterSensorModule.h"

WaterSensorModule::WaterSensorModule(int pin) : _pin(pin) {}

void WaterSensorModule::setup() {
  // Initialize the pin for water sensor
  pinMode(_pin, INPUT);
}

bool WaterSensorModule::isWaterPresent() {
  // Read the water sensor pin and return whether water is detected
  return digitalRead(_pin) == HIGH;
}