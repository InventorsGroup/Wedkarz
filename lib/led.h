#include "../config.h"
#include <avr/io.h> 
#include <util/delay.h>


/*
Intializes LEDs
*/

extern volatile unsigned char STATUS;
extern volatile unsigned char GO_TO_POWER_DOWN;
extern volatile unsigned char NIGHT;
extern volatile unsigned char ANTI_THEFT;
extern volatile unsigned char PAIRING;
extern unsigned volatile char SENSIVITY;
extern volatile unsigned char comm_changed;

extern unsigned volatile char COLOR;
extern unsigned volatile char BRIGHTNESS;
extern volatile unsigned char led_turn_off;
extern volatile unsigned char led_pwr;	
extern volatile unsigned char led_button_pwr;

void led_brightness_to_power();
void led_init();

/*
Sets the Nth led to the color specified, without turning it on
Color codes:
0 - off, 1 - red, 2 - green, 3 - blue, 4 - yellow, 5 - purple, 6 - sky, 7 - white

Led Numbers:
0-5 - bar leds, 6 - RGB topled (left), 7 - white topled (right), 8 - button led

Annotation:
setting any color >0 on led 7 will turn it on
led 8 takes color for it's brightness
*/
void led_set(unsigned char n, unsigned char color);

/*
Turns 0-7 leds off (0) and on (1)
*/
void led_enable(unsigned char s);

/*
Sets % power of the LEDs 0-7
*/
void led_power(unsigned char p);

/*
Sets physical state on leds as preset with led_set function (LEDs 0-7)
*/
void led_push();

/*
Clears LEDs 0-7
*/
void led_clear();

/*
Clears LEDs 0-5
*/
void led_bar_clear();

/*
Displays LED bar on LEDs 0-5
n - number of LEDs turned on
c - color
dir - direction
*/
void led_bar(unsigned char n, unsigned char c, unsigned char dir);