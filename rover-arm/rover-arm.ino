#include <SoftwareSerial.h>
#include "Actuator.h"
#include <Servo.h>

#define primaryChannel 3
#define secondaryChannel 2
#define wristChannel 7
#define gripperChannel 8

#define armed data[5] == '3'
#define DEBUG true

Actuator primary(10, 11);
Actuator secondary(3, 5);
Actuator wrist(6, 9);

Servo gripperServo;
#define gripperServoPin 2

String data;
bool received = false;
byte channelAmount = 8;

SoftwareSerial ss1(A4, A5);

void setup() {
  Serial.begin(115200);
  ss1.begin(4800);
  gripperServo.attach(gripperServoPin);

  delay(4000);
}

void loop() {
  receive();
}

void useGripper(int val) {
  if (val == 1)
    gripperServo.writeMicroseconds(1200);
  else if (val == 3)
    gripperServo.writeMicroseconds(1800);
  else
    gripperServo.writeMicroseconds(1500);
}

void manipulate() {
  primary.movement(data[primaryChannel] - '0');
  secondary.movement(data[secondaryChannel] - '0');
  wrist.movement(data[wristChannel] - '0');
  useGripper(data[gripperChannel] - '0');
}

void halt() {
  primary.movement(2);
  secondary.movement(2);
  wrist.movement(2);
}

void receive() {
  data = "";
  received = false;
  while (ss1.available()) {
    data = ss1.readStringUntil('E');
    if (DEBUG)
      Serial.println(data);
    received = true;
  }
  if (DEBUG and !received) {
    // Serial.println("no data recieved");
    // delay(100);
  }
  if (armed) {
    manipulate();
  } else if (!armed)
    halt();
}