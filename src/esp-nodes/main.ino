#include "DHTModule.h"
#include "WaterSensorModule.h"

const int DHT_PIN = 15;
const int WATER_SENSOR_PIN = 16;

DHTModule dhtModule(DHT_PIN);
WaterSensorModule waterSensorModule(WATER_SENSOR_PIN);

void setup() {
  Serial.begin(115200);

  dhtModule.setup();
  waterSensorModule.setup();
}

void loop() {
  // Read and print temperature and humidity
  float temperature = dhtModule.readTemperature();
  float humidity = dhtModule.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Check if water is detected
  bool isWaterDetected = waterSensorModule.isWaterPresent();
  
  if (isWaterDetected) {
    Serial.println("Water detected!");
  } else {
    Serial.println("No water detected.");
  }

  delay(2000);
}