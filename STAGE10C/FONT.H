#ifndef FONTS_H
#define FONTS_H

#include "types.h"

#define FONT_HEIGHT 8
#define FONT_BUFFER 1
#define LETTER_OFFSET 'a'
#define BIG_FONT_HEIGHT 31

extern const UINT32 big_font_letters[][BIG_FONT_HEIGHT];
extern const UINT8 font_numbers[][FONT_HEIGHT];
extern const UINT8 font_letters[][FONT_HEIGHT];


#endif