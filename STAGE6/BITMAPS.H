#ifndef BITMAPS_H
#define BITMAPS_H

#include "types.h"
#include "defines.h"

#define BOMB_TRACKER_HEIGHT 5
#define SMALL_EXPLOSION_HEIGHT 6
#define MEDIUM_EXPLOSION_HEIGHT 15
#define LARGE_EXPLOSION_HEIGHT 32
#define INVADER_HEIGHT 16
#define TANK_HEIGHT 16
#define PACMAN_HEIGHT 15
#define AIRPLANE_HEIGHT 24
#define MIRANDA_HEIGHT 29
#define BIG_FONT_HEIGHT 31
#define ALIEN_HEIGHT 28
#define MEDIUM_FONT_HEIGHT 11
#define ARROW_HEIGHT 17
#define CANNON_HEIGHT 14


extern const UINT8 missile_tracker_bitmap[BOMB_TRACKER_HEIGHT];
extern const UINT8 missile_bitmap[MISSILE_HEIGHT];
extern const UINT8 small_explosion_bitmap[][SMALL_EXPLOSION_HEIGHT];
extern const UINT8 font_y_bitmap[MEDIUM_FONT_HEIGHT];


/* player -> cannon angle -> bitmap */
extern const UINT16 cannon_bitmap
	[NUM_PLAYERS][CANNON_ANGLES][CANNON_HEIGHT];
extern const UINT16 pacman_bitmap[PACMAN_HEIGHT];
extern const UINT16 square_bitmap[INVADER_HEIGHT];
extern const UINT16 invader_bitmap[INVADER_HEIGHT];
extern const UINT16 medium_explosion_bitmap[MEDIUM_EXPLOSION_HEIGHT];

extern const UINT32 alien_bitmap[ALIEN_HEIGHT];	
extern const UINT32 tank_bitmap[TANK_HEIGHT];	
extern const UINT32 tank2_bitmap[TANK_HEIGHT];	
extern const UINT32 arrow_bitmap[][ARROW_HEIGHT];
extern const UINT32 miranda_bitmap[MIRANDA_HEIGHT];
extern const UINT32 airplane_bitmap[AIRPLANE_HEIGHT];
extern const UINT32 tank_explosion[][LARGE_EXPLOSION_HEIGHT];
extern const UINT32 bigT_bitmap[BIG_FONT_HEIGHT];

	
#endif
