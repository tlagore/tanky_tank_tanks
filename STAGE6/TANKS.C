#include <osbind.h>
#include <stdio.h>
#include <stdlib.h>
#include "tanks.h"
#include "input.h"
#include "model.h"
#include "events.h"
#include "renderer.h"

UINT8 secondaryBuffer[BYTES_IN_BUFFER];

int main()
{
	Model md =
	{
		STAGING,
		PLAYER_ONE,
		STARTUP,
		-3,
		GRAVITY,
		{	
			/* Tanks */
			/* Tank 1 @ x = 64, y = 300 */
			{ 
				P1_START_X,
				TANK_Y,
				P1_LEFTB,
				P1_RIGHTB,
				TANK_WIDTH,
				TANK_HEIGHT,
				START_HP,
				START_POW,
				START_ANG,
				START_DELTA,
				0
			},

			/* Tank 2 @ x = 543, y = 300 */			
			{ 
				P2_START_X,
				TANK_Y,
				P2_LEFTB,
				P2_RIGHTB,
				TANK_WIDTH,
				TANK_HEIGHT,
				START_HP,
				START_POW,
				START_ANG,
				START_DELTA,
				0
			}
		},
		{
			/* Staging areas */
		
			{ P1_LEFTB, P1_RIGHTB, 300 }, 
			/* Staging area for Tank 1 from x: 0.159 y: 316 */
			{ P2_LEFTB, P2_RIGHTB - 1, 300 } 
			/* Staging area for Tank 2 from x: 480.639 y: 316 */
		},
		
		{
			/* Terrain Objects */
			
			{ 0, 0, 0}, /* Terrain object 1 */
			{ 0, 0, 0}, /* Terrain object 2 */
			{ 0, 0, 0}, /* Terrain object 3 */
			{ 0, 0, 0}, /* Terrain object 4 */
			{ 0, 0, 0}, /* Terrain object 5 */
			{ 0, 0, 0}, /* Terrain object 6 */
			{ 0, 0, 0}, /* Terrain object 7 */
			{ 0, 0, 0}, /* Terrain object 8 */
			{ 0, 0, 0}, /* Terrain object 9 */
			{ 0, 0, 0}, /* Terrain object 10 */
			{ 0, 0, 0}, /* Terrain object 11 */
			{ 0, 0, 0}, /* Terrain object 12 */
			{ 0, 0, 0}, /* Terrain object 13 */
			{ 0, 0, 0}, /* Terrain object 14 */
			{ 0, 0, 0}, /* Terrain object 15 */
			{ 0, 0, 0}, /* Terrain object 16 */
			{ 0, 0, 0}, /* Terrain object 17 */
			{ 0, 0, 0}, /* Terrain object 18 */
			{ 0, 0, 0}, /* Terrain object 19 */
			{ 0, 0, 0}, /* Terrain object 20 */
			{ 0, 0, 0}, /* Terrain object 21 */
			{ 0, 0, 0}, /* Terrain object 22 */
			{ 0, 0, 0}, /* Terrain object 23 */
			{ 0, 0, 0}, /* Terrain object 24 */
			{ 0, 0, 0}, /* Terrain object 25 */
			{ 0, 0, 0}, /* Terrain object 26 */
			{ 0, 0, 0}, /* Terrain object 27 */
			{ 0, 0, 0}, /* Terrain object 28 */
			{ 0, 0, 0}, /* Terrain object 29 */
			{ 0, 0, 0}, /* Terrain object 30 */
			{ 0, 0, 0}, /* Terrain object 31 */
			{ 0, 0, 0}, /* Terrain object 32 */
			{ 0, 0, 0}, /* Terrain object 33 */
			{ 0, 0, 0}, /* Terrain object 34 */
			{ 0, 0, 0}, /* Terrain object 35 */
			{ 0, 0, 0}, /* Terrain object 36 */
			{ 0, 0, 0}, /* Terrain object 37 */
			{ 0, 0, 0}, /* Terrain object 38 */
			{ 0, 0, 0}, /* Terrain object 39 */
			{ 0, 0, 0} /* Terrain object 40 */
		},           
		
		/* Missile */
		{ 0, 0, 0, 0, 0}, /* Missile Object, uninstantiated except for
								width/height */
		P2_START_X,
		0
	};

	/* MAIN BEGIN */
	UINT32 	latestTick, timeNow, timeElapsed, *fBfrPtr, *bBfrPtr,
			bBfrAddress, mBfrAddress, *bfrPtrs[2];
	UINT16 curBuffer = 0;
	
	
	mBfrAddress = (UINT32)Physbase();
	bfrPtrs[0] = (UINT32*)mBfrAddress;
	
	bBfrAddress = (UINT32)(secondaryBuffer + 256);
	bBfrAddress &= ~(0xFF);
	bfrPtrs[1] = (UINT32*)bBfrAddress;
	
	/*bBfrPtr = (UINT32*)bBfrAddress;*/
	
	srand(get_time());
	

	/*fBfrPtr = (UINT32*)fBfrAddress;*/

	
	randomize_terrain(md.terrain);
	render(&md, bfrPtrs[0]);
	render(&md, bfrPtrs[1]);
	md.state = STAGING;
	
	latestTick = get_time();

	while (md.state != GAME_OVER_FINISH)
	{
		if (input_available())
			handle_async (&md, bfrPtrs[curBuffer]);
			
		timeNow = get_time();
		if (timeNow != latestTick)
		{
			handle_sync(&md, bfrPtrs[curBuffer], (timeNow - latestTick));
			render (&md, bfrPtrs[curBuffer]);
			Setscreen(-1, bfrPtrs[curBuffer], -1);
			latestTick = get_time();
			curBuffer = 1 - curBuffer;
		}

	}
	
	Setscreen(-1, bfrPtrs[0], -1);	
	return 0;
}

