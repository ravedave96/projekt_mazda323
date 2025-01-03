#include <Arduino.h>
#include "Wire.h"

#define MPU6050_ADDR 0x68 // Alternativ AD0 auf HIGH setzen --> Adresse = 0x69

int16_t accX, accY, accZ, gyroX, gyroY, gyroZ, tRaw; // Rohdaten für Beschleunigung, Gyroskop und Temperatur
char result[7]; // Temporärer String für die Umwandlung in Text

void setup() {
  Serial.begin(9600); // Serielle Kommunikation starten
  Wire.begin(); // I²C-Kommunikation initialisieren

  // MPU6050 aufwecken
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1-Register
  Wire.write(0);    // Wake-up-Befehl
  Wire.endTransmission(true);
}

void loop() {
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

  // Berechnung des größten Absolutwerts (nur X und Y)
  float maxAccXY = max(abs(accX_g), abs(accY_g));

  // Ausgabe der größten Beschleunigung (nur X und Y)
  Serial.print("Max Acceleration (X/Y) = "); Serial.print(maxAccXY, 3); // auf 3 Dezimalstellen gerundet
  Serial.println(" g");
}


