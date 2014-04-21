#include "button.h"

void button_init()
{
	DDRB &= ~(1 << PB0);
	PORTB |= (1 << PB0);
	
	DDRC &=  ~(1 << PC3);
	PORTC |=  (1 << PC3);

	DDRD &= ~( (1 << PD2) | (1 << PD3) | (1 << PD7));
	PORTD |= (1 << PD2) | (1 << PD3) | (1<< PD7);
	
	
	PCICR |= (1 << PCIE1) | (1 << PCIE0) | (1 << PCIE2);
	PCMSK0 |= (1 << PCINT0);
	PCMSK1 |= (1 << PCINT11);
	PCMSK2 |= (1 << PCINT23) | (1 << PCINT18) ;

	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS00) | (1 << CS02);
	TIMSK0 |= (1 << OCIE0A);
	OCR0A = 255;
}

volatile unsigned char center_btn_counter = 0;
volatile unsigned char top_btn_counter = 0;
volatile unsigned char spk_tmp = 0;
volatile unsigned char night_tmp = 0;
volatile unsigned int branie_counter = 500, branie_counter2 = 0;
volatile unsigned char branie_dir = 0;
volatile unsigned char config_blink_counter = 0;
volatile unsigned char theft_alarm_light_counter = 0;
volatile unsigned char wedka_cnter = 0;
volatile unsigned char kometa_cnter = 0, kometa_pos = 0;
unsigned volatile char next_fast = 0, fast_cnter = 0, fast_timer = 0;

volatile unsigned int silent_times[] = {1350, 1800, 2700, 3600, 5400};
unsigned static volatile char sens_tab[] = {10, 20, 30, 45, 60, 80};
unsigned static volatile char fast_tab[] = {12, 10, 9, 6, 4, 2};
//od kontakrona
unsigned volatile char kon1 = 0;
unsigned volatile char dir = 0;
unsigned volatile char pos2 = 0;

