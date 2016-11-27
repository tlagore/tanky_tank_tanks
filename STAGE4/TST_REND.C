#include <stdio.h>
#include <osbind.h>
#include <stdlib.h>
#include <unistd.h>

#include "events.h"
#include "renderer.h"
#include "defines.h"

struct Model ts =
{
	STARTUP,
	0,
	{	
		/* Tanks */
		/* Tank 1 @ x = 64, y = 300 */
		{ 64, 284, 0, 159, 32, 16, 100, 100, 0, 0 },

		/* Tank 2 @ x = 543, y = 300 */			
		{ 543, 284, 480, 639, 32, 16, 100, 100, 0, 0} 
	},
	{
		/* Staging areas */
	
		{ 0, 159, 300 }, 
		/* Staging area for Tank 1 from x: 0.159 y: 316 */
		{ 480, 639, 300 } 
		/* Staging area for Tank 2 from x: 480.639 y: 316 */
	},
	
	{
		/* Terrain Objects */
		
		{ 0, 0, 0, 8}, /* Terrain object 1 */
		{ 0, 0, 0, 8}, /* Terrain object 2 */
		{ 0, 0, 0, 8}, /* Terrain object 3 */
		{ 0, 0, 0, 8}, /* Terrain object 4 */
		{ 0, 0, 0, 8}, /* Terrain object 5 */
		{ 0, 0, 0, 8}, /* Terrain object 6 */
		{ 0, 0, 0, 8}, /* Terrain object 7 */
		{ 0, 0, 0, 8}, /* Terrain object 8 */
		{ 0, 0, 0, 8}, /* Terrain object 9 */
		{ 0, 0, 0, 8}, /* Terrain object 10 */
		{ 0, 0, 0, 8}, /* Terrain object 11 */
		{ 0, 0, 0, 8}, /* Terrain object 12 */
		{ 0, 0, 0, 8}, /* Terrain object 13 */
		{ 0, 0, 0, 8}, /* Terrain object 14 */
		{ 0, 0, 0, 8}, /* Terrain object 15 */
		{ 0, 0, 0, 8}, /* Terrain object 16 */
		{ 0, 0, 0, 8}, /* Terrain object 17 */
		{ 0, 0, 0, 8}, /* Terrain object 18 */
		{ 0, 0, 0, 8}, /* Terrain object 19 */
		{ 0, 0, 0, 8}, /* Terrain object 20 */
		{ 0, 0, 0, 8}, /* Terrain object 21 */
		{ 0, 0, 0, 8}, /* Terrain object 22 */
		{ 0, 0, 0, 8}, /* Terrain object 23 */
		{ 0, 0, 0, 8}, /* Terrain object 24 */
		{ 0, 0, 0, 8}, /* Terrain object 25 */
		{ 0, 0, 0, 8}, /* Terrain object 26 */
		{ 0, 0, 0, 8}, /* Terrain object 27 */
		{ 0, 0, 0, 8}, /* Terrain object 28 */
		{ 0, 0, 0, 8}, /* Terrain object 29 */
		{ 0, 0, 0, 8}, /* Terrain object 30 */
		{ 0, 0, 0, 8}, /* Terrain object 31 */
		{ 0, 0, 0, 8}, /* Terrain object 32 */
		{ 0, 0, 0, 8}, /* Terrain object 33 */
		{ 0, 0, 0, 8}, /* Terrain object 34 */
		{ 0, 0, 0, 8}, /* Terrain object 35 */
		{ 0, 0, 0, 8}, /* Terrain object 36 */
		{ 0, 0, 0, 8}, /* Terrain object 37 */
		{ 0, 0, 0, 8}, /* Terrain object 38 */
		{ 0, 0, 0, 8}, /* Terrain object 39 */
		{ 0, 0, 0, 8} /* Terrain object 40 */
	},
	
	/* Missile */
	{ 0, 0, 0, 0, 0, 0, 8, 8} /* Missile Object, uninstantiated except for
							width/height */
};


int main()
{
	UINT16 i;
	UINT16 player = PLAYER_ONE;
	UINT16 playerHit;
	int oldX;
	int oldY;
	int wind = rand() & 15;
	int gameOver = 0;
	UINT32 x2Hex;
	UINT8 *base8 = Physbase();
	UINT16 *base16 = Physbase();
	UINT32 *base32 = Physbase();
	
	randomize_terrain(ts.terrain);
	render (&ts, base32, PLAYER_ONE);

	ts.gameCondition = STAGING;
	
	ts.tanks[0].deltaX = -4;
	
	for (i = 0; i < 10; i ++)
	{
		event_tank_move(&ts.tanks[player]);
		ts.tanks[player].power = 70;
		ts.tanks[player].angle = (i & 3);
		sleep(1);
		render(&ts, base32, PLAYER_ONE);
		player = switch_player (player);
	}
	
	for (i = 0; i < 40 && !gameOver; i++)
	{
		player = switch_player (player);
		ts.tanks[player].angle = (rand() & 3);
		ts.tanks[player].power = (rand() % 11) * 10;
		render(&ts, base32, player);
		
		event_missile_fired(&ts.missile, ts.tanks[player], player);
		ts.gameCondition = MISSILE_FIRED;
		ts.gameCondition = event_move_missile(ts.terrain, &ts.missile, ts.tanks);
		while (!ts.gameCondition)
		{
			render (&ts, base32, player);
			ts.missile.deltaY += 2;
			ts.gameCondition = event_move_missile(ts.terrain, &ts.missile, 
				ts.tanks);	
			sleep(1);
		}
		render(&ts, base32, player);
		
		if (ts.gameCondition == 41 || ts.gameCondition == 42)
		{
			playerHit = ts.gameCondition - TANK_COLLISION_OFFSET;
			if (ts.tanks[playerHit].health == 0)
			{
				render_tank_explosion(
					&ts.tanks[player],
					player,
					base32);
				
				gameOver = 1;
				printf("Game Over. \n");
			}
		}
		
		ts.gameCondition = STAGING;
		
	}
	
	
	return 0;
}

	
