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
	int x;
	int x_prev;
	
	led_init();
	pot_init();
	button_init();

	sei();
	
	led_power(100);
	
	led_set(9, 1);
	led_push();
   
   while(1)
   {     		
		x = adc[7]/160;

		 if (x != x_prev) 
		{
			led_set(7, 1);
			led_push();
			_delay_ms(50);
			led_set(7, 0);
			led_push();
			led_bar(x, 6, 1);
		}
		x_prev = x;
   }  
 }