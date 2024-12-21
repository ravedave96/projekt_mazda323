# projekt_mazda323

Mein Mazda323 Jg. 1990 erhält einige Upgrades. 
1. Das rückwärts Einparken soll mit zwei wasserdichten JSN-SR04T Ultraschallsensoren erleichtert werden. 
Mit einem Drucktaster kann die Parkhilfe eingeschaltet oder ausgeschaltet werden, wenn möglich wird die Parkhilfe beim rückwärts Fahren automatisch aktiviert. Ein akkustisches Signal soll, mit steigender oder sinkender Frequenz, den Abstand zum Hindernis angeben.
Die Sensoren werden Links und Rechts am Heck verbaut.
2. Die Innentemperatur, sowie die Beschleunigungskraft [G-Kraft] des Autos sollen während der Fahrt gemessen und angezeigt werden. Hierfür wird der Sensor Gy-521 verwendet, sowie ein geeignetes Ausgabegerät.


## Anforderungen
| Anforderungen      | Muss                | Wunsch              |
|:------------------ |:-------------------:| :------------------:|
|Arduino muss Signale der Sensoren empfangen.|x||
|Arduino gibt Temperatur auf einem Bildschirm aus.|x||
|Arduino gibt Beschleunigung in G auf einem Bildschirm aus.||x|
|Einparkhilfe mit Drucktaster ein- oder ausschalten|x||
|Einparkhilfe wird aktiviert bei rückwärtsbewegung||x|
|Einparkhilfe meldet via Buzzer o.ä. akkustisch die Distanz|x||
|Gemessene Distanz von Auto kann visuel (Balken o.ä.) auf einem Bildschirm angezeigt werden. ||x|


## Projektplan
- 01.12.2024 - Abgabe Projektplan
- 07.12.2024 - Kompatibilität von Hardware überprüft
- 07.12.2024 - Komponenten getestet
- 07.12.2024 - Flussdiagramm erstellt
- 14.12.2024 - Softwarecode geschrieben
- 21.12.2024 - Tests abgeschlossen
- 04.01.2025 - Projektabschluss (Video, Dokumentation, Präsentation)

## Flussdiagramm
In diesem Flussdiagramm wird das Auslesen und die Verzweigungen des Projekts aufgezeigt.

Funktionen:
- Gelb = Muss
- Türkis = Wunsch
![„arduino mazda323“ (2)](https://github.com/user-attachments/assets/a6a8830c-5227-48ca-9794-3d8871c89bba)

## Projektbeschreibung
### Erstellung der Programmstruktur
1. Funktionen definiert:
   - umschalten() : Funktion zum manuellen Umschalten von Beschleunigungsmesser auf Parkhilfe
   - Parhilfe() : jeweils LH/RH, Funktion zur Parkhilfe
     - dinstance() : jeweils LH/RH, Funktion zur Distanzmessung in regelmässigen Abständen
     - buzzer() : jeweils LH/RH, Funktion zur Steuerung des Buzzers basierend auf der Distanz
   - Beschleunigung() : Funktion zur G-Kräfte Messung
2. Zuweisung Pin auf Arduino Board
3. void setup() erstellt
   
### Erestellung einzelner Funktionen
1. void loop()
   - umschalten wird aufgerufen:
     - Überwacht den Taster und wechselt den Zustand von schalter (true/false), wenn der Taster gedrückt wird.
   - Verzweigung basierend auf schalter:
      - Wenn der Schalter aktiv (true) ist:
        - Die Funktionen ParkhilfeLH und ParkhilfeRH werden aufgerufen, um die Parkhilfen zu steuern.
      - Wenn der Schalter inaktiv (false) ist:
        - Die Funktion Beschleunigung wird aufgerufen, um die Beschleunigungsfunktion zu steuern.

3. umschalten()
   Die Funktion umschalten() hat folgende Aufgaben:

Sie liest den Zustand eines Tasters ein, der mit dem Pin tasterPin verbunden ist.
Sie erkennt einen Tastendruck, indem sie prüft, ob der Taster von LOW nach HIGH wechselt.
Sie schaltet einen logischen Schalter (schalter) um.
Sie steuert zwei Ausgänge (beschleunigungPin und parkPin) basierend auf dem Zustand des Schalters:
Einer der Ausgänge wird auf HIGH gesetzt, während der andere auf LOW bleibt.
Sie verwendet eine kurze Verzögerung (50 Millisekunden), um den Taster zu entprellen und unerwünschte Zustandswechsel zu verhindern.
Das Programm sorgt dafür, dass durch jeden Tastendruck der Zustand der Ausgänge zuverlässig gewechselt wird, ohne durch Prellen gestört zu werden.

5. Parhilfe()
6. Beschleunigung()

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

## Tests
Kurze Beschreibung der Vorgehensweise

- Was sind die Testscenarios?
- Beschreibung oder Bild das Testziel aufzeigt
- Gibt es spezielle Gründe warum diese Tests gewählt wurden?
