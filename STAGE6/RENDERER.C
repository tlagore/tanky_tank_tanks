/*
Name: renderer

Purpose: 
The renderer module allows for the visual representation of an entire
md, or specific parts of the md based on the current state
of the objects within the md->

Author: Tyrone Lagore
Version: March 10, 2014
*/

#include "defines.h"
#include "renderer.h"
#include "offsets.h"
#include "bitmaps.h"
#include "font.h"
#include "raster.h"
#include <unistd.h>

typedef struct Snapshot
{
	int wind;
	struct Tank tanks[2]; /* 0 = player 1, 1 = player 2 (or computer) */
	struct Missile missile;
	struct Terrain terrain[40];
	int state;
	UINT16 player;
}Snapshot;


Snapshot snaps[2];
UINT16 curSnap = 0;


/*
Name: render
Purpose: Renders the entire md to the screen
*/
void render(const Model * md, UINT32 * base32)
{
	UINT8 *base8 = (UINT8*) base32;
	UINT16 playerStruck;

	snaps[curSnap].state = md->state;
	
	if (snaps[1 - curSnap].state == GAME_OVER || 
		(snaps[1 - curSnap].state == MISSILE_EXPLODING &&
		snaps[curSnap].state != MISSILE_EXPLODING))
				render_affected(md, base32);

	switch (md->state)
	{
		case STARTUP:
			render_startup(base32, md);
			break;
		case STAGING:
			render_staging(base32, md);
			break;
		case MISSILE_FIRED:
			render_missile(md, base8);
			if (md->player == PLAYER_TWO)
				render_staging(base32, md); 
				/* renders the last movement of the ai to the alt buffer */
			break;
		case MISSILE_EXPLODING:
			render_missile_impact(md, base8);
			break;
		case GAME_OVER:
			render_tank_explosion(base32, md);
	};
	
	render_turn_end(base32, md);
	curSnap = 1 - curSnap;

}

/*
Name: render_startup
Purpose: renders the first frame of the model.

*/
void render_startup(UINT32 *base32, const Model *md)
{
	UINT8 *base8 = (UINT8*) base32;
	UINT16 i;
	
	take_snapshot(md);
	
	clear_screen(base32);
	render_terrain(md->terrain, base32);
	render_constants(base8);
	render_current_player(base8, PLAYER_ONE);
	
	render_wind(base32, md->wind);	
	
	for (i = 0; i < NUM_PLAYERS; i++)
	{
		render_tank(&md->tanks[i], i, base32);
		plot_hline(base32, md->stages[i].x1, md->stages[i].x2,
				md->stages[i].y);
			
		render_health(md->tanks[i].health, i,base32);		
		render_power(md->tanks[i].power, i, base32);
	}
} 

/*
Name: render_constants
Purpose: Renders the constants that never change throughout the 
	game progression.
*/
void render_constants(UINT8 *base)
{
	render_word(base, WIND_X - BYTE, WIND_WORD_Y, "wind");
	render_word(base, PLAYER_X, PLAYER_Y, "player");
	
	render_word(base, P1_HPWORD_X, HPWORD_Y, "health");
	render_word(base, P2_HPWORD_X, HPWORD_Y, "health");
	          
	render_word(base, P1_POW_WORD_X, POW_Y, "power");
	render_word(base, P2_POW_WORD_X, POW_Y, "power");
}

/*
Name: render_turn_end
Purpose: Renders the end of the turn essentials.  This includes
	the first frame of animation of the missile explosion

*/


void render_turn_end(UINT32 *base32, const Model *md)
{
	UINT16 playerStruck;
	
	if (md->wind != snaps[curSnap].wind)
	{
		render_wind(base32, md->wind);
		snaps[curSnap].wind = md->wind;
	}	
	if (md->player != snaps[curSnap].player)
	{
		render_current_player((UINT8*)base32, md->player);
		snaps[curSnap].player = md->player;
	}
}


void render_staging(UINT32 * base32, const Model *md)
{
	UINT16 player = md->player; /* to shorten calls for readability */
	
	if (md->tanks[player].x != snaps[curSnap].tanks[player].x ||
		md->tanks[player].angle != snaps[curSnap].tanks[player].angle)
	{
		render_tank(&(md->tanks[player]), player, base32);
		snaps[curSnap].tanks[player].x = md->tanks[player].x;
		snaps[curSnap].tanks[player].angle = md->tanks[player].angle;
	}
	
	if (md->tanks[player].power != snaps[curSnap].tanks[player].power)
	{
		render_power(md->tanks[player].power, player, base32);
		snaps[curSnap].tanks[player].power = md->tanks[player].power;
	}
}

