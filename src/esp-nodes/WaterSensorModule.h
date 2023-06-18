#ifndef WaterSensorModule_h
#define WaterSensorModule_h

class WaterSensorModule {
public:
  WaterSensorModule(int pin);
  void setup();
  bool isWaterPresent();

private:
  int _pin;
};

#endif
