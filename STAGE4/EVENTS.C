/*
Name: events

Purpose:
The events source file is called when certain events occur in 
the game.  It is closely linked with the model module as many
events cause a change in the model.

Author: Tyrone Lagore
Version: March 7, 2014
*/

#include <stdlib.h>
#include "defines.h"
#include "events.h"
#include "offsets.h"

/*
Name: event_cannon_ up
Purpose: The cannon angle of a tank has been signalled to increase

Input: 
	struct Tank * tank: The tank object for which the request has been
		issued.
*/
void event_cannon_up(struct Tank *tank)
{
	if(tank->angle < 3)
		tank->angle += 1;
}

/*
Name: event_cannon_ down
Purpose: The cannon angle of a tank has been signalled to decrease

Input: 
	struct Tank * tank: The tank object for which the request has been
		issued.
*/
void event_cannon_down(struct Tank *tank)
{
	if(tank->angle > 0)
		tank->angle -= 1;
}

/*
Name: event_tank_left
Purpose: A movement of the tank to the left has been signalled.

Input:
	struct Tank * tank: The tank object for which the request has been
		issued.
*/
void event_tank_move(struct Tank *tank)
{
	move_tank(tank);
}


/*
Name: event_increase_power
Purpose: An increase of the power of the current tanks shot has been
	signalled.
	
Input:
	struct Tank * tank: The tank object for which the request has been
		issued.
*/
void event_increase_power(struct Tank *tank)
{
	if(tank->power < 100)
		tank->power += 10; 
}

/*
Name: event_decrease_power
Purpose: A decrease of the power of the current tanks shot has been
	signalled.
	
Input:
	struct Tank * tank: The tank object for which the request has been
		issued.
*/
void event_decrease_power(struct Tank *tank)
{
	if(tank->power > 0)
		tank->power -= 10;
}


/* Synchronous events */
/*
Name: event_move_missile
Purpose: Moves the missile one time based on it's
	current deltaX and deltaY by calling move_missile
	in the model.  Returns a collision code provided
	by move_missile.
*/
UINT16 event_move_missile(struct Terrain terrain[],
	struct Missile *missile, struct Tank tanks[])
{
	return move_missile (terrain, missile, tanks);
}
/*

*/
void event_ai_angle_adjust(struct Tank * tank)
{
	tank->angle = (rand() & 3);
}

/*
Name: event_ai_position_adjust
Purpose: Randomly selects a new x coordinate for the
	tank to move to and sets it's deltaX is the direction
	of the new position.

Input: struct Tank * tank - a reference to the tank randomizing

Assumptions: Caller will handle the actual move process of moving
	the tank to the new position.

*/
UINT16 event_ai_position_adjust(struct Tank * tank)
{
	UINT16 newPosition = (rand() & 127);
	tank->deltaX = newPosition < tank->x ? -5 : 5;
	return newPosition;
}

/* Condition-based events */

void event_missile_fired(struct Missile * missile, 
	const struct Tank tank, UINT16 player)
{
	missile->x = tank.x + missile_spawn_x[player][tank.angle];
	missile->y = tank.y - missile_spawn_y[tank.angle];
	
	missile->deltaX = (((tank.power + 10) << 1) * 
			missile_power_num_x[tank.angle]) >> 8;
	missile->deltaY = ~((((tank.power + 10) << 1) *
			missile_power_num_y[tank.angle]) >> 8) + 1;
			
	if (player == PLAYER_TWO)
		missile->deltaX = ~(missile->deltaX) + 1;
}

void event_tank_destroyed()
{
	/* show tank explosion and sound */
}

/*ts
Name: event_tank_hit
Purpose: A tank has been struck.  Health is reduced by 25.
*/
void event_tank_hit(struct Tank *tank)
{
	if (tank->health > 0)
		tank->health -= 25;
}

/*
Name: terrain_hit
Purpose: A terrain object has been struck by a missile.  The terrain's
	position is lowered to simulate being damaged.  The surrounding 
	terrain object on either side is also lowered by half the amount.
	
	A terrain object's position is only lowered 
		a) the object is within the range of the terrain array and
		b) if reducing the position will not lower it beyond the
			lowest terrain point.

Input: 
	struct Terrain *terrain[]: The array of terrain objects in which a 
		collision has occurred.
	int index: The index of the terrain object that was struck.
*/
void event_terrain_hit(struct Terrain terrain[], int index)
{	

	int i;
	for (i = (index - 1); i <= (index + 1); i++ )
	{
		if (i >= 0 && i < NUM_TERRAIN && 
			(terrain[i].y + COLLISION) <= LOWEST_TERRAIN_POINT)
		{
			terrain[i].y += COLLISION;
			terrain[i].height -= COLLISION;
		}else
		{
			terrain[i].y = LOWEST_TERRAIN_POINT - 1;
			terrain[i].height = 1;
		}
	}
}

