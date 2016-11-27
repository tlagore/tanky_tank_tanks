/*
Name: renderer

Purpose: 
The renderer module allows for the visual representation of an entire
model, or specific parts of the model based on the current state
of the objects within the model->

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
Purpose: Renders the entire model to the screen
*/
void render(const struct Model * model, UINT32 * base32,
	UINT16 curPlayer)
{
	UINT8 *base8 = (UINT8*) base32;
	UINT16 player;
	
	if (model->gameCondition > MISSILE_IN_AIR)
	{
		render_missile_impact(&model->missile, base8);
		render_current_player(base8, 
					switch_player(curPlayer));
	}
	
	if (model->gameCondition == STARTUP)
		render_startup(base32, model);
				
	else if(model->gameCondition == STAGING &&
			model->tanks[curPlayer].deltaX != 0)
		render_tank(&model->tanks[curPlayer],
				curPlayer, 
				base32);
		
	else if (model->gameCondition == MISSILE_IN_AIR)
		render_missile(&model->missile,
				model->missile.previousX,
				model->missile.previousY,
				base8);
		
	else if (model->gameCondition >= TERRAIN_MIN_CODE
		&& model->gameCondition <= TERRAIN_MAX_CODE)
			render_indexed_terrain(base32, 
					model->terrain, 
					model->gameCondition - 1);
				
	else if (model->gameCondition == TANK_STRUCK1
			|| model->gameCondition == TANK_STRUCK2)
	{
		player = model->gameCondition - TANK_COLLISION_OFFSET;
		
		render_tank(&model->tanks[player],
				player, base32);		
			
		render_health
			(model->tanks[player].health,
			player, 
			base32);
	}
	
	render_wind (base32, model->wind);
}

void render_startup(UINT32 * base32, const struct Model * model)
{
	UINT8 *base8 = (UINT8*) base32;
	UINT16 i;
	
	clear_screen(base32);
	
	render_terrain(model->terrain, base32);		
	render_word(base8, PLAYER_X, PLAYER_Y, "player", 6);
	render_current_player(base8, 0);
	
	render_word(base8, P1_HPWORD_X, HPWORD_Y, "health", 6);
	render_word(base8, P2_HPWORD_X, HPWORD_Y, "health", 6);
	
	render_word(base8, P1_POW_WORD_X, POW_Y, "power", 5);
	render_word(base8, P2_POW_WORD_X, POW_Y, "power", 5);
	
	for (i = 0; i < NUM_PLAYERS; i++)
	{
		render_tank(&model->tanks[i], 
				i, 
				base32);
			
		plot_hline(base32,
				model->stages[i].x1,
				model->stages[i].x2,
				model->stages[i].y);
			
		render_health(model->tanks[i].health, 
				i,
				base32);	
				
		render_power(model->tanks[i].power, 
				i, 
				base32);
		
		render_wind(base32, model->wind);
	}
} 

void render_missile(const struct Missile *missile, int oldX,
	int oldY, UINT8 * base8)
{
	UINT32 * base32 = (UINT32*) base8;
	if ((int)(missile->y + missile->height) > 0)
	{
		if (oldY <= 0)
			clear_area(base32,
				oldX + FONT_BUFFER,
				oldX + LONG,
				0,
				LONG);
	
		clear_area(base32, 
			oldX, 
			oldX + missile->width,
			oldY, 
			oldY + missile->height - 1);
		
		plot_bitmap_8(base8,
			missile->x, 
			missile->y, 
			missile_bitmap,
			MISSILE_HEIGHT);
	}else
	{
		clear_area(base32,
			oldX,
			oldX + LONG,
			0,
			LONG);
		
		plot_bitmap_8(base8,
			missile->x,
			0,
			missile_tracker_bitmap,
			BOMB_TRACKER_HEIGHT);
		if (missile->x < 611) 	
			render_number(base8,
				missile->x,
				BOMB_TRACKER_HEIGHT + FONT_BUFFER,
				missile->y + missile->height);
	}
}

void render_terrain(const struct Terrain terrain[], UINT32 * base)
{
	int i;
	for (i = 0; i < NUM_TERRAIN; i++)
		plot_square(base, 
			terrain[i].x, 
			terrain[i].y, 
			terrain[i].width,
			terrain[i].height, 
			FILL);
}

