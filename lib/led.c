#include "led.h"

/*
	LED Connection array
	
	Led No. / Color    R   G   B
			1
			2
			3
		   ...
*/

unsigned volatile char COLOR = 1;
unsigned volatile char BRIGHTNESS = 6;
	
volatile unsigned char led_pwr = 100;	
volatile unsigned char led_button_pwr = 100;
volatile unsigned char led_turn_off = 0;

volatile static unsigned char led_array[3][8] = {{0, 3, 6, 11, 12, 23, 27, 20},
												 {1, 4, 7, 10, 13, 17, 26, 20},
												 {2, 5, 15, 9, 22, 16, 25, 20}};


volatile static unsigned char leds[36];
void led_init()
{

	//clear the array
	for (int i=0; i<36; i++) leds[i] = 0;
	
	//set DDRs
	SDI_DDR |= (1 << SDI);
	OE_DDR |= (1 << OE);
	CLK_DDR |= (1 << CLK);
	LA_DDR |= (1 << LA);

	//PWM
	TCCR1A |= (1 << COM1A1) | (1 << WGM10) | (1 << WGM12);
	TCCR1B |= (1 << CS11) | (1 << CS10);
	
	//ir led
	DDRC |= (1 << PC0);
	

	//driver led on
	DDRD |= (1 << PD5);
	PORTD |= (1 << PD5);

	_delay_ms(20);

	//swinger
	DDRB |= (1 << PB7);
}

void led_enable(unsigned char s)
{
	if (s > 0 && OCR1A == 255) 
	{		
		PORTD |= (1 << PD5);
		_delay_ms(20);

		led_power(led_pwr);
	}
	else if(s == 0)
	{
		OCR1A = 255;
		PORTD &= ~(1 << PD5);
	}
}

void led_brightness_to_power()
{
	led_power(BRIGHTNESS * 17);
}

void led_power(unsigned char p)
{
	if (p < 0) p = 0;
	else if (p > 100) p = 100;
	led_pwr =p;
	OCR1A =  255-p;
}


void led_set(unsigned char n, unsigned char color)
{
	if (n < 7)
	{
		switch (color){
			case 0: // Off 
						leds[35-led_array[0][n]] = 0;
						leds[35-led_array[1][n]] = 0;
						leds[35-led_array[2][n]] = 0;
					break;
			case 1: // Red
						leds[35-led_array[0][n]] = 1;
						leds[35-led_array[1][n]] = 0;
						leds[35-led_array[2][n]] = 0;
					break;
			case 2: // Green
						leds[35-led_array[0][n]] = 0;
						leds[35-led_array[1][n]] = 1;
						leds[35-led_array[2][n]] = 0;
					break;
			case 3: // Blue
						leds[35-led_array[0][n]] = 0;
						leds[35-led_array[1][n]] = 0;
						leds[35-led_array[2][n]] = 1;
					break;
			case 4: // Yellow
						leds[35-led_array[0][n]] = 1;
						leds[35-led_array[1][n]] = 1;
						leds[35-led_array[2][n]] = 0;
					break;
			case 5: // Purple
						leds[35-led_array[0][n]] = 1;
						leds[35-led_array[1][n]] = 0;
						leds[35-led_array[2][n]] = 1;
					break;
			case 6: // Sky
						leds[35-led_array[0][n]] = 0;
						leds[35-led_array[1][n]] = 1;
						leds[35-led_array[2][n]] = 1;
					break;
			case 7: // White
						leds[35-led_array[0][n]] = 1;
						leds[35-led_array[1][n]] = 1;
						leds[35-led_array[2][n]] = 1;
					break;
			}
	}
		else if ((n == 7)) //white topled exception	
		{
			if (color > 0) leds[15] = 1;
			else leds[15] = 0;
		}
		else if (n == 8) // button led exception 
		{
			if (color > 0) leds[17] = 1;
			else leds[17] = 0;
		}
		else if ((n == 9)) //ir led
		{
			if (color > 0) PORTC |= (1 << PC0);
			else PORTC &= ~(1 << PC0);
		}

		if(n == 6) //swinger
		{
			if(color > 0)
				PORTD |= (1 << PB7);
			else
				PORTD &= ~(1<< PB7);                                  
		}
}

void led_push()
{

	for (int i = 0; i<36; i++){
		
		if (leds[i] == 1) SDI_PORT |= (1 << SDI);
		else SDI_PORT &= ~(1<<SDI);
		
		CLK_PORT |= (1 << CLK);
		CLK_PORT &= ~(1 << CLK);
	}
	LA_PORT |= (1 << LA);
	LA_PORT &= ~(1 << LA); // Latch output

	led_enable(1);
	led_turn_off = 1;
}

void led_clear()
{
	for (int i=0; i<36; i++) leds[i] = 0;
	
	led_enable(0);
}

void led_bar_clear()
{
	for (int i = 0; i < 6; i++) led_set(i, 0);
}

void led_bar(unsigned char n, unsigned char c, unsigned char dir)
{
	led_bar2(n, c, dir, 0);
}

void led_bar2(unsigned char n, unsigned char c, unsigned char dir, unsigned char comet)
{
	volatile int i = 0;
	led_bar_clear();
	if (n < 0) n = 0;
	else if (n > 6) n = 6;
	for(i = 0; i < n; i++)
	{	
	if(comet  == 0 || (comet > 0 && ((n > 4 && (n-4) < i) || n <=4)))
		if (dir > 0) led_set(5-i, c);
		else led_set(i, c);
	}

	led_push();
	
}
