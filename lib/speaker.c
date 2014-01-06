#include "speaker.h"

unsigned volatile char spk_cnt = 0;
unsigned volatile char FREQ = 0;
volatile char VOL = 0;
unsigned volatile char vol_tab[] = {15, 30, 60, 80, 90};
unsigned volatile char freq_tab[] = {100, 110, 120, 140, 150, 160, 180};
volatile unsigned int silent_time = 0;
unsigned volatile char TIME = 1;
unsigned volatile char ACTUAL_FREQ = 100;
unsigned volatile char ACTUAL_VOL = 90;
volatile unsigned char THEFT_ALARM = 0;

void speaker_init()
{	
	//TCCR2B |= (1 << CS22);
	DDRD |= (1 << PD6);
	TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B);
	OCR2A = 1;
	OCR2B = 2;
}

void set_speaker(char state)
{
	if(state > 0 && ((silent_time == 0 && VOL > -1) || THEFT_ALARM > 0))
	{
		TCCR2B |= (1 << CS22);
	}
	else if(state == 0)
	{
		TCCR2B &= ~(1 << CS22);
		PORTD &= ~(1 << PD6);
		TCNT2 = 0;
		OCR2A = 1;
		OCR2B = 2;
	}
}

void play_speaker(int length)
{
	if(TIME > 1 && ((silent_time == 0 && VOL > -1) || THEFT_ALARM > 0))
	{
		return;
	}
		ACTUAL_FREQ = freq_tab[FREQ];
		ACTUAL_VOL = vol_tab[VOL];
		set_speaker(1);
		spk_cnt = length / 50;
}

void play_speaker_alt(int length)
{
	if(TIME > 1 && ((silent_time == 0 && VOL > -1) || THEFT_ALARM > 0))
	{
		return;
	}
	
		if(FREQ == 0)
			ACTUAL_FREQ = freq_tab[FREQ+1];
		else
			ACTUAL_FREQ = freq_tab[FREQ-1];

		ACTUAL_VOL = vol_tab[VOL];
		set_speaker(1);
		spk_cnt = length / 50;
}

void set_custom_speaker(unsigned char v, unsigned char f)
{
	ACTUAL_VOL = v;
	ACTUAL_FREQ = f;
}

ISR(TIMER2_COMPA_vect)
{

	unsigned volatile int volume = ACTUAL_FREQ / 2;
	volume = volume * ACTUAL_VOL;
	volume /= 100;
	
	if(OCR2A + volume > 255)
		OCR2B = ((OCR2A +  volume) - 256);
	else
		OCR2B = OCR2A +  volume;
	
	if(OCR2A + ACTUAL_FREQ > 255)
		OCR2A = ((OCR2A+ ACTUAL_FREQ) - 256);
	else
		OCR2A += ACTUAL_FREQ;

		PORTD |= (1 << PD6);

}

ISR(TIMER2_COMPB_vect)
{
	PORTD &= ~(1 << PD6);
}
