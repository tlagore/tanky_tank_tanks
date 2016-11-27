/*
Name: music
Purpose: Contains the music arrays for the songs within the game.

Allows for the starting of a specific track, and updating of the
current track playing.  

Author: Tyrone Lagore
Version: March 22, 2014
*/
#include "music.h"
#include "psg.h"
#include "defines.h"

#define REST_VOLUME 0
#define MASTER_VOLUME 12
#define TWO_THIRDS_V 8
#define HALF_VOLUME 6	
#define QUARTER_VOLUME 3

UINT16 get_note_index(char note, UINT16 sharp);
void check_change(int *duration, UINT16 *count, UINT16 end, Note track[],
	UINT16 channel);
void start_track(UINT16 channel, Note track[], UINT16 *counter, int *currentNoteDur);

UINT16 music_on = FALSE;
UINT16 volume_reduced = FALSE;  /* for simulating the envelope, expanded on later */
int currentNoteDurA;    /* The duration of the note being played on channel a */
int currentNoteDurB;    /* The duration of the note being played on channel b */
UINT16 musicCounter = 0;   /* The current note that the main track is on */
UINT16 backingCounter = 0; /* The current note that the backing track is on (if being played) */

UINT16 currentTrack = 0;  
UINT16 currentBackingTrack = 0;

/* duration of each track */
const UINT16 trackEnd[] =
{
	MOIST_END,
	MK_END
};

/* duration of the backing track */
const UINT16 backingEnd[] =
{
	MOIST_BACKING_END
};

/* Note frequency value list 
	Set at the lower end of the frequencies
	to allow for easy octave changes */
	
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

/* Notes follow the form:

note | sharp | duration | octave | volume to reduce to */


/* Song tracks
	Set as a 2D array to allow the 
	adding of more songs 
	
   Volume indicated on each note
	is the volume that it will be
	reduced to after half it's duration*/
