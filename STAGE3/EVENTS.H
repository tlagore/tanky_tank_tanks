#ifndef EVENTS_H
#define EVENTS_H

#include "model.h"

/* Async */
void event_cannon_up(struct Tank *tank);
void event_cannon_down(struct Tank *tank);
void event_tank_move(struct Tank *tank);
void event_tank_right(struct Tank *tank);
void event_increase_power(struct Tank *tank);
void event_decrease_power(struct Tank *tank);
/* void event_fire_missile(struct Tank *tank, struct Missile *missile); */

/* Sync */
/*
void ai_canon_adjust(struct Tank *enemy);
void ai_tank_adjust(struct Tank *enemy);
void ai_power_adjust(struct Tank *enemy);
*/
int event_move_missile(struct Terrain terrain[],
	struct Missile *missile, struct Tank tanks[]);

/* Condition based */
/* void event_missile_fired(); */
void tank_destroyed();
void event_tank_hit(struct Tank *tank);
void event_terrain_hit(struct Terrain terrain[], int index);

#endif