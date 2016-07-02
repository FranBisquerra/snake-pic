/******************************************************************************/
/*                                                                            */
/*  Description: Uart library (source)                                        */
/*                                                                            */
/*  Authors: Francesc Bisquerra Castell y Maria Calzada Gonzalez              */
/*                                                                            */
/******************************************************************************/

#include "UARTlib.h"
#include <p30F4011.h>

#define FXT 7372800 // Oscillator frequency
#define PLL 16
#define FCY (FXT * PLL) / 4 // Real system frequency

#define BAUD_RATE 115200 // UART bitrate
#define BRG (FCY / (16L * BAUD_RATE)) - 1L

void UARTInit() {
    U1MODE = 0x0000;
    U1MODEbits.UARTEN = 1;
    U1MODEbits.ALTIO = 1;
    U1MODEbits.PDSEL = 9; //Bit de paridad 
    U1MODEbits.STSEL = 1; //Bit de parada
    U1STA = 0x0000;
    U1STAbits.UTXEN = 1;
    U1BRG = BRG;
}

void UARTSendString(unsigned char *buffer) {

   char * temp_ptr = (char *) buffer;

    /* transmit till NULL character is encountered */

    if(U1MODEbits.PDSEL == 3)        /* check if TX is 8bits or 9bits */
    {
        while(*buffer != '\0') 
        {
            while(U1STAbits.UTXBF); /* wait if the buffer is full */
            U1TXREG = *buffer++;    /* transfer data word to TX reg */
        }
    }
    else
    {
        while(*temp_ptr != '\0')
        {
            while(U1STAbits.UTXBF);  /* wait if the buffer is full */
            U1TXREG = *temp_ptr++;   /* transfer data byte to TX reg */
        }
    }

}


void WriteUART(unsigned int data)
{
    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

void UARTSendInt(unsigned int data) {
    U1TXREG = data;
}

char UARTRecv() {
    while (!U1STAbits.URXDA);
    return (U1RXREG);
}

char BusyUART(void)
{  
    return(!U1STAbits.TRMT);
}