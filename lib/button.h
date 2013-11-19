#include "../config.h"
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include "led.h"

#define TOP_BTN (!(PIND & (1<<PD7)))
#define CENTER_BTN (!(PINB & (1<<PB0)))
#define KONTAKTR_TOP (!(PINC & (1<<PC0)))
#define KONTAKTR_BOT (!(PINC & (1<<PC3)))

static unsigned volatile char SENSIVITY = 0;
void button_init();
ISR(INT1_vect);
ISR(PCINT0_vect);
ISR(PCINT1_vect);
ISR(PCINT2_vect);