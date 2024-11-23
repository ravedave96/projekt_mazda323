# projekt_mazda323

Mein Mazda323 Jg. 1990 erhält einige Upgrades. 
1. Das rückwärts Einparken soll mit zwei wasserdichten JSN-SR04T Ultraschallsensoren erleichtert werden. 
Mit einem Drucktaster kann die Hilfe eingeschaltet oder ausgeschaltet werden. Ein akkustisches Signal soll die Distanz, mit steigender oder sinkender Frequenz, den Abstand andeuten.
Die Sensoren werden Links und Rechts am Heck verbaut.
2. Die Innentemperatur, sowie die Beschleunigungskraft [G-Kraft] des Autos sollen gemessen und angezeigt werden. Hierfür wird der Sensor Gy-521 verwendet, sowie ein geeignetes Ausgabegerät.


## Anforderungen
| Anforderungen      | Muss                | Wunsch              |
|:------------------ |:-------------------:| :------------------:|
|Arduino muss Signale der Sensoren empfangen.|x||
|Arduino gibt Temperatur auf einem Bildschirm aus.|x||
|Arduino gibt Beschleunigung in G auf einem Bildschirm aus.||x|
|Einparkhilfe mit Drucktaster ein- oder ausschalten|x||
|Einparkhilfe meldet via Buzzer o.ä.akkustisch die Distanz|x||
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
![„arduino mazda323“](https://github.com/user-attachments/assets/61f1ce16-cf61-45d2-b012-9e24b1b75494)


## Projektbeschreibung
Kurze Beschreibung der Vorgehensweise

- Welche Teilfunktionen habt ihr einzeln getestet?
- Gab es dabei Probleme, wenn ja welche?
- Konnten alle Muss-Anforderungen umgesetzt werden?
- Nein, warum konnten nicht alle umgesetzt werden?
- War etwas unerwartet beim Schreiben des Programcodes?

## Tests
Kurze Beschreibung der Vorgehensweise

- Was sind die Testscenarios?
- Beschreibung oder Bild das Testziel aufzeigt
- Gibt es spezielle Gründe warum diese Tests gewählt wurden?
