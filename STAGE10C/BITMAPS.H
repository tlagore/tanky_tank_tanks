#ifndef BITMAPS_H
#define BITMAPS_H

#include "types.h"
#include "defines.h"

#define MOUSE_HEIGHT 6
#define MISSILE_HEIGHT 6
#define SMALL_EXPLOSION_HEIGHT 6
#define LARGE_EXPLOSION_HEIGHT 32
#define TANK_HEIGHT 16
#define ARROW_HEIGHT 17
#define CANNON_HEIGHT 14
#define QUESTION_HEIGHT 23
#define HOURGLASS_HEIGHT 15

extern const UINT8 mouse_bitmap[MOUSE_HEIGHT];
extern const UINT8 missile_bitmap[MISSILE_HEIGHT];
extern const UINT8 small_explosion_bitmap[][SMALL_EXPLOSION_HEIGHT];

extern const UINT16 hourglass_bitmap[HOURGLASS_HEIGHT];
extern const UINT16 question_bitmap[QUESTION_HEIGHT];
extern const UINT16 cannon_bitmap
	[NUM_PLAYERS][CANNON_ANGLES][CANNON_HEIGHT];
	
extern const UINT32 tank_bitmap[TANK_HEIGHT];	
extern const UINT32 arrow_bitmap[][ARROW_HEIGHT];

extern const UINT32 tank_explosion[][LARGE_EXPLOSION_HEIGHT];
#endif