void handle_sync(Model * md, UINT32 * base, UINT32 timeElapsed)
{
	UINT16 i;
	switch (md->state)
	{
		case TURN_END:
			if (md->player == PLAYER_TWO)
				event_ai_position_adjust(&(md->tanks[PLAYER_TWO]),
						&(md->aiTankNewX));
						
			event_turn_end(md);
			break;
		
		case STAGING:
			if (md->player == PLAYER_TWO)
			{
				if (md->tanks[md->player].x == md->aiTankNewX)  
					event_randomized_attack(md);		
													/*When tank position is within */  
													/*2 pixels of new position, attack*/
				else
					event_move_ai_tank(&md->tanks[md->player], md->aiTankNewX);	
													/* else keep moving */
			}
			break;
			
		case MISSILE_FIRED:
			event_move_missile(md, timeElapsed);
			break;
		case MISSILE_EXPLODING:
			event_missile_explosion(md);
			break;
		case GAME_OVER:
			if (md->tanks[md->player].expCounter == 0)
				event_missile_explosion(md);
			if (md->missile.expCounter == 0)
				event_tank_explosion(md);
			break;
	};
}

void handle_async(Model * md, UINT32 * base)
{
	char input = get_input();
	if (md->state == STAGING && md->player != PLAYER_TWO)
	{
		switch (input)
		{
			case LEFT:
				event_tank_move_left(&md->tanks[md->player]);
				break;
			case RIGHT:
				event_tank_move_right(&md->tanks[md->player]);
				break;
			case UP:
				event_cannon_up(&md->tanks[md->player]);
				break;
			case DOWN:
				event_cannon_down(&md->tanks[md->player]);
				break;
			case W_PRESS:
				event_increase_power(&md->tanks[md->player]);
				break;
			case S_PRESS:
				event_decrease_power(&md->tanks[md->player]);
				break;
			case SPACE:
				event_missile_fired(&md->missile, 
					&(md->tanks[md->player]),
					md->player,
					&(md->state),
					&(md->collision));
				break;
			case ESCAPE:
				md->state = GAME_OVER;
				break;
		};
	}
}

void switch_buffers(UINT32 *bffrPtr1, UINT32 *bffrPtr2)
{
	UINT32 *temp, timeNow = get_time();
	
	while (timeNow - get_time() == 0);
	
	Setscreen(-1, bffrPtr2, -1);
	
	temp = bffrPtr2;
	bffrPtr2 = bffrPtr1;
	bffrPtr1 = temp;
}

UINT32 get_time()
{
	UINT32 old_ssp;
	UINT32 *timer = (UINT32*)TIMER;
	UINT32 timeNow;
	old_ssp = Super(0);
	timeNow = *timer;
	Super(old_ssp);
	return timeNow;
}
