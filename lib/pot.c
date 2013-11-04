#include "pot.h"


volatile static char ch = 0;


ISR (ADC_vect)
{
	adc[ch] = (ADCL | (ADCH << 8));
	
	ch++;
	if (ch == NC) ch++;
	if (ch == 8) ch = 0;
	ADMUX &= ~(1<< MUX3) & ~(1<< MUX2) & ~(1<< MUX1) & ~(1<< MUX0);
	ADMUX |= ch;
	ADCSRA |= 1<<ADSC;		// Start Conversion
}


void pot_init()
{
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	DIDR0 |= (1 << ADC5D) | (1 << ADC4D) | (1 << ADC3D) | (1 << ADC2D) | (1 << ADC1D) | (1 << ADC0D); 
	ADCSRA |= (1 << ADSC);

}

