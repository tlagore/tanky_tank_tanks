#ifndef EFFECTS_H
#define EFFECTS_H

#include "types.h"

#define MISSILE_TYPE 0
#define TANK_TYPE 1
#define EXPLOSION_FREQ 0xFF
#define EXPLOSION_ENV_PERIOD 0x4000
#define MISSILE_ENV_PERIOD 0x1000
#define EXPLOSION_DURATION 46
#define TANK_EXP_DURATION 104

void play_explosion(UINT8 type);
void stop_effects(UINT32 timeElapsed);
void check_effect_stop(int *count, UINT16 max, UINT32 timeElapsed);

#endif