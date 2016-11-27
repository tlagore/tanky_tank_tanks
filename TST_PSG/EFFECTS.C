#include "psg.h"
#include "effects.h"

void play_explosion(UINT32 timeElapsed)
{
	write_psg(NOISE_GEN_CNTRL, EXPLOSION_FREQ);
	enable_channel(CHANNEL_C, OFF, ON);
	set_volume(CHANNEL_C, ENVELOPE_ON);
	set_envelope(EXPLOSION_ENV_PERIOD, FADE_OUT);
}