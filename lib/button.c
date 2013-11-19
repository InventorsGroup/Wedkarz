#include "button.h"


void button_init()
{
	DDRB &= ~(1 << PB0);
	PORTB |= (1 << PB0);
	
	DDRC &= ~((1 << PC0) | (1 << PC3));
	PORTC |= (1 << PC0) | (1 << PC3);

	DDRD &= ~( (1 << PD2) | (1 << PD3) | (1 << PD7));
	PORTD |= (1 << PD2) | (1 << PD3) | (1<< PD7);
	
	
	EICRA |= (1 << ISC11);
	EIMSK |= (1 << INT1);
	PCICR |= (1 << PCIE1) | (1 << PCIE0) | (1 << PCIE2);
	PCMSK0 |= (1 << PCINT0);
	PCMSK1 |= (1 << PCINT11) | (1 << PCINT8);
	PCMSK2 |= (1 << PCINT23);
}


ISR(INT1_vect)
{	
	
}

ISR(PCINT0_vect) // CENTER_BTN
{
	led_bar(3, 5, 0);	
}


unsigned volatile char kon1 = 0;
unsigned volatile char kon2 = 0;
unsigned volatile char dir = 0;
unsigned volatile char pos = 0;
unsigned volatile char pos2 = 0;


ISR(PCINT1_vect) // KONTAKTR_BOT, KONTAKTR_TOP
{
	if(KONTAKTR_BOT && !KONTAKTR_TOP)
	{
		char dirprev = dir;
	
		if(kon1 == 1 && kon2 == 1)
			dir = 1;
		else if(kon1 == 0 && kon2 == 0)
			dir = 0;	
			
		if(dir != dirprev)
		{
			pos = 0;
			pos2 = 0;
		}
	}

	kon1 = KONTAKTR_BOT;
	kon2 = KONTAKTR_TOP;	
	

	if(pos2 > SENSIVITY)
	{
		 pos2 = 0;
			 
		 if(pos > 5)
			pos = 0;
		else
			pos++;
	}
	else
		 pos2++;	
			 
	led_bar(pos, 3, dir);
}

ISR(PCINT2_vect) // TOP_BTN
{
	led_bar(3, 5, 1);	
}
