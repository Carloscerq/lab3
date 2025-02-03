#ifndef INCLUDE_EFFECTS_AUTOWAH_H_
#define INCLUDE_EFFECTS_AUTOWAH_H_

#include <stdlib.h>
#include "tistdtypes.h"
#include "math.h"
#include "string.h"

float bandPassFilter(float sample, float centerFreq, float q, float sampleRate, float *state);
void initAutoWah(void);
void processAutoWah(const Int16 *input, Int16 *output, size_t blockSize);


#endif /* INCLUDE_EFFECTS_AUTOWAH_H_ */
