#include "../config.h"
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

void speaker_init();
ISR(TIMER2_COMPA_vect);
void set_speaker(char state);
void play_speaker(int length);
extern unsigned volatile char FREQ;
extern unsigned volatile char VOL;