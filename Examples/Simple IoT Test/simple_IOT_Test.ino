#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Replace with your WiFi credentials
const char* ssid = "MAK";
const char* password = "05071989";

// Pin D0 (GPIO 16) for controlling the LED
const int ledPin = D0;

// Replace with the API endpoint URL
const char* apiUrl = "http://prayalabs.com/rest/api/simple-iot";

WiFiClient client;
HTTPClient http;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
}

void loop() {
  // Check the API for status
  int status = getStatusFromAPI();

  // Control the LED based on the status
  if (status == 1) {
    digitalWrite(ledPin, HIGH); // Turn the LED on
  } else {
    digitalWrite(ledPin, LOW); // Turn the LED off
  }

  // Wait for a short interval before checking the status again
  delay(1000);
}

int getStatusFromAPI() {
  int status = 0; // Default status is off

  // Make a GET request to the API
  if (http.begin(client, apiUrl)) {
    int httpCode = http.GET();

    // If the request was successful, parse the JSON response
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.print("API response: ");
      Serial.println(payload);

      // Parse the JSON response
      StaticJsonDocument<200> doc; // Adjust the buffer size as per your response
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.println("Failed to parse JSON");
      } else {
        // Extract the "status" value from JSON
        status = doc["status"];
      }
    } else {
      Serial.printf("Failed to connect to API, HTTP code: %d\n", httpCode);
    }

    http.end();
  } else {
    Serial.println("Unable to connect to the API");
  }

  return status;
}
