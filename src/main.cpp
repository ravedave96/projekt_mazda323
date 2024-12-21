#include <Arduino.h>
#include <Wire.h>

#define trigPinLH 6   // Pin für den Trigger des Ultraschallsensors
#define echoPinLH 7   // Pin für das Echo des Ultraschallsensors
#define buzzerPinLH 10 // Pin für den Summer (Buzzer)

// Globale Variablen
float distanzLH = 0;                      // Speichert die gemessene Distanz
unsigned long lastMeasurementTimeLH = 0;  // Speichert die Zeit des letzten Messvorgangs
unsigned long measurementIntervalLH = 500; // Zeitintervall zwischen den Messungen in Millisekunden

// Funktionsprototypen
void distanceLH(); // Funktion zur Distanzmessung
void buzzerLH();   // Funktion zur Steuerung des Summers

void setup() {
    Serial.begin(9600);                 // Serielle Kommunikation starten
    pinMode(trigPinLH, OUTPUT);         // Trigger-Pin als Ausgang definieren
    pinMode(echoPinLH, INPUT);          // Echo-Pin als Eingang definieren
    pinMode(buzzerPinLH, OUTPUT);       // Buzzer-Pin als Ausgang definieren
}

void loop() {
    // Funktion zur Distanzmessung aufrufen
    distanceLH();
    // Funktion zur kontinuierlichen Steuerung des Summers aufrufen
    buzzerLH();
}

// Funktion zur Distanzmessung in regelmäßigen Abständen
void distanceLH() {
    // Überprüfen, ob das Zeitintervall seit der letzten Messung abgelaufen ist
    if (millis() - lastMeasurementTimeLH < measurementIntervalLH) {
        return; // Noch nicht Zeit für die nächste Messung
    }
    lastMeasurementTimeLH = millis(); // Zeit der letzten Messung aktualisieren

    float zeit = 0; // Variable zur Speicherung der Echo-Zeit

    // Ultraschallsensor auslösen (Trigger senden)
    digitalWrite(trigPinLH, LOW);       // Trigger auf LOW setzen
    delayMicroseconds(2);               // Kurze Pause
    digitalWrite(trigPinLH, HIGH);      // Trigger auf HIGH setzen (10 µs Impuls)
    delayMicroseconds(10);
    digitalWrite(trigPinLH, LOW);       // Trigger wieder auf LOW setzen

    // Dauer des Echos messen
    zeit = pulseIn(echoPinLH, HIGH, 30000); // Echo-Puls messen (Timeout: 30 ms)
    if (zeit == 0) { // Kein Echo empfangen
        Serial.println("Kein Echo empfangen");
        distanzLH = -1; // Ungültigen Distanzwert zuweisen
        return;
    }

    // Distanz in cm berechnen
    distanzLH = (zeit / 2) * 0.0344;

    // Distanz zur Debug-Ausgabe ausgeben
    Serial.print("Distanz LH = ");
    Serial.print(distanzLH);
    Serial.println(" cm");
}

// Funktion zur Steuerung des Summers basierend auf der Distanz
void buzzerLH() {
    int interval = 1000; // Standardintervall für den Ton

    // Tonintervall basierend auf der gemessenen Distanz bestimmen
    if (distanzLH < 0) { // Ungültige Distanz
        noTone(buzzerPinLH); // Summer ausschalten
        return;
    } else if (distanzLH >= 100) { // Grossse Distanz
        interval = 1000; // Langsamer Rhythmus
    } else if (distanzLH < 100 && distanzLH > 55) { // Mittlere Distanz
        interval = 500;  // Mittlerer Rhythmus
    } else { // Kleine Distanz
        interval = 200;  // Schneller Rhythmus
    }

    // Ton mit der berechneten Dauer abspielen
    tone(buzzerPinLH, 523, interval); // Frequenz: 523 Hz (C4), Dauer: interval
}