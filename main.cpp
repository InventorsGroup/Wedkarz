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
	led_clear();
	led_push();
	led_power(100);
	
	sei();	
	
	while(1)
	{     

	
	}  
 }