#ifndef INCLUDE_GLYPHS_H_
#define INCLUDE_GLYPHS_H_

#include "tistdtypes.h"

#define GLYPH_COUNT 40 // N�mero de caracteres disponiveis

typedef struct {
    char character;
    Uint8 pattern[4];
} Glyph;

static const Glyph glyphs[GLYPH_COUNT] = {
    {'A', {0x7E, 0x09, 0x09, 0x7E}},
    {'B', {0x7F, 0x49, 0x49, 0x36}},
    {'C', {0x3E, 0x41, 0x41, 0x22}},
    {'D', {0x7F, 0x41, 0x41, 0x3E}},
    {'E', {0x7F, 0x49, 0x49, 0x41}},
    {'F', {0x7F, 0x09, 0x09, 0x01}},
    {'G', {0x3E, 0x41, 0x51, 0x32}},
    {'H', {0x7F, 0x08, 0x08, 0x7F}},
    {'I', {0x41, 0x7F, 0x41, 0x00}},
    {'J', {0x20, 0x40, 0x40, 0x3F}},
    {'K', {0x7F, 0x08, 0x14, 0x63}},
    {'L', {0x7F, 0x40, 0x40, 0x40}},
    {'M', {0x7F, 0x06, 0x06, 0x7F}},
    {'N', {0x7F, 0x06, 0x18, 0x7F}},
    {'O', {0x3E, 0x41, 0x41, 0x3E}},
    {'P', {0x7F, 0x09, 0x09, 0x06}},
    {'Q', {0x3E, 0x41, 0x61, 0x7E}},
    {'R', {0x7F, 0x09, 0x19, 0x66}},
    {'S', {0x26, 0x49, 0x49, 0x32}},
    {'T', {0x01, 0x7F, 0x01, 0x01}},
    {'U', {0x3F, 0x40, 0x40, 0x3F}},
    {'V', {0x1F, 0x20, 0x40, 0x3F}},
    {'W', {0x7F, 0x30, 0x30, 0x7F}},
    {'X', {0x63, 0x14, 0x14, 0x63}},
    {'Y', {0x03, 0x04, 0x78, 0x07}},
    {'Z', {0x61, 0x59, 0x4D, 0x43}},
    {'0', {0x3E, 0x45, 0x49, 0x3E}},
    {'1', {0x00, 0x42, 0x7F, 0x40}},
    {'2', {0x62, 0x51, 0x49, 0x46}},
    {'3', {0x22, 0x49, 0x49, 0x36}},
    {'4', {0x18, 0x14, 0x12, 0x7F}},
    {'5', {0x27, 0x49, 0x49, 0x31}},
    {'6', {0x3E, 0x49, 0x49, 0x32}},
    {'7', {0x01, 0x71, 0x09, 0x07}},
    {'8', {0x36, 0x49, 0x49, 0x36}},
    {'9', {0x26, 0x49, 0x49, 0x3E}},
    {'b', {0x7F, 0x48, 0x48, 0x30}},
    {'-', {0x08, 0x08, 0x08, 0x08}},
    {':', {0x00, 0x36, 0x36, 0x00}},
    {'!', {0x00, 0x00, 0xBE, 0x00}},
    {'.', {0x00, 0x00, 0x80, 0x00}},
};

#endif /* INCLUDE_GLYPHS_H_ */