void render_affected(const Model *md, UINT32 *base32)
{
	UINT16 playerStruck;
	clear_area(base32, snaps[curSnap].missile.x, 
		snaps[curSnap].missile.x + BYTE,
		snaps[curSnap].missile.y, 
		snaps[curSnap].missile.y + MISSILE_HEIGHT);
	
	clear_area(base32, snaps[curSnap].missile.x + FONT_BUFFER, 
			snaps[curSnap].missile.x + LONG, 0, LONG);
		
	
	if (md->collision >= TERRAIN_MIN_CODE &&
				md->collision <= TERRAIN_MAX_CODE)
	{
		render_indexed_terrain (base32, md->terrain, 
					md->collision - 1);
	}else if (md->collision == TANK1_STRUCK || md->collision == TANK2_STRUCK)
	{
		playerStruck = md->collision - TANK_COLLISION_OFFSET;
		
		render_tank(&(md->tanks[playerStruck]), playerStruck, base32);		
	
		render_health(md->tanks[playerStruck].health,
			playerStruck, base32);
	}
}


void render_missile(const Model *md, UINT8 * base8)
{
	UINT32 * base32 = (UINT32*) base8;
	
	if ((int)(md->missile.y + MISSILE_HEIGHT) > 0)
	{
		if (snaps[curSnap].missile.y < 0 || snaps[1 - curSnap].missile.y < 0)
			clear_area(base32, snaps[curSnap].missile.x + FONT_BUFFER, 
				snaps[curSnap].missile.x + LONG, 0, LONG);
		else
			clear_area(base32, snaps[curSnap].missile.x, 
				snaps[curSnap].missile.x + MISSILE_WIDTH, 
				snaps[curSnap].missile.y, 
				snaps[curSnap].missile.y + MISSILE_HEIGHT);
		
		plot_bitmap_8(base8, md->missile.x, md->missile.y, missile_bitmap,
				MISSILE_HEIGHT);
	}else 
	{
		clear_area(base32, snaps[curSnap].missile.x - 1, snaps[curSnap].missile.x + BYTE, 0, 
				BYTE + BOMB_TRACKER_HEIGHT);

		plot_bitmap_8(base8, md->missile.x, 0, 
				missile_tracker_bitmap,
				BOMB_TRACKER_HEIGHT);
	}
		
	snaps[curSnap].missile.x = md->missile.x;
	snaps[curSnap].missile.y = md->missile.y;
}

void render_terrain(const struct Terrain terrain[], UINT32 * base)
{
	int i;
	for (i = 0; i < NUM_TERRAIN; i++)
		plot_square(base, terrain[i].x, terrain[i].y, TERRAIN_WIDTH,
			terrain[i].height, NOFILL);
}

void render_tank(const struct Tank *tank, UINT16 player, UINT32 *base32)
{
	UINT16 *base16 = (UINT16*) base32;

	clear_area(base32, snaps[curSnap].tanks[player].x + cannon_clear_offset[player], 
		snaps[curSnap].tanks[player].x + TANK_WIDTH + cannon_x_offset[player],
		snaps[curSnap].tanks[player].y - CANNON_Y_OFFSET,
		snaps[curSnap].tanks[player].y + TANK_HEIGHT);
	
	plot_bitmap_32(base32, tank->x, tank->y, tank_bitmap, TANK_HEIGHT);
	
	plot_bitmap_16(base16, tank->x + cannon_x_offset[player],
		tank->y - CANNON_Y_OFFSET, cannon_bitmap[player][tank->angle],
		CANNON_HEIGHT);
		
	snaps[curSnap].tanks[player].x = tank->x;
	snaps[curSnap].tanks[player].angle = tank->angle;
}

