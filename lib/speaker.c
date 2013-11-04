#include "speaker.h"

void speaker_init()
{
	TCCR0A |= (1 << COM0A1)  | (1 << WGM02) | (1 << WGM00) ;
	TCCR0B |= (1 << CS01) | (1 << CS00);
}