const Note track[][LONGEST_SONG]=
{
	/*moist */
	{  
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'e', 0, QUARTER, 4, HALF_VOLUME },
		{ 'b', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'g', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, HALF, 4, HALF_VOLUME },
		{ 'g', 0, HALF, 5, HALF_VOLUME },
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'c', 0, QUARTER, 5, HALF_VOLUME },
		{ 'b', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, HALF, 5, HALF_VOLUME },
		{ 'c', 0, QUARTER, 5, HALF_VOLUME },
		
		/* END FIRST BAR */
		
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'f', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'f', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'f', 0, QUARTER, 5, HALF_VOLUME },
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		
		/* END SECOND BAR */
		
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'e', 0, QUARTER, 4, HALF_VOLUME },
		{ 'b', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'g', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, HALF, 4, HALF_VOLUME },
		{ 'g', 0, HALF, 5, HALF_VOLUME },
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'c', 0, QUARTER, 5, HALF_VOLUME },
		{ 'b', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, HALF, 5, HALF_VOLUME },
		{ 'c', 0, QUARTER, 5, HALF_VOLUME },
		
		/* END THIRD BAR */
		
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		
		{ 'f', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'f', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'f', 0, QUARTER, 5, HALF_VOLUME },
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		/* END FOURTH BAR */
		
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		{ 'f', 0, QUARTER, 5, HALF_VOLUME },
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		{ 'c', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'g', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 5, HALF_VOLUME },
		{ 'g', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'g', 0, HALF, 4, HALF_VOLUME },
		{ 'e', 0, HALF, 4, HALF_VOLUME },
		{ 'd', 0, HALF, 4, HALF_VOLUME },
		
		/*END FIFTH BAR */
		
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'e', 0, QUARTER, 4, HALF_VOLUME },
		{ 'b', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		
		{ 'g', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, HALF, 4, HALF_VOLUME },
		
		{ 'g', 0, HALF, 5, HALF_VOLUME },
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'c', 0, QUARTER, 5, HALF_VOLUME },
		
		{ 'b', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, HALF, 5, HALF_VOLUME },
		{ 'c', 0, QUARTER, 5, HALF_VOLUME },
		
		/* END SIXTH BAR */
		
			
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		{ 'f', 0, QUARTER, 5, HALF_VOLUME },
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		{ 'c', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'g', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 5, HALF_VOLUME },
		{ 'g', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'g', 0, HALF, 4, HALF_VOLUME },
		{ 'e', 0, HALF, 4, HALF_VOLUME },
		{ 'd', 0, HALF, 4, HALF_VOLUME },
		
		/* END SEVENTH BAR */
		
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		{ 'f', 0, QUARTER, 5, HALF_VOLUME },
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		{ 'c', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'g', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 5, HALF_VOLUME },
		{ 'g', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'g', 0, HALF, 4, HALF_VOLUME },
		{ 'e', 0, HALF, 4, HALF_VOLUME },
		{ 'd', 0, HALF, 4, HALF_VOLUME },
		
		/* END EIGHTH BAR */
	},
	{  /* mortal kombat */
		{ 'a', 0, QUARTER,  3, HALF_VOLUME},
		{ 'a', 0, QUARTER,  3, HALF_VOLUME},
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, QUARTER, 3, HALF_VOLUME },
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, QUARTER, 3, HALF_VOLUME },
		{ 'e', 0, QUARTER, 4, HALF_VOLUME },
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'e', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'g', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'e', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		
		{ 'g', 0, QUARTER, 3, HALF_VOLUME },
		{ 'g', 0, QUARTER, 3, HALF_VOLUME },
		{ 'b', 0, QUARTER, 3, HALF_VOLUME },
		{ 'g', 0, QUARTER, 3, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'g', 0, QUARTER, 3, HALF_VOLUME },
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		
		{ 'f', 0, QUARTER, 3, HALF_VOLUME },
		{ 'f', 0, QUARTER, 3, HALF_VOLUME },
		{ 'a', 0, QUARTER, 3, HALF_VOLUME },
		{ 'f', 0, QUARTER, 3, HALF_VOLUME },
		{ 'b', 0, QUARTER, 3, HALF_VOLUME },
		{ 'f', 0, QUARTER, 3, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'b', 0, QUARTER, 3, HALF_VOLUME },
		
		{ 'a', 0, QUARTER, 3, HALF_VOLUME },
		{ 'a', 0, QUARTER, 3, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, QUARTER, 3, HALF_VOLUME },
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, QUARTER, 3, HALF_VOLUME },
		{ 'e', 0, QUARTER, 4, HALF_VOLUME },
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'e', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'g', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'e', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		
		{ 'g', 0, QUARTER, 3, HALF_VOLUME },
		{ 'g', 0, QUARTER, 3, HALF_VOLUME },
		{ 'b', 0, QUARTER, 3, HALF_VOLUME },
		{ 'g', 0, QUARTER, 3, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'g', 0, QUARTER, 3, HALF_VOLUME },
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		
		{ 'f', 0, QUARTER, 3, HALF_VOLUME },
		{ 'f', 0, QUARTER, 3, HALF_VOLUME },
		{ 'a', 0, QUARTER, 3, HALF_VOLUME },
		{ 'f', 0, QUARTER, 3, HALF_VOLUME },
		{ 'b', 0, QUARTER, 3, HALF_VOLUME },
		{ 'f', 0, QUARTER, 3, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'b', 0, QUARTER, 3, HALF_VOLUME },
		
		{ 'a', 0, QUARTER+EIGHTH, 3, HALF_VOLUME 	},
		{ 'a', 0, QUARTER+EIGHTH, 3, HALF_VOLUME 	},
		{ 'a', 0, QUARTER+EIGHTH, 3, HALF_VOLUME 	},
		{ 'a', 0, QUARTER+EIGHTH, 3, HALF_VOLUME  	},
		{ 'g', 0, QUARTER, 3, HALF_VOLUME 		 	},
		{ 'c', 0, QUARTER, 4, HALF_VOLUME 		 	},
		
		{ 'a', 0, QUARTER+EIGHTH, 3, HALF_VOLUME	},
		{ 'a', 0, QUARTER+EIGHTH, 3, HALF_VOLUME 	},
		{ 'a', 0, QUARTER+EIGHTH, 3, HALF_VOLUME 	},
		{ 'a', 0, QUARTER+EIGHTH, 3, HALF_VOLUME 	},
		{ 'g', 0, QUARTER, 3, HALF_VOLUME 			},
		{ 'e', 0, QUARTER, 3, HALF_VOLUME 			},

		{ 'a', 0, QUARTER+EIGHTH, 3, HALF_VOLUME	},
		{ 'a', 0, QUARTER+EIGHTH, 3, HALF_VOLUME 	},
		{ 'a', 0, QUARTER+EIGHTH, 3, HALF_VOLUME 	},
		{ 'a', 0, QUARTER+EIGHTH, 3, HALF_VOLUME 	},
		{ 'g', 0, QUARTER, 3, HALF_VOLUME 			},
		{ 'c', 0, QUARTER, 4, HALF_VOLUME 			},
		
		{ 'a', 0, QUARTER + EIGHTH, 3, HALF_VOLUME 	},
		{ 'a', 0, QUARTER + EIGHTH, 3, HALF_VOLUME 	},
		{ 'a', 0, QUARTER, 3, HALF_VOLUME 			},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME 			},
		{ 'a', 0, QUARTER, 3, HALF_VOLUME 			},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME 			},
		{ 'a', 0, HALF, 3, HALF_VOLUME 				},
		
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME	},
		{ 'e', 0, QUARTER, 4, HALF_VOLUME	},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME	},
		{ 'c', 0, QUARTER, 4, HALF_VOLUME	},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME	},
		{ 'a', 1, QUARTER, 3, HALF_VOLUME	},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME	},
		{ 'c', 0, QUARTER, 4, HALF_VOLUME	},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME	},
		{ 'a', 1, EIGHTH, 3, HALF_VOLUME	},
		{ 'g', 0, QUARTER, 3, HALF_VOLUME	},
		
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME	},
		{ 'e', 0, QUARTER, 4, HALF_VOLUME	},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME	},
		{ 'c', 0, QUARTER, 4, HALF_VOLUME	},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME	},
		{ 'a', 1, QUARTER, 3, HALF_VOLUME	},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME	},
		{ 'c', 0, QUARTER, 4, HALF_VOLUME	},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME	},
		{ 'a', 1, EIGHTH, 3, HALF_VOLUME	},
		{ 'g', 0, QUARTER, 3, HALF_VOLUME	},
		
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME	},
		{ 'e', 0, QUARTER, 4, HALF_VOLUME	},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME	},
		{ 'c', 0, QUARTER, 4, HALF_VOLUME	},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME	},
		{ 'a', 1, QUARTER, 3, HALF_VOLUME	},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME	},
		{ 'c', 0, QUARTER, 4, HALF_VOLUME	},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME	},
		{ 'a', 1, EIGHTH, 3, HALF_VOLUME	},
		{ 'g', 0, QUARTER, 3, HALF_VOLUME	},
		
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME 	},
		{ 'a', 0, QUARTER, 3, HALF_VOLUME 	},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME 	},
		{ 'a', 0, QUARTER, 3, HALF_VOLUME 	},
		{ 'a', 0, EIGHTH, 3, HALF_VOLUME 	},
		{ 'a', 0, HALF, 3, HALF_VOLUME 		}
	}	
};

