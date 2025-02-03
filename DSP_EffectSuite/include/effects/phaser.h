#ifndef INCLUDE_EFFECTS_PHASER_H_
#define INCLUDE_EFFECTS_PHASER_H_

#include <stdlib.h>
#include "tistdtypes.h"

void initPhaser(void);
void processPhaser(const Int16 *input, Int16 *output, size_t blockSize);

#endif /* INCLUDE_EFFECTS_PHASER_H_ */
