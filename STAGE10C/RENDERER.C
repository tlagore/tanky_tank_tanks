/*
Name: renderer

Purpose: 
The renderer module allows for the visual representation of an entire
model, or specific parts of the model based on the current state
of the objects within the model.

Author: Tyrone Lagore
Version: March 27, 2014
*/

#include "defines.h"
#include "renderer.h"
#include "offsets.h"
#include "bitmaps.h"
#include "font.h"
#include "raster.h"
	
void render_startup(UINT32 *base32, const Model *model);
void render_constants(UINT8 *base);
void render_splash_tanks(UINT32 *base32);	
void render_turn_end(UINT32 *base32, const Model *md);
void render_staging(UINT32 * base32, const Model *model);
void render_affected(const Model *md, UINT32 *base32);	
void render_missile(const Model *md, UINT8 * base8);
void render_terrain(const struct Terrain terrain[], UINT32 *base);
void render_tank(const struct Tank *tank, UINT16 player, 
	UINT32 *base32);
void render_health(const UINT16 health, int player, UINT32 * base);
void render_power(const UINT16 power, int player, UINT32 *base);
void render_missile_impact(const Model *md, UINT8 *base8);
void render_tank_explosion(UINT32 *base32, const Model *md);
void render_indexed_terrain(UINT32 * base32, 
	const struct Terrain terrain[], int index);
void render_wind (UINT32 * base, const int wind);
void render_current_player(UINT8 * base, UINT16 player);
void render_game_over(UINT8 *base8, UINT16 player);
void render_string (UINT8 * base8, UINT16 x,UINT16 y, char word[],
	UINT16 eight);




typedef struct Snapshot
{
	int wind;
	struct Tank tanks[NUM_PLAYERS]; /* 0 = player 1, 1 = player 2 (or computer) */
	struct Missile missile;
	struct Terrain terrain[NUM_TERRAIN];
	int state;
	UINT16 player;
}Snapshot;


Snapshot snaps[NUM_BUFFERS];
UINT16 curSnap = 0;
Mouse mouseSnap =
{
	-1,
	-1,
	WAITING,
	FALSE,
	FALSE
};

/*
Name: render
Purpose: Renders the entire md to the screen
*/
void render(const Model * md, UINT32 * base32)
{
	UINT8 *base8 = (UINT8*) base32;
	UINT16 playerStruck;

	snaps[curSnap].state = md->state;
	
	if (snaps[1 - curSnap].state == MISSILE_EXPLODING &&
		snaps[curSnap].state != MISSILE_EXPLODING)
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
			render_staging(base32, md); 
			/* renders the last movement of the tank to the alt buffer */
			break;
		case MISSILE_EXPLODING:
			render_missile_impact(md, base8);
			break;
		case GAME_OVER:
			if (md->tanks[md->player].expCounter <= 1)
				render_affected(md, base32);
			render_tank_explosion(base32, md);
			break;
		case GAME_OVER_FINISH:
			render_game_over((UINT8*)base32, md->player);
			break;
	};
	
	render_turn_end(base32, md);
	curSnap = 1 - curSnap;

}

/*
Name: render_game_over
Purpose: Renders the player that won the game to the screen
*/
void render_game_over(UINT8 *base8, UINT16 player)
{
	render_string (base8, 262, 60, "player", TRUE);
	render_number(base8, 320, 59, (1 - player) + 1);
	render_string (base8, 336, 60, " wins", TRUE);
	render_string(base8, 224, 70, "press any key to continue", TRUE);
}

void render_mouse(Mouse *mouse, UINT8 *base8)
{
	plot_bitmap_8(base8, mouseSnap.x, mouseSnap.y, mouse_bitmap,
			MOUSE_HEIGHT);
	plot_bitmap_8(base8, mouse->x, mouse->y, mouse_bitmap,
			MOUSE_HEIGHT);
			
	mouseSnap.x = mouse->x;
	mouseSnap.y = mouse->y;

}

