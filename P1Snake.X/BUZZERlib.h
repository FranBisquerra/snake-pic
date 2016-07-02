
/*#ifndef LIB_BUZZ_H

	#define LIB_BUZZ_H	1
*/
/* ================================ */
/*	Libreria Buzzer		    */
/* ================================ */
//#include "delay.h"

#define DO   1
#define RE   3
#define MI   5
#define FA   6
#define SOL  8
#define LA  10
#define SI  12


#define NUMERO_NOTAS	7

const unsigned char escalaNotas [NUMERO_NOTAS] = {DO, RE, MI, FA, SOL, LA, SI};

void initBuzzer();

// Plays the buzzer
// freq: frequency in Hertz of the sound (must be inside interval)
// duration: time in miliseconds the sound is on
void playBuzzer(unsigned int freq, unsigned int duration);


// Plays a note in the buzzer
// note: code of the note to play
// octave: octave of the note
void playNote(unsigned char note, unsigned char octave, unsigned int duration);


//#endif // LIB_BUZZ_H