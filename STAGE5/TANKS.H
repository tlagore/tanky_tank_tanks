#ifndef TANKS_H
#define TANKS_H

#include "types.h"
#include "defines.h"
#include "model.h"

UINT32 get_time();
void randomized_attack (struct Model *model, struct Snapshot *snap,
				UINT32 *base);
void handle_sync(struct Model * md, struct Snapshot *snap,
		UINT32 * base, UINT32 timeElapsed);
void handle_async(struct Model * md, struct Snapshot *snap,
		UINT32 * base);

#endif