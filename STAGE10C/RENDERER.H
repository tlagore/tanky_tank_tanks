#ifndef RENDERER_H
#define RENDERER_H

#include "model.h"
#include "types.h"
#include "input.h"

#define NUM_BUFFERS 2

#define SPLASH_SSX 80
#define SPLASH_SSY 120
#define SPLASH_SW 200
#define SPLASH_SH 50

#define INSTRUCTIONS_X 137

#define HOW_X 460
#define HOW_Y 133

#define ONEP_TANK_X 156
#define ONEP_TANK_Y 140

#define TWOP_TANK1_X 108
#define TWOP_TANK_Y 240

#define TWOP_TANK2_X 220

#define WAITING_X 585
#define WAITING_Y 240

#define SERIAL_X 158
#define SERIAL_Y 242

#define SPLASH_XO 288
#define SPLASH_YO 100

void render_mouse(const Mouse *mouse, UINT8 *base8);
void render(const Model *model, UINT32 * base32);
void render_splash(UINT32 *base32, bool first_rend, bool waiting_for_player);
void render_how(UINT32 *base32);
void take_snapshot(Model *md);
void render_number (UINT8 * base, UINT16 x, UINT16 y, int number);
void reset_mouse();

#endif