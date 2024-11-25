#include <Arduino.h>

int push = 0;       // Aktueller Zustand des Tasters
int lastpush = 0;   // Letzter Zustand des Tasters
bool schalter = false;  // Aktueller Zustand des Ausgangs (umschaltbar)

void setup()
{
    Serial.begin(9600);
    pinMode(2, INPUT);       // Eingang  2, Taster
    pinMode(12, OUTPUT);     // Ausgang 12, Parkhilfe
    pinMode(13, OUTPUT);     // Ausgang 13, Beschleunigung

    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
}


void loop() {
    void ummschalten ();

    if (digitalRead(12) == HIGH && digitalRead(13) == LOW) {
        void Parkhilfe ();
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
void Parkhilfe () {

}
void Beschleunigung () {}