/*
	If tanks don't start with 100 health, must be changed to 
	first draw the border square of the health.

*/
void render_health(const UINT16 health, int player, UINT32 * base)
{
	UINT8 * base8 =(UINT8*)base;
	switch (player)
	{
		case PLAYER_ONE:
			clear_area(base, P1_HEALTH_X, P1_HEALTH_X + HP_W_CLEAR - 1, 
				HEALTH_Y+ 1, HEALTH_Y + HP_H_CLEAR);
			
			plot_square(base, P1_HEALTH_X, HEALTH_Y,
				health, HP_HEIGHT, FILL);
				
			render_number (base8, P1_HPNUM_X,
				HEALTH_Y, health);
				
			break;
		
		case PLAYER_TWO:
			clear_area(base, P2_HEALTH_X+1, P2_HEALTH_X + HP_W_CLEAR, 
				HEALTH_Y+1, HEALTH_Y + HP_H_CLEAR);
			
			plot_square(base, P2_HEALTH_X + HP_W_CLEAR - health,
				HEALTH_Y, health, HP_HEIGHT, FILL);
				
			render_number (base8, P2_HPFONT_X,
				HEALTH_Y, health);
				
			break;
	}
	
	snaps[curSnap].tanks[player].health = health;
}

void render_power(const UINT16 pow, int player, UINT32 *base32)
{
	UINT8 *base8 = (UINT8*) base32;
	
	switch (player)
	{
		case PLAYER_ONE:
		
			clear_area(base32, P1_POW_X+1, P1_POW_X + POW_W_CLEAR, 
				POW_Y+1, POW_Y + POW_H_CLEAR);
				
			plot_square(base32, P1_POW_X, POW_Y + POW_H_CLEAR - (pow >> 1),
				POW_WIDTH, (pow >> 1), FILL);
				
			render_number (base8, P1_POWNUM_X, POWNUM_Y, pow);
			
			break;
		
		case PLAYER_TWO:
		
			clear_area(base32, P2_POW_X+1, P2_POW_X + POW_W_CLEAR, 
				POW_Y+1, POW_Y + POW_H_CLEAR);
			
			plot_square(base32, P2_POW_X, POW_Y + POW_H_CLEAR - (pow >> 1),
				POW_WIDTH, (pow >> 1), FILL);
				
			render_number (base8, P2_POWNUM_X, POWNUM_Y, pow);
				
			break;
	}
	
	snaps[curSnap].tanks[player].power = pow;
}

void render_indexed_terrain(UINT32 * base32, 
	const struct Terrain terrain[], int index)
{
	int currIndex;
	int start = -2; /* start at 2 left of the struck terrain */
	int finish = 3; /* end 2 after the struck terrain */
	
	
	/* if start terrain is 0 or 1, don't write before */
	if (index == 0 || index == 1)
		start = index == 0 ? start += 2 : start ++;
		
	/* if start terrain is 38 or 39, don't write after */
	else if (index == 38 || index == 39)
		finish = index == 39 ? finish -=2 : finish --;
	
	/* clear the affected terrain.  index - 1 -> index + 1 */
	clear_area(base32, terrain[index + start + 1].x - BYTE, 
		terrain[index + start + 1].x + ((finish - start - 2) << 3) + BYTE,
		HIGHEST_TERRAIN_POINT, LOWEST_TERRAIN_POINT);	
	
	currIndex = index + start;
	
	/* plot changed terrain plus 1 before and 1 after */
	while (start < finish)
	{
		if (currIndex > 0 && currIndex < 39)
			plot_square(base32, terrain[currIndex].x,
				terrain[currIndex].y, TERRAIN_WIDTH,
					terrain[currIndex].height, NOFILL);
		currIndex++;
		start++;
	}
}



void render_missile_impact(const Model *md, UINT8 *base8)
{
	UINT32 *base32 = (UINT32*) base8;
	
	if (md->missile.expCounter == 0)
		clear_area(base32, snaps[curSnap].missile.x - FONT_BUFFER, 
			snaps[curSnap].missile.x + LONG, 0, LONG);
	

	if ((int)(snaps[curSnap].missile.y + MISSILE_HEIGHT) > 0)
	{
		clear_area(base32, snaps[curSnap].missile.x, snaps[curSnap].missile.x + MISSILE_WIDTH + 1,
			snaps[curSnap].missile.y, (snaps[curSnap].missile.y + MISSILE_HEIGHT));
			
		clear_area(base32, md->missile.x, md->missile.x + MISSILE_WIDTH + 1,
			md->missile.y, md->missile.y + MISSILE_HEIGHT);
	
			
		plot_bitmap_8 (base8, md->missile.x, 
			md->missile.y,
			small_explosion_bitmap[(md->missile.expCounter >> 3)],
			SMALL_EXPLOSION_HEIGHT);
	}

	
	if (md->missile.expCounter == NUM_EXPLOSIONS)
		render_affected (md, base32);
}

