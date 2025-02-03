#ifndef INCLUDE_EFFECTS_AUTOWAH_H_
#define INCLUDE_EFFECTS_AUTOWAH_H_

#include <stdlib.h>
#include "tistdtypes.h"
#include "math.h"
#include "string.h"

float bandPassFilter(float sample, float centerFreq, float q, float sampleRate, float *state);
void initAutoWah(void);
void processAutoWah(const Int16 *input, Int16 *output, size_t blockSize);
float envelope_follower(float input, float prev_envelope, float alpha);
void bandpass_filter(float input, float fc, float Q, float sample_rate);

#endif /* INCLUDE_EFFECTS_AUTOWAH_H_ */
