#ifndef RASTER_H
#define RASTER_H

#include "types.h"

void set_video_base(UINT16 *);
UINT16 *get_video_base();
void clear_area (UINT32 * base, int x1, int x2, int y1, int y2);
void plot_vline(UINT32 * base, int x, int y1, int y2);
void plot_hline(UINT32 * base, int x1, int x2, int y);
void plot_pixel(UINT8 * base, int x, int y);
void plot_bitmap_8(UINT8 * base, int x, int y,
			const UINT8 * bitmap, UINT16 height);
void plot_bitmap_16(UINT16 * base, int x, int y, const UINT16 * bitmap,
			UINT16 height);
void plot_square (UINT32 * base, int x, int y, 
			UINT16 width, UINT16 height, UINT16 fill);
void plot_bitmap_32(UINT32 * base, int x, int y,
			const UINT32 * bitmap, UINT16 height);
void clear_screen(UINT32 * base);

#endif