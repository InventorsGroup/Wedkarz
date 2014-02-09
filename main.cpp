#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include "config.h"
#include "lib/led.h"
#include "lib/pot.h"
#include "lib/speaker.h"
#include "lib/button.h"
#include "lib/comm.h"
#include "lib/rfm12.h"

volatile unsigned int x_prev[3], x[3], x_prev2[3];
volatile uint8_t *bufcontents;

ISR(INT1_vect)
{	
}

void read_config()
{
	COLOR = eeprom_read_byte((uint8_t*)0);
    BRIGHTNESS = eeprom_read_byte((uint8_t*)8);
    TIME = eeprom_read_byte((uint8_t*)16);
    SYG_ID[1] = eeprom_read_byte((uint8_t*)24);
    SYG_ID[2] = eeprom_read_byte((uint8_t*)32);
    SYG_ID[3] = eeprom_read_byte((uint8_t*)40);

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
		
		if ((x[0] != x_prev[0] && adc_diff > 50) || comm_changed == 1) 
		{
			if(comm_changed == 1)
			{		
				comm_changed = 0;
			}
			else
			{
				VOL = 4 - x[0];
				x_prev[0] = x[0];
				x_prev2[0] = adc[POT1];		
			}

			led_bar(VOL + 2, COLOR, 1);			
			play_speaker(100);
		}

		adc_diff = adc[POT2]- x_prev2[1];
		if(adc_diff<0)adc_diff*=-1;
		
		if ((x[1] != x_prev[1]&& adc_diff > 50) || comm_changed == 2)
		{				
			if(comm_changed == 2)
			{
				comm_changed = 0;
			}
			else
			{
				SPK_FREQ = x[1];
				x_prev[1] = x[1];	
				x_prev2[1] = adc[POT2];		
			}
			
			led_bar(6-SPK_FREQ, COLOR, 1);			
			play_speaker(100);	
		}

		adc_diff = adc[POT3]- x_prev2[2];
		if(adc_diff<0)adc_diff*=-1;

		if ((x[2] != x_prev[2] && adc_diff > 50) || comm_changed == 3)
		{				
			if(comm_changed == 3)
			{
				comm_changed = 0;
			}
			else
			{
				SENSIVITY = x[2];
				x_prev2[2] = adc[POT3];	
				x_prev[2] = x[2];	
			}
		
			led_bar(SENSIVITY + 1, COLOR, 1);
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
			send_command(0x03, COLOR);	
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

#define F_OSC 8000000
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_OSC / (USART_BAUDRATE * 16UL))) - 1) 
void uart_init()
{
	UBRR0H = (unsigned char)(BAUD_PRESCALE>>8);
	UBRR0L = (unsigned char)BAUD_PRESCALE;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C = (3<<UCSZ00);	
	
	DDRD &= ~(1 << PD0);
	DDRD |= (1 << PD1);
}



int main(void) 
 {  
 	button_init();
	read_config(); 
	pot_init();
	led_init();
	speaker_init();
	rfm12_init();
	rfm12_set_wakeup_timer(0x1FFF);
	uart_init();

	_delay_ms(100);
	sei();
	read_silent_values();
	branie_counter = 500;
	
	//rfm12_set_wakeup_timer(0x804);

	while(1)
	{     
		if(GO_TO_POWER_DOWN > 0 && THEFT_ALARM == 0)
		{
			GO_TO_POWER_DOWN = 0;
			rfm12_power_down();
			power_down();
		}
		else
		{
			if(THEFT_ALARM == 1)
			{
				if(theft_alarm_counter < 300)		
				{	
					if(theft_alarm_counter == 10)
						send_command(0x02, 0x01);
					
					if(theft_alarm_light_counter < 6)
					{
						led_bar(6, 1, 1);
						led_set(6, 1);
						led_set(7,1);
						led_set(8,1);
						led_push();			
						set_custom_speaker(90, 130);
						set_speaker(1);
					}
					else
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
				normal_mode();

			}

			if (rfm12_rx_status() == STATUS_COMPLETE)
			{
			    bufcontents = rfm12_rx_buffer();
			//	uart_put(bufcontents[0]);
				parse_buffer(rfm12_rx_buffer(), rfm12_rx_len());     
		        rfm12_rx_clear();
			}
			
			rfm12_poll();
		}

	}  
 }

