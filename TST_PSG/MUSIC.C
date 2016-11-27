#include "music.h"
#include "psg.h"

#define REST_VOLUME 0
#define MASTER_VOLUME 12


const UINT16 scale[]=
{
	LOW_A,
	LOW_B,
	LOW_C,
	LOW_D,
	LOW_E,
	LOW_F,
	LOW_G,
	LOW_ASH,
	LOW_C, /* B# = C */
	LOW_CSH,
	LOW_DSH,
	LOW_F, /* E# = F */
	LOW_FSH,
	LOW_GSH,
	0,
	0,
	0,
	REST
};

UINT16 pause = OFF;
int currentNoteDurA;
int currentNoteDurB;
UINT16 mc = 0;
UINT16 bc = 0;


const Note moist[]=
{
	{ 'd', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'b', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 4, ENVELOPE_ON },
	
	{ 'g', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'a', 0, HALF, 4, ENVELOPE_ON },
	
	{ 'g', 0, HALF, 5, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 5, ENVELOPE_ON },
	
	{ 'b', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'c', 0, HALF, 5, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 5, ENVELOPE_ON },
	
	/* END FIRST BAR */
	
	{ 'd', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	
	{ 'f', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 4, ENVELOPE_ON },
	
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'f', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'f', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	
	/* END SECOND BAR */
	
	{ 'd', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'b', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 4, ENVELOPE_ON },
	
	{ 'g', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'a', 0, HALF, 4, ENVELOPE_ON },
	
	{ 'g', 0, HALF, 5, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 5, ENVELOPE_ON },
	
	{ 'b', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'c', 0, HALF, 5, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 5, ENVELOPE_ON },
	
	/* END THIRD BAR */
	
	{ 'd', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	
	{ 'f', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 4, ENVELOPE_ON },
	
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'f', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'f', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	/* END FOURTH BAR */
	
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'f', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'g', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'a', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'g', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'g', 0, HALF, 4, ENVELOPE_ON },
	{ 'e', 0, HALF, 4, ENVELOPE_ON },
	{ 'd', 0, HALF, 4, ENVELOPE_ON },
	
	/*END FIFTH BAR */
	
	{ 'd', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'b', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 4, ENVELOPE_ON },
	
	{ 'g', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'a', 0, HALF, 4, ENVELOPE_ON },
	
	{ 'g', 0, HALF, 5, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 5, ENVELOPE_ON },
	
	{ 'b', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'c', 0, HALF, 5, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 5, ENVELOPE_ON },
	
	/* END SIXTH BAR */
	
		
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'f', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'g', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'a', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'g', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'g', 0, HALF, 4, ENVELOPE_ON },
	{ 'e', 0, HALF, 4, ENVELOPE_ON },
	{ 'd', 0, HALF, 4, ENVELOPE_ON },
	
	/* END SEVENTH BAR */
	
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'f', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'g', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'a', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'g', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'g', 0, HALF, 4, ENVELOPE_ON },
	{ 'e', 0, HALF, 4, ENVELOPE_ON },
	{ 'd', 0, HALF, 4, ENVELOPE_ON }
	
	/* END EIGHTH BAR */
	
};


