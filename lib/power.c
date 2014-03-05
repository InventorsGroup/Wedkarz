#include "power.h"


void power_down()
{	
    cli();
    _delay_ms(200);

    led_clear();
    TCCR0B = 0;//button timer 0
    PCICR = 0;

	set_speaker(0);
	STATUS = 0;

    EICRA &= ~((1 << ISC11) | (1 << ISC10));
	EIMSK |= (1 << INT1); // ustaw inta

    ANTI_THEFT = 0;
    PAIRING = 0;
    
    pot_deinit();
    led_enable(0);
    TCCR1A = 0;
    TCCR1B = 0;
    OE_PORT &= ~(1 << OE);
    SDI_PORT &= ~(1 << SDI);

    _delay_ms(10);

    SMCR |= (1 << SM1) | (1 << SE);
    sei();
    sleep_cpu();

    wake_up();
}

volatile char sleeped = 0;
void sleep()
{
	if(ANTI_THEFT > 0 || STATUS == 2  || STATUS == 4)
		return;
	
	TCCR0B = 0;//button timer 0	
	TCCR1A = 0;
    TCCR1B = 0;
	sleeped = 1;
}

volatile unsigned char anti_theft_delay_cnter = 0;
void wake_up_after_sleep(char anti_theft)
{

	if(ANTI_THEFT > 0 && anti_theft_delay_cnter > 5 && anti_theft > 0)
		send_command(0x02, 0x01);

	if(sleeped == 0)
	return;
	
	led_turn_off = 1;
	TCCR0B |= (1 << CS00) | (1 << CS02);
    TCCR1A |= (1 << COM1A1) | (1 << WGM10) | (1 << WGM12);
    TCCR1B |= (1 << CS11) | (1 << CS10);
	speaker_init();
	sleeped = 0;
}

void wake_up()
{	
	EIMSK &= ~(1 << INT1);
	SMCR &= ~(1 << SE);
	
   
    _delay_ms(1000);

    STATUS = 1;

    if(!TOP_BTN)
    {
        GO_TO_POWER_DOWN = 1;
		return;
    }

	pot_init();
    TCCR0B |= (1 << CS00) | (1 << CS02);
    TCCR1A |= (1 << COM1A1) | (1 << WGM10) | (1 << WGM12);
    TCCR1B |= (1 << CS11) | (1 << CS10);
	
	rfm12_power_up();
	speaker_init();
	
    if(CENTER_BTN)
    {
    	STATUS = 5;
    	play_speaker(1000, 0);
    }
    else
    {      

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

        set_custom_speaker(15, SPK_FREQ);
        play_speaker_custom(400);
        led_bar_clear();
    }

    while(TOP_BTN || CENTER_BTN);

    PCICR |= (1 << PCIE1) | (1 << PCIE0) | (1 << PCIE2);
}
