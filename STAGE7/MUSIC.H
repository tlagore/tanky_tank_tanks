#ifndef MUSIC_H
#define MUSIC_H

#include "types.h"

#define DOUBLE 96
#define WHOLE 48
#define HALF 24
/* off right now */
#define QUARTER 12
/* off right now */
#define EIGHTH 6
#define SIXTEENTH 4
#define THIRTYSECOND 2
#define BREAK 1

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

#define LEAD_END 109
#define BACKING_END 82

#define MK_END 87



typedef struct Note
{
	char note;
	UINT16 sharp; /* 1 to set the selected note to a sharp */
	int duration; /* time to play note */
	UINT16 octave; /* which octave to play the note in */
	UINT16 volume;
} Note;

void start_music();
void toggle_pause(UINT16 *pause);
void stop_music();
void update_music (UINT32 timeElapsed, UINT16 pause);
UINT16 get_note_index(char note, UINT16 sharp);


check_change(int *duration, UINT16 *count, UINT16 end, Note track[],
	UINT16 channel);

extern const Note moist[];
extern const Note sail[];
extern const Note backing[];
extern const UINT16 scale[];


#endif