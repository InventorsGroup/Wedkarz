#include "power.h"

volatile unsigned char STATE = 1; //0 - OFF, 1 - NORMAL, 2 - CONFIG
volatile unsigned char NIGHT = 0;
volatile unsigned char ANTI_THEFT = 0;
volatile unsigned char GO_TO_POWER_DOWN = 1;
volatile unsigned char CONF_ENTER = 0;
volatile unsigned char THEFT_ALARM = 0;


void power_down()
{	
	led_clear();
	set_speaker(0);
	STATE = 0;
    SMCR |= (1 << SM1) | (1 << SE);

    EICRA &= ~((1 << ISC11) | (1 << ISC10));
	EIMSK |= (1 << INT1);
	//TCCR0B &= ~((1 << CS00) | (1 << CS02));
    ANTI_THEFT = 0;
    NIGHT = 0;
    led_enable(0);
    sleep_cpu();
}

ISR(INT1_vect)
{	
	wake_up();
}


void wake_up()
{	

	EIMSK &= ~(1 << INT1);
	SMCR &= ~(1 << SE);
    STATE = 1;
    


    _delay_ms(1000);

 //   TCCR0B |= (1 << CS00) | (1 << CS02);
    led_enable(1);
    set_speaker(0);

    if(CENTER_BTN)
    {
    	STATE = 2;
    	play_speaker(1000);
    	CONF_ENTER = 1;
    }
    else
    if(!TOP_BTN)
    {
    	GO_TO_POWER_DOWN = 1;
    }
    else
    {
    	play_speaker(500);
    	led_bar(3,COLOR, 0);
    	_delay_ms(200);
    	led_bar(0,COLOR, 0);
    	play_speaker(500);
    	led_bar(3,COLOR, 1);
    	_delay_ms(200);
    	led_bar(0,COLOR, 0);

    }

}
