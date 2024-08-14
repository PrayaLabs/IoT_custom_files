#include <ESP8266WiFi.h>

const int servoPin = D4;  // GPIO pin connected to the servo
const int servoMin = 500;  // Minimum pulse width in microseconds (approx 0 degrees)
const int servoMax = 2400; // Maximum pulse width in microseconds (approx 180 degrees)

void setup() {
  pinMode(servoPin, OUTPUT); // Set the pin as an output
}

void loop() {
  // Move to 0 degrees
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(servoMin);
  digitalWrite(servoPin, LOW);
  delay(20);  // Wait for the rest of the period (20 ms)

  delay(500); // Wait for 2 seconds

  // Move to 180 degrees
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(servoMax);
  digitalWrite(servoPin, LOW);
  delay(20);  // Wait for the rest of the period (20 ms)

  delay(500); // Wait for 2 seconds
}
