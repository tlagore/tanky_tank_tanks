#ifndef TANKS_H
#define TANKS_H

#include "types.h"
#include "defines.h"
#include "model.h"

#define OFF 0
#define ON 0
#define TIMER 0x462
#define BYTES_IN_BUFFER 32256

UINT32 get_time();
void handle_sync(Model * md, UINT32 * base, UINT32 timeElapsed);
void handle_async(Model * md, UINT32 * base);
void switch_buffers(UINT32 *bffrPtr1, UINT32 *bffrPtr2);


#endif