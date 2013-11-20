#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "config.h"
#include "lib/led.h"
#include "lib/pot.h"
#include "lib/speaker.h"
#include "lib/button.h"

volatile unsigned int adc[8];

int main(void) 
 {  
	int x[3];
	int x_prev[3];
	
	led_init();
	pot_init();
	button_init();
	led_clear();
	led_push();
	led_power(100);
	
	sei();	
	
	_delay_ms(30);
	
	x_prev[0] = adc[POT1]/160;
	x_prev[1] = adc[POT2]/160;
	x_prev[2] = adc[POT3]/160;
	
	while(1)
	{     
		x[0] = adc[POT1]/160;

		if (x[0] != x_prev[0]) 
		{			
			led_bar(6, x[0]+1, 0);
			COLOR = x[0]+1;
		}
		x_prev[0] = x[0];
		

		x[1] = adc[POT2]/160;

		if (x[1] != x_prev[1]) 
		{			
			led_bar(x[1], COLOR, 1);
			SENSIVITY = 7-x[1];
		}
		x_prev[1] = x[1];
	
	}  
 }