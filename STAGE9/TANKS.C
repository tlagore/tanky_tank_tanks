#include <osbind.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "defines.h"
#include "input.h"
#include "model.h"
#include "events.h"
#include "renderer.h"
#include "music.h"
#include "effects.h"
#include "raster.h"
#include "psg.h"

#define SPLASH 0
#define ONE_PLAYER 1
#define TWO_PLAYER 2
#define HOW 3
#define QUIT 4

#define OFF 0
#define ON 0
#define TIMER 0x462
#define BYTES_IN_BUFFER 32256

#define VBL_IRQ 28

typedef void (*Vector)();

Vector install_vector(int num, Vector vector);

void run_game_loop(UINT32 *bfrPtrs[]);
void run_splash_screen(UINT32 *bfrPtrs[]);
UINT32 get_time();
void handle_sync(UINT32 timeElapsed);
void handle_async(Model * md);
void handle_splash_input();
UINT32 run_how_screen(UINT32 *base32, UINT32 latestTick);
void reset_model(Model *md);
/* For when ISRs are introduced
UINT32 timeNow = 0;
UINT32 latestTick = 0;
*/
UINT8 secondaryBuffer[BYTES_IN_BUFFER];
UINT16 paused = OFF;
UINT16 curTrack = MOIST;

Model md =
{
	STAGING,
	PLAYER_ONE,
	STARTUP,
	0,
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
	P2_START_X, /* AI randomized position */
	0, /* windTick start */
	SPLASH
};

int main()
{
	/* MAIN BEGIN */
	UINT32 bBfrAddress;
	UINT32 *bfrPtrs[2];
		
	bfrPtrs[0] = (UINT32*)get_video_base();
	
	bBfrAddress = (UINT32)(secondaryBuffer + 256);
	bBfrAddress &= ~(0xFF);
	bfrPtrs[1] = (UINT32*)bBfrAddress;

	srand(get_time());
	
	do
	{
		while (md.playStyle == SPLASH)
			run_splash_screen(bfrPtrs);
		
		play_explosion(TANK_TYPE);
		if (md.playStyle != QUIT)
		{
			reset_model(&md);
			run_game_loop(bfrPtrs);
			md.playStyle = SPLASH;
		}
	}while (md.playStyle != QUIT);
	
	stop_sound();
	
	set_video_base((UINT16*)bfrPtrs[0]);
	
	return 0;
}

void run_splash_screen(UINT32 *bfrPtrs[])
{
	UINT16 curBuffer = 0;
	UINT32 latestTick, timeNow;
	
	render_splash(bfrPtrs[curBuffer]);
	
	set_video_base((UINT16*)bfrPtrs[curBuffer]);
	
	render_splash(bfrPtrs[1 - curBuffer]);
	render_how(bfrPtrs[1 - curBuffer]);
	
	start_music(MOIST);
	
	latestTick = get_time();
	while (md.playStyle == SPLASH)
	{
		if (input_available())
			handle_splash_input();
			
		if (md.playStyle == HOW)
		{
			play_menu_select();
			latestTick = run_how_screen(bfrPtrs[1 - curBuffer], latestTick);
			play_menu_select();
			set_video_base((UINT16*)bfrPtrs[curBuffer]);
		}
			
		timeNow = get_time();	
		if (timeNow != latestTick)
		{
			update_music (timeNow - latestTick, paused);
			latestTick = get_time();
		}
	}
}

void handle_splash_input()
{
	char input = get_input();
	switch(input)
	{
		case ONE_PRESS:
			md.playStyle = ONE_PLAYER;
			break;
		case TWO_PRESS:
			md.playStyle = TWO_PLAYER;
			break;
		case THREE_PRESS:
			md.playStyle = HOW;
			break;
		case FOUR_PRESS:
			md.playStyle = QUIT;
			break;
	};
	
}

UINT32 run_how_screen(UINT32 *base32, UINT32 latestTick)
{
	UINT32 timeNow;
	char killInput;
	int true = TRUE;
	
	set_video_base((UINT16*)base32);
	while (!input_available())
	{
		timeNow = get_time();
		
		if (timeNow != latestTick)
		{	
			update_music (timeNow - latestTick, paused);
			latestTick = get_time();
		}
	}
	
	killInput = get_input();
	md.playStyle = SPLASH;
	
	return latestTick;
}

