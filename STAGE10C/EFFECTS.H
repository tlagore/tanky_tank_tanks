#ifndef EFFECTS_H
#define EFFECTS_H

#include "types.h"

#define MISSILE_TYPE 0
#define TANK_TYPE 1
#define EXPLOSION_FREQ 0xFF
#define EXPLOSION_ENV_PERIOD 0x4000
#define MISSILE_ENV_PERIOD 0x1000
#define MENU_SELECT_ENV_PERIOD 0x1300
#define ANGLE_CHANGE_ENV_PERIOD 0x0400

void play_menu_select();
void play_angle_change();
void play_explosion(UINT8 type);
void stop_effects(UINT32 timeElapsed);

#endif