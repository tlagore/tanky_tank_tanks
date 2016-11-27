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


/*
Name: render
Purpose: Renders the entire md to the screen
*/
void render(const struct Model * md, struct Snapshot *snap,
	UINT32 * base32)
{
	UINT8 *base8 = (UINT8*) base32;
	UINT16 playerStruck;
	
	if (md->wind != snap->wind)
		render_wind(base32, md->wind, &(snap->wind));
	
	if (snap->missile.expCounter != 0)
		render_missile_impact(&(snap->missile), (UINT8*)base32);
	
	switch (md->state)
	{
		case STARTUP:
			render_startup (base32, md, snap);
			break;
		case TURN_END:
			render_turn_end(base32, md, snap);
			break;
		case STAGING:
			render_staging(base32, md, snap);
			break;
		case MISSILE_IN_AIR:
			render_missile(&(md->missile), &(snap->missile), base8);
			break;
		case GAME_OVER:
			/*render_game_over(md);*/
			break;
	}
}

void render_startup(UINT32 *base32, const struct Model *md,
	const struct Snapshot *snap)
{
	UINT8 *base8 = (UINT8*) base32;
	UINT16 i;
	
	clear_screen(base32);
	render_terrain(md->terrain, base32);
	render_constants(base8);
	render_wind(base32, md->wind, &(snap->wind));	
	
	for (i = 0; i < NUM_PLAYERS; i++)
	{
		snap->tanks[i].x = md->tanks[i].x;
	
		render_tank(&md->tanks[i], &(snap->tanks[i]), i, base32);
		plot_hline(base32, md->stages[i].x1, md->stages[i].x2,
				md->stages[i].y);
			
		render_health(md->tanks[i].health, i,base32);		
		render_power(md->tanks[i].power, i, base32);
	}
} 

/*


*/
void render_constants(UINT8 *base)
{
	render_word(base, PLAYER_X, PLAYER_Y, "player");
	render_current_player(base, PLAYER_ONE);
	
	render_word(base, P1_HPWORD_X, HPWORD_Y, "health");
	render_word(base, P2_HPWORD_X, HPWORD_Y, "health");
	          
	render_word(base, P1_POW_WORD_X, POW_Y, "power");
	render_word(base, P2_POW_WORD_X, POW_Y, "power");
}

void render_turn_end(UINT32 *base32, const struct Model *md, 
	struct Snapshot *snap)
{
	UINT16 playerStruck;
	
	render_missile_impact(&(snap->missile), (UINT8*)base32);
	render_current_player((UINT8*)base32, md->player);
	
	if (md->collision >= TERRAIN_MIN_CODE
			&& md->collision <= TERRAIN_MAX_CODE)
		render_indexed_terrain(base32, md->terrain, 
			(md->collision) - 1);
			
	else if (md->collision != OUT_OF_BOUNDS)
	{
		playerStruck = md->collision - TANK_COLLISION_OFFSET;
		
		render_tank(&(md->tanks[playerStruck]),
			&(snap->tanks[playerStruck]), playerStruck, base32);		
	
		render_health(md->tanks[playerStruck].health,
			playerStruck, base32);
	}
}
void render_staging(UINT32 * base32, const struct Model *md,
	struct Snapshot *snap)
{
	UINT16 player = md->player; /* to shorten calls for readability */
	
	if (md->tanks[player].x != snap->tanks[player].x ||
		md->tanks[player].angle != snap->tanks[player].angle)
		render_tank(&(md->tanks[player]), &(snap->tanks[player]),
			player, base32);
	
	if (md->tanks[player].power != snap->tanks[player].power)
	{
		render_power(md->tanks[player].power, player, base32);
		snap->tanks[player].power = md->tanks[player].power;
	}
}
void render_missile(const struct Missile *missile, 
	struct Missile *oldMissile, UINT8 * base8)
{
	UINT32 * base32 = (UINT32*) base8;
	
