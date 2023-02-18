#include "Arduino.h"
#include "Actuator.h"

Actuator::Actuator(int pwm_f, int pwm_b) {
  pinMode(pwm_f, OUTPUT);
  pinMode(pwm_b, OUTPUT);
  Actuator::pwm_f = pwm_f;  //forward pwm
  Actuator::pwm_b = pwm_b;  //backward pwm

  // digitalWrite(pwm_f, LOW);
  // digitalWrite(pwm_b, LOW);
}

void Actuator::movement(int value) {
  Serial.println(value);
  if (value == 3) {
    digitalWrite(pwm_b, LOW);
    digitalWrite(pwm_f, HIGH);
  } else if (value == 1) {
    digitalWrite(pwm_f, LOW);
    digitalWrite(pwm_b, HIGH);
  } else {
    digitalWrite(pwm_f, LOW);
    digitalWrite(pwm_b, LOW);
  }
}
