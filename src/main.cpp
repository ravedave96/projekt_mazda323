#include <Arduino.h>
#include <Wire.h>

// Zuweisung Pin auf Arduino Board
#define tasterPin 2
#define parkPin 12
#define beschleunigungPin 13

#define trigPinLH 6 // Pin für den Trigger des Ultraschallsensors (links)
#define echoPinLH 7 // Pin für das Echo des Ultraschallsensors (links)
#define trigPinRH 8 // Pin für den Trigger des Ultraschallsensors (rechts)
#define echoPinRH 9 // Pin für das Echo des Ultraschallsensors (rechts)
#define buzzerPinLH 10 // Pin für den Buzzer (links)(analog)
#define buzzerPinRH 11 // Pin für den Buzzer (rechts)(analog)

// Globale Variablen
int push = 0;       // aktueller Zustand des Tasters
int lastpush = 0;   // letzter Zustand des Tasters
bool schalter = false;  // aktueller Zustand des Ausgangs (umschaltbar)

// Globale Variablen für die linke Seite
float distanzLH = 0;                      // Speichert die gemessene Distanz (links)
unsigned long lastMeasurementTimeLH = 0;  // Speichert die Zeit des letzten Messvorgangs (links)
unsigned long previousBuzzerTimeLH = 0; // Zeit der letzten Buzzer-Aktivierung (links)

// Globale Variablen für die rechte Seite
float distanzRH = 0;                      // Speichert die gemessene Distanz (rechts)
unsigned long lastMeasurementTimeRH = 0;  // Speichert die Zeit des letzten Messvorgangs (rechts)
unsigned long previousBuzzerTimeRH = 0; // Zeit der letzten Buzzer-Aktivierung (rechts)

// Gemeinsame Konfiguration
unsigned long measurementIntervalLH = 500; // Mindestzeitabstände zwischen zwei Messungen für die linke Seite (in Millisekunden).
unsigned long measurementIntervalRH = 500; // Mindestzeitabstände zwischen zwei Messungen für die rechte Seite (in Millisekunden).

// Main-Funktionsprototypen
void umschalten();
void ParkhilfeLH();
void ParkhilfeRH();
void Beschleunigung();

//Sub-Funktionsprototypen
void distanceLH(); // Funktion zur Distanzmessung (links)
void buzzerLH(unsigned long intervalLH); // Tonsteuerung für Buzzer (links)
void distanceRH(); // Funktion zur Distanzmessung (rechts)
void buzzerRH(unsigned long intervalRH); // Tonsteuerung für Buzzer (rechts)

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
    if (distanzLH > 0 && distanzLH < 300) { // Nur innerhalb eines gültigen Bereichs 0-3m
        unsigned long intervalLH = (0.11 * (distanzLH * distanzLH) + 50); // quadratische Funktion, so dass die Dauer(Intervall) des Ton mit abnehmender Distanz kürzer wird
        //map() kann auch eingefügt werden. map(distanz, 1, 200, 50, 1000) skaliert die Distanz auf eine Tonlänge zwischen 50 ms und 1000 ms.
        buzzerLH(intervalLH); // Aktiviert den Buzzer mit dem berechneten Intervall.
    } else {
        analogWrite(buzzerPinLH, LOW); // Buzzer ausschalten, wenn Distanz ausserhalb des Bereichs
    }
    // Funktion zur kontinuierlichen Steuerung des Buzzer aufrufen
}

// Funktion zur Distanzmessung in regelmässigen Abständen linke Seite
void distanceLH() {
    // Führt die Messung nur aus, wenn der Mindestzeitabstand (measurementIntervalLH) überschritten wurde (500ms).
    if (millis() - lastMeasurementTimeLH < measurementIntervalLH) {
        return; // Noch nicht Zeit für die nächste Messung
    }
    lastMeasurementTimeLH = millis(); // Zeit der letzten Messung aktualisieren

    float zeit = 0;

    // Ultraschallsensor (links) auslösen, sendet ein 10-µs-Signal über den trigPinLH
    digitalWrite(trigPinLH, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinLH, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinLH, LOW);

    // Dauer des Echos messen
    // Misst die Zeit, die das Echo benötigt, um den Sensor zu erreichen (mit Timeout von 17442 µs für 300 cm).
    zeit = pulseIn(echoPinLH, HIGH, 17442); // 11630ms=200cm, 17442ms=300cm, 30000ms=500cm, (30.000μs/2*0.0344≈516cm)
    if (zeit == 0) {
        Serial.println("Kein Echo empfangen (links)");
        distanzLH = -1; // Gibt -1 zurück, wenn kein Echo empfangen wurde.
        return;
    }

    // Distanz berechnen basierend auf der Schallgeschwindigkeit (0,0344 cm/µs).
    distanzLH = (zeit / 2) * 0.0344;

    Serial.print("Distanz LH = ");
    Serial.print(distanzLH);
    Serial.println(" cm");
}

