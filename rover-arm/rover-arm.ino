#include "Actuator.h"
#include <Servo.h>

Actuator primary(10, 11);
Actuator secondary(9, 8);
Actuator wrist(6, 7);
Actuator gripper(5, 4);


//RC channels:
#define primaryChannel 3
#define secondaryChannel 2
#define wristChannel 7
#define gripperChannel 8

#define armed data[5] == '3'
#define DEBUG true

String data;
bool received = false;
byte channelAmount = 8;


void setup() {
  Serial.begin(9600);
  Serial3.begin(4800);
  Serial.println("started");
}

void loop() {
  receive();
  if (armed) {
    manipulate();
  } else
    halt();
}


void manipulate() {
  primary.movement(data[primaryChannel] - '0');
  secondary.movement(data[secondaryChannel] - '0');
  wrist.movement(data[wristChannel] - '0');
  gripper.movement(data[gripperChannel] - '0');
}

void halt() {
  primary.movement(2);
  secondary.movement(2);
  wrist.movement(2);
  gripper.movement(2);
}

void receive() {
  data = "";
  received = false;
  while (Serial3.available()) {
    data = Serial3.readStringUntil('E');
    if (DEBUG)
      Serial.println(data);
    received = true;
  }
  if (DEBUG and !received) {
    Serial.println("no data recieved");
    delay(100);
  }
}