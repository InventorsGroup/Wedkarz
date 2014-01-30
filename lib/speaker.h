#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

void speaker_init();
ISR(TIMER2_COMPA_vect);
void play_speaker_alt(int length);
void play_speaker(int length);
void play_speaker_custom(int length);
void set_speaker(char state);
void set_custom_speaker(unsigned char v, unsigned char f);
extern volatile unsigned int silent_time;
extern unsigned volatile char TIME;
extern unsigned volatile char SPK_FREQ;
extern volatile char VOL;
extern unsigned volatile char spk_cnt;
extern volatile unsigned char THEFT_ALARM;