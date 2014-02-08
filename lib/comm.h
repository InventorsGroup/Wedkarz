#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include "power.h"
#include "led.h"
#include "speaker.h"


extern uint8_t SYG_ID[];
void parse_buffer(uint8_t *bufcontents, uint8_t length);
