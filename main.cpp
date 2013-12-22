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

    if(COLOR > 6 || COLOR < 1)
    	COLOR = 1;

    if(BRIGHTNESS > 6 || BRIGHTNESS < 1)
    	BRIGHTNESS = 1;

    led_brightness_to_power();
}

 void normal_mode()
 {

		if (x[0] != x_prev[0]) 
		{		
			VOL = x[0] - 1;
			led_bar(x[0]+1, COLOR, 1);
			play_speaker(200);
			x_prev[0] = x[0];			
		}


		if (x[1] != x_prev[1]) 
		{				
			FREQ = x[1];
			led_bar(x[1]+1, COLOR, 1);
			play_speaker(200);
			x_prev[1] = x[1];			
		}


		if (x[2] != x_prev[2]) 
		{				
			SENSIVITY = 4-(x[2]*2);
			led_bar((x[2]+1)*2, COLOR, 1);
			x_prev[2] = x[2];			
		}
}


unsigned volatile int tmpcnt = 0;
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
			BRIGHTNESS = x[1]+1;
			led_brightness_to_power();
			led_bar(x[1]+1, COLOR, 1);
			x_prev[1] = x[1];			
		}

		if(tmpcnt == 2000)
		{
			led_set(7, 1);
			led_push();
		}
		else if(tmpcnt == 4000)
		{
			led_set(7, 0);
			led_push();
			tmpcnt = 0;
		}

		tmpcnt++;


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


//	x_prev[0] = adc[POT1]/200; // volume or color
//	x_prev[1] = adc[POT2]/200; // freq or brigthness
//	x_prev[2] = adc[POT3]/450; // sensivity

	
	while(1)
	{     

		if(GO_TO_POWER_DOWN > 0)
		{
			GO_TO_POWER_DOWN = 0;
			power_down();
		}

		x[0] = adc[POT1]/200; // volume or color
		x[1] = adc[POT2]/200; // freq or brigthness
		x[2] = adc[POT3]/450; // sensivity

		if(STATE == 2)
		{
			config_mode();
		}
		else
		{
			normal_mode();
		}

	}  
 }

