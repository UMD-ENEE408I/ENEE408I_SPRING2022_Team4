#include <Arduino.h>
#include "movement.h"

const unsigned int left_IN_1 = 13;
const unsigned int left_IN_2 = 12;
const unsigned int right_IN_1 = 25;
const unsigned int right_IN_2 = 14;

const unsigned int left_IN_1_CHANNEL = 1;
const unsigned int left_IN_2_CHANNEL = 2;
const unsigned int right_IN_1_CHANNEL = 3;
const unsigned int right_IN_2_CHANNEL = 4;

const unsigned int left_I_SENSE = 35;
const unsigned int right_I_SENSE = 34;

const float M_I_COUNTS_TO_A = (3.3 / 1024.0) / 0.120;

const unsigned int PWM_VALUE = 255; // out of 255

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

void left_backward(int PWM_VALUE) {
  ledcWrite(left_IN_1_CHANNEL, PWM_VALUE);
  ledcWrite(left_IN_2_CHANNEL, 0);
}

void left_forward(int PWM_VALUE) {
  ledcWrite(left_IN_1_CHANNEL, 0);
  ledcWrite(left_IN_2_CHANNEL, PWM_VALUE);
}

void left_stop() {
  ledcWrite(left_IN_1_CHANNEL, 0);
  ledcWrite(left_IN_2_CHANNEL, 0);
}

void right_backward(int PWM_VALUE) {
  ledcWrite(right_IN_1_CHANNEL, PWM_VALUE);
  ledcWrite(right_IN_2_CHANNEL, 0);
}

void right_forward(int PWM_VALUE) {
  ledcWrite(right_IN_1_CHANNEL, 0);
  ledcWrite(right_IN_2_CHANNEL, PWM_VALUE);
}

void right_stop() {
  ledcWrite(right_IN_1_CHANNEL, 0);
  ledcWrite(right_IN_2_CHANNEL, 0);
}

void all_forward(int LEFT_PWM_VALUE, int RIGHT_PWM_VALUE) {
  left_forward(LEFT_PWM_VALUE);
  right_forward(RIGHT_PWM_VALUE);
}

void all_stop() {
  left_stop();
  right_stop();
}

void turn_left(){
  int i;
  for(i = 0; i < 10 ;i++){
    left_forward(PWM_VALUE);
    right_backward(PWM_VALUE);
  
    delay(10);

    left_stop();
    right_stop();

    delay (100);
  }
}

void turn_right(){
  int i;
  for(i = 0; i < 10 ;i++){
    left_backward(PWM_VALUE);
    right_forward(PWM_VALUE);
  
    delay(10);

    left_stop();
    right_stop();

    delay (100);
  }
}

void turn_around(){
  int i;
  for(i = 0; i < 20 ;i++){
    left_forward(PWM_VALUE);
    right_backward(PWM_VALUE);
  
    delay(10);

    all_stop();

    delay (100);
  }
}

