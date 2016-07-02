/* 
 * File:   main.c
 * Author: Maria
 *
 * Created on June 24, 2016, 5:06 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p30f4011.h>
#include "TERMlib.h"
#include "LEDlib.h"
#include "KEYBlib.h"
#include "LCDlib.h"
#include "TIMERlib.h"
#include "CANlib.h"
#include "delay.h"
#include "CADlib.h"
#include "BUZZERlib.h"
#include <math.h>
#include <string.h>

/******************************************************************************/
/* Global Variable and macros declaration                                     */
/******************************************************************************/
int key;
unsigned int CADValue = 0;
int SID;

/******************************************************************************/
/* Procedures declaration                                                     */
/******************************************************************************/
void transmit(char c, int sid);

/******************************************************************************/
/* Main                                                                       */
/******************************************************************************/

void _ISR _T1Interrupt(void) {

    toggleLED(1);

    CADValue = CADgetValue();

    switch (CADValue) {

        case 0 ... 255:
            transmit('1', 3);
            break;
        case 256 ... 511:
            transmit('2', 3);
            break;
        case 512 ... 767:
            transmit('3', 3);
            break;
        case 768 ... 1024:
            transmit('4', 3);
            break;
    }

    TIMER1ClearInt();
}

int main(void) {

    LEDInit();
    KeyInit();
    LCDInit();
    CANInit(0);
    CADInit();

    //Initialize Timer
    TIMER1Init(0b11, 11500, 0, 1); //100 miliseconds
    TIMER1Start();
    
    //Initialize CAD
    CADStart();

    while (1) {
        toggleLED(0);

        key = getKeyNotBlock();

        switch (key) {

            case 1:
                transmit('T', 2);
                break;
            case 3:
                transmit('L', 2);
                break;
            case 4:
                transmit('S', 1);
                break;
            case 5:
                transmit('R', 2);
                break;
            case 7:
                transmit('B', 2);
                break;
        }
    }
    return 0;
}

void transmit(char c, int sid) {
    char buffer[3] = {0};
    sprintf(buffer, "%c", c);
    CANsendMessage(sid, buffer, sizeof (buffer));
    memset(buffer, 0, sizeof (buffer));
}