void run_game_loop(UINT32 *bfrPtrs[])
{
	UINT16 curBuffer = 0;
	UINT32 latestTick, timeNow, timeElapsed;
	
	render(&md, bfrPtrs[curBuffer]);
	
	set_video_base((UINT16*)bfrPtrs[curBuffer]);
	
	render(&md, bfrPtrs[1 - curBuffer]);
	md.state = STAGING;
	
	latestTick = get_time();
	curTrack = MORTAL_KOMBAT;
	start_music(curTrack);
	
	while (md.state != GAME_OVER_FINISH)
	{
		if (input_available())
			handle_async (&md);
			
		timeNow = get_time();
		if (paused)
			latestTick = get_time();
		if (timeNow != latestTick && !paused)
		{
			update_music (timeNow - latestTick, paused);
			handle_sync(timeNow - latestTick);
			render (&md, bfrPtrs[curBuffer]);
			set_video_base((UINT16*)bfrPtrs[curBuffer]);
			
			latestTick = get_time();
			curBuffer = 1 - curBuffer;
		}
	}

	while (!input_available())
	{	
		timeNow = get_time();	
		if (timeNow != latestTick)
		{
			update_music (timeNow - latestTick, paused);
			render (&md, bfrPtrs[curBuffer]);
			set_video_base((UINT16*)bfrPtrs[curBuffer]);
			latestTick = get_time();
			curBuffer = 1 - curBuffer;
		}
	}
	
	stop_music();
}

void handle_sync(UINT32 timeElapsed)
{
	UINT16 i;
	switch (md.state)
	{
		case TURN_END:
			if (md.player == PLAYER_TWO)
				event_ai_position_adjust(&(md.tanks[PLAYER_TWO]),
						&(md.aiTankNewX));
						
			event_turn_end(&md);
			break;
		
		case STAGING:
			if (md.player == PLAYER_TWO && md.playStyle == ONE_PLAYER)
			{
				if (md.tanks[md.player].x == md.aiTankNewX)  
					event_randomized_attack(&md);		
													/*When tank position is within */  
													/*2 pixels of new position, attack*/
				else
					event_move_ai_tank(&md.tanks[md.player], md.aiTankNewX);	
													/* else keep moving */
			}
			break;
			
		case MISSILE_FIRED:
			event_move_missile(&md, timeElapsed);
			break;
		case MISSILE_EXPLODING:
			event_missile_explosion(&md);
			break;
		case GAME_OVER:
			if (md.missile.expCounter == 0)
				event_tank_explosion(&md, timeElapsed);
			break;
	};
}


void handle_async(Model * md)
{
	char input = get_input();
	if (input == P_PRESS)
		toggle_pause(&paused);
		
	if (md->state == STAGING && 
		(md->player != PLAYER_TWO || md->playStyle == TWO_PLAYER) && !paused)
	{
		
		switch (input)
		{
			case H_PRESS:
				curTrack = 1 - curTrack;
				start_music(curTrack);
				break;
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

void reset_model(Model *md)
{
	int i;
	
	for (i = 0; i < NUM_PLAYERS; i ++)
	{
		md->tanks[i].angle = START_ANG;
		md->tanks[i].power = START_POW;
		md->tanks[i].health = START_HP;
		md->tanks[i].deltaX = 0;
		md->tanks[i].expCounter = 0;
	}

	md->collision = STAGING;
	md->player = PLAYER_ONE;
	md->state = STARTUP;
	md->wind = (rand() & 7 + 1);
	md->tanks[0].x = P1_START_X;
	md->tanks[1].x = P2_START_X;


	md->missile.x = md->missile.y = 0;
	md->windTick = 0;
	
	randomize_terrain(md->terrain);
}
/*
void do_VBL()
{
	timeNow ++;
	
	update_music (timeNow - latestTick, paused);
	handle_sync(timeNow - latestTick);
	render (&md, bfrPtrs[curBuffer]);
	set_video_base((UINT16*)bfrPtrs[curBuffer]);
			
	latestTick = get_time();
	curBuffer = 1 - curBuffer;
	
	latestTick = timeNow;
}
*/
Vector install_vector(int num, Vector vector)
{
	Vector orig;
	Vector *vectp = (Vector *)((long)num << 2);
	long old_ssp = Super(0);

	orig = *vectp;
	*vectp = vector;

	Super(old_ssp);
	return orig;
}
