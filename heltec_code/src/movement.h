#ifndef MOVEMENT_H 
#define MOVEMENT_H
#include <Adafruit_MCP3008.h>

void left_forward(int PWM);
void left_backward(int PWM);
void left_stop();

void right_forward(int PWM);
void right_backward(int PWM);
void right_stop();

void all_stop();
void turn_left();
void turn_right();
void turn_around();

// PID Movement
void PID_turn_left(int PWM);
void PID_turn_right(int PWM);
void move_straight(double distance, Adafruit_MCP3008& adc1,Adafruit_MCP3008& adc2);

#endif 