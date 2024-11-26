#include <Arduino.h>
#include <Wire.h>

void ummschalten();
void ParkhilfeLH();
void ParkhilfeRH();
void Beschleunigung();

int push = 0;       // aktueller Zustand des Tasters
int lastpush = 0;   // letzter Zustand des Tasters
bool schalter = false;  // aktueller Zustand des Ausgangs (umschaltbar)

#define tasterPin 2
#define parkPin 12
#define beschleunigungPin 13

#define trigPinLH 6
#define echoPinLH 7
#define trigPinRH 8
#define echoPinRH 9
#define buzzerPinLH 10
#define buzzerPinRH 11

void setup() {
    Serial.begin(9600);

    pinMode(tasterPin, INPUT);      // Eingang  2, Taster
    pinMode(parkPin, OUTPUT);    // Ausgang 12, Parkhilfe
    pinMode(beschleunigungPin, OUTPUT);    // Ausgang 13, Beschleunigung

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
    ummschalten();

    if (schalter) {
        ParkhilfeLH();
        ParkhilfeRH();
    } else {
        Beschleunigung();
    }
}

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

void ParkhilfeLH () {
    float zeit = 0, distanz = 0;

    // Trigger-Signal erzeugen
    digitalWrite(trigPinLH, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinLH, HIGH); // Ultraschallsignal senden
    delayMicroseconds(10);
    digitalWrite(trigPinLH, LOW);

    // Echo-Zeit messen
    zeit = pulseIn(echoPinLH, HIGH, 30000); //liest echoPin und misst die Zeit zwischen HIGH LOW und wieder HIGH
    if (zeit == 0) {
        // Keine valide Messung, Rückkehr aus der Funktion
        Serial.println("Kein Echo empfangen");
        noTone(buzzerPinLH); // Buzzer ausschalten, falls aktiv
        return;
    }

    //Distanz berechnen in cm, sound umgerechnet von m/s in cm/µs (0.0344 cm/µs)
    distanz = (zeit / 2) * 0.0344;

    Serial.print("Distanz LH = ");
    Serial.print(distanz);
    Serial.println(" cm");

    // Audio Distanz Signal
    if (distanz >= 110) {
        tone(buzzerPinLH, 523, 1000); // C4
    } else if (distanz < 110 && distanz > 50) {
        tone(buzzerPinLH, 523, 500);
    } else {
        tone(buzzerPinLH, 523, 100);
    }
    // Wartezeit vor der nächsten Messung
    delay(500);
}

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
