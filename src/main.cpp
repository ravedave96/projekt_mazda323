#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//OLED-Display Settings, Quelle: Adafruit Bibliothek https://registry.platformio.org/libraries/adafruit/Adafruit%20SSD1306/examples/ssd1306_128x32_i2c/ssd1306_128x32_i2c.ino
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Zuweisung Pin auf Arduino Board
#define tasterPin 2

#define MPU6050_ADDR 0x68 // Alternativ AD0 auf HIGH setzen --> Adresse = 0x69

// MPU-6050 SCL Anschluss auf Pin A5
// MPU-6050 SDA Anschluss auf Pin A4
#define trigPinLH 6 // Pin für den Trigger des Ultraschallsensors (links)
#define echoPinLH 7 // Pin für das Echo des Ultraschallsensors (links)
#define trigPinRH 8 // Pin für den Trigger des Ultraschallsensors (rechts)
#define echoPinRH 9 // Pin für das Echo des Ultraschallsensors (rechts)
#define buzzerPinLH 10 // Pin für den Buzzer (links)(analog)
#define buzzerPinRH 11 // Pin für den Buzzer (rechts)(analog)

// Globale Variablen
int push = 0;       // aktueller Zustand des Tasters
int lastpush = 0;   // letzter Zustand des Tasters
bool schalter = false;  // aktueller Zustand des Ausgangs (umschaltbar)

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

// Globale Variablen Beschleunigung
int16_t accX, accY, accZ, gyroX, gyroY, gyroZ, tRaw; // Rohdaten für Beschleunigung, Gyroskop und Temperatur
char result[7]; // Temporärer String für die Umwandlung in Text
unsigned long lastAccelTime = 0;
const unsigned long accelInterval = 1000; // Intervall für Beschleunigungsmessung (1 Sekunde)

// Main-Funktionsprototypen
void umschalten();
void ParkhilfeLH();
void ParkhilfeRH();
void Beschleunigung();

//Sub-Funktionsprototypen
void distanceLH(); // Funktion zur Distanzmessung (links)
void buzzerLH(unsigned long intervalLH); // Tonsteuerung für Buzzer (links)
void distanceRH(); // Funktion zur Distanzmessung (rechts)
void buzzerRH(unsigned long intervalRH); // Tonsteuerung für Buzzer (rechts)

void setup() {
    Serial.begin(9600);
    Wire.begin(); // I²C-Kommunikation initialisieren

    // MPU6050 aufwecken
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x6B); // PWR_MGMT_1-Register
    Wire.write(0);    // Wake-up-Befehl
    Wire.endTransmission(true);

    pinMode(tasterPin, INPUT);          // Eingang  2, Taster

    pinMode(trigPinLH, OUTPUT);     // Ausgang 6, TriggerLH
    pinMode(echoPinLH, INPUT);      // Eingang 7, EchoLH
    pinMode(trigPinRH, OUTPUT);     // Ausgang 8, TriggerRH
    pinMode(echoPinRH, INPUT);      // Eingang 9, EchoRH
    pinMode(buzzerPinLH, OUTPUT);   // Ausgang 10, Buzzer LH
    pinMode(buzzerPinRH, OUTPUT);   // Ausgang 11, Buzzer RH

    // OLED Display, SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    delay(200);
    display.clearDisplay();
}

void loop() {
   umschalten();
        if (schalter) {
            ParkhilfeLH();
            ParkhilfeRH();

            // Daten für Parkhilfe ausgeben auf OLED-Display, besser hier als in Funktionen, da die Funktionen getrennt ablaufen.
            // Display Ausgabe mithilfe Bibliothek adafruit/Adafruit SSD1306
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(25, 20);
            display.print("Parking-Mode");

            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(1, 1);
            display.print("LH=");
            display.print(distanzLH,0);
            display.print(" cm");

            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(70,1);
            display.print("RH=");
            display.print(distanzRH,0);
            display.print(" cm");

            display.display();

        } else {
            Beschleunigung();
            analogWrite(buzzerPinLH, LOW);
            analogWrite(buzzerPinRH, LOW);
        }
}

// Funktion zum manuellen Umschalten von Beschleunigungsmesser auf Parkhilfe
void umschalten () {
    // Umschalttaste auslesen
    push = digitalRead(tasterPin);

    // Prüfen, ob der Taster gedrückt wurde (Zustandsänderung von LOW nach HIGH)
    if (push == HIGH && lastpush == LOW) {
        schalter = !schalter;   // Taster-Zustand umkehren
    }
    // Verzögerung für Entprellung
    delay(50);

    // Aktuellen Zustand Tasters speichern
    lastpush = push;
}

// Funktion zur Parkhilfe linke Seite
void ParkhilfeLH () {
    // Funktion zur Distanzmessung aufrufen
    distanceLH();
    if (distanzLH > 0 && distanzLH < 300) { // Nur innerhalb eines gültigen Bereichs 0-3m
        unsigned long intervalLH = (5*(distanzLH) + 10); // quadratische Funktion, so dass die Dauer(Intervall) des Ton mit abnehmender Distanz kürzer wird
        //map() kann auch eingefügt werden. map(distanz, 1, 200, 50, 1000) skaliert die Distanz auf eine Tonlänge zwischen 50 ms und 1000 ms.
        buzzerLH(intervalLH); // Aktiviert den Buzzer mit dem berechneten Intervall.
    } else {
        analogWrite(buzzerPinLH, LOW); // Buzzer ausschalten, wenn Distanz ausserhalb des Bereichs
    }
    // Funktion zur kontinuierlichen Steuerung des Buzzer aufrufen
}

