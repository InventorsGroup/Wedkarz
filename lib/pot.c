#include "pot.h"


volatile static char ch = 0;
volatile static char channels[] = {1, 2, 5, 6, 7};
volatile unsigned int adc[8];

ISR (ADC_vect)
{
	adc[ch] = ADC;//(ADCL | (ADCH << 8));
	
	ch++;
	if (ch > 4) ch = 0;
	
	ADMUX &= ~(1<< MUX3) & ~(1<< MUX2) & ~(1<< MUX1) & ~(1<< MUX0);
	ADMUX |= channels[ch];
	ADCSRA |= 1<<ADSC;		// Start Conversion	
}


void pot_init()
{
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	DIDR0 |= (1 << ADC5D) | (1 << ADC2D) | (1 << ADC1D); 
	ADCSRA |= (1 << ADSC);

}

