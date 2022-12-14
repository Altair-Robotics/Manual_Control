#define rotateFactor 1.0
#define lateralChannel 3
#define rotateChannel 4

#include "PPMReader.h"
#include "Motor.h"


#define armed ch[4]>1200
#define debug true

Motor right(7, 6);
Motor left(5, 3);

int ch[8];
// Initialize a PPMReader on digital pin 2 with 8 expected channels.
byte interruptPin = 2;
byte channelAmount = 8;
PPMReader ppm(interruptPin, channelAmount);


int demandx = 0;
int demandz = 0;



void MapRCToPWM() {
  demandx = map(ch[lateralChannel - 1], 1000, 2000, -50, 50);
  demandz = map(ch[rotateChannel - 1], 1000, 2000, -50, 50);
  //  Serial.println(demandx);
  //  Serial.println(demandz);
}

int demand_speed_left = 0;
int demand_speed_right = 0;

void setup() {

  Serial.begin(115200);
  delay(2000);

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

void loop() {

  for (byte channel = 1; channel <= channelAmount; ++channel) {
    ch[channel - 1] = ppm.latestValidChannelValue(channel, 0);
    //    Serial.print(ch[channel - 1]);
    //    Serial.print(" ");
  }
  Serial.println();

  if (armed)
  {
    drive();
  }
  else
    halt();

  if (debug)
  {
    Serial.println(demand_speed_left);
    Serial.println(demand_speed_right);
  }
}
