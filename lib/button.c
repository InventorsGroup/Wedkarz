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
	PCMSK1 |= (1 << PCINT11);
	PCMSK2 |= (1 << PCINT23) | (1 << PCINT18);

	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS00) | (1 << CS02);
	TIMSK0 |= (1 << OCIE0A);
	OCR0A = 255;
}

volatile unsigned char center_btn_counter = 0;
volatile unsigned char top_btn_counter = 0;
volatile unsigned char spk_tmp = 0;
volatile unsigned char night_tmp = 0, night_tmp2 = 0;
volatile unsigned int branie_counter = 500;
volatile unsigned char branie_dir = 0;
volatile unsigned char config_blink_counter = 0;
volatile unsigned int theft_alarm_counter = 0;
volatile unsigned char theft_alarm_light_counter = 0;
volatile unsigned char kontaktr_set_delay = 0;
volatile unsigned char wedka_polozona = 0;
volatile unsigned char wedka_cnter = 0;
ISR(TIMER0_COMPA_vect)
{
		
	if(TIME > 1 && silent_time > 0)
	{
		silent_time++;

		if(silent_time > TIME * 200)
			silent_time = 0;

	}

	if(kontaktr_set_delay > 0)
	{
		kontaktr_set_delay++;

		if(kontaktr_set_delay > 10)
			kontaktr_set_delay = 0;
	}
	
	if(spk_cnt > 0)
	{
		spk_tmp++;
		if(spk_tmp > spk_cnt)
		{
			set_speaker(0);
			spk_tmp = 0;
			spk_cnt = 0;
		}
	}

	if(THEFT_ALARM == 1)
	{
		if(theft_alarm_counter < 310)
			theft_alarm_counter++;

		if(theft_alarm_light_counter < 30)
			theft_alarm_light_counter++;
		else
			theft_alarm_light_counter = 0;

	}

	if(STATUS == 5)
	{
		config_blink_counter++;

		if(config_blink_counter > 60)
		{
			config_blink_counter = 0;
			led_set(8,1);
			led_push();
		}
		else if(config_blink_counter > 30)
		{
			led_set(8,0);
			led_push();
		}
	} 
	else if(STATUS != 0)
	{
	
		if(adc[FOTO1] > 980 && wedka_cnter < 40)
		{
			wedka_cnter++;
		}
		
		if(adc[FOTO1] < 900 && wedka_cnter > 0)
		{
			wedka_cnter--;
		}

		if(TIME > 1 &&  wedka_cnter > 20 && wedka_polozona == 0 && silent_time == 0)
		{
			silent_time = 1;   
			wedka_polozona = 1;
		}
		
		if(TIME > 1 && wedka_cnter < 10 && wedka_polozona == 1)
		{
			wedka_polozona = 0;
		}
	
		if(branie_counter < 400)
		{
			branie_counter++;			
			if(branie_dir == 0)
			{
				if(branie_counter % 2 == 0)
				{
					led_set(6, COLOR);
					led_push();
				}
				else
				{
					led_set(6, 0);
					led_push();
				}
			}			
			if(branie_counter == 400)
			{
				led_set(7, 0);
				led_set(6, 0);
				led_push();			
		
			}
		}
	
		if(ANTI_THEFT == 0 && led_turn_off > 0)
		{
			led_turn_off++;

			if(led_turn_off > 150)
			{
				led_turn_off = 0;
				led_bar_clear();
			}
		}

		if((STATUS == 2 || STATUS == 4))
		{
			/*if(adc[FOTO2] > 500 && night_tmp < 50)
			{
				night_tmp++;
			}
			else if(night_tmp > 0)
			{
				night_tmp--;
			}

			if(night_tmp > 25)
			{*/
				night_tmp2++;

				if(night_tmp2 == 25)
				{
					led_set(8, 1);
					led_push();
				}
				else if(night_tmp2 > 50)
				{
					led_set(8, 0);
					led_push();
					night_tmp2 = 0;
				}
		/*	}
			else if(night_tmp == 0)
			{	
					led_set(6, 0);
					led_push();
			}*/
		}

		if(ANTI_THEFT > 0 && adc[FOTO1] < 400)
		{
			 THEFT_ALARM = 1;
		}

		if(center_btn_counter > 0)
		{
			if(CENTER_BTN)
			{
				center_btn_counter++;

				if(center_btn_counter > 30)
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
						led_set(9, 1); //led IR
						led_push();
						theft_alarm_counter = 0;
					}
					else
					{
						led_set(6, 3);
						led_push();
						_delay_ms(300);
						led_set(6, 0);
						led_set(9,0);
						led_push();


						THEFT_ALARM = 2;
					}
				
					center_btn_counter = 0;
				}
			}
			else if(center_btn_counter >= 1 && center_btn_counter < 15)
			{
			
				if(STATUS < 4)
					STATUS++;
				else
					STATUS = 1;
			
				for(int i = 0; i < STATUS; i++)
				{
					led_set(8, 1);
					_delay_ms(150);
					led_set(8, 0);
					_delay_ms(150);
				}
				
				branie_counter = 500;
				led_clear();
				
				if(ANTI_THEFT > 0)
				{
					led_set(9, 1); //led IR
					led_push();
				}

				center_btn_counter = 0;
			}
		}
	}


	if(TOP_BTN && top_btn_counter > 0)
	{
		top_btn_counter++;

		if(top_btn_counter == 30)
		{			
			for(int i = 0; i < 6; i++)
			{
				led_bar(i+1, COLOR, 0);
				_delay_ms(50);
			}

			for(int i = 6; i > 0; i--)
			{
				led_bar(i, COLOR, 0);
				_delay_ms(50);
			}	

			GO_TO_POWER_DOWN = 1;
			top_btn_counter = 0;
		}
	}

}


