#include <Arduino.h>

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

    digitalWrite(12, LOW);
    digitalWrite(13, LOW);

    pinMode(trigPinLH, INPUT);      // Eingang 6, TriggerLH
    pinMode(echoPinLH, INPUT);      // Eingang 7, EchoLH
    pinMode(trigPinRH, INPUT);      // Eingang 8, TriggerRH
    pinMode(trigPinLH, INPUT);      // Eingang 9, EchoRH
    pinMode(buzzerPinLH, OUTPUT);   // Ausgang 10, Buzzer LH
    pinMode(buzzerPinRH, OUTPUT);   // Ausgang 11, Buzzer RH

    tone(buzzerPinLH, 1000, 2000);
    tone(buzzerPinRH, 1000, 2000);
}


void loop() {
    void ummschalten ();

    if (digitalRead(12) == HIGH && digitalRead(13) == LOW) {
        void ParkhilfeLH ();
        void ParkhilfeRH ();
    }
    else {
        void Beschleunigung ();
    }
}

void ummschalten () {
    // Umschalttaste auslesen
    push = digitalRead(2);

    // Prüfen, ob der Taster gedrückt wurde (Zustandsänderung von LOW nach HIGH)
    if (push == HIGH && lastpush == LOW)
    {
        // Taster-Zustand umkehren
        schalter = !schalter;

        // Zustand der Ausgänge basierend auf dem Taster-Zustand setzen
        if (schalter) {
            digitalWrite(13, HIGH); // Ausgang 13 HIGH
            digitalWrite(12, LOW);  // Ausgang 12 LOW
        } else {
            digitalWrite(13, LOW);  // Ausgang 13 LOW
            digitalWrite(12, HIGH); // Ausgang 12 HIGH
        }
    }

    // Verzögerung für Entprellung
    delay(50);

    // Aktuellen Zustand des Tasters speichern
    lastpush = push;

}

void ParkhilfeLH () {
    float zeit, distanz;

    digitalWrite(trigPinLH, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPinLH, HIGH); //soundwave wird gesendet für 10ms
    delayMicroseconds(10);
    digitalWrite(trigPinLH, LOW);

    zeit = pulseIn(echoPinLH, HIGH); //liest echoPin und misst die Zeit zwischen HIGH LOW und wieder HIGH
    distanz = (zeit / 2) * 0.0344; // Distanz in cm, sound umgerechnet von m/s in cm/µs (0.0344 cm/µs)

    Serial.print("Distanz = ");
    Serial.print(distanz);
    Serial.println(" cm");
    delay(500);

    if (distanz >= 110) {
        tone(buzzerPinLH, 523); // C4
        delay(1000);}
    else if (distanz < 110 && distanz > 50) {
        tone(buzzerPinLH, 523);
        delay(500);}
    else {
        tone(buzzerPinLH, 523);
        delay(100);

    }
}

void ParkhilfeRH () {
    float zeit, distanz;

    digitalWrite(trigPinRH, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPinRH, HIGH); //soundwave wird gesendet für 10ms
    delayMicroseconds(10);
    digitalWrite(trigPinRH, LOW);

    zeit = pulseIn(echoPinRH, HIGH); //liest echoPin und misst die Zeit zwischen HIGH LOW und wieder HIGH
    distanz = (zeit / 2) * 0.0344; // Distanz in cm, sound umgerechnet von m/s in cm/µs (0.0344 cm/µs)

    Serial.print("Distanz = ");
    Serial.print(distanz);
    Serial.println(" cm");
    delay(500);

    if (distanz >= 110) {
        tone(buzzerPinRH, 523); // C4
        delay(1000);}
    else if (distanz < 110 && distanz > 50) {
        tone(buzzerPinRH, 523);
        delay(500);}
    else {
        tone(buzzerPinRH, 523);
        delay(100);

    }
}

void Beschleunigung () {}
