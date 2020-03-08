 /*
  External Library Requiremens:
     DualShock4_lib     ->      https://github.com/nikh1508/DualShock4_lib
     PID_v1             ->      https://github.com/br3ttb/Arduino-PID-Library
  Rest of the libraries are included in /src folder
*/

#include <Arduino.h>
#include <Wire.h>
#include <PID_v1.h>
#include <DualShock4_lib.h>
#include "src/Encoder/Encoder.h"
#include "src/IMU/IMU.h"
#include "src/HolonomicDrive/HolonomicDrive.h"
#include <Servo.h>
#include<String.h>
#include <PID_Tuner.h>

float kp, ki, kd;
bool power;


Servo myservo;


constexpr int MAX_SPEED = 40;
constexpr int AXIS_DEAD_ZONE = 1500;
constexpr int AXIS_DEAD_ZONE_L = 1500;
int pos, i;
//  Function Prototypes:
void setup();
void loop();
bool checkForRoutines();
void forestRoutine();
void decodeData(byte toDecode[], byte totalRecvd, byte &decodedByteCount, byte decodedBytes[], byte specialByte);
bool ReadBytes(byte data[], byte toRead, char toSend);
void convert(byte toConvert[], unsigned int converted[], long &res);
bool funcData(double values[]);
void Serial3Flush();

constexpr motor front(10, 8, 9);  //DO | D1 | PWM        //CHANGE-HERE
constexpr motor left(4, 2, 3 ); //CHANGE-HERE
constexpr motor right(7, 5, 6);   //CHANGE-HERE

//  PID Constants
constexpr double linearConst[] = {0.0, 0.0, 0.0};          //Kp | Ki | Kd
constexpr double rotationalConst[] = {0.021, 0.015, 0.0032}; //{0.03, 0.135, 0.011} //avg:{0.03, 0.117, 0.018}

HolonomicDrive bot(front, left, right, linearConst, rotationalConst, MAX_SPEED); //Front | Left | Right | Linear Constant | Rotational Constant | Max Speed
DualShock4 ds4(Serial3);

bool powerOn = false;

constexpr int powerLed = 53;
//PIDTuner tuner(&powerOn, &kp, &ki, &kd, Serial3);
int Lx, Ly, Rx, Ry;
