#include <osbind.h>
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
#include "bitmaps.h"
#include "psg.h"
#include "system.h"
#include "serial.h"

#define SPLASH 0
#define ONE_PLAYER 1
#define TWO_PLAYER 2
#define HOW 3
#define WAITING 4

#define NONE -1
#define HOST 0
#define CLIENT 1

#define PING_PASSWORD 0x7F

#define CLOCK_TICK 1

#define OFF 0
#define ON 0
#define TIMER 0x462
#define BYTES_IN_BUFFER 32256

#define VBL_IRQ 28
#define IKBD_IRQ 70


void do_statics_start(UINT32 *bfrPtrs[], UINT16 curBuffer);
void run_game_loop(UINT32 *bfrPtrs[]);
void run_two_player_host(UINT32 *bfrPtrs[]);
void run_two_player_client (UINT32 *bfrPtrs[]);
void run_splash_screen(UINT32 *bfrPtrs[]);
void handle_sync(UINT32 timeElapsed);
void handle_local_async(Model * md);
void handle_splash_input(bool *waiting_for_player);
void run_how_screen(UINT32 *base32);
void end_game_loop(UINT32 *bfrPtrs[], UINT16 curBuffer);

/* ISR declarations */
void vbl_isr();
void input_isr();

/* ISR helper functions */
void do_VBL();

UINT32 clockCounter = 0;
bool render_request = FALSE;

UINT8 who_am_i;
UINT8 secondaryBuffer[BYTES_IN_BUFFER];
UINT16 paused = OFF;
UINT16 curTrack = MOIST;

Mouse gameMouse = 
{
	0,
	0,
	WAITING,
	FALSE,
	FALSE
};

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

/*volatile UINT8 * const MFP_VECTOR_BASE		= 0xFFFFFA17;*/

int main()
{
	/* MAIN BEGIN */
	UINT32 bBfrAddress;
	UINT32 *bfrPtrs[2];
	UINT32 old_ipl;
	Vector orig_vbl;
	Vector orig_ikbd;
	
	initialize();

	enable_midi(FALSE);
			
	orig_vbl = install_vector (VBL_IRQ, vbl_isr);
	orig_ikbd = install_vector (IKBD_IRQ, input_isr);
	
	/*serial_setup();*/
	
	super_mask_ipl(2); 
	
	bfrPtrs[0] = (UINT32*)get_video_base();
	
	bBfrAddress = (UINT32)(secondaryBuffer + 256);
	bBfrAddress &= ~(0xFF);
	bfrPtrs[1] = (UINT32*)bBfrAddress;

	do
	{
		reset_mouse();
		reset_model(&md);
		run_splash_screen(bfrPtrs);
		play_explosion(TANK_TYPE);
		
		srand(clockCounter);
		reset_model(&md);
		
		if (md.playStyle == ONE_PLAYER)
			run_game_loop(bfrPtrs);
		else
		{
			if (who_am_i == HOST)
				run_two_player_host(bfrPtrs);
			else 
				run_two_player_client(bfrPtrs);
		}
			
		md.playStyle = SPLASH;
		who_am_i = NONE;
	}while (TRUE);
	
	stop_sound();
	
	set_video_base((UINT16*)bfrPtrs[0]);
	
	install_vector (VBL_IRQ, orig_vbl);
	install_vector (IKBD_IRQ, orig_ikbd);
	
	serial_teardown();
	
	enable_midi(TRUE);
	
	return 0;
}
/*
Name: run_splash_screen
Purpose:  Handles the running and display of the splash screen. 
	Renders the splash screen to the back buffer, flips it to the
	front and renders the instructions to the new back buffer.
	
	This allows for seemless switching between the instructions
	and main splash screen without having to re-render ever.  
	The splash screen is mouse driven, left clicking within the
	bounding boxes of any of the choices will execute that 
	selection.
*/
void run_splash_screen(UINT32 *bfrPtrs[])
{
	UINT16 curBuffer = 0;
	char killInput;
	bool waiting_for_player = FALSE;
	bool ping_received = FALSE;
	int ping_password;
	/* Renders the game model as the backdrop for the splash screen */
	md.state = STARTUP;
	render(&md, bfrPtrs[1 - curBuffer]);			
	/* Then renders the splash screen over top */
	render_splash(bfrPtrs[1 - curBuffer], TRUE, waiting_for_player);

	/* Splash screen does not implement double buffering, but rather
		toggles between the 2 buffers for main splash/instructions */
	set_video_base((UINT16*)bfrPtrs[1-curBuffer]);

	render(&md, bfrPtrs[0]);
	md.state = SPLASH;

	/* Screen is swapped twice to avoid the flickering of first render */
	render_splash(bfrPtrs[curBuffer], TRUE, FALSE);
	set_video_base((UINT16*)bfrPtrs[curBuffer]);
	
	render_how(bfrPtrs[1 - curBuffer]);
	start_music(MOIST);
	
	while (md.playStyle == SPLASH || ping_received)
	{
		if (gameMouse.leftClicked)
			handle_splash_input(&waiting_for_player);
			
		if (input_available())
			killInput = get_input();
			
		if (render_request)
		{
			get_mouse_snapshot(&gameMouse);
			render_mouse(&gameMouse, (UINT8*)bfrPtrs[curBuffer]);
			render_splash(bfrPtrs[curBuffer], FALSE, waiting_for_player);
			render_request = FALSE;
		}
			
		if (md.playStyle == HOW)
		{
			run_how_screen(bfrPtrs[1 - curBuffer]);
			play_menu_select();
			set_video_base((UINT16*)bfrPtrs[curBuffer]);
		}
		
		if (serial_can_read() > 0)
		{
			serial_read((char *)&ping_password, 1);
			if (ping_password == PING_PASSWORD)
				md.playStyle = TWO_PLAYER;
		}
	}
	stop_music();
}

