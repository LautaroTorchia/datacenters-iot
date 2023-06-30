#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>

// Información de la red WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Información del Broker MQTT
const char* mqttServer = "137.184.125.122";
const int mqttPort = 1883;

// Información del sensor DHT
#define DHT_PIN 4
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

  while (!client.connected()) {
    Serial.println("Conectando al Broker MQTT...");
    
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado al Broker MQTT");
    } else {
      Serial.print("Error al conectar al Broker MQTT. Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  
  // Inicializar el sensor DHT
  dht.setup(DHT_PIN, DHTesp::DHT22);
}

void loop() {
  // Obtener lecturas de temperatura y humedad
  TempAndHumidity dhtData = dht.getTempAndHumidity();
  float temperature = dhtData.temperature;
  float humidity = dhtData.humidity;
  // Obtener estado de la detección de agua
  //bool waterDetected = digitalRead(WATER_PIN);

  // Publicar los datos en el Broker MQTT
  String temperaturePayload = String(temperature);
  String humidityPayload = String(humidity);
  //String waterPayload = waterDetected ? "1" : "0";
  
  client.publish("datacenter/temperature", temperaturePayload.c_str());
  client.publish("datacenter/humidity", humidityPayload.c_str());
  //client.publish("datacenter/water", waterPayload.c_str());

  Serial.println("Datos publicados en el Broker MQTT");

  delay(5000);
}