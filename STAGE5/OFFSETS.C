/* offsets.c */
#include "defines.h"
#include "offsets.h"

/* missile x numerator offset for power multiplier on shot */
const int missile_power_num_x[CANNON_ANGLES]=
{ 65, 55, 45, 35 };

/* missile y numerator offset for power multiplier on shot */
const int missile_power_num_y[CANNON_ANGLES]=
{ 20, 30, 40, 50 };

const int missile_spawn_x[NUM_PLAYERS][CANNON_ANGLES] =
{
	{ 33, 30, 25, 23 },
	{ -9, -5, -7, -7 }
};
const int missile_spawn_y [CANNON_ANGLES] =
	{ 8, 14, 17, 19 };

const int cannon_x_offset[] = {17, -1};
const int cannon_clear_offset[] = {0, -2};
