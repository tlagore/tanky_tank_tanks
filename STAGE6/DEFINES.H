#ifndef DEFINES_H
#define DEFINES_H

/* GENERAL DEFINES */
#define TIMER 0x462

#define PLAYER_ONE 0
#define PLAYER_TWO 1
#define NUM_PLAYERS 2

/* TANK STARTING STATS */

#define P1_START_X 64
#define P1_PREVIOUS_X 64
#define P1_LEFTB 0
#define P1_RIGHTB 160

#define P2_START_X 543
#define P2_PREVIOUS_X 543
#define P2_LEFTB 480
#define P2_RIGHTB 640

#define TANK_Y 284
#define START_HP 100
#define START_POW 100
#define START_ANG 0
#define TANK_WIDTH 32
#define START_DELTA 0
#define TANK_HEIGHT 16

#define GRAVITY 2
#define GRAVITY_SHIFT 1

#define WIND_TICK 6

/* GAME CONDITION DEFINES */
#define GAME_OVER_FINISH -5

#define GAME_OVER -4

#define STARTUP -3
#define STAGING -2

#define MISSILE_IN_AIR 0

#define TERRAIN_MIN_CODE 1
#define TERRAIN_MAX_CODE 40

#define TANK1_STRUCK 41
#define TANK2_STRUCK 42

#define OUT_OF_BOUNDS 43


#define TANK_STRUCK 44
#define TURN_END 45
#define TURN_END_REND 46

#define MISSILE_EXPLODING 47

#define MISSILE_FIRED 48

#define TANK_COLLISION_OFFSET 41

/* BITMAP DEFINES */
#define BYTE 7
#define WORD 15
#define LONG 31

#define TANK_EXP_Y 16

#define MISSILE_WIDTH 6
#define MISSILE_HEIGHT 6

#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 640
#define NUM_BYTES 80
#define NUM_WORDS 40

#define NUM_LONGS 20

#define FILL 1
#define NOFILL 0

/* RENDER DEFINES */
#define CANNON_ANGLES 4

#define NUM_EXPLOSIONS 32

#define TANK_EXPLOSIONS 97

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
#define PLAYER_Y 385

#define PLAYER_NUM_X 345

/* Wind position */

#define WIND_X 303
#define WIND_Y 330

#define WIND_WORD_Y WIND_Y+LONG

#define LEFT_WIND 0
#define RIGHT_WIND 1

#endif