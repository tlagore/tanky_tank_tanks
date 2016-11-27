#ifndef PSG_H
#define PSG_H

#include "types.h"

#define PSG_SELECT 0xFF8800
#define PSG_READ 0xFF8800
#define PSG_WRITE 0xFF8802

/* General PSG defines */
#define NUM_CHANNELS 3
#define VOL_REG 8
#define TUNE_SHIFT 1
#define COARSE_OFFSET 1
#define FOUR_BITS 15
#define FIVE_BITS 31
#define EIGHT_BITS 255
#define TWELVE_BITS 4095
#define NOISE_OFFSET 3
#define OFF 0
#define ON 1
#define CHANNEL_A 0
#define CHANNEL_B 1
#define CHANNEL_C 2
#define COARSE_SHIFT 8

#define ENVELOPE_ON 16

#define FADE_OUT 0x00
#define FADE_IN_DROP 0x04
#define SHARKTOOTH_DOWN 0x08
#define WAVE_HIGH_LOW 0x0A
#define FADE_OUT_JUMP 0x0B
#define SHARKTOOTH_UP 0x0C
#define FADE_IN 0x0D
#define WAVE_LOW_HIGH 0x0E

#define LOW_FADE 9
#define MEDIUM_FADE 8
#define SHARK 7
#define PLINK_FADE 5
#define VIBRATO_WAVE 5
#define FLUTE_WAVE 6
#define FADE_SHIFT 8

/* register selects */
#define A_FINE 0x0
#define A_COARSE 0x1
#define B_FINE 0x2
#define B_COARSE 0x3
#define C_FINE 0x4
#define C_COARSE 0x5
#define NOISE_GEN_CNTRL 0x6
#define MIXER 0x7
#define A_VOL 0x8
#define B_VOL 0x9
#define C_VOL 0xA
#define ENV_FINE_TUNE 0xB
#define ENV_COARSE_TUNE 0xC
#define ENV_SHAPE 0xD

void write_psg(int reg, UINT8 val);
void set_tone(int channel, int tuning);
void set_volume(int channel, int volume);
void set_envelope (UINT16 frequency, UINT8 shape);
void enable_channel(int channel, int tone_on, int noise_on);
UINT8 combine_masks (UINT8 maskOne, UINT8 maskTwo, int AND);
void stop_sound();

UINT8 read_psg(int reg);
/*optional*/
#endif