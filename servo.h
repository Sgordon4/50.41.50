/*
 * servo.h
 *
 *  Created on: Oct 23, 2018
 *      Author: msolaro
 */

#ifndef SERVO_H_
#define SERVO_H_


void move_servoButton(int direction,double degrees);
//moves the servo in a direction a certain amount of degrees
void move_servo(int direction, double degrees);


void servo_init(void);
//initializes servo

void TIMER1B_Handler(void);
//handler

void sweep(int direction);
#endif /* SERVO_H_ */
