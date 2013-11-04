#include "button.h"

void button_init()
{
	DDRD |= (1 << PD2) | (1 << PD3);
	//PORTD |= (1 << PD2) | (1 << PD2);
	EICRA |= (1 << ISC10) | (1 << ISC11);
	EIMSK |= (1 << INT1);
}

ISR (INT1_vect)
{
	led_set(7, 1);
	led_push();
	_delay_ms(200);
	led_set(7, 0);
	led_push();
}
