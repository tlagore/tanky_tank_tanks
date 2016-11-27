#include "psg.h"
#include "effects.h"

/*

*/
void play_explosion(UINT8 type)
{
	UINT16 envPeriod = type == MISSILE_TYPE ? MISSILE_ENV_PERIOD :
			EXPLOSION_ENV_PERIOD;
			
	set_noise(EXPLOSION_FREQ);
	enable_channel(CHANNEL_C, OFF, ON);
	enable_channel(CHANNEL_B, OFF, ON);
	set_volume(CHANNEL_C, ENVELOPE_ON);
	set_volume(CHANNEL_B, ENVELOPE_ON);
	set_envelope(envPeriod, FADE_OUT);
}

void play_angle_change()
{

	set_noise(EXPLOSION_FREQ);
	enable_channel(CHANNEL_C, OFF, ON);
	enable_channel(CHANNEL_B, OFF, ON);
	set_volume(CHANNEL_C, ENVELOPE_ON);
	set_volume(CHANNEL_B, ENVELOPE_ON);
	set_envelope(ANGLE_CHANGE_ENV_PERIOD, FADE_OUT);
}
#define LOW_A 0x8E1
#define LOW_ASH 0x861
#define LOW_B 0x7E9
#define LOW_C 0xEEE
#define LOW_CSH 0xE18
#define LOW_D 0xD4D
#define LOW_DSH 0xC8E
#define LOW_E 0xBDA
#define LOW_F 0xB2F
#define LOW_FSH 0xA8F
#define LOW_G 0x9F7
#define LOW_GSH 0x968
#define REST 0x000

void play_menu_select()
{
	enable_channel(CHANNEL_C, ON, OFF);
	set_tone(CHANNEL_C, LOW_A >> 3);
	set_volume(CHANNEL_C, ENVELOPE_ON);
	set_envelope(MENU_SELECT_ENV_PERIOD, FADE_OUT);
}