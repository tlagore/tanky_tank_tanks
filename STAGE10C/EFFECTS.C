#include <osbind.h>

#include "psg.h"
#include "effects.h"

#define LOW_A 0x8E1

#define VBL_PRIOR 4

/*

*/

int mask_ipl(int mask);

void play_explosion(UINT8 type)
{
	UINT16 envPeriod = type == MISSILE_TYPE ? MISSILE_ENV_PERIOD :
			EXPLOSION_ENV_PERIOD;
	UINT16 old_ipl;
	UINT32 old_ssp = Super(1);
	
	if (old_ssp == 0)
		old_ssp = Super(0);
		
	old_ipl = mask_ipl(VBL_PRIOR);
	
	set_noise(EXPLOSION_FREQ);
	enable_channel(CHANNEL_C, OFF, ON);
	enable_channel(CHANNEL_B, OFF, ON);
	set_volume(CHANNEL_C, ENVELOPE_ON);
	set_volume(CHANNEL_B, ENVELOPE_ON);
	set_envelope(envPeriod, FADE_OUT);
	
	mask_ipl(old_ipl);
	
	if (old_ssp != -1)
		Super(old_ssp);
	

}

void play_angle_change()
{
	UINT32 old_ssp = Super(1);
	UINT16 old_ipl;
	
	if (old_ssp == 0)
		old_ssp = Super(0);
	
	old_ipl = mask_ipl(VBL_PRIOR);

	set_noise(EXPLOSION_FREQ);
	enable_channel(CHANNEL_C, OFF, ON);
	enable_channel(CHANNEL_B, OFF, ON);
	set_volume(CHANNEL_C, ENVELOPE_ON);
	set_volume(CHANNEL_B, ENVELOPE_ON);
	set_envelope(ANGLE_CHANGE_ENV_PERIOD, FADE_OUT);
	
		
	mask_ipl(old_ipl);
	
	if (old_ssp != -1)
		Super(old_ssp);
	
}

void play_menu_select()
{
	UINT32 old_ssp = Super(1);
	UINT16 old_ipl;
	
	if (old_ssp == 0)
		old_ssp = Super(0);
	
	old_ipl = mask_ipl(VBL_PRIOR);


	enable_channel(CHANNEL_C, ON, OFF);
	set_tone(CHANNEL_C, LOW_A >> 3);
	set_volume(CHANNEL_C, ENVELOPE_ON);
	set_envelope(MENU_SELECT_ENV_PERIOD, FADE_OUT);
	
	mask_ipl(old_ipl);
	
	if (old_ssp != -1)
		Super(old_ssp);
}