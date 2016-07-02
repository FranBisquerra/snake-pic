/******************************************************************************/
/*                                                                            */
/*  Description: CAN library (source)                                         */
/*                                                                            */
/*  Authors: Francesc Bisquerra Castell y Maria Calzada Gonzalez              */
/*                                                                            */
/******************************************************************************/

#include "UARTlib.h"
#include "LCDlib.h"
#include "LEDlib.h"
#include <p30F4011.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void CANInit(unsigned char mode) {
    C1CTRLbits.REQOP = 0b100; // Set configuration mode
    while (C1CTRLbits.OPMODE != 0b100); // Wait until configuration mode
    C1CTRLbits.CANCKS = 1; // FCAN = FCY
    /* BTR */
    // BTR1
    C1CFG1bits.BRP = 1; // 1Mbps --> ((((1*10^-6)/8)*29491200)/2)-1 = 0,8432

    //Controlar velocidad de transmission mediante SJW, PRSEG, SEG1PH, SEG2PH
    C1CFG1bits.SJW = 0; // 1 TQs
    // BTR2
    C1CFG2bits.PRSEG = 0; // 1 TQs
    C1CFG2bits.SEG1PH = 3; // 4 TQs
    C1CFG2bits.SEG2PH = 1; // 2 TQs
    /* Tx buffer 0 */
    C1TX0CONbits.TXREQ = 0; // Clear tx request
    /* Rx buffer 0 */
    C1RX0CONbits.RXFUL = 0; // Clear rx status
    // Disable double buffer
    C1RX0CONbits.DBEN = 0;

    // Configure acceptance masks
    C1RXM0SIDbits.SID = 0; // No bits to compare
    C1RXM0SIDbits.MIDE = 1; // Determine as EXIDE -- permite configurar exide
    // Configure acceptance filters
    C1RXF0SIDbits.SID = 0; // Doesn't matter the value as mask is ?0?
    C1RXF0SIDbits.EXIDE = 0; // Enable filter for standard identifier
    /* Interrupts */
    IEC1bits.C1IE = 1; // Enable global CAN interrupt
    IFS1bits.C1IF = 0; // Clear global CAN interrupt flag
    // Enable Configure interrupts
    C1INTE = 0;
    C1INTEbits.RX0IE = 1; // Enable interrupt associated to rx buffer 0
    C1INTFbits.RX0IF = 0; // Clear interrupt flag associated to rx buffer 0

    C1INTEbits.TX0IE = 1; // Enable interrupt associated to tx buffer 0
    C1INTFbits.TX0IF = 0; // Clear interrupt flag associated to tx buffer 0

    C1CTRLbits.REQOP = mode; // Set normal mode
    while (C1CTRLbits.OPMODE != mode); // Wait until normal mode
}

void CANsendMessage(unsigned int sid, unsigned char * data, unsigned char size) {
    C1TX0DLCbits.TXRTR = 0; // Set data frame
    C1TX0SIDbits.TXIDE = 0; // Set standard identifier
    // Set identifier 1
    C1TX0SIDbits.SID5_0 = sid & 0b0111111; // Set the lowest 6 bits
    C1TX0SIDbits.SID10_6 = (sid & 0b011111000000) >> 6; // Set the highest 5 bits

    C1TX0DLCbits.DLC = size;

    // Set data  
    int i = 0;
    for (i = 0; i < size; i++) {
        *((unsigned char *) &C1TX0B1 + i) = data[i];
    }

    //sprintf(&C1TX0B1, "%s", data);
    C1TX0CONbits.TXREQ = 1; // Transmit
    while (C1TX0CONbits.TXREQ != 0); // Wait for transmission
}