//written by rakinrkz
//this motor driver class is written for IBT2 or BTS 7960 motor driver


#include "Arduino.h"

class Actuator {
  public:
    //Constructor : the 2 pwm pins of ibt2 motor driver
    Actuator(int pwm_f, int pwm_b );
    //Spin the motor with a percentage value
    void movement(int value);
    //Motor driver inputs: f is forward and b is backward
    int pwm_f;
    int pwm_b;
};

