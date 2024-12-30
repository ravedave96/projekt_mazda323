# projekt_mazda323
## 1. Kurzbeschrieb
Mein Mazda323 Jg. 1990 erhält einige Upgrades. 
1. Das rückwärts Einparken soll mit zwei Ultraschallsensoren erleichtert werden. 
Mit einem Drucktaster kann die Parkhilfe eingeschaltet oder ausgeschaltet werden, wenn möglich wird die Parkhilfe beim rückwärts Fahren automatisch aktiviert. Ein akkustisches Signal soll, mit steigender oder sinkender Dauer des Signals, den Abstand zum Hindernis angeben.
Die Sensoren werden Links und Rechts am Heck verbaut.
2. Die Innentemperatur, sowie die Beschleunigungskraft [G-Kraft] des Autos sollen während der Fahrt gemessen und angezeigt werden. Hierfür wird der Sensor Gy-521 verwendet, sowie ein geeignetes Ausgabegerät.


## 2. Anforderungen
| Anforderungen      | Muss                | Wunsch              |Erfüllt             |
|:------------------ |:-------------------:| :------------------:|:------------------:|
|Arduino muss Signale der Sensoren empfangen.|x||x|
|Arduino gibt Temperatur auf einem Bildschirm aus.|x||x|
|Arduino gibt Beschleunigung in G auf einem Bildschirm aus.||x||
|Einparkhilfe mit Drucktaster ein- oder ausschalten|x||x|
|Einparkhilfe wird aktiviert bei rückwärtsbewegung||x||
|Einparkhilfe meldet via Buzzer akkustisch die Distanz|x||x|
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
![„arduino mazda323“ (2)](https://github.com/user-attachments/assets/f4e6e1af-789b-440e-8bbf-4d9d30e6d221)
## 5. Parts
- 1x Arduino UNO R3
- 1x Breadboard
- 2x HC-SR04 Ultrasonic sensor
- 1x MPU6050 Beschleunigungssensor
- 1x Drucktaster
- 2x Piezo-Buzzer oder Lautsprecher
- 1x OLED-Display
## 6. Projektbeschreibung
### 6.1 Programmstruktur
#### 6.1.1 Definition Funktionen
   - **umschalten()** : Funktion zum manuellen Umschalten von Beschleunigungsmesser auf Parkhilfe
   - **Parkhilfe()** : jeweils LH/RH, Funktion zur Parkhilfe
     - **dinstance()** : jeweils LH/RH, Funktion zur Distanzmessung in regelmässigen Abständen
     - **buzzer()** : jeweils LH/RH, Funktion zur Steuerung des Buzzers basierend auf der Distanz
   - **Beschleunigung()** : Funktion zur G-Kräfte Messung
### 6.2. Beschreibung einzelner Funktionen
#### 6.2.1 Dauerschleife void loop()
   - umschalten wird aufgerufen:
     - Überwacht den Taster und wechselt den Zustand von schalter (true/false), wenn der Taster gedrückt wird.
   - Verzweigung basierend auf schalter:
      - Wenn der Schalter aktiv (true) ist:
        - Die Funktionen ParkhilfeLH und ParkhilfeRH werden aufgerufen, um die Parkhilfen zu steuern.
      - Wenn der Schalter inaktiv (false) ist:
        - Die Funktion Beschleunigung wird aufgerufen, um die Beschleunigungsfunktion zu steuern.
#### 6.2.2 Funktion umschalten()
Das Programm sorgt dafür, dass durch jeden Tastendruck der Zustand der Ausgänge zuverlässig gewechselt wird, ohne durch Prellen gestört zu werden.

   Die Funktion umschalten() hat folgende Aufgaben:
   - Sie liest den Zustand eines Tasters ein, der mit dem Pin tasterPin verbunden ist.
   - Sie erkennt einen Tastendruck, indem sie prüft, ob der Taster von LOW nach HIGH wechselt.
   - Sie schaltet einen logischen Schalter (schalter) um.
   - Sie steuert zwei Ausgänge (beschleunigungPin und parkPin) basierend auf dem Zustand des Schalters:
     - Einer der Ausgänge wird auf HIGH gesetzt, während der andere auf LOW bleibt.
   - Sie verwendet eine kurze Verzögerung (50 Millisekunden), um den Taster zu entprellen und unerwünschte Zustandswechsel zu verhindern.

#### 6.2.3 Funktion Parkhilfe()
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
   Durch diese Logik wird ein akustisches Feedback erzeugt, das die Distanz zur gemessenen Oberfläche repräsentiert.
   
   - Die Funktion passt das Intervall für den Buzzer-Ton an, basierend auf der gemessenen Distanz:
   - Wenn keine gültige Distanz gemessen wird (z. B. kein Echo), wird der Buzzer ausgeschaltet.
#### 6.2.4 Funktion Beschleunigung()


- Welche Teilfunktionen habt ihr einzeln getestet?
- Gab es dabei Probleme, wenn ja welche?
- Konnten alle Muss-Anforderungen umgesetzt werden?
- Nein, warum konnten nicht alle umgesetzt werden?
- War etwas unerwartet beim Schreiben des Programcodes?
  
## 7. Tests
Vorgehen: 
7.1 Testen einzelner Funktionen in einem anderen Branch.
7.2 erfolgreich gestestete Funktion in Main Branch einfliessen lassen und Main Branch testen
### 7.1 Vorgehen: 
1. Testen einzelner Funktionen in einem anderen Branch.
2. erfolgreich gestestete Funktion in Main Branch einfliessen lassen
3. Main Branch testen
### 7.1 Test Funktion Parkhilfe() nur LH-Seite
Beanspruchte Hilfsquellen: 
- https://docs.arduino.cc/language-reference/
- https://www.circuitbasics.com/how-to-set-up-an-ultrasonic-range-finder-on-an-arduino/
- https://how2electronics.com/jsn-sr04t-waterproof-ultrasonic-sensor-with-arduino-guide/
- https://www.circuitbasics.com/how-to-use-active-and-passive-buzzers-on-the-arduino/
#### 7.1.1.2 Problemstellung 1
Zuerst wurde die Distanzmessung sowie der Buzzer in derselben Funktion programmiert.
Jedoch ergab sich das Problem, dass nach jeder Verzögerung (delay) der Ton auch pausiert wird. 
Die Überschneidung der Speildauer des Tons und der Verzögerung am Ende der Funktion erzeugt einen abgehacktn Ton und er wird nicht kontinuierlich gespielt, währenddem die Messung durchgeführt wird.
#### 7.1.1.3 Lösung 1
1. Die Funktion Parkhilfe wird in zwei Unterfunktionen aufgeteilt.
   - dinstance(): Funktion zur Distanzmessung in regelmässigen Abständen
   - buzzer(): Funktion zur Steuerung des Buzzers basierend auf der Distanz
2. Verwendung von millis() für das Timing anstatt von delay(). Dadurch wird sichergestellt, dass die Buzzer-Steuerung den Rest des Programms nicht blockiert. Durch den Verzicht auf delay() kann das Programm die Abstandsmessung und die Ausführung anderer Aufgaben fortsetzen, ohne blockiert zu werden.
#### 7.1.1.4 Problem 2
Code funktioniert nicht, der Buzzer gibt einen kontinuierlichen Ton aus.
Die Methode tone() erzeugt den Ton, aber die Dauer (interval) ist nur die Zeit, für die der Ton gespielt wird, nicht die Pause zwischen den Tönen. Es gibt keinen Mechanismus, um den Ton zu stoppen oder eine Pause einzufügen, bevor die nächste Distanzmessung erfolgt.
#### 7.1.1.4 Lösung 2
Um das gewünschte Verhalten zu erzielen (abwechselndes Ein- und Ausschalten des Buzzers basierend auf der Distanz), muss ein Mechanismus eingeführt werden, der den Ton nach einer bestimmten Zeit ausschaltet und dann eine Pause macht.
Mit einer buzzerOn-Logik wird der Zustand des Buzzers (ein/aus) durch die Variable buzzerOn gesteuert.
Bei jeder Änderung wird der Zustand umgeschaltet.

### 7.1.2 Test Funktion Parkhilfe() LH und RH zusammen
#### 7.1.2.2 Problem 1
Buzzer von LH und RH funktionieren, jedoch blockieren sie sich gegenseitig, sodass jeweils nur LH oder RH einen Ton ausgibt.
Das Problem liegt in der Verwendung von tone() und noTone() für die Steuerung der Buzzer. Beide Funktionen arbeiten mit demselben Timer. Wenn eine Seite den Timer verwendet, blockiert dies die Steuerung des anderen Buzzers, da beide auf denselben Timer zugreifen möchten.
#### 7.1.2.3 Lösung 1
Um das gleichzeitige Abspielen von Tönen auf zwei Lautsprechern zu ermöglichen, können wir statt der tone()-Funktion die Digitalen Ausgänge direkt ansteuern. Dadurch lassen sich unabhängig voneinander unterschiedliche Dauer des Tones erzeugen, ohne dass die Buzzer sich gegenseitig blockieren. 
#### 7.1.2.5 Problem 2
Die Frequenz, also Tonhöhe lässt sich nicht genau einstellen. Die Funktion funktioniert, das Geräusch ist einfach nicht sehr angenehm. Gerne möchte ich ein Ton C4 erzeugen, welches eine Frequenz von 261 Hz hat. 
#### 7.1.2.6 Lösung
Da die tone()-Funktion nicht verwendet werden darf, muss man die HIGH- und LOW-Zustände des Buzzers genauer steuern, um eine Rechteckwelle mit der gewünschten Frequenz zu erhalten.
Quelle: BLACKBOXAI
Um den Buzzer in der Frequenz C4 (261,63 Hz) zu betreiben, können wir die Funktion analogWrite() verwenden, um ein PWM-Signal zu erzeugen. Da die Frequenz von analogWrite() auf den meisten Arduino-Boards (wie dem Uno) standardmäßig 490 Hz beträgt, können wir die PWM-Duty-Cycle anpassen, um den gewünschten Ton zu erzeugen.

Somit habe ich nun in der Buzzer Funktion den digitalWrite() zu analogWrite() geändert. 

Quelle BLACKBOXAI:
Wenn buzzerStateRH true ist (Buzzer ein), wird 128 übergeben, was etwa 50% der maximalen PWM-Leistung entspricht (auf einem 8-Bit-PWM-System, wo 255 die maximale Leistung ist).

Die Anwendung funktioniert mit jeweils eigenem Intervall und einer einstellbaren Frequenz.
Test erfolgreich bestanden.
### 7.1 Test Funktion Beschleunigung() im Branch MPU6050
Für die Muss Funktion benötigen wir nur die Temperatur Ausgabe.
Mit folgendem Code bekommen wir die Rohdaten aus dem Sensor.
Auf dem Serieal Monitor wird sie ausgegeben.
Da dieser Sensor sehr komplex ist und es fast keine gute Bibliothek gibt, habe ich den Code übernommen.
Quelle: https://wolles-elektronikkiste.de/mpu6050-beschleunigungssensor-und-gyroskop
#### Problem 1
Die delay() Funktion hat das Umschalten verunmöglicht, da der main loop() blockiert wurde, konnte die Betätigung des Druckschalters nicht erfasst werden.
#### Lösung 1
Ersetzung von delay() durch millis().
### 7.2 Test Hauptprogram
#### 7.2.1  Funktion umschalten() 
Das Umschalten von einer auf die andere Funktion wird mit einer einfachen Toggle-Logik gemacht. Mit einer LED wurde gestestet, ob der Drucktaster ein Signal zurückgibt. Das Umschalten it einfach zu testen, entweder die Distanzsensoren sind aktiv oder der Beschleunigungssensor.
#### 7.2.1 Parkhilfe()
Die Sub-Funktionen dinstance() und buzzer() wurden im ultrasonic Branch erfolgreich gestestet.
Im Main Branch funktionieren sie wie gewünscht.
Testszenario:
Mit der Hand als Hinderniss, werden jeweils beim linken und rechten Abtsandsensor verschiedene Distanzen simuliert.
Testziel:
Die Buzzers geben im Verhältnis zur Distanz eine andere Dauer des Tons aus. Auf dem Serial Monitor sind jeweils die Distanzen zur Kontrolle abzulesen.
#### 7.2.1 Beschleunigung()
Die Temperatur wird vom Sensor gelesen und auf dem OLED-Dsiplay ausgegeben. Wenn der Pushbutton gedrückt wird, bleibt die letzte gemessene Temperatur auf dem Display stehen und die Parkhilfe schaltet sich ein.



- Was sind die Testscenarios?
- Beschreibung oder Bild das Testziel aufzeigt
- Gibt es spezielle Gründe warum diese Tests gewählt wurden?
