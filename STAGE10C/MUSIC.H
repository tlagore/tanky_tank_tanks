#ifndef MUSIC_H
#define MUSIC_H

#include "types.h"

#define DOUBLE 96
#define WHOLE 48
#define HALF 24
#define QUARTER 12
#define EIGHTH 6
#define SIXTEENTH 3

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

#define SHARP 1
#define REG 0
#define REST 0

#define SHARP_OFFSET 7
#define MUSIC_OFFSET 'a'
#define MUSIC_END 53

#define MOIST 0
#define MORTAL_KOMBAT 1

#define MOIST_BACKING 0

#define NUM_TRACKS 2
#define NUM_BACKING_TRACKS 1

#define MOIST_END 109
#define MOIST_BACKING_END 82

#define MK_END 128

#define LONGEST_SONG 128


typedef struct Note
{
	char note;
	UINT16 sharp; /* 1 to set the selected note to a sharp */
	int duration; /* time to play note */
	UINT16 octave; /* which octave to play the note in */
	UINT16 volume;
} Note;

void start_splash_music();
void start_music(UINT16 trackNum);
void toggle_pause(UINT16 *pause);
void stop_music();
void update_music (UINT32 timeElapsed);

extern const UINT16 trackEnd[];
extern const UINT16 backingEnd[];

extern const Note backing[][];
extern const UINT16 scale[];


#endif