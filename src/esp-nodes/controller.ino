#include <WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRutils.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const int mqtt_port = 1883;
const char* mqtt_server = "137.184.125.122";
const char* mqtt_topic = "datacenter/actions";
const char* mqtt_client_name = "ESP32Controller";

WiFiClient espClient;
PubSubClient client(espClient);

const uint16_t kIrLed = 4;  // The ESP GPIO pin to use that controls the IR LED.
IRac ac(kIrLed);  // Create a A/C object using GPIO to sending messages with.

bool acIsOn = false;

void configAc() {
  ac.next.protocol = decode_type_t::DAIKIN;  // Set a protocol to use.
  ac.next.model = 1;  // Some A/Cs have different models. Try just the first.
  ac.next.mode = stdAc::opmode_t::kCool;  // Run in cool mode initially.
  ac.next.celsius = true;  // Use Celsius for temp units. False = Fahrenheit
  ac.next.degrees = 24;  // 24 degrees.
  ac.next.fanspeed = stdAc::fanspeed_t::kMedium;  // Start the fan at medium.
  ac.next.swingv = stdAc::swingv_t::kOff;  // Don't swing the fan up or down.
  ac.next.swingh = stdAc::swingh_t::kOff;  // Don't swing the fan left or right.
  ac.next.light = false;  // Turn off any LED/Lights/Display that we can.
  ac.next.beep = false;  // Turn off any beep from the A/C if we can.
  ac.next.econo = false;  // Turn off any economy modes if we can.
  ac.next.filter = false;  // Turn off any Ion/Mold/Health filters if we can.
  ac.next.turbo = false;  // Don't use any turbo/powerful/etc modes.
  ac.next.quiet = false;  // Don't use any quiet/silent/etc modes.
  ac.next.sleep = -1;  // Don't set any sleep time or modes.
  ac.next.clean = false;  // Turn off any Cleaning options if we can.
  ac.next.clock = -1;  // Don't set any current time if we can avoid it.
  ac.next.power = false;  // Initially start with the unit off.
}

void sendToAc(bool turnOn) {
  // For every protocol the library has ...
  for (int i = 1; i < kLastDecodeType; i++) {
    decode_type_t protocol = (decode_type_t)i;
    // If the protocol is supported by the IRac class ...
    if (ac.isProtocolSupported(protocol)) {
      Serial.println("Protocol " + String(protocol) + " / " +
                     typeToString(protocol) + " is supported.");
      ac.next.protocol = protocol;  // Change the protocol used.
      ac.next.power = turnOn ? true : false;  // Turn on or off the A/C unit.
      Serial.println("Sending a message to turn " +
                     String(turnOn ? "ON" : "OFF") + " the A/C unit.");
      Serial.println("");
      ac.sendAc();  // Have the IRac class create and send a message.
      delay(1000);
    }
  }
  Serial.println("--------");
}

void setup() {
  Serial.begin(115200);
  delay(200);

  configAc();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  client.setServer(mqtt_server, mqtt_port);
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
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("] ");
  String messageTemp;
  for (int i = 0; i < length; i++)
    messageTemp += (char)payload[i];
  Serial.println(messageTemp);

  if (messageTemp == "encender") {
    if (acIsOn) {
      Serial.println("El aire acondicionado ya está encendido");
      return;
    }
    Serial.println("Encendiendo el aire acondicionado");
    sendToAc(true);
    acIsOn = true;
  } else if (messageTemp == "apagar") {
    if (!acIsOn) {
      Serial.println("El aire acondicionado ya está apagado");
      return;      
    }
    Serial.println("Apagando el aire acondicionado");
    sendToAc(false);
    acIsOn = false;
  } else
    Serial.println("Comando desconocido");
}
