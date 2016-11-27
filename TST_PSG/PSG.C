#include <osbind.h>
#include "psg.h"

volatile UINT8 * const PSG_reg_select = (UINT8*)PSG_SELECT;
volatile UINT8 * const PSG_reg_read = (UINT8*)PSG_READ;
volatile UINT8 * const PSG_reg_write = (UINT8*)PSG_WRITE;

/* writes the given byte value to the given psg register */
void write_psg(int reg, UINT8 val)
{
	UINT32 old_ssp = Super(0);
	*PSG_reg_select = reg;
	*PSG_reg_write = (val);
	Super(old_ssp);
}

/*Loads the tone registers (coarse and fine) for the given channel.
 0 = A 1 = B 2 = C, with the given 12 bit tuning */
void set_tone(int channel, int tuning)
{
	if (tuning <= TWELVE_BITS && 
		(channel >= CHANNEL_A && channel <= CHANNEL_C)) 
	{
		write_psg	(((channel << TUNE_SHIFT) + COARSE_OFFSET),
				(UINT8)(tuning >> COARSE_SHIFT));
				
		write_psg	((channel << TUNE_SHIFT), (UINT8)tuning);
	}
}

/*
Sets the envelope frequency given a value in the form
0x[COARSE][FINE] where coarse and fine are both 8 bit values,
frequency being a 16 bit value.  Also sets the specified 
8 bit shape for the envelope.

*/
void set_envelope (UINT16 frequency, UINT8 shape)
{
	write_psg(ENV_FINE_TUNE, (UINT8)frequency);
	write_psg(ENV_COARSE_TUNE, (UINT8) (frequency >> COARSE_SHIFT));
	write_psg(ENV_SHAPE, shape);
}

/*
Sets the volume of a given channel to the specified volume.

Will not set if the volume or channel is invalid.
*/
void set_volume(int channel, int volume)
{
	if (volume <= FIVE_BITS && (channel >= CHANNEL_A
				&& channel <= CHANNEL_C))
		write_psg(channel + VOL_REG, volume);
}

void enable_channel(int channel, int tone_on, int noise_on)
{
	UINT8 regData = read_psg(MIXER);
	UINT8 tone = (1 << channel);
	UINT8 noise = (1 << (channel + NOISE_OFFSET));
	
	regData = combine_masks (regData, tone, tone_on);
	regData = combine_masks (regData, noise, noise_on);
	
	if (channel >= CHANNEL_A && channel <= CHANNEL_C)
		write_psg(MIXER, regData);
}
/* Turns the given channel's tone noise signals on/off */

UINT8 combine_masks (UINT8 maskOne, UINT8 maskTwo, int AND)
{
	if (AND)
		maskOne &= ~(maskTwo);
	else
		maskOne |= maskTwo;
		
	return maskOne;
}

void stop_sound()
{
	UINT16 channel;
	for (channel = CHANNEL_A; channel < NUM_CHANNELS; channel++)
	{
		set_volume(channel, OFF);
		enable_channel(channel, OFF, OFF);
	}
}
/* Silences all PSG sound production */
 
UINT8 read_psg(int reg)
{
	UINT8 psg_data;
	UINT32 old_ssp = Super(0);
	
	*PSG_reg_select = reg;
	psg_data = *PSG_reg_read;
	
	Super(old_ssp);
	return psg_data;
}
/*optional*/