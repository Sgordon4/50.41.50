/**
 * servo.c
 * 
 * Template file for CprE 288 lab 3
 *
 * @author Max Solaro
 * @date 10/23/2018
 */

#include "uart.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"
#include "open_interface.h"
#include "movement.h"
#include <stdbool.h>
#include <stdint.h>
#include "button.h"

//CYBOT 00

// Functions for communiticate between CyBot and Putty (via UART)
// PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1


unsigned int pwm_period = 320000;
volatile unsigned int pwm_match;
void servo_init(void)
{

    //starts system clock for portB?
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1; //clock for timer 1B

    //enable port B pin 5 to work as alternate functions
    GPIO_PORTB_AFSEL_R |= 0x20;
    //enables alterante function
    GPIO_PORTB_PCTL_R |= 0x00700000; //set PB5 to T1CCP1 function using PMC5 = 7
    //sets pin 5 as output
    GPIO_PORTB_DIR_R |= 0x20;

    //enable
    GPIO_PORTB_DEN_R |= 0x20;

    TIMER1_TBMR_R &= ~TIMER_TBMR_TBCMR;

    //disable en while we make cganges
    TIMER1_CTL_R &= ~0x100;
    TIMER1_TBMR_R |= 0x2;
    TIMER1_TBMR_R |= TIMER_TBMR_TBAMS;
    TIMER1_CFG_R = 0x4; //lsat nibble - 1100;

    pwm_match = pwm_period - 24140;

    TIMER1_TBILR_R = pwm_period & 0x0000FFFF; //lower 16 bits of period
    TIMER1_TBPR_R = pwm_period >> 16; //upper 8 bits of period

    TIMER1_TBMATCHR_R = pwm_match & 0x0000FFFF; //upper 16 bits of match value
    TIMER1_TBPMR_R = pwm_match >> 16; //upper 8 bits of match value

    //enable timer
    TIMER1_CTL_R |= 0x00000100;

}

void move_servoButton(int direction, double degrees)
{

    //clockwise rotation
    if (direction == -1)
    {
        if (pwm_match >= 285000)
        {
            if (degrees == 1.0)
            {
                pwm_match -= (27500 / 180);
            }
            else if (degrees == 2.5)
            {
                pwm_match -= (27500 / 72);
            }
            else if (degrees == 5.0)
            {
                pwm_match -= (27500 / 36);
            }
            TIMER1_TBMATCHR_R = pwm_match & 0x0000FFFF; //upper 16 bits of match value
            TIMER1_TBPMR_R = pwm_match >> 16; //upper 8 bits of match value

            TIMER1_TBILR_R = pwm_period & 0x0000FFFF; //lower 16 bits of period
            TIMER1_TBPR_R = pwm_period >> 16; //upper 8 bits of period
        }
        else
        {
            pwm_match = pwm_period - 35000;

            TIMER1_TBMATCHR_R = pwm_match & 0x0000FFFF; //upper 16 bits of match value
            TIMER1_TBPMR_R = pwm_match >> 16; //upper 8 bits of match value
            TIMER1_TBILR_R = pwm_period & 0x0000FFFF; //lower 16 bits of period
            TIMER1_TBPR_R = pwm_period >> 16; //upper 8 bits of period
        }

    }

    //counterclockwise rotation
    if (direction == 1)
    {
        if (pwm_match <= 312500)
        {
            if (degrees == 1.0)
            {
                pwm_match += (27500 / 180);
            }
            else if (degrees == 2.5)
            {
                pwm_match += (27500 / 72);
            }
            else if (degrees == 5.0)
            {
                pwm_match += (27500 / 36);
            }

            TIMER1_TBMATCHR_R = pwm_match & 0x0000FFFF; //upper 16 bits of match value
            TIMER1_TBPMR_R = pwm_match >> 16; //upper 8 bits of match value
            TIMER1_TBILR_R = pwm_period & 0x0000FFFF; //lower 16 bits of period
            TIMER1_TBPR_R = pwm_period >> 16; //upper 8 bits of period

        }
        else
        {
            pwm_match = pwm_period - 7500;

            TIMER1_TBMATCHR_R = pwm_match & 0x0000FFFF; //upper 16 bits of match value
            TIMER1_TBPMR_R = pwm_match >> 16; //upper 8 bits of match value
            TIMER1_TBILR_R = pwm_period & 0x0000FFFF; //lower 16 bits of period
            TIMER1_TBPR_R = pwm_period >> 16; //upper 8 bits of period
        }

    }

}