/* Backing track(s)
	Set as a 2D array to allow the adding of more songs
*/
const Note backing[][] = 
{
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
		
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'e', 0, QUARTER, 4, HALF_VOLUME },
		{ 'b', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'g', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, HALF, 4, HALF_VOLUME },
		{ 'g', 0, HALF, 5, HALF_VOLUME },
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'c', 0, QUARTER, 5, HALF_VOLUME },
		{ 'b', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, HALF, 5, HALF_VOLUME },
		{ 'c', 0, QUARTER, 5, HALF_VOLUME },
		
		/* END SECOND BAR */
		
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'f', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'f', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'f', 0, QUARTER, 5, HALF_VOLUME },
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		/* END THIRD BAR */
		
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'e', 0, QUARTER, 4, HALF_VOLUME },
		{ 'b', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, QUARTER, 4, HALF_VOLUME },
		{ 'g', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, HALF, 4, HALF_VOLUME },
		{ 'g', 0, HALF, 5, HALF_VOLUME },
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'c', 0, QUARTER, 5, HALF_VOLUME },
		{ 'b', 0, QUARTER, 4, HALF_VOLUME },
		{ 'c', 0, HALF, 5, HALF_VOLUME },
		{ 'c', 0, QUARTER, 5, HALF_VOLUME },
		/* END FOURTH BAR */
		
		{ 'g', 0, WHOLE, 4, HALF_VOLUME },
		{ 'g', 0, WHOLE, 4, HALF_VOLUME	},
		{ 'a', 0, WHOLE, 4, HALF_VOLUME	},
		{ 'a', 0, WHOLE, 4, HALF_VOLUME },
		
		/* END FIFTH BAR */
		
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'f', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'd', 0, QUARTER, 4, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'f', 0, QUARTER, 5, HALF_VOLUME },
		{ 'd', 0, QUARTER, 5, HALF_VOLUME },
		{ 'a', 0, QUARTER, 4, HALF_VOLUME },
		{ 'f', 0, QUARTER, 5, HALF_VOLUME },
		{ 'e', 0, QUARTER, 5, HALF_VOLUME },
		
		/* END SIXTH BAR */
		
		{ 'g', 0, WHOLE, 4, HALF_VOLUME },
		{ 'g', 0, WHOLE, 4, HALF_VOLUME	},
		{ 'a', 0, WHOLE, 4, HALF_VOLUME	},
		{ 'a', 0, WHOLE, 4, HALF_VOLUME },
		
		/* END SEVENTH BAR */ 
		
		{ 'r', 0, WHOLE, 4, HALF_VOLUME },
		{ 'r', 0, WHOLE, 4, HALF_VOLUME	},
		{ 'r', 0, WHOLE, 4, HALF_VOLUME	},
		{ 'r', 0, WHOLE, 4, HALF_VOLUME },
		/* END EIGHTH BAR */
	}
};