/*
Name: handle_splash_input
Purpose: Handles mouse clicks while the splash screen is running.

	Note on terms:
		SPLASH_SSX = SPLASH_SQUARE_START_X
		SPLASH_SSY = SPLASH_SQUARE_START_Y
		SPLASH_SW  = SPLASH_SQUARE_WIDTH
		SPLASH_SH  = SPLASH_SQUARE_HEIGHT
		SPLASH_XO  = SPLASH_X_OFFSET
		SPLASH_YO  = SPLASH_Y_OFFSET

*/
void handle_splash_input(bool *waiting_for_player)
{
	if (gameMouse.x >= SPLASH_SSX &&
		gameMouse.x <= SPLASH_SSX + SPLASH_SW &&
		gameMouse.y >= SPLASH_SSY &&
		gameMouse.y <= SPLASH_SSY + SPLASH_SH)
			md.playStyle = ONE_PLAYER;
	else if (gameMouse.x >= SPLASH_SSX + SPLASH_XO &&
		gameMouse.x <= SPLASH_SSX + SPLASH_XO + SPLASH_SW &&
		gameMouse.y >= SPLASH_SSY &&
		gameMouse.y <= SPLASH_SSY + SPLASH_SH)
	{
		md.playStyle = HOW;
		play_menu_select();		
	}
	else if (gameMouse.x >= SPLASH_SSX &&
		gameMouse.x <= SPLASH_SSX + SPLASH_SW &&
		gameMouse.y >= SPLASH_SSY + SPLASH_YO &&
		gameMouse.y <= SPLASH_SSY + SPLASH_YO + SPLASH_SH)
	{
		*waiting_for_player = 1 - *waiting_for_player;	
		play_menu_select();
		who_am_i = who_am_i == HOST ? NONE : HOST;
		/*md.playStyle = TWO_PLAYER;*/
	}
	else if (gameMouse.x >= SPLASH_SSX + SPLASH_XO &&
		gameMouse.x <= SPLASH_SSX + SPLASH_XO + SPLASH_SW &&
		gameMouse.y >= SPLASH_SSY + SPLASH_YO &&
		gameMouse.y <= SPLASH_SSY + SPLASH_YO + SPLASH_SH)
	{
		*waiting_for_player = 1 - *waiting_for_player;	
		play_menu_select();
		who_am_i = who_am_i == CLIENT ? NONE : CLIENT;
	}
			
	gameMouse.leftClicked = FALSE;
	reset_mouse_clicks();
}

