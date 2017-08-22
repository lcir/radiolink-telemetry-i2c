#include <Wire.h>

boolean set = false;

int32_t MwAltitude = 20;
int32_t MwHeading = 0;
int32_t GPS_speed = 0;
int16_t MwAngle[2] = {0, 0};

int32_t GPS_latitude;
int32_t GPS_longitude;

uint32_t GPS_distanceToHome = 0;
uint8_t GPS_numSat = 0;

uint16_t voltage = 5;


void setup() {
  Wire.begin(4);
  Wire.onRequest(onRequestRLTelemetry);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void set1() {

  int  alt = MwAltitude / 10;
  byte  altHi = highByte(alt);
  byte  altLo =  lowByte(alt);

  int  yaw = MwHeading * 100;
  byte yawHi = highByte(yaw);
  byte yawLo = lowByte(yaw);

  uint16_t  speed2 =  GPS_speed * 1000;
  byte speedHi = highByte(speed2);
  byte speedLo = lowByte(speed2);

  int  roll =  MwAngle[0];
  byte rollHi = highByte(roll);
  byte rollLo = lowByte(roll);

  int  pitch =  MwAngle[1];
  byte pitchHi = highByte(pitch);
  byte pitchLo = lowByte(pitch);

  int distance = GPS_distanceToHome * 10;
  byte distanceHi = highByte(distance);
  byte distanceLo = lowByte(distance);

  byte buffer[16] = {
    0x89, 0xAB,
    GPS_numSat,
    altHi, altLo,
    yawHi, yawLo,
    speedHi, speedLo,
    rollHi , rollLo,
    pitchHi, pitchLo,
    distanceHi, distanceLo,
    0x00
  };
  Wire.write(buffer, 16);
}

void set2() {

  union u32_tag  {
    byte  b[4];
    uint32_t ui32;
  } latit, longt;

  int  rise = 10;
  byte  riseHi = highByte(rise);
  byte  riseLo =  lowByte(rise);

  int battVoltage = voltage * 100; //15000; // 15V
  byte voltesHi = highByte(battVoltage);
  byte voltesLo = lowByte(battVoltage);

  longt.ui32 = GPS_longitude;
  latit.ui32 = GPS_latitude;

  byte buffer[16] = {
    0x89, 0xCD,
    GPS_numSat,
    riseHi, riseLo,
    voltesHi, voltesLo,
    longt.b[3], longt.b[2], longt.b[1] , longt.b[0],
    latit.b[3], latit.b[2], latit.b[1] , latit.b[0],
    0x00
  };

  Wire.write(buffer, 16);
}


void onRequestRLTelemetry() {
  if (set) {
    set = false;
    set1();
  } else {
    set = true;
    set2();
  }
}