/*
Name: start_music
Purpose: Starts the music for a given track.  Enables the channel
	and sets the frequency of the channel to the first note of the 
	song.  Also starts the backing track for the music if the 
	song in question has a backing track.  Sets the boolean 
	flag music_on to true for future updates.
	
Input:
	UINT16 trackNum - The corresponding track and backing track number.

Assumptions:
The track number corresponds to it's backing track number.  As this is
how it is maintained internally, it should cause no issues.
*/
void start_music (UINT16 trackNum)
{	
	UINT16 indexA, indexB;
				
	music_on = TRUE;
	if (trackNum >= 0 && trackNum < NUM_TRACKS)
		currentTrack = trackNum;
	else 
		currentTrack = 0;
		
	start_track(CHANNEL_A, track[currentTrack], &musicCounter,&currentNoteDurA);
	currentBackingTrack = trackNum;
	
	if (currentBackingTrack >= 0 && currentBackingTrack < NUM_BACKING_TRACKS)
		start_track(CHANNEL_B, track[currentBackingTrack], &backingCounter, &currentNoteDurB);
	else 
		enable_channel(CHANNEL_B, OFF, OFF);
}

/*
Name: start_track
Purpose: Starts a selected track on a selected channel.  Sets the duration of 
	the current note to the duration of the first note in the track.
*/
void start_track(UINT16 channel, Note track[], UINT16 *counter, int *currentNoteDur)
{
	UINT16 index;
	
	*counter = 0;
	index = get_note_index(track[*counter].note,
			track[*counter].sharp);
	
	set_tone(channel, (scale[index] >> (track[*counter].octave)) - 1);
	enable_channel(channel, ON, OFF);
	set_volume(channel, track[*counter].volume);
	*currentNoteDur = track[*counter].duration;
}