// Funktion zur Parkhilfe rechte Seite
void ParkhilfeRH () {
    // Funktion zur Distanzmessung aufrufen
    distanceRH();
    if (distanzRH > 0 && distanzRH < 300) {
        unsigned long intervalRH = (5*(distanzRH) + 10);

        buzzerRH(intervalRH);
    } else {
        analogWrite(buzzerPinRH, LOW);
    }
}

// Quelle Code Beschleunigung: https://wolles-elektronikkiste.de/mpu6050-beschleunigungssensor-und-gyroskop
// optimiert mit chatgpt.com
// Display Ausgabe mithilfe Bibliothek adafruit/Adafruit SSD1306
void Beschleunigung () {
        if (millis() - lastAccelTime >= accelInterval) {
            lastAccelTime = millis(); // Aktualisiere die Zeit der letzten Messung

            // Register von 0x3B (ACCEL_XOUT_H) anfordern
            Wire.beginTransmission(MPU6050_ADDR);
            Wire.write(0x3B); // Startadresse der Register
            Wire.endTransmission(false); // Verbindung bleibt aktiv
            Wire.requestFrom(MPU6050_ADDR, 14, true); // 14 Register (7 x 2 Bytes) anfordern

            // Rohdaten lesen und zusammenfügen
            accX = Wire.read() << 8 | Wire.read(); // ACCEL_XOUT_H und ACCEL_XOUT_L
            accY = Wire.read() << 8 | Wire.read(); // ACCEL_YOUT_H und ACCEL_YOUT_L
            accZ = Wire.read() << 8 | Wire.read(); // ACCEL_ZOUT_H und ACCEL_ZOUT_L
            tRaw = Wire.read() << 8 | Wire.read(); // TEMP_OUT_H und TEMP_OUT_L
            gyroX = Wire.read() << 8 | Wire.read(); // GYRO_XOUT_H und GYRO_XOUT_L
            gyroY = Wire.read() << 8 | Wire.read(); // GYRO_YOUT_H und GYRO_YOUT_L
            gyroZ = Wire.read() << 8 | Wire.read(); // GYRO_ZOUT_H und GYRO_ZOUT_L

            // Skalierung der Beschleunigungswerte
            float accX_g = accX / 16384.0; // Umrechnung in g für ±2g-Empfindlichkeit
            float accY_g = accY / 16384.0;

            // Berechnung des grössten Absolutwerts (nur X und Y)
            float maxAccXY = max(abs(accX_g), abs(accY_g));

            //variable für Temperatur
            float temperature = (tRaw + 12412.0) / 340.0;

            // Daten für Beschleunigung ausgeben auf OLED-Display
            // Ausgabe der grössten Beschleunigung (nur X und Y), Z-Achse wird ignoriert, da diese die Schwerkraft wiedergibt.
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(25, 1);
            display.print("T:");
            display.print(temperature, 1); // temperature mit einer Dezimalstellen
            display.print("C");

            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(25, 10);
            display.print("G-Force:");
            display.print(maxAccXY, 2); // Beschleunigung mit 2 Dezimalstellen
            display.print("g");

            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(25, 20);
            display.print("Driving-Mode");

            display.display();
        }
    }

// Funktion zur Distanzmessung in regelmässigen Abständen linke Seite
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
}

// Funktion zur Steuerung des Buzzers basierend auf der Distanz linke Seite
void buzzerLH(unsigned long intervalLH) { //intervalLH: Zeitdauer in Millisekunden, wie lange der Buzzer an- oder ausgeschaltet bleiben soll.
    static bool buzzerStateLH = false; // Aktueller Zustand des Buzzers (links), Initialwert: false (Buzzer ist zu Beginn ausgeschaltet).

    //millis(): gibt die Zeit seit dem Start des Mikrocontrollers in Millisekunden zurück.
    //aktuelle Zeit (millis()) - letzte Aktivierungszeit (previousBuzzerTimeLH) >= dem angegebenen Intervall (intervalLH)
    //wenn ja, ist es Zeit, den Zustand des Buzzers zu wechseln (An oder Aus).
    if (millis() - previousBuzzerTimeLH >= intervalLH) {
        previousBuzzerTimeLH = millis(); // Zeit aktualisieren, speichert den Zeitpunkt der letzten Buzzer-Aktivierung.
        buzzerStateLH = !buzzerStateLH; // Buzzer-Zustand wird invertiert
        analogWrite(buzzerPinLH, buzzerStateLH ? 128 : LOW); //Ternäre Bedingung: buzzerStateLH == true: 128 (50% von Frequenz 255, was einem C4 Ton entspricht), buzzerStateLH == false: LOW
        // Erklärung zu PWM: https://exp-tech.de/blogs/blog/arduino-tutorial-pulsweitenmodulation-pwm
    }
}

// Funktion zur Distanzmessung in regelmässigen Abständen rechte Seite
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

// Funktion zur Steuerung des Buzzers basierend auf der Distanz rechte Seite
void buzzerRH(unsigned long intervalRH) {
    static bool buzzerStateRH = false;

    if (millis() - previousBuzzerTimeRH >= intervalRH) {
        previousBuzzerTimeRH = millis();
        buzzerStateRH = !buzzerStateRH;
        analogWrite(buzzerPinRH, buzzerStateRH ? 96 : LOW);
    }
}
