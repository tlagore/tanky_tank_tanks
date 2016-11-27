#ifndef MODEL_H
#define MODEL_H

#include "types.h"

#define SCREEN_WIDTH 640
#define TERRAIN_OFFSET 20
#define NUM_TERRAIN 40
#define NUM_TANKS 2
#define NUM_PLAYERS 2
#define PLAYER_ONE 0
#define PLAYER_TWO 1
#define LOWEST_TERRAIN_POINT 300
#define TERRAIN_START 160
#define TERRAIN_FINISH 472
#define TERR_OFFSET 9

#define TANK_COLLISON_OFFSET 41
 

struct Missile
{
	int x,y;  /* position */
	int deltaX, deltaY; /* velocity */
	UINT16 expCounter;
	UINT16 height;
	UINT16 width;
};

struct Tank
{
	int x, y; /* position */
	UINT16 leftBoundary; 
	UINT16 rightBoundary; /* boundaries for movement */
	UINT16 width;
	UINT16 height;
	UINT16 health;
	UINT16 power; /* power of current shot */
	UINT16 angle; /* angle of current shot */
	int deltaX; /* move speed */
};

struct Stage
{
	int x1,x2,y; /* staging area for each tank */
};

struct Terrain
{
	int x, y;  /* position */
	UINT16 height, width;
};

struct Model
{	
	int collision;
	UINT16 player;
	int state;
	int wind;
	UINT16 gravity;
	struct Tank tanks[2]; /* 0 = player 1, 1 = player 2 (or computer) */
	struct Stage stages[2]; /* tank staging areas */
	struct Terrain terrain[40]; /* terrain in between staging areas */
	struct Missile missile; /* only 1 missile will ever be present */
	int aiTankNewX;
};

struct Snapshot
{
	int wind;
	struct Tank tanks[2]; /* 0 = player 1, 1 = player 2 (or computer) */
	struct Missile missile;
};


UINT16 check_terrain_collision (int newX, int yCollisionPoint,	
	struct Terrain terrain[], int terrainPos);
UINT16 single_terrain_check(struct Terrain terrain[], int newY, 
	int index);
void move_missile(struct Terrain terrain[], struct Missile *missile,
	const struct Tank tanks[], int * collision);
void move_tank(struct Tank *tank);
int check_collision(UINT16 obj1X, UINT16 obj1Y, UINT16 obj1Wid,
	UINT16 obj1Ht,UINT16 obj2X, UINT16 obj2Y, UINT16 obj2Wid,
	UINT16 obj2Ht);
void randomize_terrain(struct Terrain terrain[]);
 


#endif