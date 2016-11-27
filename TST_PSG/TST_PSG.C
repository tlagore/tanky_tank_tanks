#include <osbind.h>

#include "types.h"
#include "psg.h"
#include "music.h"
#include "input.h"
#include "effects.h"

#define EXIT -1

#define TIMER 0x462

UINT32 get_time();


int main()
{
	char input;
	UINT16 exit = 0;
	UINT16 pause = OFF;
	UINT16 value = 0x00;
	UINT32 timeThen, timeNow;
	/*start_music();*/
	timeThen = get_time();
	/*
	write_psg(0x7, 0x3E);
	write_psg(0x8, 0x0F);
	*/
	
			play_explosion(get_time() - timeThen);
	while (!exit)
	{
		if (input_available())
		{
			switch(get_input())
			{
				case ESCAPE:
					exit = 1;
					break;
				case P_PRESS:
					if (pause == ON)
					{
						play_music();
						pause = OFF;
					}else
					{
						pause_music();
						pause = ON;
					}
					break;
				case UP:
					volume_up();
					break;
				case DOWN:
					volume_down();
					break;
				default:
					break;
			};
		}
		while (get_time() - timeThen == 0)
			;
		

		
		/*
		value += 2;
		if (value <= 0xC2)
			if (value < 0xC1)
				write_psg(0x0, value);
			else
				play_explosion();
		
		/*
		update_music(get_time() - timeThen);*/
		timeThen = get_time();
	}
	stop_sound();
	return 0;

}


UINT32 get_time()
{
	UINT32 *timer = (UINT32*)TIMER;
	UINT32 timeNow;
	UINT32 old_ssp = Super(0);
	timeNow = *timer;
	Super(old_ssp);
	return timeNow;
}