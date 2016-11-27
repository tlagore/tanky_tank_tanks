#include <osbind.h>
#include "input.h"
#include "system.h"
#define CLEAR_SERVICE 0xBF

#define BUFFER_SIZE 128
#define MAKE_BREAK 0x80
#define CLEAN 0x7F
#define MAKE 0
#define BREAK 1
#define LEFT_MOUSE 0x02
#define RIGHT_MOUSE 0x01

#define MIDI_CNTRL_MASK 0xBF

#define AUTO_REPEAT 9

int mask_ipl(int mask);
void handle_key_input(UINT8 input);
void do_get_input();
void update_mouse(UINT8 input);
void auto_repeat_check(UINT8 keyMade, UINT8 madeKey,UINT8 *counter);

				UINT8	* 	const 	MIDI_CNTRL_ADDR	= 0xFFFFFC04;
volatile const	UINT8	*	const 	IKBD_RDR		= 0xFFFFFC02;
volatile 		UINT8	* 	const  	ISR_B_IN_SERVICE= 0xFFFFFA11;


UINT8 keyBuffer[BUFFER_SIZE];
UINT8 keyMadeA = FALSE;
UINT8 keyMadeB = FALSE;
UINT8 madeKeyA;
UINT8 madeKeyB;
UINT8 fill = 0;
UINT8 head = 0;
UINT8 tail = 0;
UINT8 autoRepeatA = 0;
UINT8 autoRepeatB = 0;


/* Mouse object, starting position of (320,75) */
Mouse mouse = 
{
	320,
	75,
	WAITING,
	FALSE,
	FALSE
};

/*
Name: get_input
Purpose: Gets input from the keyBuffer, if any is available.

Masks any interrupts for the duration of the grab to ensure
that the ACIA does not interrupt, possibly corrupting the data.

Output:
	The returned character
*/
char get_input()
{
	char toReturn;
	int old_ipl;
	
	old_ipl = super_mask_ipl(6);
	
	if (fill > 0)
	{
		fill--;
		toReturn = keyBuffer[head];
		head = (head + 1) & (BUFFER_SIZE - 1);
	}
	
	super_mask_ipl(old_ipl);

	return toReturn;
}

/*
Name: input_available()
Purpose: Reports if there is input in the buffer to read or not.
*/
int input_available()
{
	return fill != 0;
}

/*
Name: do_get_input
Purpose:  Called by the IKBD ISR.  Reads the input from the IKBD
	and checks to see if it is the start of a mouse package, or
	if it is currently in the middle of receiving a mouse package.
	
	If it is not, it handles the key input.
*/
void do_get_input()
{
	UINT8 input = *IKBD_RDR;
	
	if ((input & MOUSE_HEADER) == MOUSE_HEADER || 
		mouse.mouseState != WAITING)
		update_mouse(input);
	else
		handle_key_input(input);
		
	*ISR_B_IN_SERVICE &= CLEAR_SERVICE;
}

/*
Name: update_mouse
Purpose: A mouse packet has been initiated.  update_mouse detects
	which part of the 3-byte packet it is receiving and performs
	necessary changes to the mouse structure.
	
Inputs:
	UINT8 input - The byte that was received from the ikbd rdr 

*/
void update_mouse(UINT8 input)
{
	switch (mouse.mouseState)
	{
		case WAITING:
			mouse.mouseState = DELTA_X;		
			mouse.leftClicked = input & LEFT_MOUSE;
			mouse.rightClicked = input & RIGHT_MOUSE;
			break;
		case DELTA_X:
			mouse.x += (signed char)input;
			if (mouse.x < 0)
				mouse.x = 0;
			else if (mouse.x >= 632)
				mouse.x = 631;
			mouse.mouseState = DELTA_Y;
			break;
		case DELTA_Y:
			mouse.y += (signed char)input;
			if (mouse.y < 0)
				mouse.y = 0;
			else if (mouse.y >= 394)
				mouse.y = 393;
			mouse.mouseState = WAITING;
			break;
		default:
			break;
	}
}

/*
Name: auto_repeat
Purpose: Does an auto_repeat check for up to 2 keys being held down at one
	point in time.
*/
void auto_repeat()
{
	auto_repeat_check (keyMadeA, madeKeyA, &autoRepeatA);
	auto_repeat_check (keyMadeB, madeKeyB, &autoRepeatB);
}

/*
Simulates a key press being held down.  If the keyMade 
	flag is set, it means the isr has not yet seen a break code
	come in.
*/
void auto_repeat_check(UINT8 keyMade, UINT8 madeKey,UINT8 *counter)
{
	if (keyMade)
	{
		if (*counter < AUTO_REPEAT)
			(*counter)++;
		else
		{
			*counter = 0;
			fill++;	
			keyBuffer[tail] = madeKey;
			tail = (tail + 1) & (BUFFER_SIZE - 1);
		}
	}else
		*counter = 0;
}

/*
Name: handle_key_input
Purpose: Handles the keyboard input from the ikbd rdr.

Input:
	UINT8 input - The byte that was received from the IKBD rdr

*/
void handle_key_input(UINT8 input)
{
	UINT8 keyInput = (input & CLEAN);
	if ((input & MAKE_BREAK) == MAKE)
	{
		fill++;	
		if (!keyMadeA)
		{
			madeKeyA = keyInput;
			keyMadeA = TRUE;
		}else 
		{
			madeKeyB = keyInput;
			keyMadeB = TRUE;
		}
		keyBuffer[tail] = keyInput;
		tail = (tail + 1) & (BUFFER_SIZE - 1);

	}else if (keyInput == madeKeyA)
		keyMadeA = FALSE;
	else if (keyInput == madeKeyB)
		keyMadeB = FALSE;
}

/*
Name: reset_mouse_clicks
Purpose: Used as a safety in case the mou
*/
void reset_mouse_clicks()
{
	mouse.leftClicked = FALSE;
	mouse.rightClicked = FALSE;
}

/*
Name: enable_midi
Purpose: Allows the caller to enable or disable the midi interrupts
	based on a flag.
*/
void enable_midi(bool enable)
{
	UINT32 old_ssp = Super(1);
	
	if (old_ssp == 0)
		old_ssp = Super(0);
		
	if (enable)
		*MIDI_CNTRL_ADDR |= ~MIDI_CNTRL_MASK;
	else
		*MIDI_CNTRL_ADDR &= MIDI_CNTRL_MASK;
		
	if (old_ssp != -1)
		Super(old_ssp);
}

/*
Name: get_mouse_snapshot
Purpose: Gives a snapshot of the mouses current location and 
	state to the calling function.
*/
void get_mouse_snapshot(Mouse *tankMouse)
{	
	tankMouse->x = mouse.x;
	tankMouse->y = mouse.y;
	tankMouse->leftClicked = mouse.leftClicked;	
	tankMouse->rightClicked = mouse.rightClicked;
}