#ifndef INPUT_H
#define INPUT_H

#include "types.h"
#include "defines.h"

#define ONE_PRESS 0x02
#define TWO_PRESS 0x03
#define THREE_PRESS 0x04
#define FOUR_PRESS 0x05
#define W_PRESS 0x11
#define S_PRESS 0x1F
#define LEFT 0x4B
#define RIGHT 0x4D
#define UP 0x48
#define DOWN 0x50
#define SPACE 0x39
#define P_PRESS 0x19
#define H_PRESS 0x23

#define MOUSE_HEADER 0xF8

#define WAITING 0
#define DELTA_X 1
#define DELTA_Y 2

#define ESCAPE 0x01
#define TOGGLE 

typedef struct Mouse
{
	int x;
	int y;
	UINT8 mouseState;
	bool leftClicked;
	bool rightClicked;
} Mouse;

void enable_midi(bool enable);
void auto_repeat();
void reset_mouse_clicks();
char get_input();
int input_available();
void get_mouse_snapshot(Mouse *tankMouse);

#endif
