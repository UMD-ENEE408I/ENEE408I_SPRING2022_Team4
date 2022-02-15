#ifndef MOVEMENT_H 
#define MOVEMENT_H

void left_forward(int PWM_VALUE);
void left_backward(int PWM_VALUE);
void left_stop();

void right_forward(int PWM_VALUE);
void right_backward(int PWM_VALUE);
void right_stop();

void all_forward(int LEFT_PWM_VALUE, int RIGHT_PWM_VALUE);
void all_stop();

void turn_left();
void turn_right();
void turn_around();

#endif 