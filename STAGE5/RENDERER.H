#ifndef RENDERER_H
#define RENDERER_H

#include "model.h"
#include "types.h"

void render(const struct Model *model, struct Snapshot *snap,
	UINT32 * base32);
	
void render_startup(UINT32 *base32, const struct Model *model,
	const struct Snapshot *snap);
void render_constants(UINT8 *base); 

void render_turn_end(UINT32 *base32, const struct Model *md, 
	struct Snapshot *snap);
	
void render_staging(UINT32 * base32, const struct Model *model,
	struct Snapshot *snap);
	
void render_missile(const struct Missile *missile, 
	struct Missile *oldMissile, UINT8 * base8);
void render_terrain(const struct Terrain terrain[], UINT32 *base);
void render_tank(const struct Tank *tank, struct Tank *oldTank,
	UINT16 player, UINT32 *base32);
void render_health(const UINT16 health, int player, UINT32 * base);
void render_power(const UINT16 power, int player, UINT32 *base);
void render_missile_impact(struct Missile *missile, UINT8 *base8);
void render_tank_explosion(const struct Tank *tank, int player,
		 UINT32 *base);
void render_indexed_terrain(UINT32 * base32, 
	const struct Terrain terrain[], int index);
void render_wind (UINT32 * base, const int wind, int *oldWind);
void render_current_player(UINT8 * base, UINT16 player);

void render_word (UINT8 * base8, UINT16 x,UINT16 y, char word[]);
void render_number (UINT8 * base, UINT16 x, UINT16 y, int number);

#endif