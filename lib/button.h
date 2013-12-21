#include "../config.h"
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include "led.h"
#include "power.h"

extern unsigned volatile char SENSIVITY;
void button_init();
ISR(TIMER0_COMPA_vect);
ISR(INT1_vect);
ISR(PCINT0_vect);
ISR(PCINT1_vect);
ISR(PCINT2_vect);