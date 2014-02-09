#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led.h"
#include "speaker.h"
#include "rfm12.h"

extern uint8_t SYG_ID[];
void parse_buffer(uint8_t *bufcontents, uint8_t length);
void send_command(uint8_t cmd, uint8_t param);