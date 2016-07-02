/******************************************************************************/
/*                                                                            */
/*  Description: Terminal library (source)                                    */
/*                                                                            */
/*  Authors: Francesc Bisquerra Castell y Maria Calzada Gonzalez              */
/*                                                                            */
/******************************************************************************/

#include "TERMlib.h"

#include "UARTlib.h"
#include <p30F4011.h>

#define ESC 27 // Escape

void TermNewLine() {
    while (BusyUART());
    UARTSendString((unsigned char*) "\n\r \0");
    while (BusyUART());
}

//Cambiar

void TermMove(char row, char col) {
    char row1, row2;
    char col1, col2;

    if (row < 0) return;
    if (col < 0) return;

    // Row
    row1 = (row / 10) + 48;
    row2 = (row % 10) + 48;

    // Col
    col1 = (col / 10) + 48;
    col2 = (col % 10) + 48;

    while (BusyUART());
    WriteUART(ESC);
    while (BusyUART());
    WriteUART('[');
    while (BusyUART());
    WriteUART(row1);
    while (BusyUART());
    WriteUART(row2);
    while (BusyUART());
    WriteUART(';');
    while (BusyUART());
    WriteUART(col1);
    while (BusyUART());
    WriteUART(col2);
    while (BusyUART());
    WriteUART('H');
    while (BusyUART());
}

void TermMoveRight() {
    while (BusyUART());
    WriteUART(ESC);
    while (BusyUART());
    WriteUART('[');
    while (BusyUART());
    WriteUART('C');
    while (BusyUART());
}

void TermCleanLine() {
    while (BusyUART());
    WriteUART(ESC);
    while (BusyUART());
    WriteUART('[');
    while (BusyUART());
    WriteUART('2');
    while (BusyUART());
    WriteUART('K');
    while (BusyUART());
}

void TermPrintGreen() {
    while (BusyUART());
    WriteUART(ESC);
    while (BusyUART());
    WriteUART('[');
    while (BusyUART());
    WriteUART('3');
    while (BusyUART());
    WriteUART('2');
    while (BusyUART());
    WriteUART('m');
    while (BusyUART());
}

void TermPrintCyan() {
    while (BusyUART());
    WriteUART(ESC);
    while (BusyUART());
    WriteUART('[');
    while (BusyUART());
    WriteUART('3');
    while (BusyUART());
    WriteUART('6');
    while (BusyUART());
    WriteUART('m');
    while (BusyUART());
}

void TermPrintYellow() {
    while (BusyUART());
    WriteUART(ESC);
    while (BusyUART());
    WriteUART('[');
    while (BusyUART());
    WriteUART('3');
    while (BusyUART());
    WriteUART('3');
    while (BusyUART());
    WriteUART('m');
    while (BusyUART());
}

void TermPrintRed() {
    while (BusyUART());
    WriteUART(ESC);
    while (BusyUART());
    WriteUART('[');
    while (BusyUART());
    WriteUART('3');
    while (BusyUART());
    WriteUART('1');
    while (BusyUART());
    WriteUART('m');
    while (BusyUART());
}

void TermPrintDefault() {
    //ESC[37;40m
    while (BusyUART());
    WriteUART(ESC);
    while (BusyUART());
    WriteUART('[');
    while (BusyUART());
    WriteUART('3');
    while (BusyUART());
    WriteUART('7');
    while (BusyUART());
    WriteUART('m');
    while (BusyUART());
}

void TermErase() {
    //ESC[2J
    while (BusyUART());
    WriteUART(ESC);
    while (BusyUART());
    WriteUART('[');
    while (BusyUART());
    WriteUART('2');
    while (BusyUART());
    WriteUART('J');
    while (BusyUART());
}

void TermEraseLine() {
    //ESC[2J
    while (BusyUART());
    WriteUART(ESC);
    while (BusyUART());
    WriteUART('[');
    while (BusyUART());
    WriteUART('2');
    while (BusyUART());
    WriteUART('K');
    while (BusyUART());
}

void TermTap() {
    while (BusyUART());
    WriteUART(ESC);
    while (BusyUART());
    WriteUART('[');
    while (BusyUART());
    WriteUART('H');
    while (BusyUART());
}