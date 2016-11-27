#ifndef RENDERER_H
#define RENDERER_H

#include "model.h"
#include "types.h"

void render(const struct Model *model, UINT32 * base32,
	UINT16 curPlayer);
void render_startup(UINT32 * base32, const struct Model *model);
void render_missile(const struct Missile *missile, int oldX,
	int oldY, UINT8 * base8);
void render_terrain(const struct Terrain terrain[], UINT32 *base);
void render_tank(const struct Tank *tank, UINT16 player,
			UINT32 * base32);
void render_health(const UINT16 health, int player, UINT32 * base);
void render_power(const UINT16 power, int player, UINT32 *base);
void render_missile_impact(const struct Missile *missile, UINT8 *base8);
void render_tank_explosion(const struct Tank *tank, int player,
		 UINT32 *base);
void render_indexed_terrain(UINT32 * base32, 
	const struct Terrain terrain[], int index);
void render_wind (UINT32 * base, int wind);
UINT16 switch_player (UINT16 currentPlayer);
void render_current_player(UINT8 * base, UINT16 player);
void render_word (UINT8 * base8, UINT16 x,
	UINT16 y, char word[], UINT16 length);
void render_number (UINT8 * base, UINT16 x, UINT16 y,
	int number);

#endif