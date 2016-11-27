#ifndef RASTER_H
#define RASTER_H

#define SCREEN_HEIGHT 399
#define SCREEN_WIDTH 639
#define NUM_BYTES 80
#define NUM_WORDS 40
#define NUM_LONGS 20

typedef unsigned char UINT8;
typedef unsigned int UINT16;
typedef unsigned long UINT32;


void clear_area (UINT32 * base, int x1, int x2, int y1, int y2);
void plot_vline(UINT32 * base, int x, int y1, int y2);
void plot_hline(UINT32 * base, int x1, int x2, int y);
void plot_pixel(UINT8 * base, int x, int y);
void plot_bitmap_8(UINT8 * base, int x, int y,
			const UINT8 * bitmap, unsigned int height);
void plot_bitmap_16(UINT16 * base, int x, int y, const UINT16 * bitmap,
			unsigned int height);
void plot_square (UINT32 * base, int x, int y, 
			int width, int height, int fill);
void plot_bitmap_32(UINT32 * base, int x, int y,
			const UINT32 * bitmap, unsigned int height);
void clear_screen(UINT32 * base);

#endif