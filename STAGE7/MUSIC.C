#include "music.h"
#include "psg.h"
#include "defines.h"

#define REST_VOLUME 0
#define MASTER_VOLUME 12
#define TWO_THIRDS_V 8
#define HALF_VOLUME 6	
#define QUARTER_VOLUME 3

UINT16 volume_reduced = FALSE;
int currentNoteDurA;
int currentNoteDurB;
UINT16 mc = 0;
UINT16 bc = 0;
UINT16 vol_offset = 0;

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



const Note moist[]=
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
	{ 'd', 0, HALF, 4, HALF_VOLUME }
	
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
};



const Note mortal_kombat[]=
{
	{ 'a', 0, QUARTER,  3, HALF_VOLUME},
	{ 'a', 0, QUARTER,  3, HALF_VOLUME},
	{ 'c', 0, QUARTER, 4, HALF_VOLUME },
	{ 'a', 0, QUARTER, 3, HALF_VOLUME },
	{ 'd', 0, QUARTER, 4, HALF_VOLUME },
	{ 'a', 0, QUARTER, 3, HALF_VOLUME },
	{ 'e', 0, QUARTER, 4, HALF_VOLUME },
	{ 'd', 0, QUARTER, 4, HALF_VOLUME },
	
	{ 'c', 0, QUARTER, 4, HALF_VOLUME 	},
	{ 'c', 0, QUARTER, 4, HALF_VOLUME 	},
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
	
	{ 'f', 0, QUARTER, 3, HALF_VOLUME 	},
	{ 'f', 0, QUARTER, 3, HALF_VOLUME 	},
	{ 'a', 0, QUARTER, 3, HALF_VOLUME },
	{ 'f', 0, QUARTER, 3, HALF_VOLUME },
	{ 'b', 0, QUARTER, 3, HALF_VOLUME },
	{ 'f', 0, QUARTER, 3, HALF_VOLUME },
	{ 'c', 0, QUARTER, 4, HALF_VOLUME },
	{ 'b', 0, QUARTER, 3, HALF_VOLUME },
	
	{ 'a', 0, QUARTER, 3, HALF_VOLUME 	},
	{ 'a', 0, QUARTER, 3, HALF_VOLUME 	},
	{ 'c', 0, QUARTER, 4, HALF_VOLUME },
	{ 'a', 0, QUARTER, 3, HALF_VOLUME },
	{ 'd', 0, QUARTER, 4, HALF_VOLUME },
	{ 'a', 0, QUARTER, 3, HALF_VOLUME },
	{ 'e', 0, QUARTER, 4, HALF_VOLUME },
	{ 'd', 0, QUARTER, 4, HALF_VOLUME },
	
	{ 'c', 0, QUARTER, 4, HALF_VOLUME 	},
	{ 'c', 0, QUARTER, 4, HALF_VOLUME 	},
	{ 'e', 0, QUARTER, 4, HALF_VOLUME },
	{ 'c', 0, QUARTER, 4, HALF_VOLUME },
	{ 'g', 0, QUARTER, 4, HALF_VOLUME },
	{ 'c', 0, QUARTER, 4, HALF_VOLUME },
	{ 'e', 0, QUARTER, 4, HALF_VOLUME },
	{ 'c', 0, QUARTER, 4, HALF_VOLUME },
	
	{ 'g', 0, QUARTER, 3, HALF_VOLUME 	},
	{ 'g', 0, QUARTER, 3, HALF_VOLUME 	},
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
	
	{ 'a', 0, HALF, 3, HALF_VOLUME },
	{ 'a', 0, HALF, 3, HALF_VOLUME },
	{ 'a', 0, HALF, 3, HALF_VOLUME },
	{ 'a', 0, QUARTER, 3, HALF_VOLUME 	},
	{ 'g', 0, QUARTER, 3, HALF_VOLUME 	},
	{ 'c', 0, QUARTER, 4, HALF_VOLUME 	},
	
	{ 'a', 0, HALF, 3, HALF_VOLUME		},
	{ 'a', 0, HALF, 3, HALF_VOLUME 		},
	{ 'a', 0, HALF, 3, HALF_VOLUME 		},
	{ 'a', 0, QUARTER, 3, HALF_VOLUME 		},
	{ 'g', 0, QUARTER, 3, HALF_VOLUME 	},
	{ 'e', 0, QUARTER, 3, HALF_VOLUME 	},

	{ 'a', 0, HALF, 3, HALF_VOLUME		 },
	{ 'a', 0, HALF, 3, HALF_VOLUME 		},
	{ 'a', 0, HALF, 3, HALF_VOLUME 		},
	{ 'a', 0, QUARTER, 3, HALF_VOLUME 		},
	{ 'g', 0, QUARTER, 3, HALF_VOLUME 	},
	{ 'c', 0, QUARTER, 4, HALF_VOLUME 	},
	
	{ 'a', 0, QUARTER + EIGHTH, 3, HALF_VOLUME 	},
	{ 'a', 0, QUARTER + EIGHTH, 3, HALF_VOLUME 	},
	{ 'a', 0, QUARTER, 3, HALF_VOLUME 	},
	{ 'a', 0, EIGHTH, 3, HALF_VOLUME 		},
	{ 'a', 0, QUARTER, 3, HALF_VOLUME 	},
	{ 'a', 0, EIGHTH, 3, HALF_VOLUME 		},
	{ 'a', 0, HALF, 3, HALF_VOLUME 	}
/*	
	{ 'a', 0, EIGHTH, 4, HALF_VOLUME  	},
	{ 'e', 0, QUARTER, 4, HALF_VOLUME		},
	{ 'a', 0, EIGHTH, 4, HALF_VOLUME 		},
	{ 'c', 0, QUARTER, 5, HALF_VOLUME 	},
	{ 'a', 0, QUARTER , 4, HALF_VOLUME 	},
	{ 'a', 1, QUARTER, 4, HALF_VOLUME 	},
	{ 'a', 0, EIGHTH, 4, HALF_VOLUME 		},
	{ 'c', 0, QUARTER, 5, TWO_THIRDS_V 	},
	{ 'a', 0, QUARTER, 4, TWO_THIRDS_V 	},
	{ 'a', 1, EIGHTH, 4, TWO_THIRDS_V 		},
	{ 'g', 0, QUARTER, 4, TWO_THIRDS_V 	},

	{ 'a', 0, EIGHTH, 4, HALF_VOLUME  	},
	{ 'e', 0, QUARTER, 4, HALF_VOLUME		},
	{ 'a', 0, EIGHTH, 4, HALF_VOLUME 		},
	{ 'c', 0, QUARTER, 5, HALF_VOLUME 	},
	{ 'a', 0, QUARTER , 4, HALF_VOLUME 	},
	{ 'a', 1, QUARTER, 4, HALF_VOLUME 	},
	{ 'a', 0, EIGHTH, 4, HALF_VOLUME 		},
	{ 'c', 0, QUARTER, 5, TWO_THIRDS_V 	},
	{ 'a', 0, QUARTER, 4, TWO_THIRDS_V 	},
	{ 'a', 1, EIGHTH, 4, TWO_THIRDS_V 		},
	{ 'g', 0, QUARTER, 4, TWO_THIRDS_V 	},
	
	{ 'a', 0, EIGHTH, 4, HALF_VOLUME  	}, 
	{ 'e', 0, QUARTER, 4, HALF_VOLUME		}, 
	{ 'a', 0, EIGHTH, 4, HALF_VOLUME 		}, 
	{ 'c', 0, QUARTER, 5, HALF_VOLUME 	}, 
	{ 'a', 0, QUARTER , 4, HALF_VOLUME 	}, 
	{ 'a', 1, QUARTER, 4, HALF_VOLUME 	}, 
	{ 'a', 0, EIGHTH, 4, HALF_VOLUME 		}, 
	{ 'c', 0, QUARTER, 5, TWO_THIRDS_V 	}, 
	{ 'a', 0, QUARTER, 4, TWO_THIRDS_V 	}, 
	{ 'a', 1, EIGHTH, 4, TWO_THIRDS_V 		},
	{ 'g', 0, QUARTER, 4, TWO_THIRDS_V 	}, 

	{ 'a', 0, QUARTER, 4, HALF_VOLUME 	},
	{ 'e', 0, QUARTER, 4, HALF_VOLUME 	},
	{ 'a', 0, QUARTER, 4, HALF_VOLUME 	},
	{ 'c', 0, QUARTER, 5, HALF_VOLUME 	},
	{ 'a', 0, QUARTER, 4, HALF_VOLUME 	},
	{ 'a', 0, QUARTER, 4, HALF_VOLUME 	},
	{ 'a', 0, QUARTER, 4, HALF_VOLUME 	},
	{ 'a', 0, QUARTER, 4, HALF_VOLUME 	},
	{ 'a', 0, QUARTER, 4, HALF_VOLUME 	},
	{ 'a', 0, WHOLE,   4, HALF_VOLUME },*/
};

