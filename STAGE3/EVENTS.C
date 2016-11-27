#include "events.h"

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
Name: event_tank_right
Purpose: A movement of the tank to the right has been signalled.

Input:
	struct Tank * tank: The tank object for which the request has been
		issued.
*/
void event_tank_right(struct Tank *tank)
{
	move_tank_right(tank);
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

/*
	Name: event_fire_missile
	Purpose: A tank has fired a missile.  Instantiates the missiles
		x and y values, as well as the deltaX and deltaY of the missile.
*/

/*
void event_fire_missile(struct Tank *tank, struct Missile *missile)
{
		case 0: 
			missile->x = tank->x + width + 3;
			missile->y = tank->y + 3;
			missile->xDelta = power << 4;
			missile->yDelta = 0;
			...

	event_misssile_fired();
}
*/

/* Synchronous events */
int event_move_missile(struct Terrain terrain[],
	struct Missile *missile, struct Tank tanks[])
{
	return move_missile (terrain, missile, tanks);
}

/* Condition-based events */
void event_missile_fired()
{
	/* missile fired sound */
}

void event_tank_destroyed()
{
	/* show tank explosion and sound */
}

/*
Name: event_tank_hit
Purpose: A tank has been struck.  Health is reduced by 25.
*/
void event_tank_hit(struct Tank *tank)
{
	tank->health -= 25;
}

/*
Name: terrain_hit
Purpose: A terrain object has been struck by a missile.  The terrain's
	position is lowered to simulate being damaged.  The surrounding 
	terrain object on either side is also lowered by half the amount.

Input: 
	struct Terrain *terrain[]: The array of terrain objects in which a 
		collision has occurred.
	int index: The index of the terrain object that was struck.
*/
void event_terrain_hit(struct Terrain terrain[], int index)
{	
	if (index >= 0 && index < NUM_TERRAIN && (terrain[index].y + 15) <= LOWEST_TERRAIN_POINT)
	{
		terrain[index].y += 15;
		terrain[index].height -= 15;
	}
	if (index - 1 >= 0 && (terrain[index-1].y + 7) <= LOWEST_TERRAIN_POINT)
	{
		terrain[index-1].y += 7;
		terrain[index-1].y -= 7;
	}
	if (index + 1 <= NUM_TERRAIN && (terrain[index+1].y + 7) <= LOWEST_TERRAIN_POINT)
	{
		terrain[index+1].y += 7;
		terrain[index+1].height = terrain[index+1].height - 7;
	}
}

