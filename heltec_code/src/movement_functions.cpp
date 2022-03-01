#include <Arduino.h>
#include "movement.h"
#include <Encoder.h>
#include <Adafruit_MCP3008.h>
#include <Wire.h>
#include <math.h>

// Motor Constant Values

const unsigned int left_IN_1_CHANNEL = 1;
const unsigned int left_IN_2_CHANNEL = 2;
const unsigned int right_IN_1_CHANNEL = 3;
const unsigned int right_IN_2_CHANNEL = 4;

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

// Encoder Constant Values

const unsigned int left_ENC_A = 39;
const unsigned int left_ENC_B = 38;
const unsigned int right_ENC_A = 37;
const unsigned int right_ENC_B = 36;


void left_backward(int PWM) {
  ledcWrite(left_IN_1_CHANNEL, PWM);
  ledcWrite(left_IN_2_CHANNEL, 0);
}

void left_forward(int PWM) {
  ledcWrite(left_IN_1_CHANNEL, 0);
  ledcWrite(left_IN_2_CHANNEL, PWM);
}

void left_stop() {
  ledcWrite(left_IN_1_CHANNEL, 0);
  ledcWrite(left_IN_2_CHANNEL, 0);
}

void right_backward(int PWM) {
  ledcWrite(right_IN_1_CHANNEL, PWM);
  ledcWrite(right_IN_2_CHANNEL, 0);
}

void right_forward(int PWM) {
  ledcWrite(right_IN_1_CHANNEL, 0);
  ledcWrite(right_IN_2_CHANNEL, PWM);
}

void right_stop() {
  ledcWrite(right_IN_1_CHANNEL, 0);
  ledcWrite(right_IN_2_CHANNEL, 0);
}

void all_stop() {
  left_stop();
  right_stop();
}

void turn_left(){
  int i;
  for(i = 0; i < 10 ;i++){
    left_forward(250);
    right_backward(250);
  
    delay(10);

    left_stop();
    right_stop();

    delay (100);
  }
}

void turn_right(){
  int i;
  for(i = 0; i < 10 ;i++){
    left_backward(250);
    right_forward(250);
  
    delay(10);

    left_stop();
    right_stop();

    delay (100);
  }
}

void turn_around(){
  int i;
  for(i = 0; i < 20 ;i++){
    left_forward(250);
    right_backward(250);
  
    delay(10);

    left_stop();
    right_stop();

    delay (100);
  }
}



/*////////////////
// PID Control//
////////////////

//PID constants
double kp = 2;
double ki = 5;
double kd = 1;
 
unsigned long currentTime, previousTime;
double elapsedTime;
double error1, error2;
double lastError1, lastError2;
double input1,input2, output1,output2, setpoint1, setpoint2;
double cumError1,cumError2, rateError1, rateError2;

void PID_turn_left(int PWM) {

  //Stopping Motors
  all_stop();

  //Encoder Setpoints
  setpoint1 = -255;
  setpoint2 = -192;

  //Initializing the Encoder Values
  //int start_input_left = enc_left.read();
  //int start_input_right = enc_right.read();

  //Initializing the cumulative error
  cumError1 = 0;
  cumError2 = 0;
  
  //Initializing the previous error variables
  lastError1 = 0;
  lastError2 = 0;
  
  //Initializing the previous time
  previousTime = millis();

  //while loop will continue until the error of both combined is less than 20 ticks
  while(error1 > 15 && error2 > 15){
        delay(2);
        currentTime = millis();               //get current time
        elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation
        
       // input1 = enc_left.read();
       // input2 = enc_right.read();

       // error1 = (setpoint1 - (input1 - start_input_left)) * -1;     // determine error (1)
       // error2 = (setpoint2 - (input2 - start_input_right)) * -1;    // determine error (2)
        cumError1 += error1 * elapsedTime;                // compute integral (1) Dont need
        cumError2 += error2 * elapsedTime;                // compute integral (2) Dont need
        rateError1 = (error1 - lastError1)/elapsedTime;   // compute derivative (1)
        rateError2 = (error2 - lastError2)/elapsedTime;   // compute derivative (2)

        double output1 = kp*error1 + ki*cumError1 + kd*rateError1;                //PID output (1)   
        double output2 = kp*error2 + ki*cumError2 + kd*rateError2;                //PID output (2)          

        lastError1 = error1;                                //remember current error (1)
        lastError2 = error2;                                //remember current error (2)

        previousTime = currentTime;                        //remember current time
        
        if(output1 > 250){
          output1 = 200;
        }
        if(output2 > 250){
          output2 = 200;
        }

        left_backward((int) output1);
        right_forward((int) output2);
  }
  all_stop();
}
*/
     //////////
     //////////
     //////////
     //////////