void start_music ()
{

	/*UINT16 indexTest = get_note_index(mortal_kombat[mc].note,
				mortal_kombat[mc].sharp);*/
				
	
	UINT16 indexA = get_note_index(moist[mc].note, moist[mc].sharp);
	UINT16 indexB = get_note_index(backing[bc].note, backing[bc].sharp);
	
	/*
	set_tone(CHANNEL_A, (scale[indexTest] >> (mortal_kombat[mc].octave - 1)));
	*/
	set_tone(CHANNEL_A, (scale[indexA] >> (moist[mc].octave - 1)));
	set_tone(CHANNEL_B, (scale[indexB] >> (backing[bc].octave - 1)));
	
	/*
	enable_channel(CHANNEL_A, ON, OFF);
	*/
	enable_channel(CHANNEL_A, ON, OFF);
	enable_channel(CHANNEL_B, ON, OFF);
	
	/*
	set_volume(CHANNEL_A, mortal_kombat[mc].volume);
	*/
	set_volume(CHANNEL_A, moist[mc].volume);
	set_volume(CHANNEL_B, backing[bc].volume);
	
	/*
	currentNoteDurA = mortal_kombat[mc].duration;
	*/
	currentNoteDurA = moist[mc].duration;
	currentNoteDurB = backing[bc].duration;
	
}

void toggle_pause(UINT16 *pause)
{
	int i;
	
	for (i = CHANNEL_A; i <= CHANNEL_B; i ++)
		enable_channel(i, *pause, OFF);

	*pause = 1 - *pause;		
}

void update_music (UINT32 timeElapsed, UINT16 pause)
{
	UINT16 index;
	if (pause != ON)
	{
		currentNoteDurA -= timeElapsed;
		currentNoteDurB -= timeElapsed;
		/*
		check_change(&currentNoteDurA, &mc, MK_END, mortal_kombat,
			CHANNEL_A);
		*/
		
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
		set_volume (channel, MASTER_VOLUME);
		
		volume_reduced = FALSE;
		
	}else if (*duration <= (track[(*count)].duration >> 1) && !volume_reduced)
	{
		set_volume(channel, track[(*count)].volume);
		volume_reduced = TRUE;
	}
}

UINT16 get_note_index(char note, UINT16 sharp)
{
	UINT16 index = (UINT16)(note - MUSIC_OFFSET);
	if (sharp)
		index += SHARP_OFFSET;
		
	return index;
}

void stop_music()
{
	stop_sound();
}
