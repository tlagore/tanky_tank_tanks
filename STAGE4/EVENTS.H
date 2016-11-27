#ifndef EVENTS_H
#define EVENTS_H

#include "types.h"
#include "model.h"

#define COLLISION 15

/* Async */
void event_cannon_up(struct Tank *tank);
void event_cannon_down(struct Tank *tank);
void event_tank_move(struct Tank *tank);
void event_increase_power(struct Tank *tank);
void event_decrease_power(struct Tank *tank);
/* void event_fire_missile(struct Tank *tank, struct Missile *missile); */

/* Sync */
/*
void ai_tank_adjust(struct Tank *enemy);
*/

UINT16 event_ai_position_adjust(struct Tank * tank);
void event_ai_angle_adjust(struct Tank * tank);
UINT16 event_move_missile(struct Terrain terrain[],
	struct Missile *missile, struct Tank tanks[]);

/* Condition based */
void event_missile_fired(struct Missile * missile,
	const struct Tank tank, UINT16 player);
void tank_destroyed();
void event_tank_hit(struct Tank *tank);
void event_terrain_hit(struct Terrain terrain[], int index);

#endif