void render_tank(const struct Tank *tank, UINT16 player,
	UINT32 *base32)
{
	UINT16 *base16 = (UINT16*) base32;
	UINT16 oldX = tank->x - tank->deltaX;

	clear_area(base32, 
				oldX + cannon_clear_offset[player], 
				oldX + tank->width + cannon_x_offset[player],
				tank->y - CANNON_Y_OFFSET, 
				tank->y + tank->height);
	
	plot_bitmap_32(base32, 
				tank->x, 
				tank->y, 
				tank_bitmap,
				tank->height);
	
	plot_bitmap_16(base16,
				tank->x + cannon_x_offset[player],
				tank->y - CANNON_Y_OFFSET,
				cannon_bitmap[player][tank->angle],
				CANNON_HEIGHT);
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
			clear_area(base, 
				P1_HEALTH_X+1, P1_HEALTH_X + HP_W_CLEAR - 1, 
				HEALTH_Y+1, HEALTH_Y + HP_H_CLEAR);
			
			plot_square(base, P1_HEALTH_X, HEALTH_Y,
				health, HP_HEIGHT, FILL);
				
			render_number (base8, P1_HPNUM_X,
				HEALTH_Y, health);
				
			break;
		
		case PLAYER_TWO:
			clear_area(base, P2_HEALTH_X+1, 
				P2_HEALTH_X + HP_W_CLEAR, 
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
			
			plot_square(base32, P1_POW_X, POW_Y + 
				POW_H_CLEAR - (pow >> 1),POW_WIDTH,
				(pow >> 1), FILL);
				
			render_number (base8, P1_POWNUM_X,
				POWNUM_Y, pow);
			
			break;
		
		case PLAYER_TWO:
			clear_area(base32, P2_POW_X+1, P2_POW_X + POW_W_CLEAR, 
				POW_Y+1, POW_Y + POW_H_CLEAR);
			
			plot_square(base32, P2_POW_X, POW_Y + 
				POW_H_CLEAR - (pow >> 1),POW_WIDTH,
				(pow >> 1), FILL);
				
			render_number (base8, P2_POWNUM_X,
				POWNUM_Y, pow);
				
			break;
	}
}

void render_indexed_terrain(UINT32 * base32, 
	const struct Terrain terrain[], int index)
{
	int currIndex;
	int start = -2;
	int finish = 3;
	
	if (index == 0 || index == 1)
		start = index == 0 ? start += 2 : start ++;
	else if (index == 38 || index == 39)
		finish = index == 39 ? finish -=2 : finish --;
		
	clear_area(base32, terrain[index + start + 1].x, 
		terrain[index + start + 1].x + ((finish - start - 2) << 3),
		0, LOWEST_TERRAIN_POINT);	
	
	currIndex = index + start;
	
	while (start < finish)
	{
		if (currIndex > 0 && currIndex < 39)
			plot_square(base32, terrain[currIndex].x,
				terrain[currIndex].y, terrain[currIndex].width,
					terrain[currIndex].height, FILL);
		currIndex++;
		start++;
	}
}

void render_missile_impact(const struct Missile *missile, UINT8 *base8)
{
	UINT32 *base32 = (UINT32*) base8;
	
	if (missile->y + missile->height > 0 &&
		missile->x + missile->width < SCREEN_WIDTH)
		{
			clear_area(base32,
				missile->x,
				missile->x + BYTE + 1,
				0,
				BOMB_TRACKER_HEIGHT + 1);
		
			clear_area(base32, missile->x, missile->x + missile->width,
				missile->y, missile->y + missile->height);
		}
	/*plot_bitmap_8 (base8, missile.x , missile.y, 
		small_explosion_bitmap, MISSILE_HEIGHT);*/
			
	
}

void render_tank_explosion(const struct Tank *tank, int player,
	UINT32 *base32)
{
		clear_area(base32, 
				tank->x + cannon_clear_offset[player], 
				tank->x + tank->width,
				tank->y - CANNON_Y_OFFSET, 
				tank->y + tank->height);
}

/*


*/
void render_word (UINT8 * base8, UINT16 x,
	UINT16 y, char word[], UINT16 length)
{
	UINT32 * base32 = (UINT32*)base8;
	UINT16 i;
	UINT16 currX = x;

	clear_area(base32, x, x + ((length + FONT_BUFFER) << 3),
		y, y + FONT_HEIGHT - 1);
	
	for (i = 0; i < length; i++)
	{
		plot_bitmap_8 (base8, 
				currX, 
				y, 
				font_letters[(int)(word[i] - LETTER_OFFSET)], 
				FONT_HEIGHT);
			
		currX += BYTE + FONT_BUFFER;
	}

}

/*
Name: render_wind
Purpose: Takes in the current speed of the wind and renders 
	an arrow based on the direction of the wind (if negative,
	left, if postive, right) and renders the speed underneath
	it.

*/
void render_wind (UINT32 * base, int wind)
{
	clear_area (base, WIND_X, WIND_X + LONG, WIND_Y,
		WIND_Y + ARROW_HEIGHT);
	
	plot_bitmap_32(base, 
			WIND_X, 
			WIND_Y, 
			wind < 0 ? arrow_bitmap[LEFT_WIND] : arrow_bitmap[RIGHT_WIND],
			ARROW_HEIGHT);

	render_number((UINT8*)base,
			WIND_X + BYTE + (BYTE >> 1),
			(WIND_Y - (WORD)), 
			wind);
}

UINT16 switch_player (UINT16 currentPlayer)
{
	return currentPlayer == 0 ? 1 : 0;
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
void render_number (UINT8 * base, UINT16 x, UINT16 y,
	int number)
{
	int i = 0;
	UINT16 num[3];
	UINT32 *base32 = (UINT32*)base;
	
	if (number < 0)
		number = (~(number) + 1) ;
	
	clear_area (base32, x, x + LONG + 1,
		y, y + FONT_HEIGHT);
	
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
