#include "speaker.h"

unsigned volatile char spk_cnt = 0;
unsigned volatile char SPK_FREQ = 0;
volatile char VOL = 0;
unsigned volatile char vol_tab[] = {15, 30, 45, 55, 85};
unsigned volatile char freq_tab[] = {90, 110, 140, 160, 180, 200, 250};
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
	OCR2A = 0;
	OCR2B = 1;
}

void set_speaker(char state)
{
	if(state > 0)
	{
		TCCR2B |= (1 << CS22);
	}
	else if(state == 0)
	{
		TCCR2B &= ~(1 << CS22);
		TCNT2 = 0;
		OCR2A = 0;
		OCR2B = 1;
		PORTD &= ~(1 << PD6);
	
	}
}

void play_speaker(int length, char alt)
{
	if(((TIME > 1 && silent_time > 0) || VOL == -1) &&  THEFT_ALARM == 0)
	{
		return;
	}

	if(spk_cnt != 0)
		return;

		ACTUAL_FREQ = freq_tab[SPK_FREQ + alt];
		ACTUAL_VOL = vol_tab[VOL];
		spk_cnt = length / 50;
		set_speaker(1);
		
}

void play_speaker_custom(int length)
{
	if(((TIME > 1 && silent_time > 0) || VOL == -1) &&  THEFT_ALARM == 0)
	{
		return;
	}

	if(spk_cnt != 0)
		return;

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
	
	if(OCR2A + ACTUAL_FREQ > 255)
		OCR2A = ((OCR2A+ ACTUAL_FREQ) - 256);
	else
		OCR2A += ACTUAL_FREQ;

	PORTD |= (1 << PD6);

}

ISR(TIMER2_COMPB_vect)
{
	unsigned volatile int volume = ACTUAL_FREQ / 2;
	volume = volume * ACTUAL_VOL;
	volume /= 100;
	
	if(OCR2A + volume > 255)
		OCR2B = ((OCR2A +  volume) - 256);
	else
		OCR2B = OCR2A +  volume;

	PORTD &= ~(1 << PD6);
}
