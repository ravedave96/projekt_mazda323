#include <Arduino.h>
#include <Wire.h> // Include the Wire library for I2C communication

#define MPU6050_ADDR              0x68 // MPU6050 I2C address
#define MPU6050_GYRO_CONFIG       0x1B // Gyroscope configuration register
#define MPU6050_ACCEL_CONFIG      0x1C // Accelerometer configuration register
#define MPU6050_ACCEL_XOUT_H      0x3B // Starting register for accelerometer data
#define MPU6050_PWR_MGT_1         0x6B // Power management register
#define MPU6050_SLEEP             0x06 // Sleep bit position in PWR_MGT_1 register

typedef enum {
  MPU6050_ACC_RANGE_2G,  // +/- 2g (default)
  MPU6050_ACC_RANGE_4G,  // +/- 4g
  MPU6050_ACC_RANGE_8G,  // +/- 8g
  MPU6050_ACC_RANGE_16G  // +/- 16g
} mpu6050_acc_range;

typedef enum {
  MPU6050_GYR_RANGE_250,  // +/- 250 deg/s (default)
  MPU6050_GYR_RANGE_500,  // +/- 500 deg/s
  MPU6050_GYR_RANGE_1000, // +/- 1000 deg/s
  MPU6050_GYR_RANGE_2000  // +/- 2000 deg/s
} mpu6050_gyr_range;

// Prototypes for custom functions
void MPU6050_wakeUp();
void MPU6050_sleep();
void setAccRange(mpu6050_acc_range range);
void setGyrRange(mpu6050_gyr_range range);
void writeRegister(uint16_t reg, byte value);
char* toStr(int16_t i);

// Variables for raw data
int16_t accX, accY, accZ, gyroX, gyroY, gyroZ, tRaw;
const float ACCEL_SENSITIVITY = 16384.0; // Default sensitivity for +/-2g
char result[7];

void setup() {
  Serial.begin(9600); // Start serial communication
  Wire.begin();       // Initialize I2C communication
  MPU6050_wakeUp();   // Wake up the sensor
  setAccRange(MPU6050_ACC_RANGE_16G); // Set accelerometer range to +/-16g
  setGyrRange(MPU6050_GYR_RANGE_250); // Set gyroscope range to +/-250 deg/s
}

void loop() {
  MPU6050_wakeUp(); // Ensure the sensor is active
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_ACCEL_XOUT_H); // Start reading from ACCEL_XOUT_H register
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 14, true); // Request 14 bytes of data

  // Read accelerometer, temperature, and gyroscope data
  accX = Wire.read() << 8 | Wire.read();
  accY = Wire.read() << 8 | Wire.read();
  accZ = Wire.read() << 8 | Wire.read();
  tRaw = Wire.read() << 8 | Wire.read();
  gyroX = Wire.read() << 8 | Wire.read();
  gyroY = Wire.read() << 8 | Wire.read();
  gyroZ = Wire.read() << 8 | Wire.read();

  // Convert accelerometer data to g-forces
  float gX = accX / ACCEL_SENSITIVITY;
  float gY = accY / ACCEL_SENSITIVITY;
  float gZ = accZ / ACCEL_SENSITIVITY - 1.0; // Adjust for Earth's gravity on Z-axis

  // Calculate total acceleration magnitude
  float totalAcceleration = sqrt(gX * gX + gY * gY + gZ * gZ);

  // Determine the dominant direction of force
  String direction;
  if (abs(gX) > abs(gY) && abs(gX) > abs(gZ)) {
    direction = (gX > 0) ? "Forward (+X)" : "Backward (-X)";
  } else if (abs(gY) > abs(gX) && abs(gY) > abs(gZ)) {
    direction = (gY > 0) ? "Right (+Y)" : "Left (-Y)";
  } else {
    direction = (gZ > 0) ? "Up (+Z)" : "Down (-Z)";
  }

  // Print acceleration and gyroscope data
  Serial.print("Acceleration Magnitude: ");
  Serial.print(totalAcceleration, 3);
  Serial.print(" g | Direction: ");
  Serial.println(direction);

  Serial.print("Gx = "); Serial.print(gX, 3);
  Serial.print(" | Gy = "); Serial.print(gY, 3);
  Serial.print(" | Gz = "); Serial.println(gZ, 3);

  Serial.print("GyroX = "); Serial.print(gyroX);
  Serial.print(" | GyroY = "); Serial.print(gyroY);
  Serial.print(" | GyroZ = "); Serial.println(gyroZ);

  // Sleep sensor to save power (optional)
  MPU6050_sleep();

  delay(500); // Short delay for better readability
}

char* toStr(int16_t i) {
  sprintf(result, "%6d", i); // Format integer as string
  return result;
}

void setAccRange(mpu6050_acc_range range) {
  writeRegister(MPU6050_ACCEL_CONFIG, range << 3);
}

void setGyrRange(mpu6050_gyr_range range) {
  writeRegister(MPU6050_GYRO_CONFIG, range << 3);
}

void MPU6050_wakeUp() {
  writeRegister(MPU6050_PWR_MGT_1, 0);
  delay(30); // Allow the sensor to stabilize
}

void MPU6050_sleep() {
  writeRegister(MPU6050_PWR_MGT_1, 1 << MPU6050_SLEEP);
}

void writeRegister(uint16_t reg, byte value) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(true);
}
