#include <Arduino.h>
#include <Wire.h>

#define trigPinLH 6
#define echoPinLH 7
#define buzzerPinLH 10

// Global variables
float distanz = 0;
unsigned long lastMeasurementTime = 0;  // speichert letzte Messung der Distanz in ms
unsigned long measurementInterval = 500; // interval für Distanz Messung ist 500 ms

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
    // Distanz wird alle 500ms gemessen
    distanceLH();
    // buzzer kann tone kontinuierlich ausgeben ohne von der Distanz Messung unterbrochen zu werden.
    buzzerLH();
}

void distanceLH() { //millis() wird verwendet, da delay() die ganze funktion blockiert
    // Verzweigung ob Messung ansteht oder nicht
    if (millis() - lastMeasurementTime < measurementInterval) {
        return; // noch nicht Zeit für die Messung
    }
    lastMeasurementTime = millis();

    float zeit = 0;

    //  ultrasonic senden
    digitalWrite(trigPinLH, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinLH, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinLH, LOW);

    // ultrasonic echo pulse duration messen
    zeit = pulseIn(echoPinLH, HIGH, 30000); // Timeout of 30ms
    if (zeit == 0) {
        Serial.println("Kein Echo empfangen");
        distanz = -1; // für Fehler Erkennung
        return;
    }

    // distance in cm
    distanz = (zeit / 2) * 0.0344;

    Serial.print("Distanz LH = ");
    Serial.print(distanz);
    Serial.println(" cm");
}

void buzzerLH() {
    int interval = 1000; // Voreinstellung tone interval

    // Bestimmung tone interval zur passenden Distanz
    if (distanz < 0) {
        noTone(buzzerPinLH);
        // Buzzer gibt kein Ton aus, wenn Distanz weniger 0 ist.
        // bei Fehelrmessung ist Distanz = -1, damit es klar ist.
        return;
    } else if (distanz >= 100) {
        interval = 1000; // kleiner interval für grössere Distanzen
    } else if (distanz < 100 && distanz > 55) {
        interval = 500;  // mittlerer interval für mittlere Distanzen
    } else {
        interval = 200;  // schneller interval für kleine Distanzen
    }

    // tone wird mit der bestimmten interval Länge ausgegeben
    tone(buzzerPinLH, 523, interval); // tone (C4)
}