/******************************************************************************/
/*                                                                            */
/*  Description: LCD library (source)                                         */
/*                                                                            */
/*  Authors: Francesc Bisquerra Castell y Maria Calzada Gonzalez              */
/*                                                                            */
/******************************************************************************/

#include "LEDlib.h"

#include <p30F4011.h>

/******************************************************************************/
/* Pre-processor directives                                                   */
/******************************************************************************/

// LED config registers (I/O)
#define LED0_TRIS TRISEbits.TRISE0
#define LED1_TRIS TRISEbits.TRISE1
#define LED2_TRIS TRISEbits.TRISE2
#define LED3_TRIS TRISEbits.TRISE3
#define LED4_TRIS TRISEbits.TRISE4
#define LED5_TRIS TRISEbits.TRISE5

// LED write registers (value)
#define LED0 LATEbits.LATE0
#define LED1 LATEbits.LATE1
#define LED2 LATEbits.LATE2
#define LED3 LATEbits.LATE3
#define LED4 LATEbits.LATE4
#define LED5 LATEbits.LATE5

// LEDs states
unsigned char ledsState = 0;


/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

// Initialize LED
// This function config. the port as output 
void LEDInit() {

    LED0_TRIS = 0;
    LED1_TRIS = 0;
    LED2_TRIS = 0;
    LED3_TRIS = 0;
    LED4_TRIS = 0;
    LED5_TRIS = 0;
}

// Turn on the LED selected (0-5)
void onLED(unsigned int led) {

    switch(led) {

        case 0: LED0 = 1;
        break;
        case 1: LED1 = 1;
        break;
        case 2: LED2 = 1;
        break;
        case 3: LED3 = 1;
        break;
        case 4: LED4 = 1;
        break;
        case 5: LED5 = 1;
        break;
    }
}

// Turn off the LED selected (0-5)
void offLED(unsigned int led) {

    switch(led) {

        case 0: LED0 = 0;
        break;
        case 1: LED1 = 0;
        break;
        case 2: LED2 = 0;
        break;
        case 3: LED3 = 0;
        break;
        case 4: LED4 = 0;
        break;
        case 5: LED5 = 0;
        break;
    }
}

void toggleLED(unsigned char numLED)
{
	unsigned char maskLed;

	if (numLED > 5) return;

	maskLed = 0b00000001 << numLED;

	if ( (maskLed & ledsState) ) // Led is switched on
	{
		offLED(numLED);
		ledsState = ledsState & (~maskLed);
	}
	else
	{
		onLED(numLED);
		ledsState = ledsState | maskLed;
	}

}