/*
Name: toggle_pause
Purpose: pauses the selected track (all update requests will be ignored)

*/
void toggle_pause(UINT16 *pause)
{
	int i;
	
	enable_channel(CHANNEL_A, *pause, OFF);
	if (currentBackingTrack >= 0 && currentBackingTrack < NUM_BACKING_TRACKS)
		enable_channel (CHANNEL_B, *pause, OFF);

	*pause = 1 - *pause;		
}

/*
Name: update_music
Purpose: Checks to see if the music needs to be updated for the
	current running tracks.
	
Input:
	UINT32 timeElapsed - The time that has passed since the last music update
*/
void update_music (UINT32 timeElapsed)
{
	UINT16 index;
	if (music_on)
	{
		currentNoteDurA -= timeElapsed;
		currentNoteDurB -= timeElapsed;
		
		check_change(&currentNoteDurA, &musicCounter, trackEnd[currentTrack], track[currentTrack],
			CHANNEL_A);
		if (currentBackingTrack >= 0 && currentBackingTrack < NUM_BACKING_TRACKS)
			check_change(&currentNoteDurB, &backingCounter, backingEnd[currentBackingTrack], 
				backing[currentBackingTrack], CHANNEL_B);
	}
}
/*
Name: check_change
Purpose: Checks if a particular track needs to change note or volume of 
	the note.  The volume reduce is used as an alternative to the 
	envelope so that the envelope can be used for other purposes.
	
Input: 	
	int *duration - The duration that the current note can run
	int *count - The position of the current song
	UINT16 end - The length of the song
	Note track[] - the track being changed
	UINT16 Channel - The channel that the song is playing on
*/
void check_change(int *duration, UINT16 *count, UINT16 end, Note track[],
	UINT16 channel)
{
	UINT16 index;

	if (*duration <= 0)
	{
		if ((*count) >= end)
			(*count) = 0;
		else
			(*count) ++;

		(*duration) = track[(*count)].duration;
			
		index = get_note_index(track[(*count)].note, track[(*count)].sharp);
		
		set_tone(channel, scale[index] >> (track[(*count)].octave - 1));
		set_volume (channel, MASTER_VOLUME);
		
		volume_reduced = FALSE;
		
	}else if (*duration <= (track[(*count)].duration >> 1) && !volume_reduced)
	{  /* if the note has passed half it's duration and hasn't been reduced yet,
			reduce the volume of the channel to the set volume in the note.
			Used to simulate the envelope to allow it to be used elsewhere */
		set_volume(channel, track[(*count)].volume);
		volume_reduced = TRUE;
	}
}

/*
Name: get_note_index
Purpose: Returns the index of the frequency of the note in the scale array based
	on the character value handed in and whether or not it was a sharp.
	
Input: 
	char note - the letter representation of the note 'a' - 'g' or 'r' for rest
	UINT16 sharp - whether or not the note is sharp
	
Assumptions:
- The note being handed in is between 'a'-'g' or 'r'
- The caller knows how to translate a flat to a sharp to achieve full range
- if the note being called is 'r' (for rest) it is not indicated to be sharp
		(will cause bugs).
*/
UINT16 get_note_index(char note, UINT16 sharp)
{
	UINT16 index = (UINT16)(note - MUSIC_OFFSET);
	
	if (sharp)
		index += SHARP_OFFSET;
		
	return index;
}

/*
Name: stop_music
Purpose: Stops sound being generated by the PSG and sets music_on to false so future
	calls to update music will have no effect until music is started again.
*/
void stop_music()
{
	set_volume(CHANNEL_A, OFF);
	enable_channel(CHANNEL_A, OFF, OFF);
	set_volume(CHANNEL_B, OFF);
	enable_channel(CHANNEL_B, OFF, OFF);
	music_on = FALSE;
}
