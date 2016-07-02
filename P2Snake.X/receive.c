/******************************************************************************/
/* */
/* Description: Practica 2, simulacion de calculadora */
/* */
/* Asignatura: Laboratorios de Sistemas Basados en Microcomputador */
/* */
/* Author: Francesc Bisquerra Castell y Maria Calzada Gonzalez */
/* */
/* Fecha: 30. 09. 2015 */
/* */
/******************************************************************************/

#include <p30F4011.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LCDlib.h"
#include "KEYBlib.h"
#include "CANlib.h"
#include "delay.h"

/******************************************************************************/
/* Configuration words */
/******************************************************************************/
_FOSC(CSW_FSCM_OFF & EC_PLL16);
_FWDT(WDT_OFF);
_FBORPOR(MCLR_EN & PBOR_OFF & PWRT_OFF);
_FGS(CODE_PROT_OFF);

/******************************************************************************/
/* Global Variable and macros declaration                                     */
/******************************************************************************/
#define SIZE_CAN_MSG 8
#define SIZE_RECV 20

char recvMsg [SIZE_RECV];
char * data;
unsigned char i = 0;

/******************************************************************************/
/* Procedures declaration                                                     */

/******************************************************************************/
void _ISR _C1Interrupt(void) {

	memset(recvMsg, 0, SIZE_RECV);

    if (C1INTFbits.RX0IF == 1) {

    	data = (char *)&C1RX0B1;

    	for (i = 0; i < C1TX0DLCbits.DLC; ++i){

			recvMsg[i] = data[i];

			switch(recvMsg[i]){
				case  'E' : LCDMoveLeft(); LCDPrint(" "); LCDMoveLeft(); break; //erase
	            case  'U' : LCDMoveFirstLine(); break; //up
	            case  'C' : LCDClear(); break; //clear
	            case  'L' : LCDMoveLeft(); break; //left
	            case  'D' : LCDMoveSecondLine(); break; //down
	            case  'R' : LCDMoveRight(); break; //right
				
				default: 	LCDPrint(&recvMsg[i]);
			}
		}    
        C1RX0CONbits.RXFUL = 0;	// Clear rx buffer 0      
        C1INTFbits.RX0IF = 0;	// Clear CAN reception buffer 0 interrupt
    }
    IFS1bits.C1IF = 0;	// Clear CAN interrupt

    memset(data, 0, SIZE_RECV);
}


/******************************************************************************/
/* Main                                                                       */

/******************************************************************************/

int main(void) {

    CANInit(0b000);
    LCDInit();
    KeyInit();

    while (1) {

		for (i = 0; i<40; i ++){
			Delay5ms();
		}
    }

}