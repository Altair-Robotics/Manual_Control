#include <SoftwareSerial.h>
#include "PPMReader.h"
#include "Motor.h"


#define rotateFactor 1.0
#define lateralChannel 3
#define rotateChannel 4

#define armed ch[4] > 1200 and ch[4] < 1600
#define debug true
#define mux_channel ch[4]

#define switch_to_arm ch[4] > 1600

Motor right(6, 5);
Motor left(9, 10);

int ch[8];
// Initialize a PPMReader on digital pin 2 with 8 expected channels.
byte interruptPin = 3;
byte channelAmount = 8;

PPMReader ppm(interruptPin, channelAmount);

int demandx = 0;
int demandz = 0;

void MapRCToPWM() {
  demandx = map(ch[lateralChannel - 1], 1000, 2000, -70, 70);
  demandz = map(ch[rotateChannel - 1], 1000, 2000, -70, 70);
  //  Serial.println(demandx);
  //  Serial.println(demandz);
}

int demand_speed_left = 0;
int demand_speed_right = 0;

SoftwareSerial ss1(A4, A5);  //rx, tx

void setup() {
  Serial.begin(9600);
  ss1.begin(4800);

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

  if (armed)
    drive();
  else
    halt();

  send_to_arm();
}

void debugger() {
  if (debug) {
    Serial.println(demand_speed_left);
    Serial.println(demand_speed_right);
  }
}



void drive() {
  MapRCToPWM();

  demand_speed_left = demandx - (demandz * rotateFactor);
  demand_speed_right = demandx + (demandz * rotateFactor);

  left.rotate(demand_speed_left);
  right.rotate(demand_speed_right);
}

void halt() {
  demand_speed_left = 0;
  demand_speed_right = 0;
  left.rotate(demand_speed_left);
  right.rotate(demand_speed_right);
}

void send_to_arm() {
  String data = "S";
  for (byte channel = 1; channel <= channelAmount; ++channel) {
    if (ch[channel - 1] >= 980 and ch[channel - 1] < 1300)
      data += "1";
    else if (ch[channel - 1] > 1700)
      data += "3";
    else
      data += "2";
  }
  data += "E";
  Serial.println(data);

  ss1.listen();
  ss1.print(data);

  delay(100);
}