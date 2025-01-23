#ifndef INCLUDE_EFFECTS_PHASER_H_
#define INCLUDE_EFFECTS_PHASER_H_

#include <stdlib.h>
#include <stdint.h>

void initPhaser(void);
void processPhaser(int16_t *input, int16_t *output, size_t blockSize);

#endif /* INCLUDE_EFFECTS_PHASER_H_ */
