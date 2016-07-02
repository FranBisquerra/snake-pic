

#include <p30f4011.h>
#include <math.h>
//#include "libBuzz.h"



// El registro TRIS por cada puerto sirve para configurar si es I/O
#define BUZZ_TRIS	TRISBbits.TRISB7

// El registro LAT por cada puerto es para escribir un valor
#define BUZZ_LAT	LATBbits.LATB7



void BuzzerInit()
{
	BUZZ_TRIS = 0;
}


// Plays the buzzer
// freq: frequency in Hertz of the sound (must be inside interval)
// duration: time in miliseconds the sound is on
void playBuzzer(unsigned int freq, unsigned int duration)
{
	unsigned int period      = 1000000 / freq; // in microseconds
	unsigned int half_period = period / 2;     // in microseconds
	
	unsigned int  periodoBuzz, intervNivel;
	unsigned long numPeriodosBuzz = ((unsigned long)duration * 1000) / period;
	unsigned int  numIntervNivel = half_period / 10;
	
	// Iteramos el numero de periodos que cabe dentro de duration
	// dur
	for(periodoBuzz = 0; periodoBuzz < numPeriodosBuzz; periodoBuzz++){
		BUZZ_LAT = 1;
		// Mantenemos nivel alto durante porciones de 10us hasta completar la
		// duracion de la mitad del periodo
		for(intervNivel = 0; intervNivel < numIntervNivel; intervNivel++) Delay10us();
		BUZZ_LAT = 0;
		// Mantenemos nivel bajo durante porciones de 10us hasta completar la
		// duracion de la mitad del periodo
		for(intervNivel = 0; intervNivel < numIntervNivel; intervNivel++) Delay10us();
	}
}


// Plays a note in the buzzer
// note: code of the note to play
// octave: octave of the note
// duration: time in miliseconds the sound is on
void playNote(unsigned char note, unsigned char octave, unsigned int duration){
	float v1 = (note-10.0)/12.0;
	float v2 = octave-4;
	unsigned int freq = 440 * pow(2, v1 + v2);

	playBuzzer(freq, duration);
}
