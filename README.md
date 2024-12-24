# projekt_mazda323
## 1. Kurzbeschrieb
Mein Mazda323 Jg. 1990 erhält einige Upgrades. 
1. Das rückwärts Einparken soll mit zwei wasserdichten JSN-SR04T Ultraschallsensoren erleichtert werden. 
Mit einem Drucktaster kann die Parkhilfe eingeschaltet oder ausgeschaltet werden, wenn möglich wird die Parkhilfe beim rückwärts Fahren automatisch aktiviert. Ein akkustisches Signal soll, mit steigender oder sinkender Frequenz, den Abstand zum Hindernis angeben.
Die Sensoren werden Links und Rechts am Heck verbaut.
2. Die Innentemperatur, sowie die Beschleunigungskraft [G-Kraft] des Autos sollen während der Fahrt gemessen und angezeigt werden. Hierfür wird der Sensor Gy-521 verwendet, sowie ein geeignetes Ausgabegerät.


## 2. Anforderungen
| Anforderungen      | Muss                | Wunsch              |Erfüllt             |
|:------------------ |:-------------------:| :------------------:|:------------------:|
|Arduino muss Signale der Sensoren empfangen.|x|||
|Arduino gibt Temperatur auf einem Bildschirm aus.|x|||
|Arduino gibt Beschleunigung in G auf einem Bildschirm aus.||x||
|Einparkhilfe mit Drucktaster ein- oder ausschalten|x|||
|Einparkhilfe wird aktiviert bei rückwärtsbewegung||x||
|Einparkhilfe meldet via Buzzer o.ä. akkustisch die Distanz|x|||
|Gemessene Distanz von Auto kann visuel (Balken o.ä.) auf einem Bildschirm angezeigt werden. ||x||
## 3. Projektplan
- 01.12.2024 - Abgabe Projektplan
- 07.12.2024 - Kompatibilität von Hardware überprüft
- 07.12.2024 - Komponenten getestet
- 07.12.2024 - Flussdiagramm erstellt
- 24.12.2024 - Softwarecode geschrieben
- 31.12.2024 - Tests abgeschlossen
- 04.01.2025 - Projektabschluss (Video, Dokumentation, Präsentation)
## 4. Flussdiagramm
In diesem Flussdiagramm wird das Auslesen und die Verzweigungen des Projekts aufgezeigt.

