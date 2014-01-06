#include "power.h"

volatile unsigned char STATUS = 1; //0 - OFF, 1 -4 - NORMAL, 5 - CONFIG
volatile unsigned char PREV_STATUS = 1;
volatile unsigned char ANTI_THEFT = 0;
volatile unsigned char GO_TO_POWER_DOWN = 1;
volatile unsigned char CONF_ENTER = 0;



void power_down()
{	
	led_clear();
	set_speaker(0);
	STATUS = 0;
	PREV_STATUS = STATUS;
    SMCR |= (1 << SM1) | (1 << SE);

    EICRA &= ~((1 << ISC11) | (1 << ISC10));
	EIMSK |= (1 << INT1);
	//TCCR0B &= ~((1 << CS00) | (1 << CS02));
    ANTI_THEFT = 0;
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
    
	if(PREV_STATUS != 0 && PREV_STATUS != 5)
		STATUS = PREV_STATUS;
	else
		STATUS = 1;
	
    _delay_ms(1000);

 //   TCCR0B |= (1 << CS00) | (1 << CS02);
    led_enable(1);
    set_speaker(0);

    if(!TOP_BTN)
    {
        GO_TO_POWER_DOWN = 1;
    }
    else
    if(CENTER_BTN)
    {
    	STATUS = 5;
    	play_speaker(1000);
    	CONF_ENTER = 1;
    }
    else
    {
        if(TIME > 1)
            silent_time = 1;           
        
    	for(int i = 0; i < 6; i++)
        {
            led_bar(i+1, COLOR, 1);
            _delay_ms(50);
        }

        for(int i = 6; i > 0; i--)
        {
            led_bar(i, COLOR, 1);
            _delay_ms(50);
        }

        play_speaker(500);
        led_bar_clear();
    }

}
