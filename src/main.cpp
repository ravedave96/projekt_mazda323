#include <Arduino.h>
#include <Wire.h>

// Zuweisung Pin auf Arduino Board
#define tasterPin 2
#define parkPin 12
#define beschleunigungPin 13

#define trigPinLH 6
#define echoPinLH 7
#define trigPinRH 8
#define echoPinRH 9
#define buzzerPinLH 10
#define buzzerPinRH 11

// Globale Variablen
int push = 0;       // aktueller Zustand des Tasters
int lastpush = 0;   // letzter Zustand des Tasters
bool schalter = false;  // aktueller Zustand des Ausgangs (umschaltbar)

float distanzLH = 0;                      // Speichert die gemessene Distanz
unsigned long lastMeasurementTimeLH = 0;  // Speichert die Zeit des letzten Messvorgangs
unsigned long measurementIntervalLH = 500; // Zeitintervall zwischen den Messungen in Millisekunden

float distanzRH = 0;                      // Speichert die gemessene Distanz
unsigned long lastMeasurementTimeRH = 0;  // Speichert die Zeit des letzten Messvorgangs
unsigned long measurementIntervalRH = 500; // Zeitintervall zwischen den Messungen in Millisekunden


// Funktionsprototypen
void umschalten();
void ParkhilfeLH();
void distanceLH(); // Funktion zur Distanzmessung Links
void buzzerLH();   // Funktion zur Steuerung des Buzzer Links
void ParkhilfeRH();
void distanceRH(); // Funktion zur Distanzmessung Rechts
void buzzerRH();   // Funktion zur Steuerung des Buzzer Rechts
void Beschleunigung();

void setup() {
    Serial.begin(9600);

    pinMode(tasterPin, INPUT);          // Eingang  2, Taster
    pinMode(parkPin, OUTPUT);           // Ausgang 12, Parkhilfe
    pinMode(beschleunigungPin, OUTPUT); // Ausgang 13, Beschleunigung

    digitalWrite(parkPin, LOW);
    digitalWrite(beschleunigungPin, LOW);

    pinMode(trigPinLH, OUTPUT);     // Ausgang 6, TriggerLH
    pinMode(echoPinLH, INPUT);      // Eingang 7, EchoLH
    pinMode(trigPinRH, OUTPUT);     // Ausgang 8, TriggerRH
    pinMode(echoPinRH, INPUT);      // Eingang 9, EchoRH
    pinMode(buzzerPinLH, OUTPUT);   // Ausgang 10, Buzzer LH
    pinMode(buzzerPinRH, OUTPUT);   // Ausgang 11, Buzzer RH

    tone(buzzerPinLH, 1000, 50);
    tone(buzzerPinRH, 1000, 50);
}

void loop() {
   umschalten();
        if (schalter) {
            ParkhilfeLH();
            ParkhilfeRH();
        } else {
            Beschleunigung();
        }
}

// Funktion zum manuellen Umschalten von Beschleunigungsmesser auf Parkhilfe
void umschalten () {
    // Umschalttaste auslesen
    push = digitalRead(tasterPin);

    // Prüfen, ob der Taster gedrückt wurde (Zustandsänderung von LOW nach HIGH)
    if (push == HIGH && lastpush == LOW) {
        schalter = !schalter;   // Taster-Zustand umkehren
    }
    // Verzögerung für Entprellung
    delay(50);

    // Aktuellen Zustand Tasters speichern
    lastpush = push;
}

// Funktion zur Parkhilfe linke Seite
void ParkhilfeLH () {
    // Funktion zur Distanzmessung aufrufen
    distanceLH();
    // Funktion zur kontinuierlichen Steuerung des Buzzer aufrufen
    buzzerLH();
}

// Funktion zur Distanzmessung in regelmässigen Abständen linke Seite
void distanceLH() {
    // Überprüfen, ob das Zeitintervall seit der letzten Messung abgelaufen ist
    if (millis() - lastMeasurementTimeLH < measurementIntervalLH) {
        return; // Noch nicht Zeit für die nächste Messung
    }
    lastMeasurementTimeLH = millis(); // Zeit der letzten Messung aktualisieren

    float zeitLH = 0; // Variable zur Speicherung der Echo-Zeit

    // Ultraschallsensor auslösen (Trigger senden)
    digitalWrite(trigPinLH, LOW);       // Trigger auf LOW setzen
    delayMicroseconds(2);
    digitalWrite(trigPinLH, HIGH);      // Trigger auf HIGH setzen (10 µs Impuls)
    delayMicroseconds(10);
    digitalWrite(trigPinLH, LOW);       // Trigger wieder auf LOW setzen

    // Dauer des Echos messen
    zeitLH = pulseIn(echoPinLH, HIGH, 30000); // Echo-Puls messen (Timeout: 30 ms)
    if (zeitLH == 0) { // Kein Echo empfangen
        Serial.println("LH Kein Echo empfangen");
        distanzLH = -1; // Ungültigen Distanzwert zuweisen
        return;
    }

    // Distanz in cm berechnen
    distanzLH = (zeitLH / 2) * 0.0344;

    // Distanz ausgeben
    Serial.print("Distanz LH = ");
    Serial.print(distanzLH);
    Serial.println(" cm");
}

