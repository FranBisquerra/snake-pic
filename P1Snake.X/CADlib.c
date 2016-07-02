/******************************************************************************/
/*                                                                            */
/*  Description: LCD library (source)                                         */
/*                                                                            */
/*  Authors: Francesc Bisquerra Castell y Maria Calzada Gonzalez              */
/*                                                                            */
/******************************************************************************/

#include "CADlib.h"

#include <p30F4011.h>

unsigned int ADCValue;

void CADInit(){

	//Seleccionar los pines del puerto como entradas analagicas ADPCFG. (9,8,6-0)
	ADPCFG = 0x037F;

	//Seleccione la fuente de referencia de voltaje para que coincida con rango esperado de entradas analagicas ADCON2.
	ADCON2 = 0;

	// Seleccione el reloj de conversion analagica para que coincida con la velocidad de datos deseada con reloj del procesador ADCON3. -> 2Tcy
	ADCON3 = 0x0002;

	// Determinar cuantos S/H canales se utilizarán. -> CH0
	// Determinar como se encuentran las entradas a S/H canales ADCHS. 
	ADCHS = 0x0007; //-> Conectado a AN7 como CH0 input.

	// Seleccione la secuencia ADCON1 muestra/conversion apropiado y ADCON3.
	ADCON1bits.SSRC = 0b111; //->Trigger auto
	ADCON1bits.ASAM = 1; //-> Muestreo empieza cuando SAMP se activa

	// Seleccione como se presentan los resultados de conversión en el búfer ADCON1.
	ADCON1bits.FORM = 0; //-> Integer

	ADCSSL = 0; // -> No escanear ningun input.

}

void CADStart(){
	ADCON1bits.ADON = 1; //-> Encender CAD

	while (IFS0bits.ADIF == 0);
	ADCValue = ADCBUF0; // Acabada la conversion, ADCValue tiene el dato
	IFS0bits.ADIF = 0; // Limpiar ADIF
}

unsigned int CADgetValue(){
	return ADCValue;
}