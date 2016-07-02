/******************************************************************************/
/*                                                                            */
/*  Description: KEYBOARD library (source)                                    */
/*                                                                            */
/*  Authors: Francesc Bisquerra Castell y Maria Calzada Gonzalez              */
/*                                                                            */
/******************************************************************************/
#include <p30F4011.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "KEYBlib.h"
#include "delay.h"

/******************************************************************************/
/* Pre-processor directives                                                   */
/******************************************************************************/

//column register TRISiguration
#define COL0_TRIS TRISBbits.TRISB0
#define COL1_TRIS TRISBbits.TRISB1
#define COL2_TRIS TRISBbits.TRISB2

//file register TRISiguration
#define ROW0_TRIS TRISDbits.TRISD0
#define ROW1_TRIS TRISDbits.TRISD1
#define ROW2_TRIS TRISDbits.TRISD2
#define ROW3_TRIS TRISDbits.TRISD3

//column write register
#define COL0 LATBbits.LATB0
#define COL1 LATBbits.LATB1
#define COL2 LATBbits.LATB2
//file read register
#define ROW0 PORTDbits.RD0
#define ROW1 PORTDbits.RD1
#define ROW2 PORTDbits.RD2
#define ROW3 PORTDbits.RD3

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

// Initialize KERBOARD
// This function config. the colums as output and the rows as input

void KeyInit(){

    COL0_TRIS = 0; 
    COL1_TRIS = 0;
    COL2_TRIS = 0;

    ROW0_TRIS = 1; 
    ROW1_TRIS = 1;
    ROW2_TRIS = 1;
    ROW3_TRIS = 1;

}

//Get the Key
unsigned char getKey (){

    ROW0_TRIS = 1; //Se inicializa como 1 confiigurandolo como input
    ROW1_TRIS = 1;
    ROW2_TRIS = 1;
    ROW3_TRIS = 1;

    while (1)
    {
        COL0_TRIS = 0;
        COL1_TRIS = 1;
        COL2_TRIS = 1;

        COL0 = 0;
        COL1 = 1;
        COL2 = 1;

        Delay5ms();
        Delay5ms();

        if(ROW0 == 0) return 0;
        if(ROW1 == 0) return 3;
        if(ROW2 == 0) return 6;
        if(ROW3 == 0) return 9;

        COL0_TRIS = 1;
        COL1_TRIS = 0;
        COL2_TRIS = 1;

        COL0 = 1;
        COL1 = 0;
        COL2 = 1;

        Delay5ms();
        Delay5ms();

        if(ROW0 == 0) return 1;
        if(ROW1 == 0) return 4;
        if(ROW2 == 0) return 7;
        if(ROW3 == 0) return 10;

        COL0_TRIS = 1;
        COL1_TRIS = 1;
        COL2_TRIS = 0;

        COL0 = 1;
        COL1 = 1;
        COL2 = 0;

        Delay5ms();
        Delay5ms();

        if(ROW0 == 0) return 2;
        if(ROW1 == 0) return 5;
        if(ROW2 == 0) return 8;
        if(ROW3 == 0) return 11;
    }

    COL0_TRIS = 1; 
    COL1_TRIS = 1;
    COL2_TRIS = 1;

}

unsigned char getKeyNotBlock(){

    ROW0_TRIS = 1; //Se inicializa como 1 configurandolo como input
    ROW1_TRIS = 1;
    ROW2_TRIS = 1;
    ROW3_TRIS = 1;

    COL0_TRIS = 0;
    COL1_TRIS = 1;
    COL2_TRIS = 1;

    COL0 = 0;
    COL1 = 1;
    COL2 = 1;

    Delay5ms();
    Delay5ms();

    if(ROW0 == 0) return 0;
    if(ROW1 == 0) return 3;
    if(ROW2 == 0) return 6;
    if(ROW3 == 0) return 9;

    COL0_TRIS = 1;
    COL1_TRIS = 0;
    COL2_TRIS = 1;

    COL0 = 1;
    COL1 = 0;
    COL2 = 1;

    Delay5ms();
    Delay5ms();

    if(ROW0 == 0) return 1;
    if(ROW1 == 0) return 4;
    if(ROW2 == 0) return 7;
    if(ROW3 == 0) return 10;

    COL0_TRIS = 1;
    COL1_TRIS = 1;
    COL2_TRIS = 0;

    COL0 = 1;
    COL1 = 1;
    COL2 = 0;

    Delay5ms();
    Delay5ms();

    if(ROW0 == 0) return 2;
    if(ROW1 == 0) return 5;
    if(ROW2 == 0) return 8;
    if(ROW3 == 0) return 11;

    COL0_TRIS = 1; 
    COL1_TRIS = 1;
    COL2_TRIS = 1;

    return -1;
}

//unsigned char * getMultiKey(){
//
//    ROW0_TRIS = 1; //Se inicializa como 1 confiigurandolo como input
//    ROW1_TRIS = 1;
//    ROW2_TRIS = 1;
//    ROW3_TRIS = 1;
//
//    unsigned char * values = '\0';
//
//    COL0 = 0;
//    COL1 = 1;
//    COL2 = 1;
//
//    Delay5ms();
//    Delay5ms();
//
//    if(ROW0 == 1) strcat(values, "0");
//    if(ROW1 == 1) strcat(values, "3");
//    if(ROW2 == 1) strcat(values, "6");
//    if(ROW3 == 1) strcat(values, "9");
//
//    COL0 = 1;
//    COL1 = 0;
//    COL2 = 1;
//
//    Delay5ms();
//    Delay5ms();
//
//    if(ROW0 == 1) strcat(values, "1");
//    if(ROW1 == 1) strcat(values, "4");
//    if(ROW2 == 1) strcat(values, "7");
//    if(ROW3 == 1) strcat(values, "10");;
//
//    COL0 = 1;
//    COL1 = 1;
//    COL2 = 0;
//
//    Delay5ms();
//    Delay5ms();
//
//    if(ROW0 == 1) strcat(values, "2");
//    if(ROW1 == 1) strcat(values, "5");
//    if(ROW2 == 1) strcat(values, "8");
//    if(ROW3 == 1) strcat(values, "11");;
//
//    COL0_TRIS = 1; //Se inicializa a 1 confiigurandolo como input
//    COL1_TRIS = 1;
//    COL2_TRIS = 1;
//    return -1;
//}


