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
	int  x_prev[3];
	
	led_init();
	pot_init();
	button_init();
	led_clear();
	led_push();
	led_power(100);
	
	sei();	
	
	_delay_ms(30);
	
	x_prev[0] = (adc[POT1]/160)+1;
	x_prev[1] = 7-adc[POT2]/160;
	x_prev[2] = adc[POT3]/160;
	
	while(1)
	{     
		COLOR = (adc[POT1]/160)+1;

		if (COLOR != x_prev[0]) 
		{			
			led_bar(6, x[0]+1, 0);
		}
		x_prev[0] = COLOR;
		

		SENSIVITY = 7-adc[POT2]/160;
		
		if (SENSIVITY != x_prev[1]) 
		{			
			led_bar(x[1], COLOR, 1);
			
		}
		x_prev[1] = SENSIVITY;
	
	}  
 }