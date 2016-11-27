#include <osbind.h>

#include "types.h"
#include "psg.h"
#include "music.h"
#include "input.h"

#define MUSIC_END 11

#define TIMER 0x462
#define MUSIC_OFFSET 'a'

void update_music (UINT32 timeElapsed);
void start_music ();
UINT16 get_note_index(char note, UINT16 sharp);
UINT32 get_time();

UINT16 mc = 0;

int main()
{
	UINT32 timeThen, timeNow;
	start_music();
	timeThen = get_time();
	while (!input_available())
	{
		while (get_time() - timeThen == 0);
		update_music(get_time() - timeThen);
		timeThen = get_time();
	}
		
	return 0;
}


UINT32 get_time()
{
	UINT32 old_ssp;
	UINT32 *timer = (UINT32*)TIMER;
	UINT32 timeNow;
	old_ssp = Super(0);
	timeNow = *timer;
	Super(old_ssp);
	return timeNow;
}

void start_music ()
{
	UINT16 index;

	index = get_note_index(song[mc].note, song[mc].sharp);
		
	enable_channel(CHANNEL_A, ON, OFF);
	set_tone(CHANNEL_A, (scale[index] >> (song[mc].octave - 1));
	set_volume(CHANNEL_A, 10);
}

void update_music (UINT32 timeElapsed)
{
	UINT16 index;
	song[mc].duration - timeElapsed;
	if (song[mc].duration <= 0)
	{
		if (mc == MUSIC_END)
			mc = 0;
		else
			mc++;
			
		index = get_note_index(song[mc].note, song[mc].sharp);
		set_tone(CHANNEL_A, scale[index] >> (song[mc].octave - 1));
	}
}

UINT16 get_note_index(char note, UINT16 sharp)
{
	UINT16 index = (UINT16)(note - MUSIC_OFFSET);
	if (sharp)
		index += SHARP_OFFSET;
		
	return index;
}