#include <Arduino.h>
#include "movement.h"
#include <Encoder.h>
#include <Adafruit_MCP3008.h>
#include <Wire.h>

// Motor Values

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

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

// ADC Values

Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;

extern const unsigned int ADC_1_CS = 2;
extern const unsigned int ADC_2_CS = 17;

// Buzzer Code

const unsigned int BUZZ = 26;
const unsigned int BUZZ_CHANNEL = 0;

const unsigned int octave = 4;

void setup() {
  
  //Shutting Right Motor off
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  delay(100);
  
  Serial.begin(9600);

  // ADC Setup
  
  adc1.begin(ADC_1_CS);  
  adc2.begin(ADC_2_CS);  

  // Motors Setup

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

  ledcAttachPin(BUZZ, BUZZ_CHANNEL);

  all_stop();

}

void loop() {

  all_stop();

  move_straight(1, adc1, adc2);

  delay(5000);
}