#include "button.h"

unsigned volatile char SENSIVITY = 1;

void button_init()
{
	DDRB &= ~(1 << PB0);
	PORTB |= (1 << PB0);
	
	DDRC &= ~((1 << PC0) | (1 << PC3));
	PORTC |= (1 << PC0) | (1 << PC3);

	DDRD &= ~( (1 << PD2) | (1 << PD3) | (1 << PD7));
	PORTD |= (1 << PD2) | (1 << PD3) | (1<< PD7);
	
	
	PCICR |= (1 << PCIE1) | (1 << PCIE0) | (1 << PCIE2);
	PCMSK0 |= (1 << PCINT0);
	PCMSK1 |= (1 << PCINT11) | (1 << PCINT8);
	PCMSK2 |= (1 << PCINT23);

	TCCR0A |= (1 << WGM01);
	//TCCR0B |= (1 << CS00) | (1 << CS02);
	TIMSK0 |= (1 << OCIE0A);
	OCR0A = 255;
}

volatile unsigned char center_btn_counter = 0;
volatile unsigned char top_btn_counter = 0;
ISR(TIMER0_COMPA_vect)
{
	if(TOP_BTN && top_btn_counter > 0)
	{
		top_btn_counter++;

		if(top_btn_counter > 30)
		{
			GO_TO_POWER_DOWN = 1;
			top_btn_counter = 0;
		}
	}
	else
		top_btn_counter = 0;

	if(center_btn_counter > 0)
	{
		if(CENTER_BTN)
		{
			center_btn_counter++;

			if(center_btn_counter > 30)
			{
				NIGHT = NIGHT == 0 ? 1 : 0;

				if(NIGHT > 0) led_set(6, 5);
				led_set(7, 1);
				led_push();
				_delay_ms(300);
				if(NIGHT > 0) led_set(6, 0);
				led_set(7, 0);
				led_push();
				_delay_ms(300);
				if(NIGHT > 0) led_set(6, 5);
				led_set(7, 1);
				led_push();
				_delay_ms(300);
				if(NIGHT > 0) led_set(6, 0);
				led_set(7, 0);
				led_push();
				center_btn_counter = 0;
			}
		}
		else if(center_btn_counter > 1 && center_btn_counter < 10)
		{
			ANTI_THEFT = ANTI_THEFT == 0 ? 1 : 0;

			if(ANTI_THEFT > 0)
			{
				led_set(6, 1);
				led_push();
				_delay_ms(300);
				led_set(6, 2);
				led_push();
				_delay_ms(300);
				led_set(6, 0);
				led_push();
				_delay_ms(300);
			}
			else
			{
				led_set(6, 3);
				led_push();
				_delay_ms(300);
				led_set(6, 0);
				led_push();
			}

			center_btn_counter = 0;
		}
	}

}


ISR(PCINT0_vect) // CENTER_BTN
{
	
	if(CENTER_BTN)
	{
		if(STATE == 1)
			center_btn_counter = 1;
		else if(STATE == 2 && CONF_ENTER == 0)
		{
			led_set(8, 1);
			led_push();
			_delay_ms(200);
			led_set(8, 0);
			led_push();
			_delay_ms(200);
			led_set(8, 1);
			led_push();
			_delay_ms(200);
			led_set(8, 0);
			led_push();
			_delay_ms(200);
			//TUTAJ ZAPIS DO EEPROMU
			GO_TO_POWER_DOWN = 1;
		}
		else if(CONF_ENTER == 1)
		{
			_delay_ms(500);
			CONF_ENTER = 0;
		}


	}
}


unsigned volatile char kon1 = 0;
unsigned volatile char kon2 = 0;
unsigned volatile char dir = 0;
unsigned volatile char pos = 0;
unsigned volatile char pos2 = 0;


ISR(PCINT1_vect) // KONTAKTR_BOT, KONTAKTR_TOP
{
	if(STATE != 1)
		return;
	
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
	

	if(pos2 > SENSIVITY*2)
	{
		 pos2 = 0;
			 
		 if(pos > 5)
			pos = 0;
		else
			pos++;
	}
	else
		 pos2++;	
			 
	led_bar(pos, COLOR, dir);
}

ISR(PCINT2_vect) // TOP_BTN
{
	if(TOP_BTN)
	{
		top_btn_counter = 1;
	}

}
