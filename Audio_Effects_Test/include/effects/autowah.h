#ifndef INCLUDE_EFFECTS_AUTOWAH_H_
#define INCLUDE_EFFECTS_AUTOWAH_H_

#include <stdlib.h>
#include <stdint.h>

void initAutoWah(void);
void processAutoWah(int16_t *input, int16_t *output, size_t blockSize);


#endif /* INCLUDE_EFFECTS_AUTOWAH_H_ */
