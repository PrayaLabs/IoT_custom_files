#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Arduino.h>

class ServoControl {
  private:
    int servoPin;
    const int servoMin = 500;  // Minimum pulse width in microseconds (approx 0 degrees)
    const int servoMax = 2400; // Maximum pulse width in microseconds (approx 180 degrees)

  public:
    // Constructor to initialize the servo pin
    ServoControl(int pin) {
      servoPin = pin;
      pinMode(servoPin, OUTPUT);
    }

    // Method to rotate the servo to 0 degrees
    void rotateToZero() {
      digitalWrite(servoPin, HIGH);
      delayMicroseconds(servoMin);
      digitalWrite(servoPin, LOW);
      delay(20);  // Wait for the rest of the period (20 ms)
    }

    // Method to rotate the servo to 180 degrees
    void rotateTo180() {
      digitalWrite(servoPin, HIGH);
      delayMicroseconds(servoMax);
      digitalWrite(servoPin, LOW);
      delay(20);  // Wait for the rest of the period (20 ms)
    }

    // Method to stop the servo
    void stopServo() {
      // Stop the PWM signal by not sending any HIGH pulse
      digitalWrite(servoPin, LOW);
    }
};

#endif