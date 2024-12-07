#ifndef INCLUDE_BUTTONS_H_
#define INCLUDE_BUTTONS_H_

#include "tistdtypes.h"

#define BUTTON_NEXT   1
#define BUTTON_SELECT 2

void initI2CButtons();
void checkButtons(void);
void changeButtonState(Uint8 id);
Uint8 getButtonState(Uint8 id);

#endif /* INCLUDE_BUTTONS_H_ */
