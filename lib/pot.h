#include "../config.h"
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>


volatile unsigned extern int adc[8];



SIGNAL (SIG_ADC);

void pot_init();