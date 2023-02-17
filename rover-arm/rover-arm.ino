#include <SoftwareSerial.h>
#include "Actuator.h"

#define primaryChannel 3
#define secondaryChannel 2
#define wristChannel 7

#define DEBUG true

Actuator primary(6, 5);
Actuator secondary(9, 10);
Actuator wrist(9, 10);

String data;
bool received = false;
byte channelAmount = 8;

SoftwareSerial ss1(A4, A5);

void setup() {
  Serial.begin(115200);
  ss1.begin(4800);

  delay(4000);
}

void loop() {
  receive();
  if (received) {
    manipulate();
  } else
    halt();
}

void manipulate() {
  primary.movement(data[primaryChannel] - '0');
  secondary.movement(data[secondaryChannel] - '0');
  wrist.movement(data[wristChannel] - '0');
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
    Serial.println("no data recieved");
    // delay(100);
  }
}