void move_servo(int direction, double degrees)
{
    //counterclockwise rotation
    if (direction == 1)
    {
        if (pwm_match <= 312500)

           pwm_match += (27500 / 180);

        TIMER1_TBMATCHR_R = pwm_match & 0x0000FFFF; //upper 16 bits of match value
        TIMER1_TBPMR_R = pwm_match >> 16; //upper 8 bits of match value
        TIMER1_TBILR_R = pwm_period & 0x0000FFFF; //lower 16 bits of period
        TIMER1_TBPR_R = pwm_period >> 16; //upper 8 bits of period

    }
    else
    {
        pwm_match = pwm_period - 7500;

        TIMER1_TBMATCHR_R = pwm_match & 0x0000FFFF; //upper 16 bits of match value
        TIMER1_TBPMR_R = pwm_match >> 16; //upper 8 bits of match value
        TIMER1_TBILR_R = pwm_period & 0x0000FFFF; //lower 16 bits of period
        TIMER1_TBPR_R = pwm_period >> 16; //upper 8 bits of period
    }

    if (direction == -1)
    {
        if (pwm_match >= 285000)


            pwm_match -= (27500 / 180);

        TIMER1_TBMATCHR_R = pwm_match & 0x0000FFFF; //upper 16 bits of match value
        TIMER1_TBPMR_R = pwm_match >> 16; //upper 8 bits of match value

        TIMER1_TBILR_R = pwm_period & 0x0000FFFF; //lower 16 bits of period
        TIMER1_TBPR_R = pwm_period >> 16; //upper 8 bits of period
    }
    else
    {
        pwm_match = pwm_period - 35000;

        TIMER1_TBMATCHR_R = pwm_match & 0x0000FFFF; //upper 16 bits of match value
        TIMER1_TBPMR_R = pwm_match >> 16; //upper 8 bits of match value
        TIMER1_TBILR_R = pwm_period & 0x0000FFFF; //lower 16 bits of period
        TIMER1_TBPR_R = pwm_period >> 16; //upper 8 bits of period
    }
}

void TIMER1B_Handler(void)
{
    if(TIMER1_MIS_R &TIMER_MIS_TBMMIS)//was TAMMIS
    {
        //clears the interrupt flag
        TIMER1_ICR_R = TIMER1_ICR_R |TIMER_MIS_TBMMIS; // added '_R' to TIMER1_ICR

        //set next match time
        //TIMER1_TBMATCHR_R = TIMER1_TBMATCHR_R - M; // wat is M
        if(GPIO_PORTB_DATA_R & 0x20)
        {
            GPIO_PORTB_DATA_R &= ~0x20;
        }
        else
        {
            GPIO_PORTB_DATA_R |= 0x20;
        }
    }
}

void sweep(int direction)
{
    if (direction == 1)
    {
        if (pwm_match <= 312500)

            pwm_match += 27500;

        TIMER1_TBMATCHR_R = pwm_match & 0x0000FFFF; //upper 16 bits of match value
        TIMER1_TBPMR_R = pwm_match >> 16; //upper 8 bits of match value
        TIMER1_TBILR_R = pwm_period & 0x0000FFFF; //lower 16 bits of period
        TIMER1_TBPR_R = pwm_period >> 16; //upper 8 bits of period

    }
    else
    {
        pwm_match = pwm_period - 7500;

        TIMER1_TBMATCHR_R = pwm_match & 0x0000FFFF; //upper 16 bits of match value
        TIMER1_TBPMR_R = pwm_match >> 16; //upper 8 bits of match value
        TIMER1_TBILR_R = pwm_period & 0x0000FFFF; //lower 16 bits of period
        TIMER1_TBPR_R = pwm_period >> 16; //upper 8 bits of period
    }

    if (direction == -1)
    {
        if (pwm_match >= 285000)

            pwm_match += 27500/180;
        TIMER1_TBMATCHR_R = pwm_match & 0x0000FFFF; //upper 16 bits of match value
        TIMER1_TBPMR_R = pwm_match >> 16; //upper 8 bits of match value

        TIMER1_TBILR_R = pwm_period & 0x0000FFFF; //lower 16 bits of period
        TIMER1_TBPR_R = pwm_period >> 16; //upper 8 bits of period
    }
    else
    {
        pwm_match = pwm_period - 35000;

        TIMER1_TBMATCHR_R = pwm_match & 0x0000FFFF; //upper 16 bits of match value
        TIMER1_TBPMR_R = pwm_match >> 16; //upper 8 bits of match value
        TIMER1_TBILR_R = pwm_period & 0x0000FFFF; //lower 16 bits of period
        TIMER1_TBPR_R = pwm_period >> 16; //upper 8 bits of period
    }

}
