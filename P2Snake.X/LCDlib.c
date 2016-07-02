/******************************************************************************/
/*                                                                            */
/*  Description: LCD library (source)                                         */
/*                                                                            */
/*  Authors: Francesc Bisquerra Castell y Maria Calzada Gonzalez              */
/*                                                                            */
/******************************************************************************/

#include "LCDlib.h"
#include "delay.h"

#include <p30f4011.h>

/******************************************************************************/
/* Pre-processor directives                                                   */
/******************************************************************************/

#define LCD_EN_TRIS  TRISBbits.TRISB6
#define LCD_EN       LATBbits.LATB6

#define LCD_RS_TRIS  TRISCbits.TRISC15
#define LCD_RS       LATCbits.LATC15
#define LCD_RW_TRIS  TRISEbits.TRISE8
#define LCD_RW       LATEbits.LATE8

#define LCD_DB4_TRIS TRISDbits.TRISD0
#define LCD_DB4      LATDbits.LATD0
#define LCD_DB5_TRIS TRISDbits.TRISD1
#define LCD_DB5      LATDbits.LATD1
#define LCD_DB6_TRIS TRISDbits.TRISD2
#define LCD_DB6      LATDbits.LATD2
#define LCD_DB7_TRIS TRISDbits.TRISD3
#define LCD_DB7_R    PORTDbits.RD3
#define LCD_DB7      LATDbits.LATD3


/******************************************************************************/
/* Prototypes of additional functions										 */
/******************************************************************************/

void waitLCD();

void LCDWriteCmd(unsigned char cmd);
void LCDWriteNibbleCmd(unsigned char cmd);

void LCDWriteData(unsigned char data);


/******************************************************************************/
/* Functions																  */
/******************************************************************************/

// Initialize LCD
// This function forces a reset and initialize the LCD (16x2)
void LCDInit()
{
	// Set main pins as outputs
	LCD_EN_TRIS = 0;
	LCD_RS_TRIS = 0;
	LCD_RW_TRIS = 0;

	// Reset
	Delay15ms();
	LCDWriteNibbleCmd(0x03); // <==== Completar
	Delay5ms();
	LCDWriteNibbleCmd(0x03); // <==== Completar
	Delay150us();
	LCDWriteNibbleCmd(0x03); // <==== Completar
	waitLCD();

	// End reset
	LCDWriteNibbleCmd(0x02);  // Function set 4-bits interface // <==== Completar
	waitLCD();

	// Function set 4-bits interface, number of lines and character font
	LCDWriteCmd(0x28); // <==== Completar
	waitLCD();

	// Display OFF
	LCDWriteCmd(0x08); // <==== Completar
	waitLCD();

	// Clear display
	LCDWriteCmd(0x01); // <==== Completar
	waitLCD();

	// Entry mode set incr. address and no shift
	LCDWriteCmd(0x06); // <==== Completar
	waitLCD();

	/* End Initialization */
	waitLCD();

	// Display ON
	LCDWriteCmd(0x0E); // <==== Completar
	waitLCD();
}

// Clear LCD
void LCDClear(){
	LCDWriteCmd(0x01); // <==== Completar
	waitLCD();
}

// Move the cursor of the LCD to home
void LCDMoveHome(){
	LCDWriteCmd(0x02);
	waitLCD();
}

// Move LCD cursor to first line
void LCDMoveFirstLine(){
	LCDWriteCmd(0x80);
	waitLCD();
}

// Move LCD cursor to second line
void LCDMoveSecondLine(){
	LCDWriteCmd(0xC0); // <==== Completar
	waitLCD();
}

// Writes a string to the LCD
void LCDPrint(char *str){
	while(*str != '\0'){
		waitLCD();
		LCDWriteData(*str);
		str++;
	}
}

// Move LCD cursor left
void LCDMoveLeft(){
	LCDWriteCmd(0x10); // <==== Completar
	waitLCD();
}

// Move LCD cursor right
void LCDMoveRight(){
	LCDWriteCmd(0x14);
	waitLCD();
}

// Scroll LCD Screen left
void LCDScrollLeft(){
	LCDWriteCmd(0x1A); // <==== Completar 
	waitLCD();
}

// Scroll LCD right
void LCDScrollRight(){
	LCDWriteCmd(0x1E);
	waitLCD();
}

// Hide LCD cursor
void LCDHideCursor(){
	LCDWriteCmd(0x0C);
	waitLCD();
}

// Turn on underline LCD cursor
void LCDTurnOnUnderline(){
	LCDWriteCmd(0x0E);
	waitLCD();
}

// Turn on blinking-block LCD cursor
void LCDTurnOnBlinking(){
	LCDWriteCmd(0x0F); // <==== Completar
	waitLCD();
}


/******************************************************************************/
/* Additional functions                                                       */
/******************************************************************************/

void waitLCD()
{
	char busyValue = 1;

	// Set LCD data pins as inputs
	LCD_DB7_TRIS = 1;
	LCD_DB6_TRIS = 1;
	LCD_DB5_TRIS = 1;
	LCD_DB4_TRIS = 1;

	Delay20us();

	// Read Busy Flag and Address Counter
	LCD_RS = 0; // <==== Completar pag 9, tabla 1
	LCD_RW = 1; // <==== Completar

	// Setup time
	Delay20us();
	Delay20us();

	while(busyValue){
		LCD_EN = 1;

		// Data delay time
		Delay20us();
		Delay20us();

		busyValue = LCD_DB7_R;

		Delay20us();
		Delay20us();

		// Restore default state
		LCD_EN = 0;

		// Hold on time
		Delay20us();
		Delay20us();
	}
	/* Restore default state */
	LCD_RW = 0;
}

