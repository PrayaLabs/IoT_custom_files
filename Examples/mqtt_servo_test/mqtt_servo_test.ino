#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "ServoControl.h"

// Update these with values suitable for your network.
const char* ssid = "Praya Labs";
const char* password = "7kwe6f5cut";
const char* mqtt_server = "91.121.93.94";

WiFiClient espClient;
PubSubClient client(espClient);

ServoControl myServo(D4); // Create a ServoControl object for pin D4 (GPIO 2)

bool rotating = false; // Flag to control rotation

unsigned long lastRotationTime = 0;
unsigned long rotationInterval = 500; // 500 ms interval

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  if (message == "start") {
    rotating = true;  // Start rotating the servo
  } else if (message == "stop") {
    rotating = false; // Stop the servo
    myServo.stopServo(); // Stop the servo motor
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("servo/control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (rotating) {
    unsigned long now = millis();
    if (now - lastRotationTime >= rotationInterval) {
      static bool toggle = false;
      if (toggle) {
        myServo.rotateToZero(); // Rotate to 0 degrees
      } else {
        myServo.rotateTo180(); // Rotate to 180 degrees
      }
      toggle = !toggle;
      lastRotationTime = now;
    }
  }
}
