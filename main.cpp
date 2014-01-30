#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "config.h"
#include "lib/led.h"
#include "lib/pot.h"
#include "lib/speaker.h"
#include "lib/button.h"
#include "lib/rfm12.h"

volatile unsigned int x_prev[3], x[3], x_prev2[3];

void read_config()
{
	COLOR = eeprom_read_byte((uint8_t*)0);
    BRIGHTNESS = eeprom_read_byte((uint8_t*)8);
    TIME = eeprom_read_byte((uint8_t*)16);

    if(COLOR > 6 || COLOR < 1)
    	COLOR = 1;

    if(BRIGHTNESS > 6 || BRIGHTNESS < 1)
    	BRIGHTNESS = 6;

    led_brightness_to_power();

     if(TIME> 6 || TIME < 1)
    	TIME = 1;
}

void read_silent_values()
{
	x[0] = adc[POT1]/200; // volume or color
	x[1] = adc[POT2]/200; // freq or brigthness
	x[2] = adc[POT3]/200; // sensivity
	
	VOL = 4 - x[0];
	SPK_FREQ = x[1];
	SENSIVITY = x[2];
	
	x_prev[0] = x[0];
	x_prev[1] = x[1];
	x_prev[2] = x[2];
	
}

volatile int adc_diff = 0;
 void normal_mode()
 {
		adc_diff = adc[POT1]- x_prev2[0];
		if(adc_diff<0)adc_diff*=-1;
		
		if (x[0] != x_prev[0] && adc_diff > 50) 
		{		
			VOL = 4 - x[0];
			led_bar(6-x[0], COLOR, 1);		
			x_prev[0] = x[0];
			x_prev2[0] = adc[POT1];			
			play_speaker(100);
		}

		adc_diff = adc[POT2]- x_prev2[1];
		if(adc_diff<0)adc_diff*=-1;
		
		if (x[1] != x_prev[1]&& adc_diff > 50) 
		{				
			SPK_FREQ = x[1];
			led_bar(6-x[1], COLOR, 1);			
			x_prev[1] = x[1];	
			x_prev2[1] = adc[POT2];			
			play_speaker(100);	
		}

		adc_diff = adc[POT3]- x_prev2[2];
		if(adc_diff<0)adc_diff*=-1;

		if (x[2] != x_prev[2] && adc_diff > 50) 
		{				
			SENSIVITY = x[2];
			led_bar(x[2]+ 1, COLOR, 1);
			x_prev2[2] = adc[POT3];	
			x_prev[2] = x[2];	
			play_speaker(100);		
		}
	
}

void config_mode()
{
		adc_diff = adc[POT1]- x_prev2[0];
		if(adc_diff<0)adc_diff*=-1;
		
		if (x[0] != x_prev[0] && adc_diff > 50) 
		{		
			COLOR = x[0]+1;
			led_bar(6, COLOR, 1);
			x_prev[0] = x[0];	
			x_prev2[0] = adc[POT1];			
		}

		adc_diff = adc[POT2]- x_prev2[1];
		if(adc_diff<0)adc_diff*=-1;
		
		if (x[1] != x_prev[1]&& adc_diff > 50) 
		{				
			TIME = 6-x[1];
			led_bar(6-x[1], COLOR, 1);
			x_prev[1] = x[1];			
			x_prev2[1] = adc[POT2];	
		}

		adc_diff = adc[POT3]- x_prev2[2];
		if(adc_diff<0)adc_diff*=-1;

		if (x[2] != x_prev[2] && adc_diff > 50) 
		{				
			BRIGHTNESS = 6-x[2];
			led_brightness_to_power();
			led_bar(6, COLOR, 1);
			x_prev[2] = x[2];	
			x_prev2[2] = adc[POT3];			
		}
		
}


int main(void) 
 {  
 	uint8_t tv[] = "b";

 	read_config(); 
	pot_init();
	led_init();
	button_init();
	led_clear();	
	speaker_init();
	rfm12_init();
	
	sei();	
	_delay_ms(200);	
	read_silent_values();
	
	branie_counter = 500;
	led_clear();

	while(1)
	{     
		rfm12_tx (sizeof(tv), 0, tv);
		rfm12_poll();
		rfm12_tick();

		if(GO_TO_POWER_DOWN > 0 && THEFT_ALARM == 0)
		{
			GO_TO_POWER_DOWN = 0;
			power_down();
		}

		if(THEFT_ALARM == 1)
		{
			if(theft_alarm_counter < 300)		
			{		
				
				if(theft_alarm_light_counter < 6)
				{
					led_bar(6, 1, 1);
					led_set(6, 1);
					led_set(7,1);
					led_set(8,1);
						

					set_custom_speaker(90, 130);
					set_speaker(1);
				}
				else
				{
					led_bar(6, 2, 1);
					led_set(6, 2);
					led_set(7,1);
					led_set(8,1);
					

					set_speaker(0);		
				}
			}
			else if(theft_alarm_counter == 300)
			{
				set_speaker(0);
				led_clear();
		
			}
		}
		else if(THEFT_ALARM == 2)
		{
			theft_alarm_counter = 0;
			set_speaker(0);
			led_clear();
	
			led_set(8, 0);
			THEFT_ALARM  = 0;
		}


	

		if(STATUS == 5)
		{
			x[0] = adc[POT1]/150; // volume or color
			x[1] = adc[POT2]/200; // freq or brigthness
			x[2] = adc[POT3]/200; // sensivity
			config_mode();
		}
		else
		{
			x[0] = adc[POT1]/200; // volume or color
			x[1] = adc[POT2]/200; // freq or brigthness
			x[2] = adc[POT3]/200; // sensivity
			if(kontaktr_set_delay == 0)
				normal_mode();

		}

	}  
 }