	if ((int)(missile->y + missile->height) > 0)
	{
		if (oldMissile->y < 0)
			clear_area(base32, oldMissile->x + FONT_BUFFER, 
				oldMissile->x + LONG, 0, LONG);
	
		clear_area(base32, oldMissile->x, oldMissile->x + missile->width, 
			oldMissile->y, oldMissile->y + missile->height - 1);
		
		plot_bitmap_8(base8, missile->x, missile->y, missile_bitmap,
				MISSILE_HEIGHT);
	}else
	{
		if ((int)(missile->y + missile->height) >= -4)
			clear_area(base32, oldMissile->x, oldMissile->x + BYTE, 0, 
				MISSILE_HEIGHT);
		else
			clear_area(base32, oldMissile->x, oldMissile->x + BYTE, 0, 
				BOMB_TRACKER_HEIGHT);
		plot_bitmap_8(base8, missile->x, 0, missile_tracker_bitmap,
			BOMB_TRACKER_HEIGHT);
	}	
	oldMissile->x = missile->x;
	oldMissile->y = missile->y;

}

void render_terrain(const struct Terrain terrain[], UINT32 * base)
{
	int i;
	for (i = 0; i < NUM_TERRAIN; i++)
		plot_square(base, terrain[i].x, terrain[i].y, terrain[i].width,
			terrain[i].height, NOFILL);
}

void render_tank(const struct Tank *tank, struct Tank *oldTank,
	UINT16 player, UINT32 *base32)
{
	UINT16 *base16 = (UINT16*) base32;

	clear_area(base32, oldTank->x + cannon_clear_offset[player], 
		oldTank->x + tank->width + cannon_x_offset[player],
		tank->y - CANNON_Y_OFFSET, tank->y + tank->height);
	
	plot_bitmap_32(base32, tank->x, tank->y, tank_bitmap, tank->height);
	
	plot_bitmap_16(base16, tank->x + cannon_x_offset[player],
		tank->y - CANNON_Y_OFFSET, cannon_bitmap[player][tank->angle],
		CANNON_HEIGHT);
		
	oldTank->x = tank->x;
	oldTank->angle = tank->angle;
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
	clear_area(base32, terrain[index + start + 1].x, 
		terrain[index + start + 1].x + ((finish - start - 2) << 3),
		0, LOWEST_TERRAIN_POINT);	
	
	currIndex = index + start;
	
	/* plot changed terrain plus 1 before and 1 after */
	while (start < finish)
	{
		if (currIndex > 0 && currIndex < 39)
			plot_square(base32, terrain[currIndex].x,
				terrain[currIndex].y, terrain[currIndex].width,
					terrain[currIndex].height, NOFILL);
		currIndex++;
		start++;
	}
}

void render_missile_impact(struct Missile *missile, UINT8 *base8)
{
	UINT32 *base32 = (UINT32*) base8;
	
	clear_area(base32, missile->x - FONT_BUFFER, 
		missile->x + LONG, 0, LONG);
	
	if ((missile->expCounter & 7) == 0)
	{
		if ((int)(missile->y + missile->height) > 0)
		{
			clear_area(base32, missile->x, missile->x + missile->width,
				missile->y, missile->y + missile->height);
				
			plot_bitmap_8 (base8, missile->x, missile->y,
				small_explosion_bitmap[(missile->expCounter >> 3)],
				SMALL_EXPLOSION_HEIGHT);
		}
	}
			
	if (missile->expCounter == NUM_EXPLOSIONS)
	{
		missile->expCounter = 0;
		missile->x = missile->y = 0;
	}else
		missile->expCounter++;
}

void render_tank_explosion(const struct Tank *tank, int player,
	UINT32 *base32)
{
		clear_area(base32, tank->x + cannon_clear_offset[player], 
			tank->x + tank->width, tank->y - CANNON_Y_OFFSET, 
			tank->y + tank->height);
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
void render_wind (UINT32 * base, const int wind, int * oldWind)
{
	clear_area (base, WIND_X, WIND_X + LONG, WIND_Y,
		WIND_Y + ARROW_HEIGHT);
	
	plot_bitmap_32(base, WIND_X, WIND_Y, 
			wind < 0 ? arrow_bitmap[LEFT_WIND] : arrow_bitmap[RIGHT_WIND],
			ARROW_HEIGHT);

	render_number((UINT8*)base, WIND_X + BYTE + (BYTE >> 1),
			(WIND_Y - (WORD)), wind);
			
	(*oldWind) = wind;
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
