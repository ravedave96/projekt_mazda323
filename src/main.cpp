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

  // Daten ausgeben
  Serial.print("AcX = "); Serial.print((accX));
  Serial.print(" | AcY = "); Serial.print((accY));
  Serial.print(" | AcZ = "); Serial.print((accZ));
  Serial.print(" | tmp = "); Serial.print((tRaw + 12412.0) / 340.0); // Temperatur
  Serial.print(" | GyX = "); Serial.print((gyroX));
  Serial.print(" | GyY = "); Serial.print((gyroY));
  Serial.print(" | GyZ = "); Serial.print((gyroZ));
  Serial.println();

  delay(1000); // 1 Sekunde Pause
}

// Funktion zur Umwandlung von int16-Werten in formatierte Strings
char* toStr(int16_t character) {
  sprintf(result, "%6d", character);
  return result;
}

