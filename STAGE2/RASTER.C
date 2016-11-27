#include "raster.h"

void plot_pixel(UINT8 * base, int x, int y)
{
	*(base + (y * 80) + (x >> 3)) |= 1 << 7 - (x & 7);
	return;
}

void plot_bitmap_16(UINT16 * base, int x, int y,
			const UINT16 * bitmap, unsigned int height)
{

	int i;
	UINT16 xWord = x >> 4;
	UINT16 xActual = x & 15;
	UINT16 xBit = 15 - xActual;
	UINT16 copyOne;
	UINT16 copyTwo;
	base += y * NUM_WORDS + xWord;

	for (i = 0; i < height; i++)
	{
		copyOne = *bitmap;
		copyTwo = *bitmap;

		copyOne >>= xActual;
		copyTwo <<= (xBit + 1);
		if (copyOne == *bitmap)
			*base |= *bitmap;
		else
		{
			*(base++) |= copyOne;
			*(base--) |= copyTwo;
		}
		base += NUM_WORDS;
		bitmap++;	
	}
}

void plot_bitmap_32(UINT32 * base, int x, int y,
			const UINT32 * bitmap, unsigned int height)
{
	
	int i;
	UINT32 xLong = x >> 5;
	UINT32 xActual = x & 31;
	UINT32 xBit = 31 - xActual;
	UINT32 copyOne;
	UINT32 copyTwo;

	base += y * NUM_LONGS + xLong;
	for (i = 0; i < height; i++)
	{
		if (xActual == 0)
			*base |= *bitmap;
		else
		{
			copyOne = copyTwo =  *bitmap;
			copyOne >>= xActual;
			copyTwo <<= (xBit + 1);

			*(base++) |= copyOne;
			*(base--) |= copyTwo;
		}
		base += NUM_LONGS;
		bitmap++;	
	}	
}


void plot_bitmap_8(UINT8 * base, int x, int y,
			const UINT8 * bitmap, unsigned int height)
{
		
	int i;
	int xByte = x >> 3;
	int xActual = x & 7;
	int xBit = 7 - xActual;
	UINT8 copyOne;
	UINT8 copyTwo;
		
		base += y * NUM_BYTES + xByte;

		for (i = 0; i < height; i++)
		{
			copyOne = *bitmap;
			copyTwo = *bitmap;

			copyOne >>= xActual;
			copyTwo <<= (xBit + 1);
			if (copyOne == *bitmap)
				*base |= *bitmap;
			else
			{
				*(base++) |= copyOne;	
				*(base--) |= copyTwo;
			}
			base += NUM_BYTES;
			bitmap++;	
		}
/*	}*/
}

void plot_hline(UINT32* base, int x1, int x2, int y)
{	
	UINT32 outHex = 0xFFFFFFFF;
	UINT32 temp;
	UINT32 x2Hex;
	UINT32 x1Hex;
	UINT32 x1Long;
	UINT32 x2Long;
	UINT32 *finish;	

	if (x1 > x2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;	
	} 

	x1Long = x1 >> 5;
	x2Long = x2 >> 5;	

	x1Hex = outHex >> (x1 & 31);
	x2Hex = outHex << (31 - (x2 & 31));
	
	base += y * NUM_LONGS + x1Long;

	if (x1Long == x2Long)
		*base |= (x1Hex & x2Hex);
	else
	{
		finish = (base - x1Long) + x2Long; 	
		*(base++) |= x1Hex;
		while (base != finish)
			*(base++) |= outHex;	

		*base |= x2Hex;
	}	
}

void plot_vline(UINT32* base, int x, int y1, int y2)
{
	int temp;
	UINT32 xHex = 1;
	xHex <<= (31 - (x & 31));

	if (y1 > y2)
	{
		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	base += y1 * NUM_LONGS + (x >> 5);

	while (y1 <= y2)
	{
		*base |= xHex;
		base += NUM_LONGS;
		y1++;
	}		
}

/*
Function Name: plot_square
Function Purpose: Given an x, y coordinate and a width/height, 
	the function will draw a square of the given dimensions.
	If fill is set to non-0, it will also fill the square.
*/

void plot_square (UINT32 * base, int x, int y, 
			int width, int height, int fill)
{
	int x2 = x + width;
	int y2 = y + height;

	plot_hline (base, x, x2, y);
	plot_hline (base, x, x2, y2);
	plot_vline (base, x, y, y2);
	plot_vline (base, x2, y, y2);
	
	if (fill != 0)
	{
		y++;
		while (y < y2)
		{
			plot_hline(base, x, x2, y);
			y++;			
		}
	
	}


}

void clear_area (UINT32 * base, int x1, int x2, int y1, int y2)
{
	int temp;
	int i;
	UINT32 outHex = 0;
	UINT32 x1Hex;
	UINT32 x2Hex;
	UINT32 x1Long = x1 >> 5;
	UINT32 x2Long = x2 >> 5;
	UINT32* start = base;
	UINT32* finish = base;
	if (x1 > x2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;
	}

	if (y1 > y2)
	{
		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	y1 = y1 < 0 ? 0 : y1;
	
	x1Hex = (UINT32)~(0xFFFFFFFF >> (x1 & 31));
	x2Hex = (UINT32)~(0xFFFFFFFF << (31 - (x2 & 31)));	

	start += (y1 * NUM_LONGS) + x1Long;
	base = start;
	finish += (y2 * NUM_LONGS) + x2Long;
	
	while (base <= finish)
	{
		if (x1Long == x2Long)
			*base &= (x1Hex | x2Hex);
		else
		{
			*(base++) &= x1Hex;
			for (i = x1Long; i <= (x2Long - 1); i++)
				*(base++) = outHex;
			*base &= x2Hex;
		}
		start += NUM_LONGS;
		base = start;
	}
}

void clear_screen (UINT32 * base)
{
	clear_area (base, 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
}
