#ifndef RENDERER_H
#define RENDERER_H

#include "model.h"
#include "types.h"

#define NUM_BUFFERS 2

#define SPLASH_SQUARE_START_X 80
#define SPLASH_SQUARE_START_Y 120
#define SPLASH_SQUARE_WIDTH 200
#define SPLASH_SQUARE_HEIGHT 50

#define INSTRUCTIONS_X 137

#define HOW_X 460
#define HOW_Y 133

#define ONEP_TANK_X 156
#define ONEP_TANK_Y 140

#define TWOP_TANK1_X 124
#define TWOP_TANK1_Y 240

#define TWOP_TANK2_X 188
#define TWOP_TANK2_Y 240

#define EXIT_X 454
#define EXIT_Y 242

#define SPLASH_X_OFFSET 290
#define SPLASH_Y_OFFSET 100

void render(const Model *model, UINT32 * base32);
void render_splash(UINT32 *base32);
void render_how(UINT32 *base32);
void take_snapshot(Model *md);
void render_number (UINT8 * base, UINT16 x, UINT16 y, int number);

#endif