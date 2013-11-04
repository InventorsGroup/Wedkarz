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
#define NC 4
#define POT1 7
#define POT2 1
#define POT3 2
#define FOTO1 5
#define FOTO2 6
#define HALL1 0
#define HALL2 3

//Buttons
#define B1_PORT PORTD
#define B2_PORT PORTB

#define B1_DDR DDRD
#define B2_DDR DDRB