#include "psg.h"
#include "effects.h"

int explosionCount = 0;
int tankExplosionCount = 0;

void play_explosion(UINT8 type)
{
	UINT16 envPeriod = type == MISSILE_TYPE ? MISSILE_ENV_PERIOD :
			EXPLOSION_ENV_PERIOD;
	set_noise(EXPLOSION_FREQ);
	/*enable_channel(CHANNEL_B, OFF, ON);*/
	enable_channel(CHANNEL_C, OFF, ON);
	/*set_volume(CHANNEL_B, ENVELOPE_ON);*/
	set_volume(CHANNEL_C, ENVELOPE_ON);
	set_envelope(envPeriod, FADE_OUT);
}

void stop_effects(UINT32 timeElapsed)
{
	check_effect_stop(&explosionCount, EXPLOSION_DURATION, timeElapsed);
	check_effect_stop(&tankExplosionCount, TANK_EXP_DURATION,
			timeElapsed);
}

void check_effect_stop(int *count, UINT16 max, UINT32 timeElapsed)
{
	if ((*count) != 0)
	{
		(*count) += timeElapsed;
		if ((*count) >= max)
		{
			enable_channel(CHANNEL_C, OFF, OFF);
			(*count) = 0;
		}
	}
}