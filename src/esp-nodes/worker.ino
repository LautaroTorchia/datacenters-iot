#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>

// Información de la red WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Información del Broker MQTT
const char* mqttServer = "137.184.125.122";
const int mqttPort = 1883;

// Pins de entrada para los sensores
const int DHT_PIN = 15;
const int WATER_SENSOR_SIGNAL_PIN = 32;

//
const int TIME_BETWEEN_MEASURE =  30 * 1000;
// Información del sensor DHT
DHTesp dht;
WiFiClient espClient;
PubSubClient client(espClient);

struct TemperatureAndHumidity {
  float temperature;
  float humidity;
};

void setup() {
  // Inicializar la comunicación serie
  Serial.begin(115200);

  // Conectar a la red WiFi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a la red WiFi");
  // Conectar al Broker MQTT
  client.setServer(mqttServer, mqttPort);
  // Inicializar el sensor DHT
  dht.setup(DHT_PIN, DHTesp::DHT22);
  // Inicializamos el sensor de agua
  pinMode(WATER_SENSOR_SIGNAL_PIN, INPUT);
}

void reconnect() {
    while (!client.connected()) {
    Serial.println("Conectando al Broker MQTT...");
    
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado al Broker MQTT");
    } else {
      Serial.print("Fallo la conexion");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  // Obtener lecturas de temperatura y humedad
  if (!client.connected()) {
    reconnect();
  }
  // Obtener estado de la detección de agua
  TempAndHumidity dhtData = dht.getTempAndHumidity();
  float temperature = dhtData.temperature;
  float humidity = dhtData.humidity;

  //check water
  int water = checkWater();
  
  // Publicar los datos en el Broker MQTT
  String temperaturePayload = String(temperature);
  String humidityPayload = String(humidity);
  String waterPayload = String(water);

  client.publish("datacenter/temperature", temperaturePayload.c_str());
  client.publish("datacenter/humidity", humidityPayload.c_str());
  client.publish("datacenter/water", waterPayload.c_str());

  Serial.println("Datos publicados en el Broker MQTT");

  delay(TIME_BETWEEN_MEASURE);
}

int checkWater(){
  int water = analogRead(WATER_SENSOR_SIGNAL_PIN);
  Serial.print("Water: ");
  Serial.println(water);
  return water;
}
