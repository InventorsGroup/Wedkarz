#include "../config.h"
#include "led.h"
#include "speaker.h"
#include <avr/sleep.h>
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

void power_down();
void wake_up();
ISR(INT1_vect);
extern volatile unsigned char STATUS;
extern volatile unsigned char GO_TO_POWER_DOWN;
extern volatile unsigned char NIGHT;
extern volatile unsigned char ANTI_THEFT;
extern volatile unsigned char CONF_ENTER;