#ifndef DEFINES_H
#define DEFINES_H

#define PLAYER_ONE 0
#define PLAYER_TWO 1
#define NUM_PLAYERS 2

#define STARTUP -3
#define STAGING -2
#define MISSILE_FIRED -1

#define MISSILE_IN_AIR 0

#define TERRAIN_MIN_CODE 1
#define TERRAIN_MAX_CODE 40

#define TANK_STRUCK1 41
#define TANK_STRUCK2 42

#define CANNON_ANGLES 4

#define BYTE 7
#define WORD 15
#define LONG 31

#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 640
#define NUM_BYTES 80
#define NUM_WORDS 40
#define NUM_LONGS 20


#define FILL 1
#define NOFILL 0

#define CANNON_Y_OFFSET 10

/* Health positions and dimensions */
#define HP_WIDTH 102
#define HP_HEIGHT 22
#define HP_W_CLEAR 100
#define HP_H_CLEAR 22

#define P1_HEALTH_X 9
#define P2_HEALTH_X 529
#define HEALTH_Y 369

#define P1_HPNUM_X 117
#define P2_HPFONT_X 496
#define P1_HPWORD_X 119
#define P2_HPWORD_X 472
#define HPWORD_Y 382

/* Power positions and dimensions */
#define POW_WIDTH 22
#define POW_HEIGHT 52
#define POW_W_CLEAR 21
#define POW_H_CLEAR 50

#define P1_POW_X 9
#define P2_POW_X 609
#define POW_Y 310

#define P1_POWNUM_X 40
#define POWNUM_Y 324
#define P1_POW_WORD_X 40
#define P2_POW_WORD_X 559
#define P2_POWNUM_X 569

/* Player word position */

#define PLAYER_X 290
#define PLAYER_Y 375

#define PLAYER_NUM_X 345

/* Wind position */

#define WIND_X 303
#define WIND_Y 330

#define LEFT_WIND 0
#define RIGHT_WIND 1


#define TANK_COLLISION_OFFSET 41

#endif