const Note backing[] = 
{
	{ 'r', 0, QUARTER, 4, REST_VOLUME },
	{ 'r', 0, QUARTER, 4, REST_VOLUME },
	{ 'r', 0, QUARTER, 4, REST_VOLUME },
	{ 'r', 0, QUARTER, 4, REST_VOLUME },
	{ 'r', 0, QUARTER, 4, REST_VOLUME },
	{ 'r', 0, HALF,	   4, REST_VOLUME },
	{ 'r', 0, HALF	 , 5, REST_VOLUME },
	{ 'r', 0, QUARTER, 5, REST_VOLUME },
	                     
	{ 'r', 0, QUARTER, 5, REST_VOLUME },
	{ 'r', 0, QUARTER, 5, REST_VOLUME },
	
	{ 'r', 0, QUARTER, 4, REST_VOLUME },
	{ 'r', 0, HALF,	   5, REST_VOLUME },
	{ 'r', 0, QUARTER, 5, REST_VOLUME },
	
	/*END FIRST BAR */
	
	{ 'd', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'b', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'g', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'a', 0, HALF, 4, ENVELOPE_ON },
	{ 'g', 0, HALF, 5, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'b', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'c', 0, HALF, 5, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 5, ENVELOPE_ON },
	
	/* END SECOND BAR */
	
	{ 'd', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	
	{ 'f', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 4, ENVELOPE_ON },
	
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'f', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	
	{ 'a', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'f', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	/* END THIRD BAR */
	
	{ 'd', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'b', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'g', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'a', 0, HALF, 4, ENVELOPE_ON },
	{ 'g', 0, HALF, 5, ENVELOPE_ON },
	{ 'e', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'd', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 5, ENVELOPE_ON },
	{ 'b', 0, QUARTER, 4, ENVELOPE_ON },
	{ 'c', 0, HALF, 5, ENVELOPE_ON },
	{ 'c', 0, QUARTER, 5, ENVELOPE_ON },
	/* END FOURTH BAR */
	
	{ 'g', 0, WHOLE, 4, ENVELOPE_ON },
	{ 'g', 0, WHOLE, 4, ENVELOPE_ON	},
	{ 'a', 0, WHOLE, 4, ENVELOPE_ON	},
	{ 'a', 0, WHOLE, 4, ENVELOPE_ON },
	
	/* END FIFTH BAR */
	
	{ 'g', 0, WHOLE, 4, ENVELOPE_ON },
	{ 'g', 0, WHOLE, 4, ENVELOPE_ON	},
	{ 'a', 0, WHOLE, 4, ENVELOPE_ON	},
	{ 'a', 0, WHOLE, 4, ENVELOPE_ON },
	
	/* END SIXTH BAR */
	
	{ 'g', 0, WHOLE, 4, ENVELOPE_ON },
	{ 'g', 0, WHOLE, 4, ENVELOPE_ON	},
	{ 'a', 0, WHOLE, 4, ENVELOPE_ON	},
	{ 'a', 0, WHOLE, 4, ENVELOPE_ON },
	
	/* END SEVENTH BAR */ 
	
	{ 'r', 0, WHOLE, 4, ENVELOPE_ON },
	{ 'r', 0, WHOLE, 4, ENVELOPE_ON	},
	{ 'r', 0, WHOLE, 4, ENVELOPE_ON	},
	{ 'r', 0, WHOLE, 4, ENVELOPE_ON },
	/* END EIGHTH BAR */
};

void start_music ()
{
	
	UINT16 indexA = get_note_index(moist[mc].note, moist[mc].sharp);
	UINT16 indexB = get_note_index(backing[bc].note, backing[bc].sharp);
	
	set_tone(CHANNEL_A, (scale[indexA] >> (moist[mc].octave - 1)));
	set_tone(CHANNEL_B, (scale[indexB] >> (backing[bc].octave - 1)));
	
	enable_channel(CHANNEL_A, ON, OFF);
	enable_channel(CHANNEL_B, ON, OFF);
	
	set_volume(CHANNEL_A, moist[mc].volume);
	set_volume(CHANNEL_B, backing[bc].volume);
	
	currentNoteDurA = moist[mc].duration;
	currentNoteDurB = backing[bc].duration;

}

void pause_music()
{
	enable_channel(CHANNEL_A, OFF, OFF);
	pause = ON;
}

void play_music()
{
	enable_channel(CHANNEL_A, ON, OFF);
	pause = OFF;
}

void volume_up()
{
	if (MASTER_VOLUME + 1 <= FOUR_BITS)
		/*MASTER_VOLUME ++;*/;
}

void volume_down()
{
	if (MASTER_VOLUME - 1 >= 0)
		/*MASTER_VOLUME--;*/;
}

void update_music (UINT32 timeElapsed)
{
	UINT16 index;
	if (!pause)
	{
		currentNoteDurA -= timeElapsed;
		currentNoteDurB -= timeElapsed;
		
		check_change (&currentNoteDurA, &mc, LEAD_END, moist, CHANNEL_A);
		check_change(&currentNoteDurB, &bc, BACKING_END, backing, CHANNEL_B);
	}
}

check_change(int *duration, UINT16 *count, UINT16 end, Note track[],
	UINT16 channel)
{
	UINT16 index;

	if (*duration <= 0)
	{
		if ((*count) == end)
			(*count) = 0;
		else
			(*count) ++;

		(*duration) = track[(*count)].duration;
			
		index = get_note_index(track[(*count)].note, track[(*count)].sharp);
		
		set_tone(channel, scale[index] >> (track[(*count)].octave - 1));
		set_volume (channel, track[(*count)].volume);
		set_envelope(((*duration) << LOW_FADE), FADE_OUT );
	}
}

UINT16 get_note_index(char note, UINT16 sharp)
{
	UINT16 index = (UINT16)(note - MUSIC_OFFSET);
	if (sharp)
		index += SHARP_OFFSET;
		
	return index;
}
