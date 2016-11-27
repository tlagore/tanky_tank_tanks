#ifndef RENDERER_H
#define RENDERER_H

#include "model.h"
#include "types.h"

void render(const Model *model, UINT32 * base32);
	
void render_startup(UINT32 *base32, const Model *model);
void render_constants(UINT8 *base); 
void render_turn_end(UINT32 *base32, const Model *md);
	
void render_staging(UINT32 * base32, const Model *model);

void render_affected(const Model *md, UINT32 *base32);	
	
void render_missile(const Model *md, UINT8 * base8);
void render_terrain(const struct Terrain terrain[], UINT32 *base);
void render_tank(const struct Tank *tank, UINT16 player, 
	UINT32 *base32);
void render_health(const UINT16 health, int player, UINT32 * base);
void render_power(const UINT16 power, int player, UINT32 *base);
void render_missile_impact(const Model *md, UINT8 *base8);
void render_tank_explosion(UINT32 *base32, const Model *md);
void render_indexed_terrain(UINT32 * base32, 
	const struct Terrain terrain[], int index);
void render_wind (UINT32 * base, const int wind);
void render_current_player(UINT8 * base, UINT16 player);

void render_word (UINT8 * base8, UINT16 x,UINT16 y, char word[]);
void render_number (UINT8 * base, UINT16 x, UINT16 y, int number);

void take_snapshot(Model *md);

#endif