  /*
	LED Control PINs, PORTs and DDRs
*/

//led drivers
#define SDI_PORT PORTB	
#define OE_PORT PORTB
#define CLK_PORT PORTD
#define LA_PORT	PORTC

#define SDI_DDR	DDRB
#define OE_DDR DDRB
#define CLK_DDR DDRD
#define LA_DDR DDRC

#define SDI 6
#define OE 1
#define CLK 4
#define LA 4

//button led
#define BTNLED 5
#define BTNLED_PORT PORTD
#define BTNLED_DDR DDRD

//ADC Channels
#define POT1 0
#define POT2 1
#define POT3 4
#define FOTO1 2 // anti -theft
#define FOTO2 3


//Buttons
#define B1_PORT PORTD
#define B2_PORT PORTB

#define B1_DDR DDRD
#define B2_DDR DDRB

#define TOP_BTN (!(PIND & (1<<PD7)))
#define CENTER_BTN (!(PINB & (1<<PB0)))
#define KONTAKTR_TOP (!(PINC & (1<<PC0)))
#define KONTAKTR_BOT (!(PINC & (1<<PC3)))
