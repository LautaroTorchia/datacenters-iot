#include <WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <UniversalTelegramBot.h>

// Información de la red WiFi
const char* ssid = "nombre_de_la_red";
const char* password = "contraseña_de_la_red";

// Información del Broker MQTT
const char* mqttServer = "137.184.125.122";
const int mqttPort = 1883;

// Información de Telegram
const char* telegramToken = "tu_token_de_telegram";
const int telegramChatId = tu_chat_id;

// Límites peligrosos
const float temperaturaLimite = 30.0;
const float humedadLimite = 80.0;
const bool aguaLimite = true;

// Pins de salida para el control infrarrojo
const int IR_PIN = 5;

// Cliente WiFi, MQTT y Telegram
WiFiClient espClient;
PubSubClient client(espClient);
UniversalTelegramBot bot(telegramToken, espClient);

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
      client.subscribe("datacenter/#");
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

  // Comprobar límites peligrosos y enviar notificaciones
  if (superarLimiteTemperatura() || superarLimiteHumedad() || superarLimiteAgua()) {
    enviarNotificacionTelegram();
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Intentando reconectar al Broker MQTT...");

    if (client.connect("ESP32Client")) {
      Serial.println("Conectado al Broker MQTT");
      client.subscribe("datacenter/#");
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

  // Manejar los mensajes recibidos aquí
}

bool superarLimiteTemperatura() {
  // Obtener la lectura de temperatura actual
  float temperatura = obtenerLecturaTemperatura();

  // Verificar si la temperatura supera el límite
  if (temperatura > temperaturaLimite) {
    Serial.println("¡Alerta! La temperatura ha superado el límite peligroso");
    return true;
  }

  return false;
}

bool superarLimiteHumedad() {
  // Obtener la lectura de humedad actual
  float humedad = obtenerLecturaHumedad();

  // Verificar si la humedad supera el límite
  if (humedad > humedadLimite) {
    Serial.println("¡Alerta! La humedad ha superado el límite peligroso");
    return true;
  }

  return false;
}

bool superarLimiteAgua() {
  // Obtener la lectura de detección de agua actual
  bool agua = obtenerLecturaAgua();

  // Verificar si se ha detectado agua
  if (agua) {
    Serial.println("¡Alerta! Se ha detectado agua en el datacenter");
    return true;
  }

  return false;
}

float obtenerLecturaTemperatura() {
  // Código para obtener la lectura de temperatura
  // Retorna el valor de temperatura actual
}

float obtenerLecturaHumedad() {
  // Código para obtener la lectura de humedad
  // Retorna el valor de humedad actual
}

bool obtenerLecturaAgua() {
  // Código para obtener la lectura de detección de agua
  // Retorna true si se ha detectado agua, false de lo contrario
}

void enviarNotificacionTelegram() {
  String mensaje = "¡Alerta en el datacenter!\n";

  if (superarLimiteTemperatura()) {
    mensaje += "Temperatura ha superado el límite peligroso\n";
  }

  if (superarLimiteHumedad()) {
    mensaje += "Humedad ha superado el límite peligroso\n";
  }

  if (superarLimiteAgua()) {
    mensaje += "Se ha detectado agua en el datacenter\n";
  }

  // Enviar la notificación a través de Telegram
  bot.sendMessage(telegramChatId, mensaje, "Markdown");
}
