#include <Arduino.h>
#include "movement.h"

const unsigned int left_IN_1 = 2;
const unsigned int left_IN_2 = 3;
const unsigned int right_IN_1 = 5;
const unsigned int right_IN_2 = 4;

const unsigned int left_I_SENSE = A1;
const unsigned int right_I_SENSE = A0;

const float M_I_COUNTS_TO_A = (3.3 / 1024.0) / 0.120;

const unsigned int PWM_VALUE = 255; // out of 255

void left_backward() {
  analogWrite(left_IN_1, PWM_VALUE);
  analogWrite(left_IN_2, 0);
}

void left_forward() {
  analogWrite(left_IN_1, 0);
  analogWrite(left_IN_2, PWM_VALUE);
}

void left_stop() {
  analogWrite(left_IN_1, 0);
  analogWrite(left_IN_2, 0);
}

void right_backward() {
  analogWrite(right_IN_1, PWM_VALUE);
  analogWrite(right_IN_2, 0);
}

void right_forward() {
  analogWrite(right_IN_1, 0);
  analogWrite(right_IN_2, PWM_VALUE);
}

void right_stop() {
  analogWrite(right_IN_1, 0);
  analogWrite(right_IN_2, 0);
}

void all_stop() {
  left_stop();
  right_stop();
}

void turn_left(){
  int i;
  for(i = 0; i < 10 ;i++){
    left_forward();
    right_backward();
  
    delay(10);

    left_stop();
    right_stop();

    delay (100);
  }
}

void turn_right(){
  int i;
  for(i = 0; i < 10 ;i++){
    left_backward();
    right_forward();
  
    delay(10);

    left_stop();
    right_stop();

    delay (100);
  }
}

void turn_around(){
  int i;
  for(i = 0; i < 20 ;i++){
    left_forward();
    right_backward();
  
    delay(10);

    left_stop();
    right_stop();

    delay (100);
  }
}

