#ifndef INCLUDE_EFFECTS_REVERBS_H_
#define INCLUDE_EFFECTS_REVERBS_H_

#include <stdint.h>
#include <stddef.h>

// Estrutura de configuração para filtros Comb e All-Pass
typedef struct {
    float delayMs;
    float decayGain;
    size_t delaySamples;
    float *buffer;
    size_t bufferSize;
    size_t writeIndex;
} FilterConfig;

// Estrutura de configuração do Reverb
typedef struct {
    FilterConfig *combFilters;
    size_t numCombFilters;
    FilterConfig *allPassFilters;
    size_t numAllPassFilters;
    float wetLevel;  // Nível do sinal processado
    float sampleRate;
} ReverbConfig;

void initReverbHall2(void);
void initReverbStageA(void);
void initReverbStageBb(void);
void initReverbStageDb(void);
void initReverbStageFb(void);
void initReverbStageGTHT(void);

void processReverbBlock(int16_t *input, int16_t *output, size_t blockSize);

#endif /* INCLUDE_EFFECTS_REVERBS_H_ */
