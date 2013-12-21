#include "speaker.h"

unsigned volatile char FREQ = 0;
unsigned volatile char VOL = 0;
unsigned volatile char vol_tab[] = {15, 30, 60, 80, 90};
unsigned volatile char freq_tab[] = {100, 110, 120, 140, 150, 160, 180};
void speaker_init()
{	
	//TCCR2B |= (1 << CS22);
	TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B);
	OCR2A = 1;
	OCR2B = 2;
}

void set_speaker(char state)
{
	if(state > 0 && VOL > -1)
		TCCR2B |= (1 << CS22);
	else
		TCCR2B &= ~(1 << CS22);
}

void pause(int time)
{
	for(int i = 0; i < time; i++)
		_delay_ms(1);
}

void play_speaker(int length)
{
	set_speaker(1);
	pause(length);
	set_speaker(0);
}

ISR(TIMER2_COMPA_vect)
{

	unsigned volatile int volume = freq_tab[FREQ] / 2;
	volume = volume * vol_tab[VOL];
	volume /= 100;

	if(OCR2A + volume > 255)
		OCR2B = (OCR2A +  volume) - 256;
	else
		OCR2B = OCR2A +  volume;
	
	if(OCR2A + freq_tab[FREQ] > 255)
		OCR2A = (OCR2A+ freq_tab[FREQ]) - 256;
	else
		OCR2A += freq_tab[FREQ];
	
	PORTD |= (1 << PD6);
}

ISR(TIMER2_COMPB_vect)
{
	PORTD &= ~(1 << PD6);
}
