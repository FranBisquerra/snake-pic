/******************************************************************************/
/*                                                                            */
/*  Description: Timer library (header)                                       */
/*                                                                            */
/*  Authors: Francesc Bisquerra Castell y Maria Calzada Gonzalez              */
/*                                                                            */
/******************************************************************************/


void TIMER1Init( unsigned char preescalado,unsigned int periodo, unsigned int fuenteReloj, unsigned char prioridad);

void TIMER1Start();

void TIMER1Pause();

void TIMER1Reset();

void TIMER1ReInitialize(unsigned char preescalado,unsigned int periodo, unsigned int fuenteReloj, unsigned char prioridad);

void TIMER1ClearInt();

void TIMER2Init(unsigned char preescalado, unsigned int periodo, unsigned int fuenteReloj, unsigned char prioridad);

void TIMER2Pause();

void TIMER2Start();