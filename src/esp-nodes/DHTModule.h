#ifndef DHTModule_h
#define DHTModule_h

#include <DHTesp.h>

class DHTModule {
public:
  DHTModule(int pin);
  void setup();
  float readTemperature();
  float readHumidity();

private:
  int _pin;
  DHTesp _dht;
};

#endif
