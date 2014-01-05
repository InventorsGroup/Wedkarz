#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "config.h"
#include "lib/led.h"
#include "lib/pot.h"
#include "lib/speaker.h"
#include "lib/button.h"

volatile unsigned int x_prev[3], x[3];

void read_config()
{
	COLOR = eeprom_read_byte((uint8_t*)0);
    BRIGHTNESS = eeprom_read_byte((uint8_t*)8);
    TIME = eeprom_read_byte((uint8_t*)16);

    if(COLOR > 6 || COLOR < 1)
    	COLOR = 1;

    if(BRIGHTNESS > 6 || BRIGHTNESS < 1)
    	BRIGHTNESS = 1;

    led_brightness_to_power();

     if(TIME> 6 || TIME < 1)
    	TIME = 1;
}

 void normal_mode()
 {

		if (x[0] != x_prev[0]) 
		{		
			VOL = x[0] - 1;
			led_bar(x[0]+1, COLOR, 1);		
			x_prev[0] = x[0];			
			play_speaker(100);
		}


		if (x[1] != x_prev[1]) 
		{				
			FREQ = x[1];
			led_bar(x[1]+1, COLOR, 1);			
			x_prev[1] = x[1];		
			play_speaker(100);	
		}


		if (x[2] != x_prev[2]) 
		{				
			SENSIVITY = 4-((x[2]/2)+1);
			led_bar(((x[2]/2)+1)*2, COLOR, 1);
			x_prev[2] = x[2];			
		}
	
}

void config_mode()
{
	
		if (x[0] != x_prev[0]) 
		{		
			COLOR = x[0]+1;
			led_bar(6, COLOR, 1);
			x_prev[0] = x[0];			
		}


		if (x[1] != x_prev[1]) 
		{				
			TIME = x[1]+1;
			led_bar(x[1]+1, COLOR, 1);
			x_prev[1] = x[1];			
		}

		if (x[2] != x_prev[2]) 
		{				
			BRIGHTNESS = x[2]+1;
			led_brightness_to_power();
			led_bar(x[2]+1, COLOR, 1);
			x_prev[2] = x[2];			
		}
		
}


int main(void) 
 {  

 	read_config(); 
	led_init();
	pot_init();
	button_init();
	led_clear();
	led_push();
	
	speaker_init();
	
	sei();	

	_delay_ms(200);

	
	while(1)
	{     

		if(GO_TO_POWER_DOWN > 0 && THEFT_ALARM == 0)
		{
			GO_TO_POWER_DOWN = 0;
			power_down();
		}

		if(THEFT_ALARM == 1)
		{
			

			if(theft_alarm_counter < 300)		
			{		
				set_speaker(1);
				if((theft_alarm_counter / 10) % 2 == 0)
					set_custom_speaker(90, 130);
				else
					set_custom_speaker(90, 170);
			}
			else if(theft_alarm_counter == 300)
			{
				set_speaker(0);
			}

			if(theft_alarm_light_counter < 15)
			{
				led_bar(6, 1, 1);
				led_set(6, 1);
				led_set(7,1);
				led_set(8,1);
				led_push();			
			}
			else
			{
				led_bar(6, 2, 1);
				led_set(6, 2);
				led_set(7,1);
				led_set(8,1);
				led_push();				
			}
		}
		else if(THEFT_ALARM == 2)
		{
			set_speaker(0);
			led_clear();
			led_push();
			led_set(8, 0);
			THEFT_ALARM  = 0;
		}



		x[0] = adc[POT1]/200; // volume or color
		x[1] = adc[POT2]/200; // freq or brigthness
		x[2] = adc[POT3]/200; // sensivity

		if(STATUS == 5)
		{
			config_mode();
		}
		else
		{
			if(kontaktr_set_delay == 0)
				normal_mode();
		}

	}  
 }

