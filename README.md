# projekt_mazda323

Mein Mazda323 Jg. 1990 erhält einige Upgrades. 
1. Das rückwärts Einparken soll mit zwei wasserdichten JSN-SR04T Ultraschallsensoren erleichtert werden. 
Mit einem Drucktaster kann die Parkhilfe eingeschaltet oder ausgeschaltet werden, wenn möglich wird die Parkhilfe beim rückwärts Fahren automatisch aktiviert. Ein akkustisches Signal soll, mit steigender oder sinkender Frequenz, den Abstand zum Hindernis angeben.
Die Sensoren werden Links und Rechts am Heck verbaut.
2. Die Innentemperatur, sowie die Beschleunigungskraft [G-Kraft] des Autos sollen während der Fahrt gemessen und angezeigt werden. Hierfür wird der Sensor Gy-521 verwendet, sowie ein geeignetes Ausgabegerät.


## Anforderungen
| Anforderungen      | Muss                | Wunsch              |Erfüllt             |
|:------------------ |:-------------------:| :------------------:|:------------------:|
|Arduino muss Signale der Sensoren empfangen.|x|||
|Arduino gibt Temperatur auf einem Bildschirm aus.|x|||
|Arduino gibt Beschleunigung in G auf einem Bildschirm aus.||x||
|Einparkhilfe mit Drucktaster ein- oder ausschalten|x|||
|Einparkhilfe wird aktiviert bei rückwärtsbewegung||x||
|Einparkhilfe meldet via Buzzer o.ä. akkustisch die Distanz|x|||
|Gemessene Distanz von Auto kann visuel (Balken o.ä.) auf einem Bildschirm angezeigt werden. ||x||
## Projektplan
- 01.12.2024 - Abgabe Projektplan
- 07.12.2024 - Kompatibilität von Hardware überprüft
- 07.12.2024 - Komponenten getestet
- 07.12.2024 - Flussdiagramm erstellt
- 24.12.2024 - Softwarecode geschrieben
- 31.12.2024 - Tests abgeschlossen
- 04.01.2025 - Projektabschluss (Video, Dokumentation, Präsentation)
## Flussdiagramm
In diesem Flussdiagramm wird das Auslesen und die Verzweigungen des Projekts aufgezeigt.

