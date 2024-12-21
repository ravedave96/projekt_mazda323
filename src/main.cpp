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


// Funktionsprototypen
void umschalten();
void ParkhilfeLH();
void distanceLH(); // Funktion zur Distanzmessung
void buzzerLH();   // Funktion zur Steuerung des Buzzer
void ParkhilfeRH();
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
    pinMode(trigPinRH, OUTPUT);     // Eingang 8, TriggerRH
    pinMode(trigPinLH, INPUT);      // Eingang 9, EchoRH
    pinMode(buzzerPinLH, OUTPUT);   // Ausgang 10, Buzzer LH
    pinMode(buzzerPinRH, OUTPUT);   // Ausgang 11, Buzzer RH

    tone(buzzerPinLH, 1000, 2000);
    tone(buzzerPinRH, 1000, 2000);
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

        // Zustand der Ausgänge basierend auf dem Taster-Zustand setzen
        if (schalter) {
            digitalWrite(beschleunigungPin, HIGH); // Ausgang 13 HIGH
            digitalWrite(parkPin, LOW);  // Ausgang 12 LOW
        } else {
            digitalWrite(beschleunigungPin, LOW);  // Ausgang 13 LOW
            digitalWrite(parkPin, HIGH); // Ausgang 12 HIGH
        }
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

    float zeit = 0; // Variable zur Speicherung der Echo-Zeit

    // Ultraschallsensor auslösen (Trigger senden)
    digitalWrite(trigPinLH, LOW);       // Trigger auf LOW setzen
    delayMicroseconds(2);
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

    // Distanz ausgeben
    Serial.print("Distanz LH = ");
    Serial.print(distanzLH);
    Serial.println(" cm");
}

// Funktion zur Steuerung des Buzzers basierend auf der Distanz
void buzzerLH() {
    int interval = 1000; // Standardintervall für den Ton

    // Tonintervall basierend auf der gemessenen Distanz bestimmen
    if (distanzLH < 0) { // Ungültige Distanz
        noTone(buzzerPinLH); // Buzzer ausschalten
        return;
    } else if (distanzLH >= 100) { // Grosse Distanz
        interval = 1000; // Langsamer Rhythmus
    } else if (distanzLH < 100 && distanzLH > 55) { // Mittlere Distanz
        interval = 500;  // Mittlerer Rhythmus
    } else { // Kleine Distanz
        interval = 200;  // Schneller Rhythmus
    }

    // Ton mit der berechneten Dauer abspielen
    tone(buzzerPinLH, 523, interval); // Frequenz: 523 Hz (C4), Dauer: interval
}

// Funktion zur Parkhilfe rechte Seite
void ParkhilfeRH () {
    float zeit = 0, distanz = 0;

    // Trigger-Signal erzeugen
    digitalWrite(trigPinRH, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinRH, HIGH); // Ultraschallsignal senden
    delayMicroseconds(10);
    digitalWrite(trigPinRH, LOW);

    // Echo-Zeit messen
    zeit = pulseIn(echoPinRH, HIGH, 30000); //liest echoPin und misst die Zeit zwischen HIGH LOW und wieder HIGH
    if (zeit == 0) {
        // Keine valide Messung, Rückkehr aus der Funktion
        Serial.println("Kein Echo empfangen");
        noTone(buzzerPinRH); // Buzzer ausschalten, falls aktiv
        return;
    }

    //Distanz berechnen in cm, sound umgerechnet von m/s in cm/µs (0.0344 cm/µs)
    distanz = (zeit / 2) * 0.0344;

    Serial.print("Distanz RH = ");
    Serial.print(distanz);
    Serial.println(" cm");

    // Audio Distanz Signal
    if (distanz >= 110) {
        tone(buzzerPinRH, 523, 1000); // C4
    } else if (distanz < 110 && distanz > 50) {
        tone(buzzerPinRH, 523, 500);
    } else {
        tone(buzzerPinRH, 523, 100);
    }
    // Wartezeit vor der nächsten Messung
    delay(500);
}

void Beschleunigung () {
    //Platzhalter für die Beschleunigungsfunktion
}


