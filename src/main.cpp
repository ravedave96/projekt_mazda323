#include <Arduino.h>
#include <NewTone.h> // Bibliothek für die unabhängige Steuerung der Buzzer

// Pins für die linke Seite
#define trigPinLH 6   // Pin für den Trigger des Ultraschallsensors (links)
#define echoPinLH 7   // Pin für das Echo des Ultraschallsensors (links)
#define buzzerPinLH 10 // Pin für den Buzzer (links)

// Pins für die rechte Seite
#define trigPinRH 8   // Pin für den Trigger des Ultraschallsensors (rechts)
#define echoPinRH 9   // Pin für das Echo des Ultraschallsensors (rechts)
#define buzzerPinRH 11 // Pin für den Buzzer (rechts)

// Globale Variablen für die linke Seite
float distanzLH = 0;                     // Speichert die gemessene Distanz (links)
unsigned long lastMeasurementTimeLH = 0; // Speichert die Zeit des letzten Messvorgangs (links)

// Globale Variablen für die rechte Seite
float distanzRH = 0;                     // Speichert die gemessene Distanz (rechts)
unsigned long lastMeasurementTimeRH = 0; // Speichert die Zeit des letzten Messvorgangs (rechts)

// Gemeinsame Konfiguration
unsigned long measurementInterval = 500; // Zeitintervall zwischen den Messungen in Millisekunden

// Funktionsprototypen
void distanceLH(); // Funktion zur Distanzmessung (links)
void buzzerLH();   // Funktion zur Steuerung des Buzzers (links)
void distanceRH(); // Funktion zur Distanzmessung (rechts)
void buzzerRH();   // Funktion zur Steuerung des Buzzers (rechts)

void setup() {
    Serial.begin(9600);                 // Serielle Kommunikation starten

    // Pins für die linke Seite konfigurieren
    pinMode(trigPinLH, OUTPUT);         // Trigger-Pin (links) als Ausgang definieren
    pinMode(echoPinLH, INPUT);          // Echo-Pin (links) als Eingang definieren
    pinMode(buzzerPinLH, OUTPUT);       // Buzzer-Pin (links) als Ausgang definieren

    // Pins für die rechte Seite konfigurieren
    pinMode(trigPinRH, OUTPUT);         // Trigger-Pin (rechts) als Ausgang definieren
    pinMode(echoPinRH, INPUT);          // Echo-Pin (rechts) als Eingang definieren
    pinMode(buzzerPinRH, OUTPUT);       // Buzzer-Pin (rechts) als Ausgang definieren
}

void loop() {
    // Linke Seite
    distanceLH();
    buzzerLH();

    // Rechte Seite
    distanceRH();
    buzzerRH();
}

// Funktion zur Distanzmessung (links)
void distanceLH() {
    if (millis() - lastMeasurementTimeLH < measurementInterval) {
        return; // Noch nicht Zeit für die nächste Messung
    }
    lastMeasurementTimeLH = millis(); // Zeit der letzten Messung aktualisieren

    float zeit = 0;

    // Ultraschallsensor (links) auslösen
    digitalWrite(trigPinLH, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinLH, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinLH, LOW);

    // Dauer des Echos messen
    zeit = pulseIn(echoPinLH, HIGH, 30000);
    if (zeit == 0) {
        Serial.println("Kein Echo empfangen (links)");
        distanzLH = -1;
        return;
    }

    // Distanz berechnen
    distanzLH = (zeit / 2) * 0.0344;

    Serial.print("Distanz LH = ");
    Serial.print(distanzLH);
    Serial.println(" cm");
}

// Funktion zur Steuerung des Buzzers (links)
void buzzerLH() {
    if (distanzLH < 0) {
        noNewTone(buzzerPinLH); // Ton ausschalten, wenn keine Distanz gemessen wird
        return;
    }

    int frequencyLH = max(100, 1000 - (0.11 * (distanzLH * distanzLH))); // Frequenz berechnen (Mindestfrequenz 100 Hz)
    NewTone(buzzerPinLH, frequencyLH); // Ton mit der berechneten Frequenz abspielen
}

// Funktion zur Distanzmessung (rechts)
void distanceRH() {
    if (millis() - lastMeasurementTimeRH < measurementInterval) {
        return; // Noch nicht Zeit für die nächste Messung
    }
    lastMeasurementTimeRH = millis(); // Zeit der letzten Messung aktualisieren

    float zeit = 0;

    // Ultraschallsensor (rechts) auslösen
    digitalWrite(trigPinRH, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinRH, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinRH, LOW);

    // Dauer des Echos messen
    zeit = pulseIn(echoPinRH, HIGH, 30000);
    if (zeit == 0) {
        Serial.println("Kein Echo empfangen (rechts)");
        distanzRH = -1;
        return;
    }

    // Distanz berechnen
    distanzRH = (zeit / 2) * 0.0344;

    Serial.print("Distanz RH = ");
    Serial.print(distanzRH);
    Serial.println(" cm");
}

// Funktion zur Steuerung des Buzzers (rechts)
void buzzerRH() {
    if (distanzRH < 0) {
        noNewTone(buzzerPinRH); // Ton ausschalten, wenn keine Distanz gemessen wird
        return;
    }

    int frequencyRH = max(100, 1000 - (0.11 * (distanzRH * distanzRH))); // Frequenz berechnen (Mindestfrequenz 100 Hz)
    NewTone(buzzerPinRH, frequencyRH); // Ton mit der berechneten Frequenz abspielen
}

