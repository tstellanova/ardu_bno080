

#include <Wire.h>

#include "SparkFun_BNO080_Arduino_Library.h"
BNO080 myIMU;

const int REPORT_INTERVAL_MS = 2;

//  SPI PIN assignments
//  (For Arduino Micro Pro. 3.3V version)

// 21 = !RST
// 20 = !INT
// 19 = WAK
// 18 = !CS
// 15 SCLK
// 14 MISO
// 16 MOSI



//These pins can be any GPIO
byte imuRSTPin = 21;
byte imuINTPin = 20;
byte imuWAKPin = 19;
byte imuCSPin = 18;

  
void setup()
{
//  // wake behavior?
//  pinMode(imuWAKPin, OUTPUT);
//  digitalWrite(imuWAKPin, HIGH);  
//  pinMode(imuRSTPin, OUTPUT);
//  digitalWrite(imuRSTPin, LOW);   //Reset BNO080
//  delay(2);          //Min length not specified in datasheet?
//


  Wire.begin();
  //Wire.setClock(400000); //For I2C, increase data rate to 400kHz

  delay(500);

  Serial.begin(9600);
  Serial.println();

  // Can enable debug output from driver if interested
  //myIMU.enableDebugging();

  // i2c init:
  //  while (myIMU.begin(0x4a) == false) {
  
  while (false ==  myIMU.beginSPI(imuCSPin, imuWAKPin, imuINTPin, imuRSTPin, 3000000)) {
   Serial.println(F("No IMU"));
   delay(3000);
  }
  
  myIMU.enableRotationVector(REPORT_INTERVAL_MS); //Send data update every n milliseconds

  Serial.println(F("i, j, k, real, accuracy"));
}

void(* resetFunc) (void) = 0;//declare reset function at address 0



int _timeout_count = 0;
int _report_count = 0;

void loop()
{
  //Look for reports from the IMU
  if (myIMU.dataAvailable()) {
    _report_count++;
    _timeout_count = 0;

    if (_report_count > 9) {
      float quatI = myIMU.getQuatI();
      float quatJ = myIMU.getQuatJ();
      float quatK = myIMU.getQuatK();
      float quatReal = myIMU.getQuatReal();
      float quatRadianAccuracy = myIMU.getQuatRadianAccuracy();
      Serial.println();
      Serial.print(quatI, 2);
      Serial.print(F(","));
      Serial.print(quatJ, 2);
      Serial.print(F(","));
      Serial.print(quatK, 2);
      Serial.print(F(","));
      Serial.print(quatReal, 2);
      Serial.print(F(","));
      Serial.print(quatRadianAccuracy, 2);
      Serial.print(F(","));
      Serial.println();
      _report_count = 0;
    }
    else {
      //Serial.print(_report_count);
      //Serial.print(F(" "));
    }

  }
  else {
    if (_timeout_count > 20) {
      Serial.println(F("no more data??"));
      Serial.println();
      delay(1000);
      resetFunc();
    }
  }

  delay(REPORT_INTERVAL_MS);

}