Funktionen:
- Gelb = Muss
- Türkis = Wunsch
![„arduino mazda323“ (2)](https://github.com/user-attachments/assets/a6a8830c-5227-48ca-9794-3d8871c89bba)
## Projektbeschreibung
### 1. Erstellung der Programmstruktur
#### 1.1 Funktionen definiert:
   - **umschalten()** : Funktion zum manuellen Umschalten von Beschleunigungsmesser auf Parkhilfe
   - **Parhilfe()** : jeweils LH/RH, Funktion zur Parkhilfe
     - **dinstance()** : jeweils LH/RH, Funktion zur Distanzmessung in regelmässigen Abständen
     - **buzzer()** : jeweils LH/RH, Funktion zur Steuerung des Buzzers basierend auf der Distanz
   - **Beschleunigung()** : Funktion zur G-Kräfte Messung
#### 1.2 Zuweisung Pin auf Arduino Board
#### 1.3 void setup() erstellt
### 2. Erestellung einzelner Funktionen
#### 2.1 void loop()
   - umschalten wird aufgerufen:
     - Überwacht den Taster und wechselt den Zustand von schalter (true/false), wenn der Taster gedrückt wird.
   - Verzweigung basierend auf schalter:
      - Wenn der Schalter aktiv (true) ist:
        - Die Funktionen ParkhilfeLH und ParkhilfeRH werden aufgerufen, um die Parkhilfen zu steuern.
      - Wenn der Schalter inaktiv (false) ist:
        - Die Funktion Beschleunigung wird aufgerufen, um die Beschleunigungsfunktion zu steuern.
#### 2.2 umschalten()
Das Programm sorgt dafür, dass durch jeden Tastendruck der Zustand der Ausgänge zuverlässig gewechselt wird, ohne durch Prellen gestört zu werden.

   Die Funktion umschalten() hat folgende Aufgaben:
   - Sie liest den Zustand eines Tasters ein, der mit dem Pin tasterPin verbunden ist.
   - Sie erkennt einen Tastendruck, indem sie prüft, ob der Taster von LOW nach HIGH wechselt.
   - Sie schaltet einen logischen Schalter (schalter) um.
   - Sie steuert zwei Ausgänge (beschleunigungPin und parkPin) basierend auf dem Zustand des Schalters:
     - Einer der Ausgänge wird auf HIGH gesetzt, während der andere auf LOW bleibt.
   - Sie verwendet eine kurze Verzögerung (50 Millisekunden), um den Taster zu entprellen und unerwünschte Zustandswechsel zu verhindern.

#### 2.3 Parhilfe()
Diese Funktion ruft die Funktionen distance() und buzzer() auf:
#### 2.3.1 distance()
   Die Distanz wird nur alle 500 ms gemessen, indem millis() genutzt wird. Die Steuerung des Buzzers erfolgt unabhängig von der Messung und bleibt kontinuierlich aktiv.
   Dank der Verwendung von millis() wird die Hauptschleife (loop) nicht blockiert, und der Buzzer kann kontinuierlich betrieben werden.
   
   - Zeitintervall überprüfen: Die Funktion führt eine Messung nur aus, wenn das festgelegte Zeitintervall seit der letzten Messung abgelaufen ist.
   - Trigger-Signal senden: Der Ultraschallsensor wird aktiviert, um einen Schallimpuls auszusenden.
   - Echo-Zeit messen: Die Zeit wird gemessen, die das Echo benötigt, um zurückzukehren.
   - Fehlerbehandlung: Wenn kein Echo empfangen wird, wird ein ungültiger Wert (-1) zugewiesen.
   - Distanz berechnen: Die gemessene Zeit wird in eine Distanz in Zentimetern umgerechnet.
   - Ausgabe: Die berechnete Distanz wird auf der seriellen Konsole angezeigt.
#### 2.3.2 buzzer()
   Durch diese Logik wird ein akustisches Feedback erzeugt, das die Distanz zur gemessenen Oberfläche repräsentiert.
   
   - Die Funktion passt das Intervall für den Buzzer-Ton an, basierend auf der gemessenen Distanz:
     - Grosse Distanz → Langsame Signale (1000 ms).
     - Mittlere Distanz → Mittelschnelle Signale (500 ms).
     - Kleine Distanz → Schnelle Signale (200 ms).
   - Wenn keine gültige Distanz gemessen wird (z. B. kein Echo), wird der Buzzer ausgeschaltet.
#### 2.4 Beschleunigung()

- Welche Teilfunktionen habt ihr einzeln getestet?
- Gab es dabei Probleme, wenn ja welche?
- Konnten alle Muss-Anforderungen umgesetzt werden?
- Nein, warum konnten nicht alle umgesetzt werden?
- War etwas unerwartet beim Schreiben des Programcodes?
## Parts
- 1x Arduino UNO R3
- 1x Breadboard
- 2x JSN-SR04T Ultrasonic Sensor
- 1x MPU6050 Beschleunigungssensor
- 1x Drucktaster
- 2x Piezo-Buzzer oder Lautsprecher
- 1x Visuelles Ausgabegerät, z.B LCD-Bildschirm
## 1. Tests
### 1.1 Vorgehen: 
1. Testen einzelner Funktionen in einem anderen Branch.
2. erfolgreich gestestete Funktion in Main Branch einfliessen lassen
3. Main Branch testen
### 1.2 Test Funktion Parkhilfe() in Branch ultrasonic
#### 1. Version des Codes
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

- Was sind die Testscenarios?
- Beschreibung oder Bild das Testziel aufzeigt
- Gibt es spezielle Gründe warum diese Tests gewählt wurden?
