#include <string.h>
#include "ezdsp5502.h"
#include "lcd.h"
#include "glyphs.h"

#define CMD_SCROLL_DISABLE 0x2E
#define CMD_SCROLL_ENABLE  0x2F
#define CMD_VERTICAL_SCROLL_AREA 0xA3

void oled_addSpace(Uint8 count);
void oled_writeText(const char* text, Uint8 page);
void oled_fillPage(Uint8 page, Uint8 value);

Int16 oled_start(){
    /* Initialize  Display */
    osd9616_init();
    osd9616_send(0x00,CMD_SCROLL_DISABLE);     // Deactivate Scrolling

    return 0;
}

void oled_setPage(Uint8 page) {
    osd9616_send(0x00, 0x00);       // Set low column address
    osd9616_send(0x00, 0x10);       // Set high column address
    osd9616_send(0x00, 0xB0 + page); // Set page address
}

void oled_fillPage(Uint8 page, Uint8 value) {
    oled_setPage(page);

    Int16 i;
    for (i = 0; i < 128; i++) {
        osd9616_send(0x40, value);
    }
}

void oled_addSpace(Uint8 count) {
    Int16 i;
    for (i = 0; i < count; i++) {
        osd9616_send(0x40, 0x00);
    }
}

Uint8* getGlyphForChar(char c) {
    Int8 i;
    for (i = 0; i < GLYPH_COUNT; i++) {
        if (glyphs[i].character == c) {
            return (Uint8*) glyphs[i].pattern; // Cast explícito
        }
    }
    return NULL; // Retorna NULL se o caractere não for encontrado
}

void oled_writeText(const char* text, Uint8 page) {
    oled_setPage(page);
    oled_addSpace(1);

    size_t length = strlen(text);
    Int8 i, j;

    if (length < 19) {
        for (i = 0; i < 19 - length; i++) {
            for (j = 0; j < 5; j++) {
                osd9616_send(0x40, 0x00);
            }
        }
    }

    for (i = length-1; i >= 0; i--) {
        Uint8* glyph = getGlyphForChar(text[i]);
        if (glyph) {
            printLetter(glyph[3], glyph[2], glyph[1], glyph[0]);
        } else {
            for (j = 0; j < 5; j++) {
                osd9616_send(0x40, 0x00);
            }
        }
    }
}
