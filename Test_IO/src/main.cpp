#include <Arduino.h>
#include "movement.h"

const unsigned int left_IN_1 = 2;
const unsigned int left_IN_2 = 3;
const unsigned int right_IN_1 = 5;
const unsigned int right_IN_2 = 4;

void setup() {
  pinMode(left_IN_1, OUTPUT);
  pinMode(left_IN_2, OUTPUT);
  pinMode(right_IN_1, OUTPUT);
  pinMode(right_IN_2, OUTPUT);
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