#include <WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

// Información de la red WiFi
const char* ssid = "nombre_de_la_red";
const char* password = "contraseña_de_la_red";

// Información del Broker MQTT
const char* mqttServer = "137.184.125.122";
const int mqttPort = 1883;

// Pins de salida para el control infrarrojo
const int IR_PIN = 5;

// Cliente WiFi y MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Objeto para enviar señales infrarrojas
IRsend irsend(IR_PIN);

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
      client.subscribe("datacenter/ac_control");
    } else {
      Serial.print("Error al conectar al Broker MQTT. Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Intentando reconectar al Broker MQTT...");

    if (client.connect("ESP32Client")) {
      Serial.println("Conectado al Broker MQTT");
      client.subscribe("datacenter/ac_control");
    } else {
      Serial.print("Error al reconectar al Broker MQTT. Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido en el topic: ");
  Serial.println(topic);

  if (strcmp(topic, "datacenter/ac_control") == 0) {
    if (payload[0] == '1') {
      Serial.println("Encendiendo el aire acondicionado...");
      // Enviar la señal infrarroja para encender el aire acondicionado
      irsend.sendNEC(0x20DF10EF, 32); // Ejemplo de código infrarrojo para encender
    } else if (payload[0] == '0') {
      Serial.println("Apagando el aire acondicionado...");
      // Enviar la señal infrarroja para apagar el aire acondicionado
      irsend.sendNEC(0x20DF609F, 32); // Ejemplo de código infrarrojo para apagar
    }
  }
}

void setup() {
  // Resto del código de configuración (WiFi, MQTT, etc.)

  // Asignar la función de callback al cliente MQTT
  client.setCallback(callback);
}