// Funktion zur Steuerung des Buzzers basierend auf der Distanz linke Seite
void buzzerLH() {
    int intervalLH = 1000; // Standardintervall für den Ton

    // Tonintervall basierend auf der gemessenen Distanz bestimmen
    if (distanzLH < 0) { // Ungültige Distanz
        noTone(buzzerPinLH); // Buzzer ausschalten
        return;
    } else if (distanzLH >= 100) { // Grosse Distanz
        intervalLH = 1000; // Langsamer Rhythmus
    } else if (distanzLH < 100 && distanzLH > 55) { // Mittlere Distanz
        intervalLH = 500;  // Mittlerer Rhythmus
    } else { // Kleine Distanz
        intervalLH = 200;  // Schneller Rhythmus
    }

    // Ton mit der berechneten Dauer abspielen
    tone(buzzerPinLH, 523, intervalLH); // Frequenz: 523 Hz (C4), Dauer: interval
}

// Funktion zur Parkhilfe rechte Seite
void ParkhilfeRH () {
    // Funktion zur Distanzmessung aufrufen
    distanceRH();
    // Funktion zur kontinuierlichen Steuerung des Buzzer aufrufen
    buzzerRH();
}

// Funktion zur Distanzmessung in regelmässigen Abständen rechte Seite
void distanceRH() {
    // Überprüfen, ob das Zeitintervall seit der letzten Messung abgelaufen ist
    if (millis() - lastMeasurementTimeRH < measurementIntervalRH) {
        return; // Noch nicht Zeit für die nächste Messung
    }
    lastMeasurementTimeRH = millis(); // Zeit der letzten Messung aktualisieren

    float zeitRH = 0; // Variable zur Speicherung der Echo-Zeit

    // Ultraschallsensor auslösen (Trigger senden)
    digitalWrite(trigPinRH, LOW);       // Trigger auf LOW setzen
    delayMicroseconds(2);
    digitalWrite(trigPinRH, HIGH);      // Trigger auf HIGH setzen (10 µs Impuls)
    delayMicroseconds(10);
    digitalWrite(trigPinRH, LOW);       // Trigger wieder auf LOW setzen

    // Dauer des Echos messen
    zeitRH = pulseIn(echoPinRH, HIGH, 30000); // Echo-Puls messen (Timeout: 30 ms)
    if (zeitRH == 0) { // Kein Echo empfangen
        Serial.println("RH Kein Echo empfangen");
        distanzRH = -1; // Ungültigen Distanzwert zuweisen
        return;
    }

    // Distanz in cm berechnen
    distanzRH = (zeitRH / 2) * 0.0344;

    // Distanz ausgeben
    Serial.print("Distanz RH = ");
    Serial.print(distanzRH);
    Serial.println(" cm");
}

// Funktion zur Steuerung des Buzzers basierend auf der Distanz rechte Seite
void buzzerRH() {
    int intervalRH = 1000; // Standardintervall für den Ton

    // Tonintervall basierend auf der gemessenen Distanz bestimmen
    if (distanzRH < 0) { // Ungültige Distanz
        noTone(buzzerPinRH); // Buzzer ausschalten
        return;
    } else if (distanzRH >= 100) { // Grosse Distanz
        intervalRH = 1000; // Langsamer Rhythmus
    } else if (distanzRH < 100 && distanzRH > 55) { // Mittlere Distanz
        intervalRH = 500;  // Mittlerer Rhythmus
    } else { // Kleine Distanz
        intervalRH = 200;  // Schneller Rhythmus
    }

    // Ton mit der berechneten Dauer abspielen
    tone(buzzerPinRH, 523, intervalRH); // Frequenz: 523 Hz (C4), Dauer: interval
}


void Beschleunigung () {
    //Platzhalter für die Beschleunigungsfunktion
}


