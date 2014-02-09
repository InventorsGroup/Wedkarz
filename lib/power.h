#include "../config.h"
#include "led.h"
#include "speaker.h"
#include "pot.h"
#include "comm.h"
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

void power_down();
void wake_up();
void sleep();
void wake_up_after_sleep();
extern volatile unsigned char anti_theft_delay_cnter;