/*
Name: render_splash
Purpose: Renders the static splash screen to the given buffer.

*/
void render_splash(UINT32 *base32, bool first_rend, bool waiting_for_player)
{
	UINT16 *base16 = (UINT16*) base32;
	int i, j;
	int x = SPLASH_SSX, 
		y = SPLASH_SSY,
		width = SPLASH_SW, 
		height = SPLASH_SH;
	if (first_rend)
	{
		clear_area(base32, 54, 585, 20, 60);
		render_string((UINT8*)base32, 70, 25, "tanky tank tanks", FALSE);
		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < 2; j++)
			{
				clear_area(base32, x-10, x + width + 10, y - 10, 
					y + height + 10);
				plot_square(base32, x-10, y - 10, width + 20, height + 20, NOFILL);
				plot_square(base32, x, y, width, height, FILL);
				clear_area(base32, x + 10, x + width - 10, y + 10, y + height - 10);
				x += SPLASH_XO;
			}
			x-= (SPLASH_XO << 1);
			y+= SPLASH_YO;
		}
			
		render_string ((UINT8*)base32, SERIAL_X + BYTE, SERIAL_Y, "host", TRUE);
		render_string ((UINT8*)base32, SERIAL_X + SPLASH_XO, SERIAL_Y, "client", TRUE);
		render_splash_tanks(base32);
		plot_bitmap_16(base16, HOW_X, HOW_Y, question_bitmap,
			QUESTION_HEIGHT);
	}
	
	if (waiting_for_player)
		plot_bitmap_16((UINT16*)base32, WAITING_X, WAITING_Y, hourglass_bitmap,
				HOURGLASS_HEIGHT);
	else
		clear_area (base32, WAITING_X, WAITING_X + WORD, WAITING_Y,
				WAITING_Y + HOURGLASS_HEIGHT);
}

/*
Name: render_splash_tanks
Purpose: Renders the tanks viewable at the splash screen.
*/
void render_splash_tanks(UINT32 *base32)
{
	UINT16 *base16 = (UINT16*) base32;
	UINT16 x1 = TWOP_TANK1_X,
		x2 = TWOP_TANK2_X,
		y = TWOP_TANK_Y,
		i;
	
	plot_bitmap_32(base32, ONEP_TANK_X, ONEP_TANK_Y,
		tank_bitmap, TANK_HEIGHT);
	
	plot_bitmap_16(base16, ONEP_TANK_X + cannon_x_offset[PLAYER_ONE],
		ONEP_TANK_Y - CANNON_Y_OFFSET, cannon_bitmap[PLAYER_ONE][0],
		CANNON_HEIGHT);
		
	for (i = 0; i < 2; i++)
	{
		plot_bitmap_32(base32, x1, y,
			tank_bitmap, TANK_HEIGHT);
		
		plot_bitmap_16(base16, x1 + cannon_x_offset[PLAYER_ONE],
			y - CANNON_Y_OFFSET, cannon_bitmap[PLAYER_ONE][0],
			CANNON_HEIGHT);

		plot_bitmap_32(base32, x2, y,
			tank_bitmap, TANK_HEIGHT);
		
		plot_bitmap_16(base16, x2 + cannon_x_offset[PLAYER_TWO],
			y - CANNON_Y_OFFSET, cannon_bitmap[PLAYER_TWO][0],
			CANNON_HEIGHT);
		
		x1 += SPLASH_XO;
		x2 += SPLASH_XO;
	}
}

/*
Name: render_how
Purpose: Renders the instructions screen
*/

