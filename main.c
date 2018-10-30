/*
 * main.c
 *
 *  Created on: Oct 23, 2018
 *      Author: msolaro
 */
#include "uart.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"
#include "open_interface.h"
#include "movement.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "servo.h"
#include "ping.h"
#include "ir.h"

int main(void)
{
    //configurations and inits
    lcd_init();
    GPIO_init();
    PING_init();
    servo_init();
    //calibrate();
    ADC_init();

    int direction = -1;
    double degrees = 90;
    int lcd_update = 0;
    float voltage = 0;
    int ir_distance = 0;
    while (1)
    {
        //sweep the servo
        move_servoButton(direction,2);
        if(degrees == 179 && direction ==1)
            direction =-1;
        if(degrees ==0 && direction ==-1)
            direction =1;

        if(direction == -1)
            degrees=degrees-2;
        else
            degrees+= 2;


        //calibrates ir


        voltage = ADC_read2(0);
        ir_distance = IR_Convert(voltage);

        if (lcd_update >1)
        {
            IntRegister(INT_TIMER3B, TIMER3B_Handler);
            int distance = ping_read() & 0xFFFFFFFF;

            lcd_printf(
                    "Ping Distance: %dcm\nIR Distance: %dcm\nAngle: %.0lf",
                    distance,ir_distance,degrees);
            lcd_update = 0;
        }
        lcd_update++;
        timer_waitMillis(50);
    }

}
