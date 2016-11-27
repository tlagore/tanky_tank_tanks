#include "events.h"

/*
Name: move_tank_left
Input:
	struct Tank * tank: The tank object that is moving left
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
	if (newX >= tank->leftBoundary && newX < tank->rightBoundary - tank->width)
		{tank->x = newX;}
}

/*
Name: move_tank_right
Input:
	struct Tank * tank: The tank object that is moving right
Output:
	None
*/
void move_tank_right (struct Tank * tank)
{
	if (tank->x + tank->deltaX <= tank->rightBoundary)
		tank->x += tank->deltaX;
}

/*
Name: move_missile
Purpose: Moves the missile's x and y coordinates according 
to it's current deltaX and deltaY, as long as the missile 
does not break it's x boundaries.

Input:
	struct Missile * missile: The missile object that is having
		it's coordinates re-evaluated
	struct Terrain * terrain: An array of terrain objects, used 
		in collision detection
	const struct Tank tanks: The two tanks on the board, used
		in collision detection
	
Output: 
	collision: A collision code is returned to inform the caller
		what type of collision occurred.  This is to know to check
		if a tank has died if the collision code indicates that a
		tank was struck.
		
		0 - No collision
		1 - Tank collision
		2 - Terrain collision
		3 - Boundary collision (checked last)

Assumptions: 
	Missile deltaX and deltaY changes are occurring outside of this 
	function.
*/
int move_missile(struct Terrain * terrain[], struct Missile * missile,
	const struct Tank tanks[])
{
	int i;
	UINT16 collision = 0;
	int newX = missile->x + (missile->deltaX >> 3);
	int newY = missile->y + (missile->deltaY >> 3);
	
	/* check for bounds collision */
	collision = check_terrain_collision(newX, newY + missile->height, terrain);
	
	/* check for collision with tanks */
	for (i = 0; i < NUM_TANKS && !collision; i++)
		if (check_collision(newX, newY, missile->width,
			missile->height, tanks[i].x ,tanks[i].y, tanks[i].width,
			tanks[i].height))
			{
				collision = 1;
				event_tank_hit(&tanks[i]);
			}
			
	/* if there hasn't been a collision yet, and missile is within screen bounds... */
	/* allow coordinate change */
	
	if (!collision)
		if (newX >= 0 && newX + missile->width < SCREEN_WIDTH)
		{
			missile->x = newX;
			missile->y = newY;
		}else
			collision = 3;
			
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
*/
int check_terrain_collision (int newX, int yCollisionPoint,	struct Terrain *terrain[])
{
	int terrainPos = (newX >> 3) - TERRAIN_OFFSET;
	int mod = (newX & 7);
	int collision = 0;
	
	if (terrainPos >= -1 && terrainPos < 40)
	{
		if (mod == 0 || (terrainPos == 39 && mod != 0))
			collision = single_terrain_check(terrain, yCollisionPoint, terrainPos);
		else if (terrainPos == -1)
			collision = single_terrain_check(terrain, yCollisionPoint, terrainPos + 1);
		else
		{
			collision = single_terrain_check(terrain, yCollisionPoint, terrainPos);
			if (!collision)
				collision = single_terrain_check(terrain, yCollisionPoint, terrainPos + 1);
		}
	}
	
	return collision;
}


/*


*/
int single_terrain_check(struct Terrain terrain[], int newY, int index)
{
	int collision = 0;
	
	if (newY >= terrain[index].y)
	{
		event_terrain_hit(terrain, index);
		collision = 2;
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