void render_how(UINT32 *base32)
{
	UINT8 *base8 = (UINT8*) base32;

	clear_area(base32, SPLASH_SSX, 570, 
		SPLASH_SSY, 272); 
	render_string (base8, INSTRUCTIONS_X, 140, "\t\t\twelcome to tanky tank tanks", TRUE);
	render_string (base8, INSTRUCTIONS_X, 170, "press w to increase power", TRUE);
	render_string (base8, INSTRUCTIONS_X, 186, "press s to decrease power", TRUE);
	render_string (base8, INSTRUCTIONS_X, 202, "use the left and right arrow keys to move left or right", TRUE);
	render_string (base8, INSTRUCTIONS_X, 218, "use the up and down arrow keys to change your angle", TRUE);
	render_string (base8, INSTRUCTIONS_X, 234, "dont forget to account for wind", TRUE);
	render_string (base8, INSTRUCTIONS_X, 250, "\tpress any key to return to the main menu", TRUE);
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
	render_string(base, WIND_X - BYTE, WIND_WORD_Y, "wind", TRUE);
	render_string(base, PLAYER_X, PLAYER_Y, "player", TRUE);
	
	render_string(base, P1_HPWORD_X, HPWORD_Y, "health", TRUE);
	render_string(base, P2_HPWORD_X, HPWORD_Y, "health", TRUE);
	          
	render_string(base, P1_POW_WORD_X, POW_Y, "power", TRUE);
	render_string(base, P2_POW_WORD_X, POW_Y, "power", TRUE);
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

/*
Name: render_staging
Purpose: The game is in the staging state.  In this state, the player is 
	allowed to move back and forth, change angle, and change power.  It 
	is checked to see if any of these variables differ in the model in 
	relation to the current snapshot and the specific changes are rendered
	in the case that a variance is found.
	

*/
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

/*
Name: render_affected
Purpose: Render affected takes in the model and checks what
	the collision code of the last missile impact was.  It 
	then renders based on the collision code.
	
	

*/
void render_affected(const Model *md, UINT32 *base32)
{
	UINT16 playerStruck, i;
	
	if (md->state == GAME_OVER)
		clear_area(base32, snaps[curSnap].missile.x, 
			snaps[curSnap].missile.x + BYTE,
			snaps[curSnap].missile.y, 
			snaps[curSnap].missile.y + MISSILE_HEIGHT);
	
	if (md->collision >= TERRAIN_MIN_CODE &&
				md->collision <= TERRAIN_MAX_CODE)
	{
		render_indexed_terrain (base32, md->terrain, 
					md->collision - 1);
	}else if (md->state != GAME_OVER
	 && (md->collision == TANK1_STRUCK || md->collision == TANK2_STRUCK))
	{
		playerStruck = md->collision - TANK_COLLISION_OFFSET;
		
		render_tank(&(md->tanks[playerStruck]), playerStruck, base32);		
	
		render_health(md->tanks[playerStruck].health,
			playerStruck, base32);
	}
	
	snaps[curSnap].missile.x = 0;
	snaps[curSnap].missile.y = 0;
}

/*
Name: render_missile
Purpose: Clears the position of the old missile and 
	renders the current position of the missile.  
	Updates the current snapshot to reflect where it was 
	when it last rendered.

*/
void render_missile(const Model *md, UINT8 * base8)
{
	UINT32 * base32 = (UINT32*) base8;
	
	if ((int)(md->missile.y + MISSILE_HEIGHT) > 0)
	{
		clear_area(base32, snaps[curSnap].missile.x, 
				snaps[curSnap].missile.x + MISSILE_WIDTH, 
				snaps[curSnap].missile.y, 
				snaps[curSnap].missile.y + MISSILE_HEIGHT);
		
		plot_bitmap_8(base8, md->missile.x, md->missile.y, missile_bitmap,
				MISSILE_HEIGHT);
	}else /* ensures that the area where the missile was when it left the screen is cleared */
		clear_area(base32, snaps[curSnap].missile.x - FONT_BUFFER, 
				snaps[curSnap].missile.x + LONG, 0, LONG);
				
	snaps[curSnap].missile.x = md->missile.x;
	snaps[curSnap].missile.y = md->missile.y;
}

/*
Name: render_terrain
Purpose: Renders the entirety of the terrain.
*/
void render_terrain(const struct Terrain terrain[], UINT32 * base)
{
	int i;
	for (i = 0; i < NUM_TERRAIN; i++)
		plot_square(base, terrain[i].x, terrain[i].y, TERRAIN_WIDTH,
			terrain[i].height, NOFILL);
}

/*
Name: render_tank
Purpose: Renders a tank to the screen.  Clears the position of the old tank
	based on it's current angle (as the cannon position varies) and renders
	the new one based on the current position.   Updates the snapshot accordingly.

*/
void render_tank(const struct Tank *tank, UINT16 player, UINT32 *base32)
{
	UINT16 *base16 = (UINT16*) base32;

	clear_area(base32, snaps[curSnap].tanks[player].x, 
		snaps[curSnap].tanks[player].x + TANK_WIDTH + cannon_x_clear[player],
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
Name: render_health
Purpose: Renders the specified players health.

Works by clearing the area within the health square of the specified
	player (using predefined positions of where the health is located)
	and renders a filled square within the health square based on how
	much health is located.  The health number is also rendered.

Known "bug":
If tanks didn't start with 100 health, the outer square would not be
drawn.  This function works by plotting the health 1 pixel larger
on each border than the specified clear values.  The initial 100 HP
plot ensures the outside square is drawn.
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

/*
Name: render_power
Purpose: renders the specified players power to the screen.

Works in the same manner as render health, and is susceptible 
to the same "bug".

*/
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

/*
Name: render_indexed_terrain
Purpose: Renders specific terrain objects to the screen based
	on an index given to the function. That has been struck
	by a missile.

Method: Clears from 1 prior to the indexed terrain to 1 past
	the indexed terrain.  Plots from 2 prior to the indexed 
	terrain to 2 past the indexed terrain.  The extra plot
	is to ensure that if a preceding terrain is larger that
	its cleared edge is redrawn.

Input: 
	UINT32 *base32 - The starting address of the current frame
		buffer.
	struct Terrain terrain[] - the array of terrain objects
	int index - The terrain object that was struck by a missile.
*/
void render_indexed_terrain(UINT32 * base32, 
	const struct Terrain terrain[], int index)
{
	UINT16 xClearStart, xClearEnd;
	int currIndex;
	int start = -2; /* start at 2 left of the struck terrain */
	int finish = 3; /* end 2 after the struck terrain */
	
	
	/* if start terrain is 0 or 1, don't write before */
	if (index == 0 || index == 1)
		start = index == 0 ? start += 2 : start ++;
		
	/* if start terrain is 38 or 39, don't write after */
	else if (index == 38 || index == 39)
		finish = index == 39 ? finish -=2 : finish --;
	
	xClearStart =  index + start + 1 == 0 ? 1 : index + start + 1;
	
	/* clear the affected terrain.  index - 1 -> index + 1 */
	clear_area(base32, terrain[xClearStart].x - BYTE, 
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

/*
Name: render_missile_impact
Purpose: Renders the missile impact animation based on the current 
	missile explosion counter and current missile location.
	
	At the end of the missile explosion animation, the missiles 
	coordinates are set to (0,0).
*/
void render_missile_impact(const Model *md, UINT8 *base8)
{
	UINT32 *base32 = (UINT32*) base8;
	
	if (md->collision != OUT_OF_SCREEN)
	{
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
		
		if (md->missile.expCounter <= 2)
			render_affected (md, base32);
	}else
			snaps[curSnap].missile.x = snaps[curSnap].missile.y = 0;
}

/*
Name: render_tank_explosion
Purpose: Renders the tank explosion animation based on the current
	tank explosion counter and the position of the tank of
	the player being destroyed.

*/
void render_tank_explosion(UINT32 *base32, const Model *md)
{
	UINT16 player = md->player;
	
	clear_area(base32, snaps[curSnap].tanks[player].x, 
		snaps[curSnap].tanks[player].x + TANK_WIDTH + cannon_x_offset[player],
		snaps[curSnap].tanks[player].y - TANK_EXP_Y,
		snaps[curSnap].tanks[player].y + TANK_HEIGHT);
	
	if (md->tanks[player].expCounter < TANK_EXPLOSIONS - 1)
		plot_bitmap_32 (base32, md->tanks[player].x, 
			md->tanks[player].y - TANK_EXP_Y,
			tank_explosion[(md->tanks[player].expCounter >> 3)],
			LARGE_EXPLOSION_HEIGHT);
	else
		render_health(md->tanks[player].health, md->player, base32);
}

/*
Name: render_string
Purpose: Renders a given word to the screen at the specified x,y 
	coordinates by indexing a font array.  The font array is 
	contains a-z bitmaps in order 0-25 so that array indexing 
	can be done simply by subtracting 'a' from the current 
	letter.
	
Input:
	UINT8 *base8 - a pointer to the start of the frame buffer
	UINT16 x, y - the (x,y) coordinates to being plotting at
	char word[] - the word to plot
	UINT16 eight - 1 for 8 bit word 0 for 32 bit
*/
void render_string(UINT8 * base8, UINT16 x, UINT16 y, char word[], 
	UINT16 eight)
{
	UINT32 * base32 = (UINT32*)base8;
	UINT16 i = 0;
	UINT16 currX = x;
	UINT16 offset = eight ? BYTE : LONG; /* used for spacing and tabbing */
	
	while (word[i] != '\0')
	{
		while (word[i] == ' ' || word[i] == '\t')
		{
			currX += word[i] == ' ' ? offset : (offset << 2);
			i++; /* 1 space for ' ', 3 for '\t' */
		}
		
		if (eight)
		{
			plot_bitmap_8 (base8, currX, y, 
				font_letters[(word[i] - LETTER_OFFSET)], 
				FONT_HEIGHT);
				
				currX += offset + FONT_BUFFER;
		}else
		{
			plot_bitmap_32(base32, currX, y,
				big_font_letters[(int)(word[i] - LETTER_OFFSET)],
				BIG_FONT_HEIGHT);
				
				currX += offset + FONT_BUFFER;
		}
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

/*
Name: render_current_player
Purpose: Renders the current player to the screen.
*/
void render_current_player(UINT8 * base, UINT16 player)
{
	render_number (base, PLAYER_NUM_X, PLAYER_Y - 1, player + 1);
}

/*
Name: render_number
Purpose: Given an x, y, and a number, the function will
	plot the associated bitmap to the number to the 
	screen.
	
Method: Places the number into an array by performing a
	modulo by 10, then dividing by 10 until the number is 0
	
	The size of the number is then cleared based on how many
	digits there are in the number and the number is plotted
	1 number at a time by indexing the number array in reverse 
	order.
*/
void render_number (UINT8 * base, UINT16 x, UINT16 y, int number)
{
	int i = 0;
	UINT16 num[3];
	UINT32 *base32 = (UINT32*)base;
	
	if (number < 0) /* if the number is negative it is negated */
		number = (~(number) + 1) ;
	
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

/*
Name: take_snapshot
Purpose: Initializes the snapshot of current render so the renderer knows
	what has changed between the model and the snapshots.
*/
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

void reset_mouse()
{
	mouseSnap.x = -1;
	mouseSnap.y = -1;	
}
