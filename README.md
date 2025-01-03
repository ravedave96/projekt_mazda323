# projekt_mazda323
## 1. Kurzbeschrieb
Mein Mazda323 Jg. 1990 erhält einige Upgrades. 
1. Das rückwärts Einparken soll mit zwei Ultraschallsensoren erleichtert werden. 
Mit einem Drucktaster kann die Parkhilfe eingeschaltet oder ausgeschaltet werden, wenn möglich wird die Parkhilfe beim rückwärts Fahren automatisch aktiviert. Ein akkustisches Signal soll, mit steigender oder sinkender Dauer des Signals, den Abstand zum Hindernis angeben.
Die Sensoren werden Links und Rechts am Heck verbaut.
2. Die Innentemperatur, sowie die Beschleunigungskraft [G-Kraft] des Autos sollen während der Fahrt gemessen und angezeigt werden. Hierfür wird der Sensor MPU6050 verwendet, sowie ein geeignetes Ausgabegerät.


## 2. Anforderungen
| Anforderungen      | Muss                | Wunsch              |Erfüllt             |
|:------------------ |:-------------------:| :------------------:|:------------------:|
|Arduino muss Signale der Sensoren empfangen.|x||x|
|Arduino gibt Temperatur auf einem Bildschirm aus.|x||x|
|Arduino gibt Beschleunigung in G auf einem Bildschirm aus.||x|x|
|Einparkhilfe mit Drucktaster ein- oder ausschalten|x||x|
|Einparkhilfe wird aktiviert bei rückwärtsbewegung||x||
|Einparkhilfe meldet via Buzzer akkustisch die Distanz|x||x|
|Gemessene Distanz von Auto kann visuel (Balken o.ä.) auf einem Bildschirm angezeigt werden. ||x|x|
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
![„arduino mazda323“ (2)](https://github.com/user-attachments/assets/f4e6e1af-789b-440e-8bbf-4d9d30e6d221)
## 5. Hardwarekomponenten
1. Mikrocontroller: Arduino UNO
2. OLED-Display: Adafruit SSD1306 (128x32 Pixel, I²C)
3. Ultraschallsensoren: Zwei HC-SR04 (links und rechts)
4. Beschleunigungssensor: MPU6050 (I²C-Verbindung)
5. Buzzer: Zwei Stück (für akustische Signale, links und rechts)
6. Taster: Umschaltung zwischen Parkhilfe- und Beschleunigungsmodus
7. Verbindungen:
   - I²C für OLED: A4 (SDA) und A5 (SCL)
   - I²C für MPU6050: SDA und SCL
   - Trigger- und Echo-Pins der Ultraschallsensoren: Pins 6, 7 (links) und 8, 9 (rechts)
   - Buzzer-Pins: Pins 10 (links) und 11 (rechts)
   - Taster: Pin 2 (Eingang)
## 6. Projektbeschreibung
### 6.1 Programmstruktur
- **umschalten():** Wechselt zwischen den Modi (Einparkhilfe/Beschleunigung).
- **Parkhilfe():** Stellt Distanzmessung und akustische Ausgabe sicher.
- **distance():** Misst Hindernisse regelmäßig, blockiert aber nicht den Hauptprogrammfluss.
- **buzzer():** Gibt akustische Signale proportional zur Distanz aus.
- **Beschleunigung():** Misst und gibt G-Kräfte sowie Temperatur aus.
### 6.2. Beschreibung der Hauptfunktionen
#### 6.2.1 Hauptschleife (loop)
   - umschalten wird aufgerufen:
     - Überwacht den Taster und wechselt den Zustand von schalter (true/false), wenn der Taster gedrückt wird.
   - Verzweigung basierend auf schalter:
      - Wenn der Schalter aktiv (true) ist:
        - Die Funktionen ParkhilfeLH und ParkhilfeRH werden aufgerufen, um die Parkhilfen zu steuern.
      - Wenn der Schalter inaktiv (false) ist:
        - Die Funktion Beschleunigung wird aufgerufen, um die Beschleunigungsfunktion zu steuern.
#### 6.2.2 Umschalten-Funktion
Das Programm sorgt dafür, dass durch jeden Tastendruck der Zustand der Ausgänge zuverlässig gewechselt wird, ohne durch Prellen gestört zu werden.

   Die Funktion umschalten() hat folgende Aufgaben:
   - Sie liest den Zustand eines Tasters ein, der mit dem Pin tasterPin verbunden ist.
   - Sie erkennt einen Tastendruck, indem sie prüft, ob der Taster von LOW nach HIGH wechselt.
   - Sie schaltet einen logischen Schalter (schalter) um.
   - Sie verwendet eine kurze Verzögerung (50 Millisekunden), um den Taster zu entprellen und unerwünschte Zustandswechsel zu verhindern.

#### 6.2.3 Einparkhilfe-Funktion
Die Parkhilfe verwendet Ultraschallsensoren, um Hindernisse zu erkennen, und steuert akustische Signale basierend auf der Entfernung. 
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
#### 6.2.3.2 Funktion buzzer()
Die Frequenz des akustischen Signals variiert je nach Abstand. Bei Annäherung an ein Hindernis wird das Signal schneller.
Wenn keine gültige Distanz gemessen wird (z. B. kein Echo), wird der Buzzer ausgeschaltet.
#### 6.2.4 Funktion Beschleunigung()
Quelle: https://wolles-elektronikkiste.de/mpu6050-beschleunigungssensor-und-gyroskop

Liest Beschleunigungs- und Temperaturdaten vom MPU6050 aus:
- Beschleunigung entlang der X- und Y-Achse (berechnet in g)
- Temperatur (in °C)
Die Messwerte werden auf dem OLED-Display angezeigt. Zusätzlich wird der Modus "Driving Mode" visualisiert.
Zeigt diese Daten auf dem OLED-Display an.
#### 6.3 OLED-Display
Das Display visualisiert die Betriebsmodi und Messwerte:
1. Parkhilfe-Modus:
   - Linke und rechte Abstände (z. B. LH=45 cm, RH=30 cm)
   - Betriebsstatus: Parking-Mode
2. Beschleunigungsmodus:
   - Beschleunigung (maximaler Wert von X und Y)
   - Temperatur
   - Betriebsstatus: Driving-Mode
## 7. Tests
### 7.1 Test einzelner Funktionen
#### 7.1.1 Test des Umschaltens:
Ein Drucktaster wechselt zwischen den Modi. Das Umschalten wurde erfolgreich mit einer LED-Simulation getestet.
#### 7.1.2 Test der Einparkhilfe:
Die Subfunktionen distance() und buzzer() wurden separat im Branch ultrasonic und anschliessend im Hauptprogramm getestet.
Unterschiedliche Distanzen wurden simuliert, die akustischen Signale reagierten korrekt.
#### 7.1.3 Test der Beschleunigungs- und Temperaturanzeige:
Temperaturwerte wurden korrekt gemessen und angezeigt.
Die Ausgabe erfolgt zuverlässig, auch beim Umschalten zwischen den Modi.
### 7.2 Hauptprogramm-Test
Das Zusammenspiel aller Komponenten wurde im Hauptbranch getestet.
Szenarien wie das Aktivieren der Einparkhilfe und das Anzeigen von Sensorwerten verliefen erfolgreich.
Die Einbindung des OLED-Displays wurde in diesem Test-Teil erfolgreich durchgeführt.
#### 7.2.1 Vorgehen
1. Parkhilfe-Modus:
- Aktiviere die Parkhilfe mit dem Drucktaster.
- Überprüfe die korrekte Erkennung von Hindernissen auf beiden Seiten (links und rechts).
- Kontrolliere die Ausgabe der akustischen Signale basierend auf der Entfernung.
- Schalte die Parkhilfe aus und stelle sicher, dass der Modus korrekt deaktiviert wird.

2. Beschleunigungsmodus:
- Bei Power-ON ist dieser Modus aktiviert
- Wechsel von Parkhilfe-Modus in den Beschleunigungsmodus mit dem Drucktaster.
- Überprüfe die korrekte Anzeige der Beschleunigungsdaten und der Temperatur auf dem OLED-Display.
- Stelle sicher, dass der Modus ordnungsgemäß bleibt, bis er manuell gewechselt wird.
#### 7.2.2 Szenarien
1. Rückwärts Einparken mit Hindernissen auf beiden Seiten.
2. Wechsel zwischen den Modi während der Fahrt.
3. Darstellung der Beschleunigungs- und Temperaturwerte.
#### 7.2.3 Erwartete Ergebnisse
1. Die Parkhilfe erkennt Hindernisse zuverlässig und gibt klare akustische Signale aus.
2. Die Umschaltung zwischen den Modi erfolgt fehlerfrei.
3. Beschleunigungs- und Temperaturwerte werden korrekt angezeigt.

### 7.3 Proleme/Lösungen während des codens
Beanspruchte Hilfsquellen für Troubleshooting: 
- https://docs.arduino.cc/language-reference/
- https://www.circuitbasics.com/how-to-set-up-an-ultrasonic-range-finder-on-an-arduino/
- https://how2electronics.com/jsn-sr04t-waterproof-ultrasonic-sensor-with-arduino-guide/
- https://www.circuitbasics.com/how-to-use-active-and-passive-buzzers-on-the-arduino/
#### 7.3.1 Test Funktion Parkhilfe() nur LH-Seite:
##### Problem 1
Zuerst wurde die Distanzmessung sowie der Buzzer in derselben Funktion programmiert.
Jedoch ergab sich das Problem, dass nach jeder Verzögerung (delay) der Ton auch pausiert wird. 
Die Überschneidung der Speildauer des Tons und der Verzögerung am Ende der Funktion erzeugt einen abgehacktn Ton und er wird nicht kontinuierlich gespielt, währenddem die Messung durchgeführt wird.
##### Lösung 1
1. Die Funktion Parkhilfe wird in zwei Unterfunktionen aufgeteilt.
   - dinstance(): Funktion zur Distanzmessung in regelmässigen Abständen
   - buzzer(): Funktion zur Steuerung des Buzzers basierend auf der Distanz
2. Verwendung von millis() für das Timing anstatt von delay(). Dadurch wird sichergestellt, dass die Buzzer-Steuerung den Rest des Programms nicht blockiert. Durch den Verzicht auf delay() kann das Programm die Abstandsmessung und die Ausführung anderer Aufgaben fortsetzen, ohne blockiert zu werden.
##### Problem 2
Code funktioniert nicht, der Buzzer gibt einen kontinuierlichen Ton aus.
Die Methode tone() erzeugt den Ton, aber die Dauer (interval) ist nur die Zeit, für die der Ton gespielt wird, nicht die Pause zwischen den Tönen. Es gibt keinen Mechanismus, um den Ton zu stoppen oder eine Pause einzufügen, bevor die nächste Distanzmessung erfolgt.
##### Lösung 2
Um das gewünschte Verhalten zu erzielen (abwechselndes Ein- und Ausschalten des Buzzers basierend auf der Distanz), muss ein Mechanismus eingeführt werden, der den Ton nach einer bestimmten Zeit ausschaltet und dann eine Pause macht.
Mit einer buzzerOn-Logik wird der Zustand des Buzzers (ein/aus) durch die Variable buzzerOn gesteuert.
Bei jeder Änderung wird der Zustand umgeschaltet.
### 7.3.2 Proleme/Lösungen Test Funktion Parkhilfe() LH und RH zusammen
##### Problem 1
Buzzer von LH und RH funktionieren, jedoch blockieren sie sich gegenseitig, sodass jeweils nur LH oder RH einen Ton ausgibt.
Das Problem liegt in der Verwendung von tone() und noTone() für die Steuerung der Buzzer. Beide Funktionen arbeiten mit demselben Timer. Wenn eine Seite den Timer verwendet, blockiert dies die Steuerung des anderen Buzzers, da beide auf denselben Timer zugreifen möchten.
##### Lösung 1
Um das gleichzeitige Abspielen von Tönen auf zwei Lautsprechern zu ermöglichen, können wir statt der tone()-Funktion die Digitalen Ausgänge direkt ansteuern. Dadurch lassen sich unabhängig voneinander unterschiedliche Dauer des Tones erzeugen, ohne dass die Buzzer sich gegenseitig blockieren. 
##### Problem 2
Die Frequenz, also Tonhöhe lässt sich nicht genau einstellen. Die Funktion funktioniert, das Geräusch ist einfach nicht sehr angenehm. Gerne möchte ich ein Ton C4 erzeugen, welches eine Frequenz von 261 Hz hat. 
##### Lösung 2
Da die tone()-Funktion nicht verwendet werden darf, muss man die HIGH- und LOW-Zustände des Buzzers genauer steuern, um eine Rechteckwelle mit der gewünschten Frequenz zu erhalten.
Quelle: BLACKBOX.AI
Um den Buzzer in der Frequenz C4 (261,63 Hz) zu betreiben, können wir die Funktion analogWrite() verwenden, um ein PWM-Signal zu erzeugen. Da die Frequenz von analogWrite() auf den meisten Arduino-Boards (wie dem Uno) standardmäßig 490 Hz beträgt, können wir die PWM-Duty-Cycle anpassen, um den gewünschten Ton zu erzeugen.

Somit habe ich nun in der Buzzer Funktion den digitalWrite() zu analogWrite() geändert. 

Quelle: BLACKBOX.AI
Wenn buzzerStateRH true ist (Buzzer ein), wird 128 übergeben, was etwa 50% der maximalen PWM-Leistung entspricht (auf einem 8-Bit-PWM-System, wo 255 die maximale Leistung ist).
Die Anwendung funktioniert mit jeweils eigenem Intervall und einer einstellbaren Frequenz.
#### 7.3.3 Proleme/Lösungen Test Funktion Beschleunigung() im Branch MPU6050
##### Problem 1
Die delay() Funktion hat das Umschalten verunmöglicht, da der main loop() blockiert wurde, konnte die Betätigung des Druckschalters nicht erfasst werden.
#####Lösung 1
Ersetzung von delay() durch millis().
## 8 Fazit
Das Arduino-Projekt kombiniert eine intelligente Parkhilfe mit einem Beschleunigungsmesser. Die Visualisierung und akustische Rückmeldung machen das System zu einer hilfreichen Fahrzeugunterstützung.
#### Ziele erreicht:
- Verbesserte Einparkhilfe mit akustischen Signalen und visueller Anzeige der Distanz in cm auf Display.
- Umschaltung mit Pushbutton zwischen Drive- und Park-Modus
- Anzeige von G-Kräften und Innentemperatur in Echtzeit.
#### Verbesserungspotenzial:
- Funktionen zusammenfassen, z.B. Parkhilfe LH und RH in eine Funktion.
- Funktion für OLED-Anzeige erstellen um eine zentralisierte Sammlung aller Ausgabeparameter zu erhalten.
- Akkustisches Signal im Gleichtakt ausgeben, wenn genau gleiche Distanz gemessen wird.
