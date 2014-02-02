#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include "power.h"

extern uint8_t SYG_ID[];
void parse_buffer(uint8_t *bufcontents, uint8_t length);
void send_packet(uint8_t id, uint8_t param);
