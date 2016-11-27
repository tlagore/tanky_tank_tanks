#include <osbind.h>
#include <stdio.h>
#include <unistd.h>

#include "raster.h"
#include "bitmaps.h"

int test(int x, int y);

int main()
{
	int i = 0;
	int x = 0;
	int y = 0;
	int offset;
	int fill = 0;
	UINT8* base8 = Physbase();
	UINT16* base16 = Physbase();
	UINT32* base32 = Physbase();

	getchar();
	clear_screen(base32);

	y = 40;
	for (x = 24; x < 32; x++)
	{
		plot_bitmap_8 (base8, x, y, missile_bitmap,
			MISSILE_HEIGHT);
		sleep(1);
		clear_area (base32, x, x + 7, y, y + MISSILE_HEIGHT);
	}

	x = 30;
	for (y = -7; y <= 0; y++)
	{
		x+= 12;
		plot_bitmap_8 (base8, x, y, missile_bitmap,
			MISSILE_HEIGHT);
		sleep(1);
		clear_area (base32, x, x + 7, y, y + MISSILE_HEIGHT);
	} 

	
	for (i = 0; i < 5; i++)
	{
		fill = fill == 0 ? 1: 0;	
		for (x = 0; x < 159; x+= 16)
		{
	
			for (y = 0; y < 99; y+= 16)
			{				
				plot_square(base32, x, y, 16, 16, fill);
				fill = fill == 0 ? 1: 0;
			}
		}
		sleep(1);
		clear_area(base32, 0, 400, 0, 250);
	}
	
	
	
	sleep(1);
	plot_bitmap_32(base32, 0, 0, miranda_bitmap,
				MIRANDA_HEIGHT);
	for (x = 1; x < 639; x++)
	{
		sleep(1/70);	
		clear_area (base32, x-1, (x + 31), 0, MIRANDA_HEIGHT - 1);
		plot_bitmap_32(base32, x, 0, miranda_bitmap, MIRANDA_HEIGHT);
	}
	
	clear_area (base32, x-1, (x + 31), 0, MIRANDA_HEIGHT - 1);
	
	y = 32;
	for (x = 1; x < 639; x++)
	{
		y = y == 32 ? 78:32;
		sleep(1/70);	
		plot_bitmap_32(base32, x, y, miranda_bitmap, MIRANDA_HEIGHT);
		clear_area (base32, x, (x + 31), y, y + MIRANDA_HEIGHT - 1);
	}
	
	
	

	plot_bitmap_32(base32, 64, 249, tank1_bitmap, TANK_HEIGHT);
	plot_bitmap_32(base32, 543, 241, tank2_bitmap, TANK_HEIGHT);

	for (i = 0; i < 3; i++)
		plot_hline (base32, 0, 639, 266+i);		
	
	
	for (i = 0; i < 3; i++)
		plot_square (base32, 4 + i, 4 + i, 106 - (i << 1), 25 - (i << 1), 0);
		
	for (i = 0; i < 3; i++)
		plot_square (base32, 529 + i, 4 + i, 106 - (i << 1), 25 - (i << 1), 0);
		
	sleep(1);
	
	for (i = 0; i < 3; i++)
	{
		for (x = 96; x < 511; x++)
		{
			sleep(1/70);	
			plot_bitmap_8 (base8, x, 249, missile_bitmap,
				MISSILE_HEIGHT);
			clear_area (base32, x, (x + 7), 249, 249 + MISSILE_HEIGHT - 1);
		}
		
		for (x = 511; x > 96; x--)
		{
			sleep(1/70);	
			plot_bitmap_8 (base8, x, 249, missile_bitmap,
				MISSILE_HEIGHT);
			clear_area (base32, x, (x + 7), 249, 249 + MISSILE_HEIGHT - 1);
		}
	}
	
	
	for (i = 0; i < 4; i++)
	{
		offset = i * 25;
		plot_square (base32, 7, 7, 100 - offset, 22, 1);
		plot_square (base32, 532 + offset, 7, 100 - offset, 22, 1);
		sleep(1);
		clear_area(base32, 532, 532 + 96, 7, 25);
	}
		

	for (i = 0; i < 5; i++)
	{
		plot_bitmap_32 (base32, 300, 10, arrow_right_bitmap,
				ARROW_HEIGHT );
		sleep(1);
		clear_area (base32, 300, 332, 10, 10 + ARROW_HEIGHT);

		plot_bitmap_32 (base32, 300, 10, arrow_left_bitmap,
				ARROW_HEIGHT );
		sleep(1);
	    clear_area (base32, 300, 332, 10, 10 + ARROW_HEIGHT);
	
	}
	

	plot_bitmap_16 (base16, 64, 241, medium_explosion_bitmap,
				MEDIUM_EXPLOSION_HEIGHT);
	plot_bitmap_16 (base16, 543, 241, medium_explosion_bitmap,
				MEDIUM_EXPLOSION_HEIGHT);
	sleep(1);
	plot_bitmap_16 (base16, 80, 257, medium_explosion_bitmap,
				MEDIUM_EXPLOSION_HEIGHT);
	plot_bitmap_16 (base16, 559, 257, medium_explosion_bitmap,
				MEDIUM_EXPLOSION_HEIGHT);

	sleep(1);
	
	clear_area (base32, 64, 96, 241, 241 + TANK_HEIGHT - 1);
	clear_area (base32, 543, 575, 241, 241 + TANK_HEIGHT - 1);
	
	plot_bitmap_32(base32, 64, 236, large_explosion_bitmap1, 
			LARGE_EXPLOSION_HEIGHT);
	plot_bitmap_32(base32, 543, 236, large_explosion_bitmap1, 
			LARGE_EXPLOSION_HEIGHT);
			
	sleep(1);
	
	clear_area (base32, 64, 96, 236, 236 + LARGE_EXPLOSION_HEIGHT - 1);
	clear_area (base32, 543, 575, 236, 236 + LARGE_EXPLOSION_HEIGHT - 1);
	
	plot_bitmap_8 (base8, 64, 241, small_explosion_bitmap,
				SMALL_EXPLOSION_HEIGHT);
	plot_bitmap_8 (base8, 543, 241, small_explosion_bitmap,
				SMALL_EXPLOSION_HEIGHT);
	sleep(1);
	plot_bitmap_8 (base8, 80, 257, small_explosion_bitmap,
				SMALL_EXPLOSION_HEIGHT);
	plot_bitmap_8 (base8, 559, 257, small_explosion_bitmap,
				SMALL_EXPLOSION_HEIGHT);
				
	sleep(1);
	
	clear_screen(base32);

	getchar();
	return 0;
}
