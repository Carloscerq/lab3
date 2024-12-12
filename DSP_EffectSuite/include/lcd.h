#ifndef INCLUDE_LCD_H_
#define INCLUDE_LCD_H_

#include "tistdtypes.h"

Int16 osd9616_init();
Int16 osd9616_send(Uint16 comdat, Uint16 data );
Int16 osd9616_multiSend(Uint16* data, Uint16 len);
Int16 printLetter(Uint16 l1, Uint16 l2, Uint16 l3, Uint16 l4);

#endif /* INCLUDE_LCD_H_ */
