/******************************************************************************/
/*                                                                            */
/*  Description: Timer library (source)                                       */
/*                                                                            */
/*  Authors: Francesc Bisquerra Castell y Maria Calzada Gonzalez              */
/*                                                                            */
/******************************************************************************/

#include "TIMERlib.h"
#include <p30F4011.h>

void TIMER1Init(unsigned char preescalado, unsigned int periodo, unsigned int fuenteReloj, unsigned char prioridad) {

    // Configurar el registro de control del timer 1
    T1CON = 0x0000; // timer parado, sin pre-escalado y reloj interno
    T1CONbits.TCKPS = preescalado;
    T1CONbits.TCS = fuenteReloj; // 0-> Internal clock

    TMR1 = 0x0000; // Limpiar registro contador
    PR1 = periodo;

    // Configurar interrupciones
    IEC0bits.T1IE = 1; // Activar interrupción timer 1
    IFS0bits.T1IF = 0; // Limpiar flag de estado
    IPC0bits.T1IP = prioridad; // Establecer prioridad
}

void TIMER1Start() {
    T1CONbits.TON = 1; // Encender timer
}

void TIMER1Pause() {
    T1CONbits.TON = 0; // Stop timer
}

void TIMER1Reset() {
    T1CONbits.TON = 0; // Stop timer
    TMR1 = 0x0000; // Limpiar registro contador
}

void TIMER1ReInitialize(unsigned char preescalado, unsigned int periodo, unsigned int fuenteReloj, unsigned char prioridad) {

    TIMER1Reset();
    TIMER1Init(preescalado, periodo, fuenteReloj, prioridad);
    TIMER1Start();
}

void TIMER1ClearInt() {
    IFS0bits.T1IF = 0;
}

void TIMER2Init(unsigned char preescalado, unsigned int periodo, unsigned int fuenteReloj, unsigned char prioridad) {
    // Configurar los registros de control de los timers
    T2CON = 0x000;
    T2CONbits.TCKPS = preescalado;
    T2CONbits.TCS = fuenteReloj;

    TMR2 = 0x0000;
    PR2 = periodo;

    // Configurar interrupciones
    IEC0bits.T2IE = 1; // Activar interrupción timer 1
    IFS0bits.T2IF = 0; // Limpiar flag de estado
    IPC1bits.T2IP = prioridad; // Establecer prioridad
}

void TIMER2ClearInt() {
    IFS0bits.T2IF = 0; // Limpiar flag de estado
}

void TIMER2Start() {
    T2CONbits.TON = 1; // Encender timer
}

void TIMER2Pause() {
    T2CONbits.TON = 0; // Stop timer
}

void TIMER2Reset() {
    T2CONbits.TON = 0; // Stop timer
    TMR2 = 0x0000; // Limpiar registro contador
}
