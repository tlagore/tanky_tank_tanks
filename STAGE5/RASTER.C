/*
Name: raster

Purpose:
The raster module is a library of generalized plotting routines
that allow the caller to:

 - clear a region of the screen (or the whole screen)
 - plot horizontal lines
 - plot vertical lines
 - plot squares
 - plot 8, 16, and 32 bit bitmaps

 Author: Tyrone Lagore
 Version: February 5, 2014
*/

#include "raster.h"
#include "defines.h"

/*
Name: plot_bitmap_8
Purpose: Plots an 8 bit bitmap to the specified (x,y) coordinate
	of the frame buffer.
	
Inputs:
	UINT8 *base - A (byte) pointer to the first address of the frame
			buffer.
	int x - The x coordinate to plot to.
	int y - The y coordinate to plot to.
	const UINT8 *bitmap - The bitmap being plotted
	UINT16 height - The height of the received bitmap.
	
Outputs:
	None
	
Assumptions:
- x coordinate checking is done outside of the function call
  as x clipping is not supported.
	
*/
void plot_bitmap_8(UINT8 * base, int x, int y,
			const UINT8 * bitmap, UINT16 height)
{
	int i;
	UINT16 xByte = x >> 3,
			xActual = x & 7,
			xBit = 7 - xActual;
			
	UINT8 copyOne, copyTwo;
	
	if (y + height > 0)
	{
		if (y < 0)
		{
			height += y;
			bitmap -= y;
			y = 0;
		}
			
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
	}
}


/*
Name: plot_bitmap_16
Purpose: Plots a 16 bit bitmap to the specified (x,y) coordinate
	of the frame buffer.
	
Inputs:
	UINT16 *base - A (word) pointer to the first address of the frame
			buffer.
	int x - The x coordinate to plot to.
	int y - The y coordinate to plot to.
	const UINT16 *bitmap - The bitmap being plotted
	UINT16 height - The height of the received bitmap.
	
Outputs:
	None
	
Assumptions:
- x and y coordinate checking is done outside of the function call
  as clipping
	
*/

void plot_bitmap_16(UINT16 * base, int x, int y,
			const UINT16 * bitmap, UINT16 height)
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

/*
Name: plot_bitmap_32
Purpose: Plots an 32 bit bitmap to the specified (x,y) coordinate
	of the frame buffer.
	
Inputs:
	UINT32 *base - A (long) pointer to the first address of the frame
			buffer.
	int x - The x coordinate to plot to.
	int y - The y coordinate to plot to.
	const UINT32 *bitmap - The bitmap being plotted
	UINT16 height - The height of the received bitmap.
	
Outputs:
	None
	
Assumptions:
- x and y coordinate checking is done outside of the function call
  as clipping
	
*/
void plot_bitmap_32(UINT32 * base, int x, int y,
			const UINT32 * bitmap, UINT16 height)
{
	int i;
	UINT32 copyOne, copyTwo,
			xLong = x >> 5,
			xActual = x & 31,
			xBit = 31 - xActual;
			
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

/*
Name: plot_hline
Purpose: Plots a horizontal line to the frame buffer.
	
Inputs:
	UINT32 *base - A (long) pointer to the first address of the frame
			buffer.
	int x1 - The first x coordinate to plot from
	int x2 - The second x coordinate to plot to.
	int y - The y coordinate to plot the line at
	
Outputs:
	None
	
Assumptions:
- x and y coordinate checking is done outside of the function call
  as clipping
	
*/
void plot_hline(UINT32* base, int x1, int x2, int y)
{	
	UINT32 temp, x2Hex, x1Hex, x1Long, x2Long,
			outHex = 0xFFFFFFFF;

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

/*
Name: plot_vline
Purpose: Plots a vertical line to the frame buffer.
	
Inputs:
	UINT32 *base - A (long) pointer to the first address of the frame
			buffer.
	int x - The x coordinate to plot the line at
	int y1 - The first y coordinate to plot from
	int y2 - The second y coordinate to plot to
	
Outputs:
	None
	
Assumptions:
- x and y coordinate checking is done outside of the function call
  as clipping
	
*/
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

Inputs:
	UINT32 *base - A (long) pointer to the first address of the frame
			buffer.
	int x - The x coordinate to plot the square at
	int y - The y coordinate to plot the square at
	UINT16 width - The width of the square to plot 
	UINT16 height - The height of the square to plot
	
	UINT16 fill - 0 if no fill
				- non 0 if fill
				
Assumptions:
 - x and y bounds checking is done outside of the function call 
	as clipping is not supported
*/
void plot_square (UINT32 * base, int x, int y, 
			UINT16 width, UINT16 height, UINT16 fill)
{
	int x2 = x + width,
		y2 = y + height;
		
	if (width > 0)
	{
		plot_hline (base, x, x2, y);
		plot_hline (base, x, x2, y2);
	}
	
	if (height > 0)
	{
		plot_vline (base, x, y, y2);
		plot_vline (base, x2, y, y2);
	}
	
	if (fill)
	{
		y++;
		while (y < y2)
		{
			plot_hline(base, x, x2, y);
			y++;			
		}
	}
}

/*
Function Name: clear_area
Function Purpose: Clears a section of the screen.

Inputs:
	UINT32 *base - A (long) pointer to the first address of the frame
			buffer.
	int x1 - The x coordinate clear from
	int x2 - the x coordinate to clear to
	int y1 - the y coordinate to clear from
	int y2 - The y coordinate to clear to
	
Assumptions:
 - x and y bounds checking is done outside of the function call 
	as clear clipping is not supported
*/
void clear_area (UINT32 * base, int x1, int x2, int y1, int y2)
{
	int temp, i;
	UINT32 x1Hex, x2Hex, x1Long, x2Long, 
		outHex = 0;
	UINT32* start = base;
	
	if (y2 <= 0 || x2 < 0)
		return;
	
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
	
	x1Long = x1 >> 5;
	x2Long = x2 >> 5;
	
	y1 = y1 < 0 ? 0 : y1;
	
	x1Hex = (UINT32)~(0xFFFFFFFF >> (x1 & 31));
	x2Hex = (UINT32)~(0xFFFFFFFF << (31 - (x2 & 31)));
	
	start += (y1 * NUM_LONGS) + x1Long;
	base = start;
	
	while (y1 < y2)
	{
		if (x1Long == x2Long)
			*base &= (x1Hex | x2Hex);

		else
		{
			*(base++) &= x1Hex;
			for (i = x1Long + 1; i < x2Long; i++)
				*(base++) = outHex;
			*base &= x2Hex;
		}
		start += NUM_LONGS;
		base = start;
		y1++;
	}
}
/*
Name: clear_scren
Purpose: Clears the entire screen
Method: Calls clear_area with the dimensions of the screen
*/
void clear_screen (UINT32 * base)
{
	clear_area (base, 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
}
