#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP3008.h>
#include "movement.h"
Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;
const unsigned int ADC_1_CS = 2;
const unsigned int ADC_2_CS = 17;

const unsigned int left_IN_1 = 13;
const unsigned int left_IN_2 = 12;
const unsigned int right_IN_1 = 25;
const unsigned int right_IN_2 = 14;

const unsigned int freq = 5000;
const unsigned int left_1_channel = 1;
const unsigned int left_2_channel = 2;
const unsigned int right_1_channel = 3;
const unsigned int right_2_channel = 4;
const unsigned int resolution = 8;

const float M_I_COUNTS_TO_A = (3.3 / 1024.0) / 0.120;

const unsigned int PWM_VALUE = 255;
unsigned int motor1=40;//left?
unsigned int motor2=40; //right?
const unsigned int MAX_PWM = 50;




void lineCorrection(int (&sensor)[13]) {
  int avg = (sensor[0]*(-6)+sensor[1]*(-5)+sensor[2]*(-4)+sensor[3]*(-3)+sensor[4]*(-2)+
	sensor[5]*(-1)+sensor[6]*(0)+sensor[7]*(1)+sensor[8]*(2)+sensor[9]*(3)+
	sensor[10]*(4)+sensor[11]*(5)+sensor[12]*(6))/3;

  if (avg < -1) { // left sensors detecting line so tilt to the left
    motor2 -= avg;
  } else if (avg > 1) { // right sensors detecting line
    motor1 += avg;
  }

  if (motor1 > MAX_PWM || motor2 > MAX_PWM) {
    motor1 -= 5;
    motor2 -= 5;

  }
  // delay(5);
}



int SensorRead(int (& linesense)[13]){
  int amount=0;
  for (int i = 0; i < 8; i++) {
    if((adc1.readADC(i))<700){
    linesense[i] = 1;
    amount++;
    }
    else{
    linesense[i] = 0;
    }
    if(i<6){
      if((adc2.readADC(i))<700){
      linesense[i+8] = 1;
      amount++;
      }
      else{
      linesense[i+8] = 0;
      }
    }
  }
  return(amount);
  }


  void JunctionCheck(int (&sensor)[13]){
    Serial.println("Junction Detected");
    delay(50);
    all_stop(); // using the function defined in movement_functions.cpp
    //StopMotor();
    
    // ignoring loop detection for now
    // implementing a left wall hugging algo
    // at junction, prioritize turns in the following order:
    // left, straight, right
    
    
    // left turn if it exists
    int left = 0;
    for (int i = 0; i < 6; i++) {
      left += sensor[i];
    }
    if (left > 3) {
      turn_left();
      return; // return after turning left
    }

    // going straight if no left turn but straight path exists
    int middle = 0;
    for (int i = 4; i < 8; i++) {
      middle += sensor[i];
    }
    if (middle > 3) {
      // don't need to change heading so just return
      return;
    }

    // going right if no left or straight
    int right = 0;
    for (int i = 0; i < 6; i++) {
      right += sensor[12-i];
    }
    if (right > 3) {
      turn_right();
      return;
    }
    
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // pinMode(left_IN_1, OUTPUT);
  // pinMode(left_IN_2, OUTPUT);
  // pinMode(right_IN_1, OUTPUT);
  // pinMode(right_IN_2, OUTPUT);
  
  ledcSetup(left_1_channel, freq, resolution);
  ledcSetup(left_2_channel, freq, resolution);
  ledcSetup(right_1_channel, freq, resolution);
  ledcSetup(right_2_channel, freq, resolution);

  ledcAttachPin(left_IN_1, left_1_channel);
  ledcAttachPin(left_IN_2, left_2_channel);
  ledcAttachPin(right_IN_1, right_1_channel);
  ledcAttachPin(right_IN_2, right_2_channel);

  adc1.begin(ADC_1_CS);  
  adc2.begin(ADC_2_CS);
  int savedadc[13];
  delay(2000);
  all_stop();
  delay(2000);
}


void loop() {
  int savedadc[13];
  int check=0;

  // all_stop();
  // delay(2000);
  // all_forward();
  // delay(2000);
  // all_stop();
  // put your main code here, to run repeatedly:
  check=SensorRead(savedadc);
  if(check>3){
    JunctionCheck(savedadc);
  }
  else{
    // possible that we've reached a dead end,
    // in which case we need to turn around
    if (check < 2) {
      all_stop();
      delay(10);
      turn_around();
      delay(10);
    }
  }
  all_forward(motor1,motor2); // using function defined in movement_functions.cpp
  lineCorrection(savedadc);
  
}
