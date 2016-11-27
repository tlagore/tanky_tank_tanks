/* offsets.c */
#include "defines.h"
#include "offsets.h"

/* missile x numerator offset for power multiplier on shot */
const int missile_power_num_x[CANNON_ANGLES]=
{ 110, 90, 64, 33 };

/* missile y numerator offset for power multiplier on shot */
const int missile_power_num_y[CANNON_ANGLES]=
{ 64, 90, 110, 123 };

const int missile_spawn_x[NUM_PLAYERS][CANNON_ANGLES] =
{
	{ 35, 33, 31, 29 },
	{ -3, -1, 1, 3 }
};
const int missile_spawn_y [CANNON_ANGLES] =
	{ 11, 14, 17, 17 };

const int cannon_x_offset[] = {18, -2};
const int cannon_clear_offset[] = {0, -2};
