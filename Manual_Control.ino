#include <SoftwareSerial.h>
#include "PPMReader.h"
#include "Motor.h"
#include "Actuator.h"
#include <Servo.h>

#define rotateFactor 1.0
#define lateralChannel 3
#define rotateChannel 4

#define rover_armed ch[4] > 1200 and ch[4] < 1600
#define debug true
#define mux_channel ch[4]

#define switch_to_arm ch[4] > 1600


#define primaryChannel 3     //4th
#define secondaryChannel 2   //3rd
#define wristPitchChannel 7  //1st
#define wristRollChannel 6   //1st
#define gripperChannel 8

#define gripperServoPin 13


#define arm_armed ch[4] > 1700 and ch[4] < 2100
#define DEBUG true

Actuator primary(26, 27);
Actuator secondary(28, 29);
Actuator wrist1(30, 31);
Actuator wrist2(32, 33);

Motor rightF(2, 3);
Motor leftF(4, 5);
Motor rightB(6, 7);
Motor leftB(8, 9);

int ch[8];
// Initialize a PPMReader on digital pin 2 with 8 expected channels.
byte interruptPin = 20;
byte channelAmount = 8;

PPMReader ppm(interruptPin, channelAmount);

int demandx = 0;
int demandz = 0;



Servo gripperServo;

String data;
bool received = false;

void MapRCToPWM() {
  demandx = map(ch[lateralChannel - 1], 1000, 2000, -70, 70);
  demandz = map(ch[rotateChannel - 1], 1000, 2000, -70, 70);
  //  Serial.println(demandx);
  //  Serial.println(demandz);
}

int demand_speed_left = 0;
int demand_speed_right = 0;

SoftwareSerial ss1(A4, A5);

void setup() {
  Serial.begin(115200);
  gripperServo.attach(gripperServoPin);


  delay(2000);
}

void loop() {
  readRC();
  debugger();
}

void readRC() {
  for (byte channel = 1; channel <= channelAmount; ++channel) {
    ch[channel - 1] = ppm.latestValidChannelValue(channel, 0);
    Serial.print(ch[channel - 1]);
    Serial.print(" ");
    // delay(100);
  }
  Serial.println();

  if (rover_armed)
    drive();
  else if (arm_armed)
    manipulate();
  else
    halt();
}

void debugger() {
  if (debug) {
    Serial.println(demand_speed_left);
    Serial.println(demand_speed_right);
  }
}


void useGripper(int val) {
  Serial.println(val);
  if (val == 1)
    gripperServo.writeMicroseconds(50);
  else if (val == 3)
    gripperServo.writeMicroseconds(1500);
  else
    gripperServo.writeMicroseconds(1700);
}

void manipulate() {
  String data = "S";
  for (byte channel = 1; channel <= channelAmount; ++channel) {
    if (ch[channel - 1] >= 980 and ch[channel - 1] < 1300)
      data += "1";
    else if (ch[channel - 1] > 1400 and ch[channel - 1] < 1600)
      data += "2";
    else if (ch[channel - 1] > 1700)
      data += "3";
    else data += "0";
  }
  data += "E";
  Serial.println(data);
  primary.movement(data[primaryChannel] - '0');
  secondary.movement(data[secondaryChannel] - '0');
  wrist1.movement(data[wristPitchChannel] - '0');
  wrist2.movement(data[wristPitchChannel] - '0');


  useGripper(data[gripperChannel] - '0');
}


void drive() {
  MapRCToPWM();

  demand_speed_left = demandx - (demandz * rotateFactor);
  demand_speed_right = demandx + (demandz * rotateFactor);

  leftF.rotate(demand_speed_left);
  leftB.rotate(demand_speed_left);
  rightF.rotate(demand_speed_right);
  rightB.rotate(demand_speed_right);
}

void halt() {
  demand_speed_left = 0;
  demand_speed_right = 0;
  leftF.rotate(demand_speed_left);
  leftB.rotate(demand_speed_left);
  rightF.rotate(demand_speed_right);
  rightB.rotate(demand_speed_right);
  primary.movement(2);
  secondary.movement(2);
  wrist1.movement(2);
  wrist2.movement(2);
}
