#include <Arduino.h>
#include <Wire.h>

#define trigPinLH 6
#define echoPinLH 7
#define buzzerPinLH 10

void setup() {
    Serial.begin(9600);
    pinMode(trigPinLH, OUTPUT);
    pinMode(echoPinLH, INPUT);
    pinMode(buzzerPinLH, OUTPUT);
    tone(buzzerPinLH, 1000, 2000);
}

void loop () {
    float zeit = 0, distanz = 0;

    digitalWrite(trigPinLH, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinLH, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinLH, LOW);

    zeit = pulseIn(echoPinLH, HIGH, 30000);
    if (zeit == 0) {
        Serial.println("Kein Echo empfangen");
        noTone(buzzerPinLH);
        return;
    }

    distanz = (zeit / 2) * 0.0344;

    Serial.print("Distanz LH = ");
    Serial.print(distanz);
    Serial.println(" cm");

    if (distanz >= 100) {
        tone(buzzerPinLH, 523, 1000); // C4
    } else if (distanz < 110 && distanz > 50) {
        tone(buzzerPinLH, 523, 500);
    } else {
        tone(buzzerPinLH, 523, 100);
    }

    delay(500);
}