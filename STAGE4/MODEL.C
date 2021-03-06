/*
Name: model

Purpose:
The model source file allows for the manipulation of objects
fields based on the events occurring within the game.  Some
model functions may also cause certain other events to occur
should certain conditions be met (such as a missile colliding
with an object).

Author: Tyrone Lagore
Version: March 7, 2014
*/
#include "defines.h"
#include "model.h"
#include "events.h"

#include <stdlib.h>

/*
Name: move_tank
Input:
	struct Tank * tank: The tank object that is moving
Output:
	None

Assumptions:
	Tank is always moving 4 pixels at a time.
	When used in AI tank moving, timed delays are done outside
	of function call. 
*/
void move_tank(struct Tank * tank)
{
	int newX = tank->x + tank->deltaX;
	if (newX >= tank->leftBoundary && newX < 
			tank->rightBoundary - tank->width)
		{tank->x = newX;}
}

/*
Name: move_missile
Purpose: Moves the missile's x and y coordinates according 
to it's current deltaX and deltaY, as long as the missile 
does not break it's x boundaries or collide with another 
object.

Input:
	struct Missile * missile: The missile object that is having
		it's coordinates re-evaluated
	struct Terrain * terrain: An array of terrain objects, used 
		in collision detection
	const struct Tank tanks: The two tanks on the board, used
		in collision detection
	
Output: 
	collision: A collision code is returned to inform the caller
		what type of collision occurred.  This allows the caller
		to know to check if a tank has died as result of 
		the returned indicated collision.
		
		0 - No collision
		1-40 - Terrain Collision.  Code indicates index hit (+1)
		41/42 - Tank collision.  Code indicates tank hit 
				(41 -> player 1, 42 -> player 2)
		43 - Boundary collision (checked last)

Assumptions: 
	Missile deltaX and deltaY changes are occurring outside of this 
	function.
*/
UINT16 move_missile(struct Terrain terrain[], struct Missile * missile,
	const struct Tank tanks[])
{
	int i;
	UINT16 collision = 0;
	int newX = missile->x + (missile->deltaX >> 3);
	int newY = missile->y + (missile->deltaY >> 3);
	int terrainPos = (newX >> 3) - TERRAIN_OFFSET;
	
	/* check for bounds collision */
	if (terrainPos >= -1 && terrainPos <= 39)
		collision = check_terrain_collision(newX, 
					newY + missile->height,
					terrain, 
					terrainPos);
		
	
	/* check for collision with tanks */
	for (i = 0; i < NUM_TANKS && !collision; i++)
		if (check_collision(newX,
				newY, missile->width,
				missile->height,
				tanks[i].x,
				tanks[i].y,
				tanks[i].width,
				tanks[i].height))
				{
					collision = TANK_COLLISON_OFFSET + i;
					event_tank_hit(&tanks[i]);
				}
	
	
	/* if there hasn't been a collision yet, and missile is within screen bounds... */
	/* allow coordinate change */
	if (!collision)
		if (newX >= 0 && (newX + missile->width) < SCREEN_WIDTH 
			&& newY + (int)missile->height <= LOWEST_TERRAIN_POINT)
		{
			missile->previousX = missile->x;
			missile->previousY = missile->y;
			missile->x = newX;
			missile->y = newY;
		}else
			collision = 43;	
			
	return collision;
}

/*
Name: check_terrain_collision
Purpose: Checks for a collision within the array of destructible terrain.

Method: First the index of the array element that the missile is currently
	over top of is found.  This is done by using the knowledge that the 
	array of terrain objects are 8 bits wide (the same width as the missile)
	and that they begin exactly 20 bytes into the screen.  By dividing the
	missiles coordinates by 8 (bit shifting by 3) and subtracting 20, the 
	element index is attained.
	
	The modulo of the x coordinate by 8 (x & 7) is attained in order 
	to see if the missile is currently directly over top of or straddling 
	
Inputs:
	int newX - the value that 
	int yCollisionPoint
	struct Terrain terrain
	UINT16 & collision
	int terrainPos
*/
UINT16 check_terrain_collision (int newX, int yCollisionPoint,	
	struct Terrain terrain[], int terrainPos)
{
	UINT16 collision = 0;
	int mod = (newX & 7);	