Funktionen:
- Gelb = Muss
- Türkis = Wunsch
![„arduino mazda323“ (2)](https://github.com/user-attachments/assets/a6a8830c-5227-48ca-9794-3d8871c89bba)
## 5. Parts
- 1x Arduino UNO R3
- 1x Breadboard
- 2x JSN-SR04T Ultrasonic Sensor
- 1x MPU6050 Beschleunigungssensor
- 1x Drucktaster
- 2x Piezo-Buzzer oder Lautsprecher
- 1x Visuelles Ausgabegerät, z.B LCD-Bildschirm
## 6. Projektbeschreibung
### 6.1 Erstellung der Programmstruktur
#### 6.1.1 Funktionen definiert:
   - **umschalten()** : Funktion zum manuellen Umschalten von Beschleunigungsmesser auf Parkhilfe
   - **Parhilfe()** : jeweils LH/RH, Funktion zur Parkhilfe
     - **dinstance()** : jeweils LH/RH, Funktion zur Distanzmessung in regelmässigen Abständen
     - **buzzer()** : jeweils LH/RH, Funktion zur Steuerung des Buzzers basierend auf der Distanz
   - **Beschleunigung()** : Funktion zur G-Kräfte Messung
#### 6.1.2 Zuweisung Pin auf Arduino Board
#### 6.1.3 void setup() erstellt
### 6.2. Erestellung einzelner Funktionen
#### 6.2.1 void loop()
   - umschalten wird aufgerufen:
     - Überwacht den Taster und wechselt den Zustand von schalter (true/false), wenn der Taster gedrückt wird.
   - Verzweigung basierend auf schalter:
      - Wenn der Schalter aktiv (true) ist:
        - Die Funktionen ParkhilfeLH und ParkhilfeRH werden aufgerufen, um die Parkhilfen zu steuern.
      - Wenn der Schalter inaktiv (false) ist:
        - Die Funktion Beschleunigung wird aufgerufen, um die Beschleunigungsfunktion zu steuern.
#### 6.2.2 umschalten()
Das Programm sorgt dafür, dass durch jeden Tastendruck der Zustand der Ausgänge zuverlässig gewechselt wird, ohne durch Prellen gestört zu werden.

   Die Funktion umschalten() hat folgende Aufgaben:
   - Sie liest den Zustand eines Tasters ein, der mit dem Pin tasterPin verbunden ist.
   - Sie erkennt einen Tastendruck, indem sie prüft, ob der Taster von LOW nach HIGH wechselt.
   - Sie schaltet einen logischen Schalter (schalter) um.
   - Sie steuert zwei Ausgänge (beschleunigungPin und parkPin) basierend auf dem Zustand des Schalters:
     - Einer der Ausgänge wird auf HIGH gesetzt, während der andere auf LOW bleibt.
   - Sie verwendet eine kurze Verzögerung (50 Millisekunden), um den Taster zu entprellen und unerwünschte Zustandswechsel zu verhindern.

#### 6.2.3 Parhilfe()
Diese Funktion ruft die Funktionen distance() und buzzer() auf:
#### 6.2.3.1 distance()
   Die Distanz wird nur alle 500 ms gemessen, indem millis() genutzt wird. Die Steuerung des Buzzers erfolgt unabhängig von der Messung und bleibt kontinuierlich aktiv.
   Dank der Verwendung von millis() wird die Hauptschleife (loop) nicht blockiert, und der Buzzer kann kontinuierlich betrieben werden.
   
   - Zeitintervall überprüfen: Die Funktion führt eine Messung nur aus, wenn das festgelegte Zeitintervall seit der letzten Messung abgelaufen ist.
   - Trigger-Signal senden: Der Ultraschallsensor wird aktiviert, um einen Schallimpuls auszusenden.
   - Echo-Zeit messen: Die Zeit wird gemessen, die das Echo benötigt, um zurückzukehren.
   - Fehlerbehandlung: Wenn kein Echo empfangen wird, wird ein ungültiger Wert (-1) zugewiesen.
   - Distanz berechnen: Die gemessene Zeit wird in eine Distanz in Zentimetern umgerechnet.
   - Ausgabe: Die berechnete Distanz wird auf der seriellen Konsole angezeigt.
#### 6.2.3.2 buzzer()
   Durch diese Logik wird ein akustisches Feedback erzeugt, das die Distanz zur gemessenen Oberfläche repräsentiert.
   
   - Die Funktion passt das Intervall für den Buzzer-Ton an, basierend auf der gemessenen Distanz:
     - Grosse Distanz → Langsame Signale (1000 ms).
     - Mittlere Distanz → Mittelschnelle Signale (500 ms).
     - Kleine Distanz → Schnelle Signale (200 ms).
   - Wenn keine gültige Distanz gemessen wird (z. B. kein Echo), wird der Buzzer ausgeschaltet.
#### 6.2.4 Beschleunigung()

- Welche Teilfunktionen habt ihr einzeln getestet?
- Gab es dabei Probleme, wenn ja welche?
- Konnten alle Muss-Anforderungen umgesetzt werden?
- Nein, warum konnten nicht alle umgesetzt werden?
- War etwas unerwartet beim Schreiben des Programcodes?
## 7. Tests
### 7.1 Vorgehen: 
1. Testen einzelner Funktionen in einem anderen Branch.
2. erfolgreich gestestete Funktion in Main Branch einfliessen lassen
3. Main Branch testen
### 7.1.2 Test Funktion Parkhilfe() in Branch ultrasonic, nur LH
#### Version des Codes
```
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
```
#### Problemstellung
Zuerst wurde die Distanzmessung sowie der Buzzer in derselben Funktion programmiert.
Jedoch ergab sich das Problem, dass nach jeder Verzögerung (delay) der Ton auch pausiert wird. 
Die Überschneidung der Speildauer des Tons und der Verzögerung am Ende der Funktion erzeugt einen abgehacktn Ton und er wird nicht kontinuierlich gespielt, währenddem die Messung durchgeführt wird.
#### Lösung
1. Die Funktion Parkhilfe wird in zwei Unterfunktionen aufgeteilt.
   - dinstance(): Funktion zur Distanzmessung in regelmässigen Abständen
   - buzzer(): Funktion zur Steuerung des Buzzers basierend auf der Distanz
3. Verwendet millis() für das Timing anstelle von delay(). Dadurch wird sichergestellt, dass die Buzzer-Steuerung den Rest des Programms nicht blockiert. Durch den Verzicht auf delay()
   kann das Programm die Abstandsmessung und die Ausführung anderer Aufgaben fortsetzen, ohne blockiert zu werden.
   
#### 2. Version des Codes
```
// Funktion zur Distanzmessung in regelmässigen Abständen
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

// Funktion zur Steuerung des Buzzer basierend auf der Distanz
void buzzerLH() {
    int interval = 1000; // Standardintervall für den Ton

    // Tonintervall basierend auf der gemessenen Distanz bestimmen
    if (distanzLH < 0) { // Ungültige Distanz
        noTone(buzzerPinLH); // Buzzer ausschalten
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
```
#### Problem
Code funktioniert nicht, der Buzzer gibt einen kontinuierlichen Ton aus.
Die Methode tone() erzeugt den Ton, aber die Dauer (interval) ist nur die Zeit, für die der Ton gespielt wird, nicht die Pause zwischen den Tönen. Es gibt keinen Mechanismus, um den Ton zu stoppen oder eine Pause einzufügen, bevor die nächste Distanzmessung erfolgt.
#### Lösung
Um das gewünschte Verhalten zu erzielen (abwechselndes Ein- und Ausschalten des Buzzers basierend auf der Distanz), muss ein Mechanismus eingeführt werden, der den Ton nach einer bestimmten Zeit ausschaltet und dann eine Pause macht.
Mit einer buzzerOn-Logik wird der Zustand des Buzzers (ein/aus) durch die Variable buzzerOn gesteuert.
Bei jeder Änderung wird der Zustand umgeschaltet.
Jetzt funktioniert die Distanzmessung, sowie die Buzzer ausgabe zumindest für jeweils eine Seite.
### 7.1.3 Test Funktion Parkhilfe() in Branch ultrasonic, LH und RH zusammen
#### 1. Version des Codes
```
// Globale Variablen für die linke Seite
float distanzLH = 0;                      // Speichert die gemessene Distanz (links)
unsigned long lastMeasurementTimeLH = 0;  // Speichert die Zeit des letzten Messvorgangs (links)
unsigned long lastBuzzerToggleLH = 0;     // Speichert die Zeit der letzten Buzzer-Aktivierung (links)
bool buzzerOnLH = false;                  // Zustand des Buzzers (links)

// Globale Variablen für die rechte Seite
float distanzRH = 0;                      // Speichert die gemessene Distanz (rechts)
unsigned long lastMeasurementTimeRH = 0;  // Speichert die Zeit des letzten Messvorgangs (rechts)
unsigned long lastBuzzerToggleRH = 0;     // Speichert die Zeit der letzten Buzzer-Aktivierung (rechts)
bool buzzerOnRH = false;                  // Zustand des Buzzers (rechts)

// Gemeinsame Konfiguration
unsigned long measurementInterval = 500; // Zeitintervall zwischen den Messungen in Millisekunden

// Funktionsprototypen
void distanceLH(); // Funktion zur Distanzmessung (links)
void buzzerLH();   // Funktion zur Steuerung des Buzzer (links)
void distanceRH(); // Funktion zur Distanzmessung (rechts)
void buzzerRH();   // Funktion zur Steuerung des Buzzer (rechts)

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

// Funktion zur Steuerung des Buzzer (links)
void buzzerLH() {
    int interval = 1000;

    if (distanzLH < 0) {
        noTone(buzzerPinLH);
        buzzerOnLH = false;
        return;
    } else {
        interval = (0.11 * (distanzLH * distanzLH) + 50);
    }

    if (millis() - lastBuzzerToggleLH >= interval) {
        lastBuzzerToggleLH = millis();

        if (buzzerOnLH) {
            noTone(buzzerPinLH);
        } else {
            tone(buzzerPinLH, 523);
        }

        buzzerOnLH = !buzzerOnLH;
    }
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

// Funktion zur Steuerung des Buzzer (rechts)
void buzzerRH() {
    int interval = 1000;

    if (distanzRH < 0) {
        noTone(buzzerPinRH);
        buzzerOnRH = false;
        return;
    } else {
        interval = (0.11 * (distanzRH * distanzRH) + 50);
    }

    if (millis() - lastBuzzerToggleRH >= interval) {
        lastBuzzerToggleRH = millis();

        if (buzzerOnRH) {
            noTone(buzzerPinRH);
        } else {
            tone(buzzerPinRH, 523);
        }

        buzzerOnRH = !buzzerOnRH;
    }
}
```
#### Problem
Buzzer von LH und RH funktionieren, jedoch blockieren sie sich gegenseitig, sodass jeweils nur LH oder RH einen Ton ausgibt.
Das Problem liegt in der Verwendung von tone() und noTone() für die Steuerung der Buzzer. Beide Funktionen arbeiten mit demselben Timer. Wenn eine Seite den Timer verwendet, blockiert dies die Steuerung des anderen Buzzers, da beide auf denselben Timer zugreifen möchten.
#### Lösung
Um das gleichzeitige Abspielen von Tönen auf zwei Lautsprechern zu ermöglichen, können wir statt der tone()-Funktion die Digitalen Ausgänge direkt ansteuern. Dadurch lassen sich unabhängig voneinander unterschiedliche Dauer des Tones erzeugen, ohne dass die Buzzer sich gegenseitig blockieren. Jedoch lässt sich die Frequenz nicht einstellen.
#### 2. Version des Codes
```
#include <Arduino.h>

// Pins für die linke Seite
#define trigPinLH 6   // Pin für den Trigger des Ultraschallsensors (links)
#define echoPinLH 7   // Pin für das Echo des Ultraschallsensors (links)
#define buzzerPinLH 10 // Pin für den Buzzer (links)(analog)

// Pins für die rechte Seite
#define trigPinRH 8   // Pin für den Trigger des Ultraschallsensors (rechts)
#define echoPinRH 9   // Pin für das Echo des Ultraschallsensors (rechts)
#define buzzerPinRH 11 // Pin für den Buzzer (rechts)(analog)

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

// Funktionsprototypen
void distanceLH(); // Funktion zur Distanzmessung (links)
void buzzerLH(unsigned long intervalLH); // Tonsteuerung für Buzzer (links)
void distanceRH(); // Funktion zur Distanzmessung (rechts)
void buzzerRH(unsigned long intervalRH); // Tonsteuerung für Buzzer (rechts)

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
    if (distanzLH > 0 && distanzLH < 300) { // Nur innerhalb eines gültigen Bereichs 0-3m
        unsigned long intervalLH = (0.11 * (distanzLH * distanzLH) + 50); // quadratische Funktion, so dass die Dauer(Intervall) des Ton mit abnehmender Distanz kürzer wird
        //map() kann auch eingefügt werden. map(distanz, 1, 200, 50, 1000) skaliert die Distanz auf eine Tonlänge zwischen 50 ms und 1000 ms.
        buzzerLH(intervalLH); // Aktiviert den Buzzer mit dem berechneten Intervall.
    } else {
        digitalWrite(buzzerPinLH, LOW); // Buzzer ausschalten, wenn Distanz ausserhalb des Bereichs
    }

    // Rechte Seite
    distanceRH();
    if (distanzRH > 0 && distanzRH < 300) { 
        unsigned long intervalRH = (0.11 * (distanzRH * distanzRH) + 50);
        
        buzzerRH(intervalRH); 
    } else {
        digitalWrite(buzzerPinRH, LOW); 
    }
}

// Funktion zur Distanzmessung (links)
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

// Funktion zur Tonsteuerung (links)
void buzzerLH(unsigned long intervalLH) { //intervalLH: Zeitdauer in Millisekunden, wie lange der Buzzer an- oder ausgeschaltet bleiben soll.
    static bool buzzerStateLH = false; // Aktueller Zustand des Buzzers (links), Initialwert: false (Buzzer ist zu Beginn ausgeschaltet).

    //millis(): gibt die Zeit seit dem Start des Mikrocontrollers in Millisekunden zurück.
    //aktuelle Zeit (millis()) - letzte Aktivierungszeit (previousBuzzerTimeLH) >= dem angegebenen Intervall (intervalLH)
    //wenn ja, ist es Zeit, den Zustand des Buzzers zu wechseln (An oder Aus).
    if (millis() - previousBuzzerTimeLH >= intervalLH) {
        previousBuzzerTimeLH = millis(); // Zeit aktualisieren, speichert den Zeitpunkt der letzten Buzzer-Aktivierung.
        buzzerStateLH = !buzzerStateLH; // Buzzer-Zustand wird invertiert
        digitalWrite(buzzerPinLH, buzzerStateLH ? HIGH : LOW); //Ternäre Bedingung: buzzerStateLH == true: HIGH, buzzerStateLH == false: LOW
    }
}

// Funktion zur Distanzmessung (rechts)
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

// Funktion zur Tonsteuerung (rechts)
void buzzerRH(unsigned long intervalRH) { 
    static bool buzzerStateRH = false; 

    if (millis() - previousBuzzerTimeRH >= intervalRH) {
        previousBuzzerTimeRH = millis(); 
        buzzerStateRH = !buzzerStateRH; 
        digitalWrite(buzzerPinRH, buzzerStateRH ? HIGH : LOW);
    }
}
```
#### Problem
Die Frequenz, also Tonhöhe lässt sich nicht einstellen. Die Funktion ist jedoch gegeben, das Geräusch ist einfach nicht sehr angenehm. Gerne möchte ich ein C4 erzeugen, welches eine Frequenz von 261 Hz hat. 
#### Lösung
Da die tone()-Funktion nicht verwendet werden darf, muss man die HIGH- und LOW-Zustände des Buzzers genauer steuern, um eine Rechteckwelle mit der gewünschten Frequenz zu erhalten.
Quelle: BLACKBOXAI
Um den Buzzer in der Frequenz C4 (261,63 Hz) zu betreiben, können wir die Funktion analogWrite() verwenden, um ein PWM-Signal zu erzeugen. Da die Frequenz von analogWrite() auf den meisten Arduino-Boards (wie dem Uno) standardmäßig 490 Hz beträgt, können wir die PWM-Duty-Cycle anpassen, um den gewünschten Ton zu erzeugen.

Somit habe ich nun in der Buzzer Funktion den digitalWrite() zu analogWrite() geändert. 

Quelle BLACKBOXAI:
Wenn buzzerStateRH true ist (Buzzer ein), wird 128 übergeben, was etwa 50% der maximalen PWM-Leistung entspricht (auf einem 8-Bit-PWM-System, wo 255 die maximale Leistung ist).

So funktioniert die Anwendung mit jeweils eigenem Intervall und einer einstellbaren Frequenz.

Bsp. Code
```
void buzzerRH(unsigned long intervalRH) {
    static bool buzzerStateRH = false;

    if (millis() - previousBuzzerTimeRH >= intervalRH) {
        previousBuzzerTimeRH = millis();
        buzzerStateRH = !buzzerStateRH;
        analogWrite(buzzerPinRH, buzzerStateRH ? 128 : LOW);
    }
}
```
#### 3. Version des Codes (Final)
```
#include <Arduino.h>

// Pins für die linke Seite
#define trigPinLH 6   // Pin für den Trigger des Ultraschallsensors (links)
#define echoPinLH 7   // Pin für das Echo des Ultraschallsensors (links)
#define buzzerPinLH 10 // Pin für den Buzzer (links)(analog)

// Pins für die rechte Seite
#define trigPinRH 8   // Pin für den Trigger des Ultraschallsensors (rechts)
#define echoPinRH 9   // Pin für das Echo des Ultraschallsensors (rechts)
#define buzzerPinRH 11 // Pin für den Buzzer (rechts)(analog)

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

// Funktionsprototypen
void distanceLH(); // Funktion zur Distanzmessung (links)
void buzzerLH(unsigned long intervalLH); // Tonsteuerung für Buzzer (links)
void distanceRH(); // Funktion zur Distanzmessung (rechts)
void buzzerRH(unsigned long intervalRH); // Tonsteuerung für Buzzer (rechts)

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
    if (distanzLH > 0 && distanzLH < 300) { // Nur innerhalb eines gültigen Bereichs 0-3m
        unsigned long intervalLH = (0.11 * (distanzLH * distanzLH) + 50); // quadratische Funktion, so dass die Dauer(Intervall) des Ton mit abnehmender Distanz kürzer wird
        //map() kann auch eingefügt werden. map(distanz, 1, 200, 50, 1000) skaliert die Distanz auf eine Tonlänge zwischen 50 ms und 1000 ms.
        buzzerLH(intervalLH); // Aktiviert den Buzzer mit dem berechneten Intervall.
    } else {
        analogWrite(buzzerPinLH, LOW); // Buzzer ausschalten, wenn Distanz ausserhalb des Bereichs
    }

    // Rechte Seite
    distanceRH();
    if (distanzRH > 0 && distanzRH < 300) {
        unsigned long intervalRH = (0.11 * (distanzRH * distanzRH) + 50);

        buzzerRH(intervalRH);
    } else {
        analogWrite(buzzerPinRH, LOW);
    }
}

// Funktion zur Distanzmessung (links)
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

// Funktion zur Tonsteuerung (links)
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

// Funktion zur Distanzmessung (rechts)
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

// Funktion zur Tonsteuerung (rechts)
void buzzerRH(unsigned long intervalRH) {
    static bool buzzerStateRH = false;

    if (millis() - previousBuzzerTimeRH >= intervalRH) {
        previousBuzzerTimeRH = millis();
        buzzerStateRH = !buzzerStateRH;
        analogWrite(buzzerPinRH, buzzerStateRH ? 128 : LOW);
    }
}
```

```
Code
```
- Was sind die Testscenarios?
- Beschreibung oder Bild das Testziel aufzeigt
- Gibt es spezielle Gründe warum diese Tests gewählt wurden?
