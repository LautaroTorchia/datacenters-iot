#include <WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const int mqtt_port = 1883;
const char* mqtt_server = "137.184.125.122";
const char* mqtt_topic = "datacenter/actions";
const char* mqtt_client_name = "ESP32Controller";

WiFiClient espClient;
PubSubClient client(espClient);

// Pins de salida para el control infrarrojo
const int IR_PIN = 5;
// Objeto para enviar señales infrarrojas
IRsend irsend(IR_PIN);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  connect();
}

void loop() {
  if (!client.connected())
    connect();
  client.loop();
}

void connect() {
  while (!client.connected()) {
    Serial.println("Conectando al Broker MQTT...");
    if (client.connect(mqtt_client_name)) {
      Serial.println("Conectado al Broker MQTT");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("Error al reconectar al Broker MQTT. Estado: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido en el topic: ");
  Serial.println(topic);

  String messageTemp;

  for (int i = 0; i < length; i++)
    messageTemp += (char)payload[i];
  Serial.println(messageTemp);

  if (messageTemp == "encender") {
    Serial.println("Encendiendo el aire acondicionado");
    irsend.sendNEC(0x20DF10EF, 32);
  } else if (messageTemp == "apagar") {
    Serial.println("Apagando el aire acondicionado");
    irsend.sendNEC(0x20DF906F, 32);
  } else
    Serial.println("Comando desconocido");
}