ISR(TIMER0_COMPA_vect)
{	
	if(ANTI_THEFT > 0 || TIME > 1)
	{
		led_set(9, 1); //led IR
	}
	else
	{
		led_set(9,0);
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
	
	if(ANTI_THEFT > 0 && anti_theft_delay_cnter < 6)
		anti_theft_delay_cnter++;

	if(THEFT_ALARM == 1)
	{
		if(theft_alarm_counter < 310)
			theft_alarm_counter++;

		if(theft_alarm_light_counter < 10)
			theft_alarm_light_counter++;
		else
			theft_alarm_light_counter = 0;
			
		if(theft_alarm_counter < 300)		
		{	
			if(theft_alarm_counter == 10 || theft_alarm_counter == 20)
			{
				send_command(0x02, 0x01);
				led_power(100);
			}
			if(theft_alarm_light_counter == 0)
			{
				set_custom_speaker(80, 110);
				set_speaker(1);
				
				led_bar(6, 1, 1);
				led_set(6, 1);
				led_set(7,1);
				led_set(8,1);
				led_push();		
			}
			else if(theft_alarm_light_counter == 5)
			{
				led_bar(6, 2, 1);
				led_set(6, 2);
				led_set(7,1);
				led_set(8,1);
				led_push();		

				set_speaker(0);		
			}
		}
		else if(theft_alarm_counter == 300)
		{
			set_speaker(0);
			led_clear();
			led_push();
			led_brightness_to_power();
		}
	}
	else if(THEFT_ALARM == 2)
	{
		theft_alarm_counter = 0;
		set_speaker(0);
		led_clear();
		led_push();
		led_set(8, 0);
		send_command(0x02, 0x02);
		THEFT_ALARM  = 0;
		led_brightness_to_power();
	}

	if(STATUS == 5)
	{
		config_blink_counter++;

		if(config_blink_counter > 40)
		{
			config_blink_counter = 0;
			led_set(8,0);
			led_set(7, 0);
			led_push();
		}
		else if(config_blink_counter > 20)
		{
			led_set(8,1);
			if(PAIRING > 0) led_set(7,1);
			led_push();
		}


		if(center_btn_counter > 0)
		{
			if(CENTER_BTN)
			{
				if(center_btn_counter < 40)
					center_btn_counter++;

				if(center_btn_counter == 30)
					PAIRING = 1;

			}
			else if(center_btn_counter > 30) //parowanko
			{
				center_btn_counter = 0;
			}
			else if(center_btn_counter < 15) //zapis
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
	            eeprom_write_byte((uint8_t*)24, (uint8_t)SYG_ID[1]);
	            while (!eeprom_is_ready());
	            eeprom_write_byte((uint8_t*)32, (uint8_t)SYG_ID[2]);
	            while (!eeprom_is_ready());
	            eeprom_write_byte((uint8_t*)40, (uint8_t)SYG_ID[3]);
	            while (!eeprom_is_ready());
	            center_btn_counter = 0;
				GO_TO_POWER_DOWN = 1;
			} 
			else
				center_btn_counter = 0;

		}

	} 
	else if(STATUS != 0)
	{

		if(THEFT_ALARM == 0 || theft_alarm_counter > 300)
		{			
			if(TIME > 1 && silent_time > 0)
			{
				silent_time++;

				if(silent_time > silent_times[TIME-2])
					silent_time = 0;
			}
			
			if(fast_timer < 20)
				fast_timer++;
			else
			{
				fast_timer =0;
					
				if(fast_cnter > fast_tab[SENSIVITY])
					next_fast = 1;					
				else
					next_fast = 0;
						
				fast_cnter = 0;
			}

			if(kometa_cnter > 0)
			{
				kometa_cnter++;

				if(kometa_cnter > 2 && kometa_pos < 6)
				{
					led_bar_clear();
					if(branie_dir == 0)
						led_set(kometa_pos, COLOR);
					else
						led_set(5-kometa_pos, COLOR);

					led_push();
				
					kometa_pos++;

					if(kometa_pos == 6)
					{
						kometa_cnter = 0;
						kometa_pos = 0;
					}
					else
						kometa_cnter = 1;
				}
			}
		
			if(adc[FOTO1] > 300 && wedka_cnter < 30 && TIME > 1)
			{
				wedka_cnter++;				
				if(wedka_cnter == 30)
				{
					silent_time = 1;
				}
			}
			
			if(adc[FOTO1] < 200 && wedka_cnter > 0 && TIME > 1 && silent_time == 0)
			{
				wedka_cnter--;				
			}
		
			if(branie_counter < 400)
			{
				branie_counter++;			
				if(branie_dir == 0)
				{
					branie_counter2++;

					if(branie_counter2 > 6)
					{
						if(branie_counter % 2 == 0)
						{
							led_set(6, COLOR);
						}
						else
						{
							led_set(6, 0);
						}
						
						led_push();
						branie_counter2 = 0;
					}
				}			
				if(branie_counter == 400)
				{
					led_turn_off = 0;
					led_clear();
					sleep();
				}


			}
			else if(led_turn_off > 0)
			{
				led_turn_off++;

				if(led_turn_off > 150)
				{
					led_turn_off = 0;
					led_clear();
					sleep();
				}
			}

			if(STATUS == 2 || STATUS == 4)
			{
				if(adc[FOTO2] < 5)
				{
					if(night_tmp < 50)
						night_tmp++;
				}
				else if(night_tmp > 0 && adc[FOTO2] > 15)
				{
					night_tmp--;
				}

				if(night_tmp > 30)
				{
					led_set(8, 1);
				}
				else if(night_tmp == 0)
				{	
					led_set(8, 0);
				}
				led_push(0);
			}

		}
		
		if(ANTI_THEFT > 0 && adc[FOTO1] < 400)
		{
			 THEFT_ALARM = 1;
		}
		else if(ANTI_THEFT > 0 && adc[FOTO1] > 800 && THEFT_ALARM == 1)
		{
			THEFT_ALARM = 2;
		}
		if(center_btn_counter > 0)
		{
			if(CENTER_BTN)
			{
				if(center_btn_counter < 40)
					center_btn_counter++;

				if(center_btn_counter > 30)
				{
					ANTI_THEFT = ANTI_THEFT == 0 ? 1 : 0;

					if(ANTI_THEFT > 0)
					{
						anti_theft_delay_cnter = 0;
						led_set(6, 1);
						led_push();
						_delay_ms(300);
						led_set(6, 2);
						led_push();
						_delay_ms(300);
						led_set(6, 0);
						led_push();
						theft_alarm_counter = 0;
					}
					else
					{
						led_set(6, 3);
						led_push();
						_delay_ms(300);
						led_set(6, 0);
						led_push();
						send_command(0x02, 0x02);
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
			
				if(STATUS == 2 || STATUS == 4)
				{
					led_set(8, 1);
					led_push();
					_delay_ms(150);
					led_set(8, 0);
					led_push();
					_delay_ms(150);
				}
				
				branie_counter = 500;
				led_clear();

				center_btn_counter = 0;
			}
		}
	}


	if(TOP_BTN && top_btn_counter > 0 && center_btn_counter == 0)
	{
		if(top_btn_counter < 40)
			top_btn_counter++;

		if(top_btn_counter > 30)
		{			
			for(char i = 0; i < 6; i++)
			{
				led_bar(i+1, COLOR, 0);
				_delay_ms(50);
			}

			for(char i = 6; i > 0; i--)
			{
				led_bar(i, COLOR, 0);
				_delay_ms(50);
			}	

			top_btn_counter = 0;
			GO_TO_POWER_DOWN = 1;
		}
	}else if(top_btn_counter > 0)
		top_btn_counter = 0;

	if(center_btn_counter > 0 && !CENTER_BTN)
		center_btn_counter = 0;


}


ISR(PCINT0_vect) // CENTER_BTN
{
	if(CENTER_BTN && center_btn_counter == 0)
	{
		center_btn_counter = 1;
	}
	
	wake_up_after_sleep(1);
}

void kontaktron_check()
{

        if(STATUS == 0 || STATUS == 5 || TOP_BTN || CENTER_BTN)
                return;
		
		char bot = KONTAKTR_BOT;
		char oor = KONTAKTR_OR;
				
        if(!bot && oor)   
             dir = kon1;     
			 
        if(bot == oor)
     	   kon1 = KONTAKTR_BOT;

        if(pos2 > sens_tab[SENSIVITY])
        {
             pos2 = 0;


                if(kometa_cnter == 0 || branie_dir != dir)
                {               
            		kometa_cnter = 1;
					branie_dir = dir;
            	}

				//send_command(0x01, (branie_dir+1) | (SPK_FREQ << 2));

				if(TIME == 1 || silent_time == 0)
				{	
					if(fast_cnter < 200)				
						fast_cnter++;
				  
					if(next_fast == 1)
						play_speaker(50, branie_dir, 1);					
					else
						play_speaker(50, branie_dir, 0);

				}
				
            	led_set(6, COLOR);
                led_set(7, 1);
                led_push();
                branie_counter = 0;
        }
        else
                 pos2++;     

        _delay_us(20);
}


ISR(PCINT1_vect) // KONTAKTR_BOT, KONTAKTR_TOP
{
	kontaktron_check();	
	wake_up_after_sleep(1);
}

ISR(PCINT2_vect) // TOP_BTN
{	
	if(TOP_BTN && top_btn_counter == 0 && center_btn_counter == 0)
	{
		top_btn_counter = 1;
	}
	else
	{
		kontaktron_check();
	}
		
	wake_up_after_sleep(1);
}

