#ifndef INCLUDE_EFFECTS_REVERBS_H_
#define INCLUDE_EFFECTS_REVERBS_H_

#include <stdint.h>
#include <stddef.h>

typedef struct {
    float delayMs;
    float decayGain;
    size_t delaySamples;
    float *buffer;
    size_t bufferSize;
    size_t writeIndex;
} FilterConfig;

typedef struct {
    FilterConfig *combFilters;
    size_t numCombFilters;
    FilterConfig *allPassFilters;
    size_t numAllPassFilters;
    float wetLevel;
    float sampleRate;
} ReverbConfig;

void initReverbHall2(void);
void processReverbBlock(const int16_t *input, int16_t *output, size_t blockSize);

#endif /* INCLUDE_EFFECTS_REVERBS_H_ */
