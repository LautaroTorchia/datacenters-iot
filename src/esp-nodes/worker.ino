#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Información de la red WiFi
const char* ssid = "nombre_de_la_red";
const char* password = "contraseña_de_la_red";

// Información del Broker MQTT
const char* mqttServer = "137.184.125.122";
const int mqttPort = 1883;

// Información del sensor DHT
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Cliente WiFi y MQTT
WiFiClient espClient;
PubSubClient client(espClient);

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
  dht.begin();
}

void loop() {
  // Obtener lecturas de temperatura y humedad
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Obtener estado de la detección de agua
  bool waterDetected = digitalRead(WATER_PIN);

  // Publicar los datos en el Broker MQTT
  String temperaturePayload = String(temperature);
  String humidityPayload = String(humidity);
  String waterPayload = waterDetected ? "1" : "0";
  
  client.publish("datacenter/temperature", temperaturePayload.c_str());
  client.publish("datacenter/humidity", humidityPayload.c_str());
  client.publish("datacenter/water", waterPayload.c_str());

  Serial.println("Datos publicados en el Broker MQTT");

  delay(5000);
}