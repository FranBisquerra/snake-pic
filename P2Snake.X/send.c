/******************************************************************************/
/*                                                                            */
/* Description: Practica 2, simulacion de calculadora                         */
/*                                                                            */
/* Asignatura: Laboratorios de Sistemas Basados en Microcomputador            */
/*                                                                            */
/* Author: Francesc Bisquerra Castell y Maria Calzada Gonzalez                */
/*                                                                            */
/* Fecha: 18. 11. 2015                                                        */
/*                                                                            */
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
/* Configuration words                                                        */
/******************************************************************************/
_FOSC(CSW_FSCM_OFF & EC_PLL16);
_FWDT(WDT_OFF);
_FBORPOR(MCLR_EN & PBOR_OFF & PWRT_OFF);
_FGS(CODE_PROT_OFF);

/******************************************************************************/
/* Global Variable and macros declaration                                     */
/******************************************************************************/
#define SIZE_CAN_MSG 8
unsigned char key;
unsigned char buffer[10];
int number = 0;
int i;

/******************************************************************************/
/* Procedures declaration                                                     */
/******************************************************************************/
void transmit();
void addToBuffer(char c);

/******************************************************************************/
/* Main                                                                       */
/******************************************************************************/
int main(void) {

    //inicializar
    LCDInit();
    KeyInit();
    CANInit(0b000);

    memset(buffer, 0, SIZE_CAN_MSG);

    while (1) {

        key = getKey();
             
        if(number  ==  SIZE_CAN_MSG){
            transmit();
        }
        else{
            switch (key) {
                case 0: addToBuffer('1');
                    break;
                case 1: addToBuffer('2');
                    break;
                case 2: addToBuffer('3');
                    break;
                case 3: addToBuffer('4');
                    break;
                case 4: addToBuffer('5');
                    break;
                case 5: transmit();
                    break;
                case 6: addToBuffer('E');
                    break;
                case 7: addToBuffer('U');
                    break;
                case 8: addToBuffer('C');
                    break;
                case 9: addToBuffer('L');
                    break;
                case 10: addToBuffer('D');
                    break;
                case 11: addToBuffer('R');
                    break;
            }
        }
        for (i = 0; i < 40; i++) Delay5ms();
        
        LCDClear();
        LCDPrint(buffer);
    }
    return 0;
}

void transmit() {
    while (C1TX0CONbits.TXREQ != 0) {}
    
	sprintf(buffer, "%s", buffer);
    CANsendMessage(1, buffer, (unsigned char) number);
    
    number = 0;
    memset(buffer, 0, SIZE_CAN_MSG);
}

void addToBuffer(char c) { 
    buffer[number] = c;
    number++;
}