void render_tank_explosion(UINT32 *base32, const Model *md)
{
	UINT16 player = md->player;

	clear_area(base32, snaps[curSnap].tanks[player].x + cannon_clear_offset[player], 
		snaps[curSnap].tanks[player].x + TANK_WIDTH + cannon_x_offset[player],
		snaps[curSnap].tanks[player].y - TANK_EXP_Y,
		snaps[curSnap].tanks[player].y + TANK_HEIGHT);
	
	plot_bitmap_32 (base32, md->tanks[player].x, 
		md->tanks[player].y - TANK_EXP_Y,
		tank_explosion[(md->tanks[player].expCounter >> 3)],
		LARGE_EXPLOSION_HEIGHT);
}

/*


*/
void render_word(UINT8 * base8, UINT16 x, UINT16 y, char word[])
{
	UINT32 * base32 = (UINT32*)base8;
	UINT16 i = 0;
	UINT16 currX = x;
	
	while (word[i] != '\0')
	{
		plot_bitmap_8 (base8, currX, y, 
			font_letters[(int)(word[i] - LETTER_OFFSET)], 
			FONT_HEIGHT);
			
		currX += BYTE + FONT_BUFFER;
		i++;
	}

}

/*
Name: render_wind
Purpose: Takes in the current speed of the wind and renders 
	an arrow based on the direction of the wind (if negative,
	left, if postive, right) and renders the speed underneath
	it.

*/
void render_wind (UINT32 * base, const int wind)
{
	clear_area (base, WIND_X, WIND_X + LONG, WIND_Y,
		WIND_Y + ARROW_HEIGHT);
	
	plot_bitmap_32(base, WIND_X, WIND_Y, 
			wind < 0 ? arrow_bitmap[LEFT_WIND] : arrow_bitmap[RIGHT_WIND],
			ARROW_HEIGHT);

	render_number((UINT8*)base, WIND_X + LONG,
			WIND_WORD_Y, wind);
			
	snaps[curSnap].wind = wind;
}

void render_current_player(UINT8 * base, UINT16 player)
{
	render_number (base, PLAYER_NUM_X, PLAYER_Y - 1, player + 1);
}

/*
Name: render_number
Purpose: Given an x, y, and a number, the function will
	plot the associated bitmap to the number to the 
	screen.

*/
void render_number (UINT8 * base, UINT16 x, UINT16 y, int number)
{
	int i = 0;
	UINT16 num[3];
	UINT32 *base32 = (UINT32*)base;
	
	if (number < 0)
		number = (~(number) + 1) ;
	
	clear_area (base32, x, x + LONG + 1,y, y + FONT_HEIGHT);
	
	do
	{
		num[i] = number % 10;
		number /= 10;
		i++;
	}while (number != 0);

	clear_area (base32, x, x + ((i + FONT_BUFFER) << 3),
		y, y + FONT_HEIGHT);
	
	i--;
	
	while (i >= 0)
	{
		plot_bitmap_8(base, x, y, font_numbers[num[i]], FONT_HEIGHT);
		x+= BYTE + FONT_BUFFER;	
		i--;
	}
}

void take_snapshot(Model *md)
{
	UINT16 i;
	snaps[curSnap].player = md->player;
	snaps[curSnap].wind = md->wind;
	snaps[curSnap].state = md->state;
	
	for (i = 0; i < NUM_TERRAIN; i++)
	{
		snaps[curSnap].terrain[i].x = md->terrain[i].x;
		snaps[curSnap].terrain[i].y = md->terrain[i].y;
		snaps[curSnap].terrain[i].height = md->terrain[i].height;
	}
	
	for (i = 0; i < NUM_PLAYERS; i++)
	{
		snaps[curSnap].tanks[i].x = md->tanks[i].x;
		snaps[curSnap].tanks[i].y = md->tanks[i].y;
		snaps[curSnap].tanks[i].power = md->tanks[i].power;
		snaps[curSnap].tanks[i].angle = md->tanks[i].angle;
	}
}