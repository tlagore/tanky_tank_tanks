/*
Name: events

Purpose:
The events source file is called when certain events occur in 
the game.  It is closely linked with the model module as many
events cause a change in the model.

Author: Tyrone Lagore
Version: March 20, 2014
*/

#include <stdlib.h>
#include "defines.h"
#include "events.h"
#include "offsets.h"
#include "effects.h"

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
	{
		tank->angle += 1;
		play_angle_change();
	}
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
	{
		tank->angle -= 1;
		play_angle_change();
	}
}

void event_move_ai_tank(struct Tank * tank, int newX)
{
	move_tank(tank);
	if (tank->x == newX)
		tank->deltaX = 0;
}

/*
Name: event_tank_move_right
Purpose: A movement of the tank to the left has been signalled.

Input:
	struct Tank * tank: The tank object for which the request has been
		issued.
*/
void event_tank_move_right(struct Tank *tank)
{
	tank->deltaX = 3;
	move_tank(tank);
	tank->deltaX = 0;
}

/*
Name: event_tank_left
Purpose: A movement of the tank to the left has been signalled.

Input:
	struct Tank * tank: The tank object for which the request has been
		issued.
*/
void event_tank_move_left(struct Tank *tank)
{
	tank->deltaX = -3;
	move_tank(tank);
	tank->deltaX = 0;
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
Name: event_ai_power_adjust

Purpose: Randomly chooses between 60 and 90 power.  If the
	wind is in the AI's favour, the power is decreased.
	If the wind is not in the AI's favour, the power is increased.
*/
void event_ai_power_adjust(struct Tank * tank, int wind)
{
	tank->power = ((rand() & 3) +  6) * 10;
	if (wind <= -4)
		event_decrease_power(tank);
	else if (wind >= 4)
		event_increase_power(tank);
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
void event_move_missile(Model *model, UINT32 timeElapsed)
{
	move_missile (model->terrain, &model->missile, model->tanks,
		&model->collision, &model->state);

	model->missile.deltaY += (timeElapsed << (GRAVITY_SHIFT));
	
	if (model->windTick == WIND_TICK){
		model->missile.deltaX += model->wind;
		model->missile.deltaX *= timeElapsed;
		model->windTick = 0;
	}else
		model->windTick++;
			
	if (model->collision != MISSILE_IN_AIR && model->state != GAME_OVER)
		model->state = MISSILE_EXPLODING;
	
	if (model->state == GAME_OVER)
		switch_player(&(model->player));
		
}
/*
Name: event_randomized_attack
Purpose: Randomly decides a power and angle and fires a missile

*/
void event_randomized_attack(Model *md)
{
	event_ai_angle_adjust(&(md->tanks[md->player]), md->wind);
	event_ai_power_adjust(&(md->tanks[md->player]), md->wind);
	event_missile_fired(&(md->missile), &(md->tanks[md->player]),
			md->player, &(md->state), &(md->collision));	
}

/*
Name: event_missile_explosion
Purpose: Initiates the missile explosion animation counter and
	plays the missile explosion sound effect. Changes the games 
	state to the end of the turn once it has finished animating,
	unless the game is in the game over state.
	
*/	
void event_missile_explosion(Model *md)
{	
	if (md->missile.expCounter == 0)
		play_explosion(MISSILE_TYPE);
		
	if (md->missile.expCounter <= NUM_EXPLOSIONS)
		md->missile.expCounter++;
	else
	{
		md->missile.expCounter = 0;
		if (md->state != GAME_OVER)
			md->state = TURN_END;
	}
}
/*
Name: event_ai_angle_adjust
Purpose: Randomly chooses an angle between 0 and 3.  To
	add a slight nudge in the direction of formulated attack,
	The AI will reduce increase angle if the wind 
	is in its favour, or decrease it if the wind is not in its 
	favour.  This change has no effect if the tank is already at 
	minimum/maximum angle.
	
Input:
	struct Tank * tank - the tank having its angle adjusted.
*/
void event_ai_angle_adjust(struct Tank * tank, int wind)
{
	tank->angle = (rand() & 3);
	if (wind <= -4)
		event_cannon_up(tank);
	else if (wind >= 4)
		event_cannon_down(tank);
}

/*
Name: event_ai_position_adjust
Purpose: Randomly selects a new x coordinate for the
	tank to move to and sets it's deltaX is the direction
	of the new position.

Input: struct Tank * tank - a reference to the tank randomizing
*/
void event_ai_position_adjust(struct Tank * tank, int * newX)
{
	*(newX) = (rand() & 127) + tank->leftBoundary;
	tank->deltaX = *(newX) < tank->x ? -1 : 1;
}

/* Condition-based events */
/*
Name: event_missile_fired
Purpose: Fires a missile with deltaX and deltaY based on 
	the current angle and power of the tank.
	
	Also plays an explosion sound as the tank shoots the missile.
*/
void event_missile_fired(struct Missile * missile, 
	const struct Tank *tank, UINT16 player, int *state,
	int *collision)
{
	missile->x = tank->x + missile_spawn_x[player][tank->angle];
	missile->y = tank->y - missile_spawn_y[tank->angle];
	
	/* deltaX and deltaY decided by taking the power, 
		multiplying it by a numerator of the sin/cos
		of the respective tank angle then dividing it 
		by 128. (the numerators have been calculated as
		having a denominator of 128 to allow bitshifting)*/
		
	missile->deltaX = ((((tank->power + 70) << 1) * 	
			missile_power_num_x[tank->angle]) >> 7);
	missile->deltaY = ~((((tank->power) << 2) *
			missile_power_num_y[tank->angle]) >> 7) + 1;
			
	/* As the lower powers have almost no use when used with this
		formula, they are increased by the current tank power to
		give them some use */
	if (tank->power < 30)
		missile->deltaY -= tank->power;
	
	/* deltaX is negated if it is player 2 shooting */
	if (player == PLAYER_TWO)
		missile->deltaX = ~(missile->deltaX) + 1;
		
		
	play_explosion(MISSILE_TYPE);
	
	*(collision) = MISSILE_IN_AIR;
	*(state) = MISSILE_FIRED;
}

/*
Name: event_tank_explosion
Purpose: Initiates the tank explosion animation counter and
	plays the tank explosion sound effect. Changes the games 
	state to the game over indication state after finished animating.

*/
void event_tank_explosion(Model *md, UINT32 timeElapsed)
{
	if (md->tanks[md->player].expCounter == 0)
		play_explosion(TANK_TYPE);
		
	if (md->tanks[md->player].expCounter <= TANK_EXPLOSIONS)
		md->tanks[md->player].expCounter+= timeElapsed;
	else
		md->state = GAME_OVER_FINISH;
}

/*
Name: event_tank_hit
Purpose: A tank has been struck.  Health is reduced by 25.
	If the tanks health drops to 0, the game over state is
	induced.
*/
void event_tank_hit(struct Tank *tank, int *state)
{
	if (tank->health > 0)
		tank->health -= 20;
		
	if (tank->health == 0)
		*(state) = GAME_OVER;

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
			if (terrain[i].y - terrain[index].y <= (COLLISION >> 1))
			{	
				terrain[i].y += COLLISION;
				terrain[i].height -= COLLISION;
			}
		}else
		{
			terrain[i].y = LOWEST_TERRAIN_POINT - 1;
			terrain[i].height = 1;
		}
	}
}

/*
Name: event_turn_end
Purpose: Performs end of turn changes. Player is changed,
	the current model state and collision code are set to
	staging, and the wind is randomized.
	
*/
void event_turn_end(Model *model)
{
	UINT16 toggle;
	UINT16 toggle = (rand() & 15);

	switch_player(&(model->player));
	model->collision = STAGING;
	model->state = STAGING;
	
	model->wind = ((rand() & 7) + 1);	
	model->wind = toggle < 7 ? model->wind : ~(model->wind) + 1;

}

/*
Name: switch_player
Purpose: Changes the current player.
*/
void switch_player (UINT16 * currentPlayer)
{
	*(currentPlayer) = 1 - *(currentPlayer);
}

