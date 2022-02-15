#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP3008.h>
#include "movement.h"
Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;
const unsigned int ADC_1_CS = A3;
const unsigned int ADC_2_CS = A2;
const unsigned int RF_CS = A4;
const unsigned int M1_IN_1 = 2;
const unsigned int M1_IN_2 = 3;
const unsigned int M2_IN_1 = 5;
const unsigned int M2_IN_2 = 4;
const unsigned int freq = 5000;
const unsigned int M1_1_channel = 0;
const unsigned int M1_2_channel = 1;
const unsigned int M2_1_channel = 2;
const unsigned int M2_2_channel = 3;
const unsigned int resolution = 8;
const unsigned int M1_I_SENSE = A1;
const unsigned int M2_I_SENSE = A0;
const float M_I_COUNTS_TO_A = (3.3 / 1024.0) / 0.120;
const unsigned int PWM_VALUE = 255;
unsigned int motor1=40;//left?
unsigned int motor2=40; //right?
const unsigned int MAX_PWM = 50;



void setup() {
  all_stop();
  // put your setup code here, to run once:
  Serial.begin(115200);
  // pinMode(M1_IN_1, OUTPUT);
  // pinMode(M1_IN_2, OUTPUT);
  // pinMode(M2_IN_1, OUTPUT);
  // pinMode(M2_IN_2, OUTPUT);
  
  ledcSetup(M1_1_channel, freq, resolution);
  ledcSetup(M1_2_channel, freq, resolution);
  ledcSetup(M2_1_channel, freq, resolution);
  ledcSetup(M2_2_channel, freq, resolution);

  ledcAttachPin(M1_IN_1, M1_1_channel);
  ledcAttachPin(M1_IN_2, M1_2_channel);
  ledcAttachPin(M2_IN_1, M2_1_channel);
  ledcAttachPin(M2_IN_2, M2_2_channel);

  pinMode(RF_CS, OUTPUT);
  digitalWrite(RF_CS, HIGH); // Without this the nRF24 will write to the SPI bus
                             // while the ADC's are also talking
  adc1.begin(ADC_1_CS);  
  adc2.begin(ADC_2_CS);
  int savedadc[13];
  delay(2000);
  all_stop();
  delay(2000);
}

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
  delay(5);
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

void loop() {
  int savedadc[13];
  int check=0;
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
  all_forward(); // using function defined in movement_functions.cpp
  //MoveForward();
  lineCorrection(savedadc);
  
}
