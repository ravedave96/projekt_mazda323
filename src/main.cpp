#include <Arduino.h>

// Pins für die linke Seite
#define trigPinLH 6   // Pin für den Trigger des Ultraschallsensors (links)
#define echoPinLH 7   // Pin für das Echo des Ultraschallsensors (links)
#define buzzerPinLH 10 // Pin für den Buzzer (links)

// Pins für die rechte Seite
#define trigPinRH 8   // Pin für den Trigger des Ultraschallsensors (rechts)
#define echoPinRH 9   // Pin für das Echo des Ultraschallsensors (rechts)
#define buzzerPinRH 11 // Pin für den Buzzer (rechts)

// Globale Variablen für die linke Seite
unsigned long distanzLH = 0;                      // Speichert die gemessene Distanz (links)
unsigned long lastMeasurementTimeLH = 0;  // Speichert die Zeit des letzten Messvorgangs (links)
unsigned long lastBuzzerToggleLH = 0;     // Speichert die Zeit der letzten Buzzer-Aktivierung (links)
bool buzzerOnLH = false;                  // Zustand des Buzzers (links)

// Globale Variablen für die rechte Seite
unsigned long distanzRH = 0;                      // Speichert die gemessene Distanz (rechts)
unsigned long lastMeasurementTimeRH = 0;  // Speichert die Zeit des letzten Messvorgangs (rechts)
unsigned long lastBuzzerToggleRH = 0;     // Speichert die Zeit der letzten Buzzer-Aktivierung (rechts)
bool buzzerOnRH = false;                  // Zustand des Buzzers (rechts)

// Gemeinsame Konfiguration
unsigned long measurementIntervalLH = 500; // Linke Seite
unsigned long measurementIntervalRH = 500; // Rechte Seite

// Funktionsprototypen
void distanceLH(); // Funktion zur Distanzmessung (links)
void buzzerLH(unsigned int frequencyLH, unsigned long intervalLH); // Tonsteuerung für Buzzer (links)
void distanceRH(); // Funktion zur Distanzmessung (rechts)
void buzzerRH(unsigned int frequencyRH, unsigned long intervalRH); // Tonsteuerung für Buzzer (rechts)

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
    if (distanzLH > 0 && distanzLH < 200) { // Nur innerhalb eines gültigen Bereichs 0-2m
        unsigned long intervalLH = (0.11 * (distanzLH * distanzLH) + 50);
        buzzerLH(526, (intervalLH * 2)); // Frequenz basierend auf der Distanz
    }

    // Rechte Seite
    distanceRH();
    if (distanzRH > 0 && distanzRH < 200) { // Nur innerhalb eines gültigen Bereichs 0-2m
        unsigned long intervalRH = (0.11 * (distanzRH * distanzRH) + 50);
        buzzerRH(526, (intervalRH * 2)); // Frequenz basierend auf der Distanz
    }
}

// Funktion zur Distanzmessung (links)
void distanceLH() {
    if (millis() - lastMeasurementTimeLH < measurementIntervalLH) {
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

// Funktion zur Tonsteuerung (links)
void buzzerLH(unsigned int frequencyLH, unsigned long intervalLH) {
    unsigned long startTime = millis();
    unsigned long halfPeriod = 1000000L / (1000 * 2); // // Halbperiode für 1 kHz (500 µs)

    while (millis() - startTime < intervalLH) {
        digitalWrite(buzzerPinLH, HIGH);
        delayMicroseconds(halfPeriod);
        digitalWrite(buzzerPinLH, LOW);
        delayMicroseconds(halfPeriod);
    }
}

// Funktion zur Distanzmessung (rechts)
void distanceRH() {
    if (millis() - lastMeasurementTimeRH < measurementIntervalRH) {
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

// Funktion zur Tonsteuerung (rechts)
void buzzerRH(unsigned int frequencyRH, unsigned long intervalRH) {
    unsigned long startTime = millis();
    unsigned long halfPeriod = 1000000L / (1000 * 2); // // Halbperiode für 1 kHz (500 µs)

    while (millis() - startTime < intervalRH) {
        digitalWrite(buzzerPinRH, HIGH);
        delayMicroseconds(halfPeriod);
        digitalWrite(buzzerPinRH, LOW);
        delayMicroseconds(halfPeriod);
    }
}