/*
Name: run_how_screen
Purpose: Displays the instructions of the game to the screen

Simply changes the video base and waits for input to return
*/
void run_how_screen(UINT32 *base32)
{
	char killInput;
	
	play_menu_select();
	
	set_video_base((UINT16*)base32);
	while (!input_available())
		;
	
	killInput = get_input();
	md.playStyle = SPLASH;
}

/*
Name: run_game_loop
Purpose: Runs the main game loop which handles game play, 
	input, and syncrhonous events.  


*/
void run_game_loop(UINT32 *bfrPtrs[])
{
	UINT16 curBuffer = 1;
	do_statics_start(bfrPtrs, curBuffer);
	
	while (md.state != GAME_OVER_FINISH)
	{
		if (input_available())
			handle_local_async (&md);
			
		if (render_request)
		{
			render (&md, bfrPtrs[curBuffer]);
			set_video_base((UINT16*)bfrPtrs[curBuffer]);
			render_request = FALSE;
			curBuffer = 1 - curBuffer;
		}
	}
	end_game_loop(bfrPtrs, curBuffer);
	
	stop_music();
}

void run_two_player_host(UINT32 *bfrPtrs[])
{
	UINT16 curBuffer = 1;	
	do_statics_start(bfrPtrs, curBuffer);
	
	while (md.state != GAME_OVER_FINISH)
	{
		if (input_available())
			handle_local_async (&md);
			
		if (render_request)
		{
			render (&md, bfrPtrs[curBuffer]);
			set_video_base((UINT16*)bfrPtrs[curBuffer]);
			render_request = FALSE;
			curBuffer = 1 - curBuffer;
		}
	}
	end_game_loop(bfrPtrs, curBuffer);


}

void run_two_player_client (UINT32 *bfrPtrs[])
{
	UINT16 curBuffer = 1;
	do_statics_start(bfrPtrs, curBuffer);

	
}

void do_statics_start(UINT32 *bfrPtrs[], UINT16 curBuffer)
{
	render(&md, bfrPtrs[curBuffer]);
	
	set_video_base((UINT16*)bfrPtrs[curBuffer]);
	
	render(&md, bfrPtrs[1 - curBuffer]);
	md.state = STAGING;
	
	curTrack = MORTAL_KOMBAT;			/*change track */
	start_music(curTrack);
}

/*
Renders the player who won the game to the screen and waits for 
any input to return to the start screen.

*/
void end_game_loop(UINT32 *bfrPtrs[], UINT16 curBuffer)
{
	while (!input_available())
	{		
		if (render_request && md.state != GAME_DONE)
		{
			render (&md, bfrPtrs[curBuffer]);
			set_video_base((UINT16*)bfrPtrs[curBuffer]);
			render_request = FALSE;
			md.state = GAME_DONE;
		}
	}
}

/*
Name: handle_sync
Purpose: Handles the synchronous events happening within 
	the game loop.  
	
Input: 
	timeElapsed - The amount of time that has passed since the last 
		model update.
*/
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
													/*When tank position is */  
													/*at new position, attack*/
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

/*
Name: handle_local_async
Purpose: Handles the asynchronous input during the main game loops

*/
void handle_local_async(Model * md)
{
	char input = get_input();
	if (input == P_PRESS && md.playStyle != TWO_PLAYER)
		toggle_pause(&paused);
		
	if (md->state == STAGING && 
		(md->player != PLAYER_TWO || md->playStyle == TWO_PLAYER) && !paused)
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

void do_VBL()
{
	clockCounter++;
	
	auto_repeat();
	
	if (!paused)
	{
		update_music (CLOCK_TICK);
		
		handle_sync(CLOCK_TICK);
		render_request = TRUE;
	}
}