void LCDWriteCmd(unsigned char cmd)
{
	// Send command to Instruction Register
	LCD_RS = 0; // <==== Completar pag 9 tabla 1
	LCD_RW = 0; // <==== Completar

	// Set LCD data pins as outputs
	LCD_DB7_TRIS = 0;
	LCD_DB6_TRIS = 0;
	LCD_DB5_TRIS = 0;
	LCD_DB4_TRIS = 0;

	// Send first nibble
	LCD_DB7 = (0x80 & cmd) >> 7; // <==== Completar
	LCD_DB6 = (0x40 & cmd) >> 6; // <==== Completar
	LCD_DB5 = (0x20 & cmd) >> 5; // <==== Completar
	LCD_DB4 = (0x10 & cmd) >> 4; // <==== Completar

	// Wait until nibble is processed
	Delay20us(); // min 20us
	LCD_EN = 1;
	Delay20us(); // min 230n
	LCD_EN = 0;
	Delay20us();

	// Send command to Instruction Register
	LCD_RS = 0; // <==== Completar
	LCD_RW = 0;// <==== Completar

	// Send second nibble
	LCD_DB7 = (0x08 & cmd) >> 3; // <==== Completar
	LCD_DB6 = (0x04 & cmd) >> 2; // <==== Completar
	LCD_DB5 = (0x02 & cmd) >> 1; // <==== Completar
	LCD_DB4 = (0x01 & cmd) >> 0; // <==== Completar

	// Wait until nibble is processed
	Delay20us(); // min 20us
	LCD_EN = 1;
	Delay20us(); // min 230n
	LCD_EN = 0;
	Delay20us();

	/* Restore default state */
	// Set LCD data pins as inputs
	LCD_DB7_TRIS = 1;
	LCD_DB6_TRIS = 1;
	LCD_DB5_TRIS = 1;
	LCD_DB4_TRIS = 1;
}

void LCDWriteNibbleCmd(unsigned char cmd)
{
	// Send command to Instruction Register
	LCD_RS = 0; // <==== Completar
	LCD_RW = 0; // <==== Completar

	// Set higher LCD data pins as outputs
	LCD_DB7_TRIS = 0;
	LCD_DB6_TRIS = 0;
	LCD_DB5_TRIS = 0;
	LCD_DB4_TRIS = 0;

	// Send nibble
	LCD_DB7 = (0x08 & cmd) >> 3; // <==== Completar
	LCD_DB6 = (0x04 & cmd) >> 2; // <==== Completar
	LCD_DB5 = (0x02 & cmd) >> 1; // <==== Completar
	LCD_DB4 = (0x01 & cmd) >> 0; // <==== Completar

	// Wait until nibble is processed
	Delay20us(); // min 20us
	LCD_EN = 1;
	Delay20us(); // min 230n
	LCD_EN = 0;
	Delay20us();

	/* Restore default state */
	// Set higher LCD data pins as inputs
	LCD_DB7_TRIS = 1;
	LCD_DB6_TRIS = 1;
	LCD_DB5_TRIS = 1;
	LCD_DB4_TRIS = 1;
}

void LCDWriteData(unsigned char data)
{
	// Write data to (DD|CG)RAM
	LCD_RS = 1; // <==== Completar
	LCD_RW = 0; // <==== Completar

	// Set LCD pins as outputs
	LCD_DB7_TRIS = 0;
	LCD_DB6_TRIS = 0;
	LCD_DB5_TRIS = 0;
	LCD_DB4_TRIS = 0;

	// Send first nibble
	LCD_DB7 = (0x80 & data) >> 7; // <==== Completar
	LCD_DB6 = (0x40 & data) >> 6; // <==== Completar
	LCD_DB5 = (0x20 & data) >> 5; // <==== Completar
	LCD_DB4 = (0x10 & data) >> 4; // <==== Completar

	// Wait until nibble is processed
	Delay20us(); // min 20us
	LCD_EN = 1;
	Delay20us(); // min 230n
	LCD_EN = 0;
	Delay20us();

	// Write data to (DD|CG)RAM
	LCD_RS = 1;// <==== Completar
	LCD_RW = 0; // <==== Completar

	// Send second nibble
	LCD_DB7 = (0x08 & data) >> 3; // <==== Completar
	LCD_DB6 = (0x04 & data) >> 2; // <==== Completar
	LCD_DB5 = (0x02 & data) >> 1; // <==== Completar
	LCD_DB4 = (0x01 & data) >> 0; // <==== Completar

	// Wait until nibble is processed
	Delay20us(); // min 20us
	LCD_EN = 1;
	Delay20us(); // min 230n
	LCD_EN = 0;
	Delay20us();

	/* Restore default state */
	// Set LCD pins as inputs
	LCD_DB7_TRIS = 1;
	LCD_DB6_TRIS = 1;
	LCD_DB5_TRIS = 1;
	LCD_DB4_TRIS = 1;

	LCD_RW = 0;
	LCD_RS = 0;
}

void LCDStoreCharPattern(unsigned char position, unsigned char *symbol)
{
	unsigned int i = 0;

    LCDWriteCmd(position);

    for (i = 0; i < 7; ++i) LCDWriteData(symbol[i]);

    LCDMoveHome();

}

void LCDShowCharPattern(unsigned char position)
{
    LCDWriteData(position);
}

