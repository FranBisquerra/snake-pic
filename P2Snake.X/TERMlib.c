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

void moveCursor (int r, int c){
   
    UARTSendString('27 91 ');
    UARTSendInt(r);
    UARTSendString(' 59 ');
    UARTSendInt(c);
    UARTSendString(' 72 ');
    
}

void TermNewLine()
{
	UARTSendString((unsigned char*)"\n\r \0");
}

//Cambiar
void TermMove(char row, char col)
{
	char row1, row2;
	char col1, col2;

	if(row < 0) return;
	if(col < 0) return;

	// Row
	row1 = (row/10) + 48;
	row2 = (row%10) + 48;

	// Col
	col1 = (col/10) + 48;
	col2 = (col%10) + 48;

    while(BusyUART());
	WriteUART(ESC);  while(BusyUART());
	WriteUART('[');  while(BusyUART());
	WriteUART(row1); while(BusyUART());
	WriteUART(row2); while(BusyUART());
	WriteUART(';');  while(BusyUART());
	WriteUART(col1); while(BusyUART());
	WriteUART(col2); while(BusyUART());
	WriteUART('H');  while(BusyUART());
}

void TermMoveRight()
{
	WriteUART(ESC); while(BusyUART());
	WriteUART('['); while(BusyUART());
	WriteUART('C'); while(BusyUART());
}

void TermCleanLine()
{
	WriteUART(ESC);
	while (BusyUART());
	WriteUART('[');
	while (BusyUART());
	WriteUART('2');
	while (BusyUART());
	WriteUART('K');
	while (BusyUART());
}