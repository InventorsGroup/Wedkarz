#include "comm.h"

uint8_t SYG_ID[] = {0xFF, 0xFE, 0xFE, 0xFE};
void parse_buffer(uint8_t *bufcontents, uint8_t length)
{
	volatile uint8_t ccmd = 0;
	volatile uint8_t vval = 0;

	for(int i = 0; i < length; i++)
	{
		if(bufcontents[i] == 0xFF && (i + 5) < length)
		{

			if(PAIRING && bufcontents[i+4] == 0x0C)
			{
				SYG_ID[1] = bufcontents[i+1];
				SYG_ID[2] = bufcontents[i+2];
				SYG_ID[3] = bufcontents[i+3]; 
				led_set(6, 1);
				led_push();
				_delay_ms(100);
				led_set(6, 0);
				led_push();
				_delay_ms(100);
				led_set(6, 1);
				led_push();
				_delay_ms(100);
				led_set(6, 0);
				led_push();
				send_command(0x04, COLOR);
				PAIRING = 0;
				return;
			}

			if(bufcontents[i+1] != SYG_ID[1] || bufcontents[i+2] != SYG_ID[2] ||
				bufcontents[i+3] != SYG_ID[3] )
				return;


			switch(bufcontents[i+4])
			{
				case 0x0A:

				vval = bufcontents[i+5] & 0x0F;
				ccmd = bufcontents[i+5] >> 4 ;

				if(ccmd > 0 && ccmd < 4 && vval > 0 && vval < 3)
				{
					switch(ccmd)
					{
						case 1:
						if(vval == 2 && VOL < 4)
							VOL++;
						else
							if(vval == 1 && VOL > -1)
								VOL--;

						break;

						case 2:
						if(vval== 1 && SPK_FREQ < 5)
							SPK_FREQ++;
						else if(vval == 2 && SPK_FREQ > 0)
							SPK_FREQ--;
						break;

						case 3:
						if(vval == 1 && SENSIVITY < 6)
							SENSIVITY++;
						else if(vval == 2 && SENSIVITY > 0)
							SENSIVITY--;
						break;

					}

					comm_changed = ccmd;

				}

				break;

				case 0x0B:

				if(bufcontents[i+5] == 0x01)
				{
					ANTI_THEFT = 1;
					send_command(0x02, 0x03);
				}
				else if(bufcontents[i+5] == 0x02)
				{
					ANTI_THEFT = 0;
					THEFT_ALARM = 2;
					send_command(0x02, 0x04);
				}

				break;

				case 0x0D:
					send_command(0x03, COLOR);
				break;
			}


			return;
		}
	}
}

void send_command(uint8_t cmd, uint8_t param)
{
	uint8_t comm[6];
	comm[0] = SYG_ID[0];
	comm[1] = SYG_ID[1];
	comm[2] = SYG_ID[2];
	comm[3] = SYG_ID[3];
	comm[4] = cmd;
	comm[5] = param;
	rfm12_tx(6, 0, comm);
}