	if (mod == 0 || (terrainPos == 39 && mod != 0))
		collision = single_terrain_check(terrain, yCollisionPoint, 
			terrainPos);
	else if (terrainPos == -1)
		collision = single_terrain_check(terrain, yCollisionPoint, 
			terrainPos + 1);
	else
	{
		collision = single_terrain_check(terrain, yCollisionPoint, 
			terrainPos);
		if (!collision)
			collision = single_terrain_check(terrain, 
				yCollisionPoint, terrainPos + 1);
	}
	
	return collision;

}


/*
Name: single_terrain_check
Purpose: Checks if the specified y collision point has exceeded the 
		y coordinate of the indexed terrain piece.

Inputs:
		struct Terrain terrain[] - The array of terrain objects
		int yCollisionPoint - The y value that, if exceeding the y
			value of the terrain object, indicates collision.
		int index - The index of the array being checked.
			
Output:
		UINT16 collision - If a collision
			occurs, this will be set equal to the index of the
			collision + 1.  The 1 is added as a 0 indicates no
			collision, if the 0th index is hit, it must be known.


*/
UINT16 single_terrain_check(struct Terrain terrain[], int yCollisionPoint,
	int index)
{	
	UINT16 collision = 0;

	if (yCollisionPoint >= terrain[index].y)
	{
		event_terrain_hit(terrain, index);
		collision = index + 1;
	}
	
	return collision;
}


/*
Name: check_collision
Purpose: Checks for a collision between two objects given the (x,y) 
	coordinates, height, and width of both objects.

Input: 
	obj1X: Object 1's x coordinate
	obj1Y: Object 1's y coordinate
	obj1Width: Object 1's width
	obj1Height: Object 1's height
	obj2X: Object 2's x coordinate
	obj2Y: Object 2's y coordinate
	obj2Width: Object 2's width
	obj2Height: Object 2's height
	
Output:
	int: Returns 1 if a collision was found, or 0 if it was not.

*/
int check_collision(UINT16 obj1X, UINT16 obj1Y, UINT16 obj1Width, UINT16
	obj1Height, UINT16 obj2X, UINT16 obj2Y, UINT16 obj2Width, 
	UINT16 obj2Height)
{
	return ((obj1X + obj1Width) >= (obj2X) &&
		(obj1X) <= (obj2X + obj2Width) &&
		(obj1Y + obj1Height) >= (obj2Y) &&
		(obj1Y) <= (obj2Y + obj2Height));
}

/*


*/
void randomize_terrain(struct Terrain terrain[])
{
	int i, change, toggle;
	UINT16 x = TERRAIN_START - 1;
	UINT16 y = LOWEST_TERRAIN_POINT - 1;

	terrain[0].x = x;			/* First terrain block is static */
	terrain[0].y = y;			/* To avoid weird clipping */
	terrain[0].height = 1;
	
	terrain[39].x = TERRAIN_FINISH;
	terrain[39].y = y;
	terrain[39].height = 1;
	
	for (i = 1; i < 39; i++)
	{
		x += terrain[i].width;
	
		change = (rand() & 15);
		toggle = (rand() & 127);
		terrain[i].x = x;
		terrain[i].y = toggle < 64 ? 
					(y + change) : (y-change);
					
		if (terrain[i].y > LOWEST_TERRAIN_POINT)
			terrain[i].y = LOWEST_TERRAIN_POINT - 1;
					
		terrain[i].height = (LOWEST_TERRAIN_POINT - terrain[i].y) > 0 ? 
				LOWEST_TERRAIN_POINT - terrain[i].y : 1;
	
		y = i < 20  ? y -= 12 : y += 12;
	}
}