// Funktion zur Steuerung des Buzzers basierend auf der Distanz linke Seite
void buzzerLH(unsigned long intervalLH) { //intervalLH: Zeitdauer in Millisekunden, wie lange der Buzzer an- oder ausgeschaltet bleiben soll.
    static bool buzzerStateLH = false; // Aktueller Zustand des Buzzers (links), Initialwert: false (Buzzer ist zu Beginn ausgeschaltet).

    //millis(): gibt die Zeit seit dem Start des Mikrocontrollers in Millisekunden zurück.
    //aktuelle Zeit (millis()) - letzte Aktivierungszeit (previousBuzzerTimeLH) >= dem angegebenen Intervall (intervalLH)
    //wenn ja, ist es Zeit, den Zustand des Buzzers zu wechseln (An oder Aus).
    if (millis() - previousBuzzerTimeLH >= intervalLH) {
        previousBuzzerTimeLH = millis(); // Zeit aktualisieren, speichert den Zeitpunkt der letzten Buzzer-Aktivierung.
        buzzerStateLH = !buzzerStateLH; // Buzzer-Zustand wird invertiert
        analogWrite(buzzerPinLH, buzzerStateLH ? 128 : LOW); //Ternäre Bedingung: buzzerStateLH == true: 128 (50% von Frequenz 255, was einem C4 Ton entspricht), buzzerStateLH == false: LOW
    }
}

// Funktion zur Parkhilfe rechte Seite
void ParkhilfeRH () {
    // Funktion zur Distanzmessung aufrufen
    distanceRH();
    if (distanzRH > 0 && distanzRH < 300) {
        unsigned long intervalRH = (0.11 * (distanzRH * distanzRH) + 50);

        buzzerRH(intervalRH);
    } else {
        analogWrite(buzzerPinRH, LOW);
    }
}

// Funktion zur Distanzmessung in regelmässigen Abständen rechte Seite
void distanceRH() {
    // Führt die Messung nur aus, wenn der Mindestzeitabstand (measurementIntervalLH) überschritten wurde (500ms).
    if (millis() - lastMeasurementTimeRH < measurementIntervalRH) {
        return; // Noch nicht Zeit für die nächste Messung
    }
    lastMeasurementTimeRH = millis(); // Zeit der letzten Messung aktualisieren

    float zeit = 0;

    // Ultraschallsensor (rechts) auslösen, sendet ein 10-µs-Signal über den trigPinLH
    digitalWrite(trigPinRH, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinRH, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinRH, LOW);

    // Dauer des Echos messen
    // Misst die Zeit, die das Echo benötigt, um den Sensor zu erreichen (mit Timeout von 17442 µs für 300 cm).
    zeit = pulseIn(echoPinRH, HIGH, 17442); // 11630ms=200cm, 17442ms=300cm, 30000ms=500cm, (30.000μs/2*0.0344≈516cm)
    if (zeit == 0) {
        Serial.println("Kein Echo empfangen (rechts)");
        distanzRH = -1;  // Gibt -1 zurück, wenn kein Echo empfangen wurde.
        return;
    }

    // Distanz berechnen basierend auf der Schallgeschwindigkeit (0,0344 cm/µs).
    distanzRH = (zeit / 2) * 0.0344;

    Serial.print("Distanz RH = ");
    Serial.print(distanzRH);
    Serial.println(" cm");
}

// Funktion zur Steuerung des Buzzers basierend auf der Distanz rechte Seite
void buzzerRH(unsigned long intervalRH) {
    static bool buzzerStateRH = false;

    if (millis() - previousBuzzerTimeRH >= intervalRH) {
        previousBuzzerTimeRH = millis();
        buzzerStateRH = !buzzerStateRH;
        analogWrite(buzzerPinRH, buzzerStateRH ? 128 : LOW);
    }
}

void Beschleunigung () {
    //Platzhalter für die Beschleunigungsfunktion
}