ISR(PCINT0_vect) // CENTER_BTN
{
	
	if(CENTER_BTN)
	{
		if(STATUS != 5)
			center_btn_counter = 1;
		else if(STATUS == 5 && CONF_ENTER == 0)
		{
			led_set(8, 1);
			led_push();
			_delay_ms(100);
			led_set(8, 0);
			led_push();
			_delay_ms(100);
			led_set(8, 1);
			led_push();
			_delay_ms(100);
			led_set(8, 0);
			led_push();

			while (!eeprom_is_ready());
            eeprom_write_byte((uint8_t*)0, (uint8_t)COLOR);
            while (!eeprom_is_ready());
            eeprom_write_byte((uint8_t*)8, (uint8_t)BRIGHTNESS);
            while (!eeprom_is_ready());
            eeprom_write_byte((uint8_t*)16, (uint8_t)TIME);
            while (!eeprom_is_ready());
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

void kontaktron_check()
{

	if(STATUS == 0 || STATUS == 5 || TOP_BTN || CENTER_BTN)
		return;
	
	if(!KONTAKTR_BOT && KONTAKTR_OR)
	{
		char dirprev = dir;
	
		if(kon1 == 1 && kon2 == 1)
			dir = 0;
		else if(kon1 == 0 && kon2 == 0)
			dir = 1;	
			
		if(dir != dirprev)
		{
			pos = 0;
			pos2 = 0;
		}
	}

	kon1 = KONTAKTR_BOT;
	kon2 = KONTAKTR_OR;	
	

	if(pos2 > SENSIVITY*2)
	{
		 pos2 = 0;
			 
		 if(pos > 5)
			pos = 0;
		else
			pos++;

		kontaktr_set_delay = 1;

		if(dir > 0)
			play_speaker(50);
		else
			play_speaker_alt(50);
	}
	else
		 pos2++;	

	if(STATUS != 3 && STATUS != 4)		 
		led_bar2(pos, COLOR, dir, 1);

		
		branie_dir = dir;
		branie_counter = 0;
		
		led_set(6, COLOR);
		led_set(7, 1);
		led_push();

}


ISR(PCINT1_vect) // KONTAKTR_BOT, KONTAKTR_TOP
{
	kontaktron_check();	
}

ISR(PCINT2_vect) // TOP_BTN
{	
	
	if(TOP_BTN)
	{
		top_btn_counter = 1;
	}
	else
	{
		kontaktron_check();
	}

}
