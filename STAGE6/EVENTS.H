#ifndef EVENTS_H
#define EVENTS_H

#include "types.h"
#include "model.h"

#define COLLISION 15

/* Async */
void event_cannon_up(struct Tank *tank);
void event_cannon_down(struct Tank *tank);
void event_tank_move_left(struct Tank *tank);
void event_tank_move_right(struct Tank *tank);
void event_increase_power(struct Tank *tank);
void event_decrease_power(struct Tank *tank);

/* Sync */
void event_move_ai_tank(struct Tank * tank, int newX);
void event_ai_position_adjust(struct Tank * tank, int * newX);
void event_ai_angle_adjust(struct Tank * tank, int wind);
void event_move_missile(Model *model, UINT32 timeElapsed);
void event_ai_power_adjust(struct Tank * tank, int wind);
void event_randomized_attack(Model *md);
void event_missile_explosion(Model *md);

/* Condition based */
void event_missile_fired(struct Missile * missile,
	const struct Tank * tank, UINT16 player, int * state,
	int *collision);
void event_tank_explosion(Model *md);
void event_tank_hit(struct Tank *tank, int * state);
void event_terrain_hit(struct Terrain terrain[], int index);
void switch_player (UINT16 *currentPlayer);
void event_turn_end(Model *model);
void event_turn_end_rend(int * state);


#endif