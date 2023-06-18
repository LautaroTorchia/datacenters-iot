#include "DHTModule.h"

DHTModule::DHTModule(int pin) : _pin(pin), _dht() {}

void DHTModule::setup() {
  _dht.setup(_pin, DHTesp::DHT22);
}

float DHTModule::readTemperature() {
  return _dht.getTemperature();
}

float DHTModule::readHumidity() {
  return _dht.getHumidity();
}