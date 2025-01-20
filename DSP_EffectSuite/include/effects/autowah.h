#ifndef INCLUDE_EFFECTS_AUTOWAH_H_
#define INCLUDE_EFFECTS_AUTOWAH_H_

#include <stdlib.h>
#include "tistdtypes.h"

void initAutoWah(void);
void processAutoWah(Int16 *input, Int16 *output, size_t blockSize);


#endif /* INCLUDE_EFFECTS_AUTOWAH_H_ */
