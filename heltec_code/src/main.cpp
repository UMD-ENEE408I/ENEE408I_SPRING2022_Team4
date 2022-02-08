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

void setup() {
  Serial.begin(115200);
  
  ledcSetup(left_IN_1_CHANNEL, freq, resolution);
  ledcSetup(left_IN_2_CHANNEL, freq, resolution);
  ledcSetup(right_IN_1_CHANNEL, freq, resolution);
  ledcSetup(right_IN_2_CHANNEL, freq, resolution);

  ledcAttachPin(left_IN_1, left_IN_1_CHANNEL);
  ledcAttachPin(left_IN_2, left_IN_2_CHANNEL);
  ledcAttachPin(right_IN_1, right_IN_1_CHANNEL);
  ledcAttachPin(right_IN_2, right_IN_2_CHANNEL);

  pinMode(left_I_SENSE, INPUT);
  pinMode(right_I_SENSE, INPUT);
}

void loop() {

  all_stop();

  delay(5000);
  
  turn_left();

  delay(5000);

  turn_right();

  delay(5000);

  turn_around();

  //
  //  Original Code
  //
  /*for(int i = 0; i < 500; i++) { 
    int left_I_counts = analogRead(left_I_SENSE);
    int right_I_counts = analogRead(right_I_SENSE);

    Serial.print(left_I_counts);
    Serial.print("\t");
    Serial.print(left_I_counts * M_I_COUNTS_TO_A);
    Serial.print("\t");
    Serial.print(right_I_counts);
    Serial.print("\t");
    Serial.print(right_I_counts * M_I_COUNTS_TO_A);
    Serial.println();
    delay(1);
  }*/

 // delay(5000);
}