////////////////////
    ////////////
        ////
         // 
//-----------------//
//   PID Control   //
//-----------------//

// Completed Flags
bool forward_complete = false;
bool right_turn_complete = false;
bool left_turn_complete = false;
bool motors_stopped = false;

// Motor Values
int left_PWM = 0;
int right_PWM = 0;

//Velocity Target in m/s
float target_velocity = .1;
int capped_speed = 90; //This is the max speed of we want the motors to go. Must be <= 245

// Encoder Vals and Targets
int left_prev_enc = 0;
int right_prev_enc = 0;
int left_current_enc = 0;
int right_current_enc = 0;
int target_left = 0;
int target_right = 0;

// PID Variables
float error_left = 0;
float error_right = 0;
float prev_error_left = 0;
float prev_error_right = 0;
float error_limit = 150; //This prevents the motor trying to correct itself too quickly
float integral_left = 0;
float integral_right = 0;
float deriv_left = 0;
float deriv_right = 0;

//ADC Arrays
int adc1_buf[8];
int adc2_buf[8];
bool array_full[16];
int ADC_Threshold = 680;

// Time
long prev_time = 0;

//Left Right Indicators
int Count_left = 0;
int Count_right = 0;
int Count_far_left = 0;
int Count_far_right = 0;

void check_for_LRS(){
    Count_left = 0;
    Count_right = 0;
    Count_far_left = 0;
    Count_far_right = 0;
    for(int i = 0; i < 7 ; i++){
      if(array_full[i] == true){
        Count_right++;
        if(i < 5){Count_far_right++;}
      }
    }
    for(int i = 6; i < 13;i++){
      if(array_full[i] == true){
        Count_left++;
        if(i > 8){Count_far_left++;}
      }
    }
}

/////                 /////
///// function Starts /////
/////                 /////

