#include <Arduino.h>
#include <Wire.h>


void ParkhilfeLH();

#define trigPinLH 6
#define echoPinLH 7
//#define buzzerPinLH 10



void setup() {
    Serial.begin(9600);

    pinMode(trigPinLH, OUTPUT);     // Ausgang 6, TriggerLH
    pinMode(echoPinLH, INPUT);      // Eingang 7, EchoLH
    //pinMode(buzzerPinLH, OUTPUT);   // Ausgang 10, Buzzer LH

    //tone(buzzerPinLH, 1000, 2000);
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
        //noTone(buzzerPinLH); // Buzzer ausschalten, falls aktiv
        return;
    }

    //Distanz berechnen in cm, sound umgerechnet von m/s in cm/µs (0.0344 cm/µs)
    distanz = (zeit / 2) * 0.0344;

    Serial.print("Distanz LH = ");
    Serial.print(distanz);
    Serial.println(" cm");
/*
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
    */
}