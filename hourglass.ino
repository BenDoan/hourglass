#include <SparkFun_ADXL345.h>
#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
/* #include <SD.h> */

/* File log_file; */


// Up: X: -13, Y: 7, Z: 191

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;


int upLightPin = 5; // D5
int upCoords[] = {14416, -30928, 16640};


int downLightPin = 6; // D6
int downCoords[] = {272, 24, 16656};


ADXL345 adxl = ADXL345();
RTC_DS1307 rtc;

void setup(){
  Serial.begin(9600);
  /* setupSD(); */
  setupRtc();
  setupAccel();
}

void loop(){
  loopAccel();
  /* turnOff(upLightPin); */
  /* turnOff(downLightPin); */
  /* loopRtc(); */
}

void loopRtc () {
  DateTime now = rtc.now();
  int unixTime = now.unixtime();
}

void setupRtc() {
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

/* void setupSD() { */
/*   log_file = SD.open("log.txt", FILE_WRITE); */
/*   if(!log_file) { */
/*     Serial.println("UNABLE TO OPEN LOG FILE!"); */
/*   } */
/* } */

/* void writeToLog(char str[]) { */
/*   log_file.print(sprintf("%s\n", str)); */
/*   log_file.flush(); */
/* } */

void setupAccel() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);

  pinMode(upLightPin, OUTPUT);
  pinMode(downLightPin, OUTPUT);
}

void loopAccel() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  /* Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L) */
  /* GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L) */
  /* GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L) */
  /* GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L) */
  /* Serial.print("AcX = "); Serial.print(AcX); */
  /* Serial.print(" | AcY = "); Serial.print(AcY); */
  /* Serial.print(" | AcZ = "); Serial.print(AcZ); */
  /* Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet */
  /* Serial.print(" | GyX = "); Serial.print(GyX); */
  /* Serial.print(" | GyY = "); Serial.print(GyY); */
  /* Serial.print(" | GyZ = "); Serial.println(GyZ); */

  /* Serial.print(AcX); Serial.print(" "); */
  /* Serial.print(AcY); Serial.print(" "); */
  /* Serial.print(AcZ); Serial.print("\n"); */

  // TODO: investigate using sum of differences to pick
  // closest value
  int coords[] = {AcX, AcY, AcZ};
  if (closeTo(coords, upCoords)) {
    Serial.println("Up");
    turnOn(upLightPin);
    turnOff(downLightPin);
  } else if (closeTo(coords, downCoords)) {
    Serial.println("Down");
    turnOn(downLightPin);
    turnOff(upLightPin);
  } else {
    Serial.println("Neither");
    turnOff(downLightPin);
    turnOff(upLightPin);
  }
  /* delay(1000); */
}

boolean closeTo(int coords1[], int coords2[]) {
  int margin = 5000;

  for (int i = 0; i < 3; ++i) {
    if (abs(coords1[i] - coords2[i]) > margin) {
      return false;
    }
  }

  return true;
}

void turnOn(int pin) {
  digitalWrite(pin, HIGH);
}

void turnOff(int pin) {
  digitalWrite(pin, LOW);
}