void move_straight(double distance, Adafruit_MCP3008& adc1,Adafruit_MCP3008& adc2){
  
  //Creating Encocder Objects
  Encoder enc_left(left_ENC_A, left_ENC_B);
  Encoder enc_right(right_ENC_A, right_ENC_B);
  
  // PID Gain Values
  float Kp_left = 0.23;
  float Ki_left = 0.05;
  float Kd_left = 0.1;

  float Kp_right = 0.2;
  float Ki_right = 0.05;
  float Kd_right = 0.1;

  float PID_output_left = 0;
  float PID_output_right = 0;
  // Initializations
  forward_complete = false;
  left_prev_enc = enc_left.read();
  right_prev_enc = /* -1 * */ enc_right.read();
  prev_time = micros();

  // Setting Targets
  target_left = left_prev_enc;
  target_right = right_prev_enc;

  float final_target_left = distance / (0.032 * M_PI) * 360 * 15 / 16 + left_prev_enc;
  float final_target_right = distance / (0.032 * M_PI) * 360 * 15 / 16 + right_prev_enc;

  Serial.print(final_target_left);
  Serial.print("\t");
  Serial.print(final_target_right);
  Serial.println();

  Serial.print("Gets to the loop");
  Serial.println();
  //Loops until movement forward has been completed (when forward_complete is true)
  while(forward_complete == false){
    Serial.print("Starting Loop");
    Serial.println();
    //Change in Time (seconds)
    long current_time = micros();
    float time_elapsed = ((float) (current_time - prev_time)) / 1000000; 
    prev_time = current_time;
    //Reading Current Encoder Values
    int left_current_enc = enc_left.read();
    int right_current_enc = -1 * enc_right.read();
    //Setting Desired Encoder Targets Based on Set Velocity
    target_left = target_left + (float) (target_velocity * time_elapsed / (0.032 * M_PI) * 360);
    target_right = target_right + (float) (target_velocity * time_elapsed / (0.032 * M_PI) * 360);

    //Beginning PID Calculations

    //Left Side
    error_left = target_left - left_current_enc;
    if(error_left > error_limit){error_left = error_limit;} //Limiting Error so as not to try to correct to quickly
    integral_left = integral_left + error_left * time_elapsed;
    deriv_left = (error_left - prev_error_left) / time_elapsed;
    
    //Right Side
    error_right = target_right - right_current_enc;
    if(error_right > error_limit){error_right = error_limit;} //Limiting Error so as not to try to correct to quickly
    integral_right = integral_right + error_right * time_elapsed;
    deriv_right = (error_right - prev_error_right) / time_elapsed;

    //Output signals (These change each loop iteration)
    float PID_output_left = 0;
    float PID_output_right = 0;
    if(motors_stopped == false){
    PID_output_left = (Kp_left*error_left) + (Ki_left*integral_left) + (Kd_left*deriv_left);
    PID_output_right = (Kp_right*error_right) + (Ki_right*integral_right) + (Kd_right*deriv_right);
    }

    // Setting Motor PWM values
    left_PWM = PID_output_left;
    right_PWM = PID_output_right;

    Serial.print("Before Capping: ");
    Serial.print("\t");
    Serial.print(left_PWM);
    Serial.print("\t");
    Serial.print(right_PWM);
    Serial.println();

     //Capping max motor speeds
    if(left_PWM > capped_speed){
      left_PWM = capped_speed;
    }
    else if(left_PWM < 0){
      left_PWM = 0;
    } 

    if(right_PWM > capped_speed){
      right_PWM = capped_speed;
    }
    else if(right_PWM < 0){
      right_PWM = 0;
    }


    //Updating error Values
    prev_error_left = error_left;
    prev_error_right = error_right;

    //################################//
    // Line Following Adjustment Code //
    //################################//
    for (int i = 0; i < 8; i++) {
      adc1_buf[i] = adc1.readADC(i);
      adc2_buf[i] = adc2.readADC(i);
    }

    //Compressing the ADC Values to true or false (true = line , false = no line)
    int array_full_index = 0;
    for (int i = 0; i < 8; i++){
      if(adc1_buf[i] < ADC_Threshold){
        array_full[array_full_index] = true;
      }
      else{
        array_full[array_full_index] = false;
      }
      array_full_index = array_full_index + 1;
      
      if(adc2_buf[i] < ADC_Threshold){
        array_full[array_full_index] = true;
      }
      else{
        array_full[array_full_index] = false;
      }
      array_full_index = array_full_index + 1;
    }
    
    //Counts the left and right sides of the sensor (Count_left , Count_right)
    check_for_LRS();

    //Detecting whether the line is veering left or right and correcting motor values
    if(Count_far_right >= 1 && Count_far_right <= 3){
      left_PWM = left_PWM + (int) (.03 * (float) left_PWM); //was 10
      right_PWM = right_PWM - (int) (.03 * (float) left_PWM);
    }
    else if(Count_far_left >= 1 && Count_far_left <= 3){
      right_PWM = right_PWM + (int) (.03 * (float) right_PWM);
      left_PWM = left_PWM - (int) (.03 * (float) right_PWM); 
    }

    if(right_PWM < 0){
      right_PWM = 0;
    }
    else if(left_PWM < 0){
      left_PWM = 0;
    }

    Serial.print("Commands to Motors:");
    Serial.print("\t");
    Serial.print(left_PWM);
    Serial.print("\t");
    Serial.print(right_PWM);
    Serial.println();
    

    left_forward(left_PWM);
    right_forward(right_PWM);

    delay(10);
    
    left_current_enc = enc_left.read();
    right_current_enc = /* -1 * */ enc_right.read();

    Serial.print(left_current_enc - final_target_left);
    Serial.print("\t");
    Serial.print(right_current_enc - final_target_right);
    Serial.println();


    Serial.println();
    Serial.print("~~~~~~~~~~~~~~~~~~~~~~~~");
    Serial.println();

    if(left_current_enc >= final_target_left || right_current_enc >= final_target_right){
      forward_complete = true;
    }
  }
  all_stop();
}