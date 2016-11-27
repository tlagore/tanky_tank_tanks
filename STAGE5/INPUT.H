#ifndef INPUT_H
#define INPUT_H

#define W_PRESS 0x11
#define S_PRESS 0x1F
#define LEFT 0x4B
#define RIGHT 0x4D
#define UP 0x48
#define DOWN 0x50
#define SPACE 0x39

#define ESCAPE 0x01
#define TOGGLE 

char get_input();
int input_available();

#endif
