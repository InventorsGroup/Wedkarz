#include "../config.h"
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "led.h"
#include "power.h"
#include "pot.h"

extern unsigned volatile char SENSIVITY;
extern volatile unsigned int theft_alarm_counter;
extern volatile unsigned char theft_alarm_light_counter;
extern volatile unsigned char kontaktr_set_delay;
extern volatile unsigned int branie_counter;
void button_init();
ISR(TIMER0_COMPA_vect);
ISR(INT1_vect);
ISR(PCINT0_vect);
ISR(PCINT1_vect);
ISR(PCINT2_vect);