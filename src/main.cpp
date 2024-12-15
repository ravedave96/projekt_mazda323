#include <Arduino.h>
#include <Wire.h>

#define trigPinLH 6
#define echoPinLH 7
#define buzzerPinLH 10

// Global variables
float distanz = 0;                      // Stores the measured distance
unsigned long lastMeasurementTime = 0;  // Tracks the last measurement time
unsigned long measurementInterval = 500; // Interval for distance measurement in ms

// Function prototypes
void distanceLH();
void buzzerLH();

void setup() {
    Serial.begin(9600);
    pinMode(trigPinLH, OUTPUT);
    pinMode(echoPinLH, INPUT);
    pinMode(buzzerPinLH, OUTPUT);
}

void loop() {
    // Call the distance measurement function
    distanceLH();
    // Continuously handle buzzer logic
    buzzerLH();
}

// Function to measure distance at regular intervals
void distanceLH() {
    // Check if it's time to measure distance
    if (millis() - lastMeasurementTime < measurementInterval) {
        return; // Not yet time for the next measurement
    }
    lastMeasurementTime = millis(); // Update the last measurement time

    float zeit = 0;

    // Trigger the ultrasonic sensor
    digitalWrite(trigPinLH, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinLH, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinLH, LOW);

    // Measure the echo pulse duration
    zeit = pulseIn(echoPinLH, HIGH, 30000); // Timeout of 30ms
    if (zeit == 0) {
        Serial.println("Kein Echo empfangen");
        distanz = -1; // Assign invalid distance for error handling
        return;
    }

    // Calculate the distance in cm
    distanz = (zeit / 2) * 0.0344;

    // Print the distance for debugging
    Serial.print("Distanz LH = ");
    Serial.print(distanz);
    Serial.println(" cm");
}

// Function to control the buzzer based on distance
void buzzerLH() {
    int interval = 1000; // Default tone duration

    // Determine the tone duration based on distance
    if (distanz < 0) {
        noTone(buzzerPinLH); // Turn off the buzzer for invalid distances
        return;
    } else if (distanz >= 100) {
        interval = 1000; // Slow rhythm for far distances
    } else if (distanz < 100 && distanz > 55) {
        interval = 500;  // Medium rhythm for mid-range distances
    } else {
        interval = 200;  // Fast rhythm for close distances
    }

    // Play the tone with the determined interval
    tone(buzzerPinLH, 523, interval); // Frequency: 523 Hz (C4), duration: interval
}