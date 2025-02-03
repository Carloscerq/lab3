#include "effects/reverbs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define fminf(a, b) ((a) < (b) ? (a) : (b))
#define fmaxf(a, b) ((a) > (b) ? (a) : (b))


// Configuração global do reverb
// Configuração global do reverb
static ReverbConfig reverb;

//---------General functions for reverb types---------
// Função auxiliar: inicializa um filtro (Comb ou All-Pass)
static void initFilter(FilterConfig *filter, float delayMs, float decayGain, float sampleRate) {
    filter->delayMs = delayMs;
    filter->decayGain = decayGain;
    filter->delaySamples = (size_t)(delayMs * sampleRate / 1000.0f);
    filter->bufferSize = filter->delaySamples + 1; // +1 para evitar acessos fora do buffer
    filter->buffer = calloc(filter->bufferSize, sizeof(float));
    filter->writeIndex = 0;
    filter->applyDec = 0;
}

static void initFilterAllPass(FilterConfigAllPass *filter, float delayMs, float decayGain, float sampleRate) {
    filter->delayMs = delayMs;
    filter->decayGain = decayGain;
    filter->delaySamples = (size_t)(delayMs * sampleRate / 1000.0f);
    filter->bufferSize = filter->delaySamples + 1; // +1 para evitar acessos fora do buffer
    filter->buffer = calloc(filter->bufferSize, sizeof(float));
    filter->bufferCopy = calloc(filter->bufferSize, sizeof(float));
    filter->writeIndex = 0;
    filter->applyDec = 0;
}

// Função auxiliar: processa uma amostra com um filtro Comb
static float processCombFilter(FilterConfig *filter, float input) {
    size_t readIndex = (filter->writeIndex + filter->bufferSize - filter->delaySamples) % filter->bufferSize;

    float output = input;

    if (filter->writeIndex >= filter->delaySamples || filter->applyDec == 1){
        output += filter->buffer[readIndex] * filter->decayGain;
        filter->applyDec = 1;
    }

    // Atualiza o buffer
    filter->buffer[filter->writeIndex] = output;
    filter->writeIndex = (filter->writeIndex + 1) % filter->bufferSize;

    return output;
}

// Função auxiliar: processa uma amostra com um filtro All-Pass
static float processAllPassFilter(FilterConfigAllPass *filter, float input) {
    size_t readIndex = (filter->writeIndex + filter->bufferSize - filter->delaySamples) % filter->bufferSize;

    float delayedSample = 0;

    if (filter->writeIndex >= filter->delaySamples || filter->applyDec == 1){
        delayedSample = filter->buffer[readIndex];
        filter->applyDec = 1;
    }

    float output = input + delayedSample * filter->decayGain;

    if (filter->applyDec){
        output -= filter->bufferCopy[readIndex] * filter->decayGain;
    }

    // Atualiza o buffer
    filter->buffer[filter->writeIndex] = output;
    filter->bufferCopy[filter->writeIndex] = input;
    filter->writeIndex = (filter->writeIndex + 1) % filter->bufferSize;

    return output;
}

// Processa um bloco de amostras com o reverb
void processReverbBlock(const Int16 *input, Int16 *output, size_t blockSize) {
    size_t i, j;
    for (i = 0; i < blockSize; ++i) {
        float drySample = (float)input[i] / 32768.0f; // Normaliza para -1 a 1
        float wetSample = 0.0f;

        // Aplica filtros Comb em paralelo
        for (j = 0; j < reverb.numCombFilters; ++j) {
            wetSample += processCombFilter(&reverb.combFilters[j], drySample);
        }

        // Normaliza o resultado dos filtros Comb
        wetSample /= reverb.numCombFilters;

        // Aplica filtros All-Pass em série
        for (j = 0; j < reverb.numAllPassFilters; ++j) {
            wetSample = processAllPassFilter(&reverb.allPassFilters[j], wetSample);
        }

        // Mixagem Dry/Wet
        float finalSample = (1.0f - reverb.wetLevel) * drySample + reverb.wetLevel * wetSample;

        // Limita o valor final para evitar estouros
        finalSample = fminf(fmaxf(finalSample, -1.0f), 1.0f);

        // Converte de volta para 16 bits
        output[i] = (Int16)(finalSample * 32768.0f);
    }
}

//---------Initialization for different reverbs---------
// Initialize the Hall2 reverb
void initReverbHall2(void) {
    static FilterConfig combFilters[4];
    static FilterConfigAllPass allPassFilters[2];

    reverb.numCombFilters = 4;
    reverb.numAllPassFilters = 2;
    reverb.combFilters = combFilters;
    reverb.allPassFilters = allPassFilters;
    reverb.wetLevel = 0.5f;
    reverb.sampleRate = 44100.0f;

    float combDelays[] = {29.7f, 18.0f, 45.0f, 60.0f};
    float combGains[] = {0.805f, 0.827f, 0.783f, 0.764f};
    size_t i;
    for (i = 0; i < reverb.numCombFilters; ++i) {
        initFilter(&reverb.combFilters[i], combDelays[i], combGains[i], reverb.sampleRate);
    }

    float allPassDelays[] = {5.0f, 1.7f};
    float allPassGains[] = {0.7f, 0.7f};
    for (i = 0; i < reverb.numAllPassFilters; ++i) {
        initFilterAllPass(&reverb.allPassFilters[i], allPassDelays[i], allPassGains[i], reverb.sampleRate);
    }
}

// Libera buffers ao reinicializar
void freeReverbBuffers() {
    size_t i;
    for (i = 0; i < reverb.numCombFilters; ++i) {
        free(reverb.combFilters[i].buffer);
    }
    for (i = 0; i < reverb.numAllPassFilters; ++i) {
        free(reverb.allPassFilters[i].buffer);
    }
}
// TODO: create starters for other reverbs
void initReverbStageA(void){
    static FilterConfig combFilters[4];
    static FilterConfigAllPass allPassFilters[2];

    reverb.numCombFilters = 4;
    reverb.numAllPassFilters = 2;
    reverb.combFilters = combFilters;
    reverb.allPassFilters = allPassFilters;
    reverb.wetLevel = 0.5f;
    reverb.sampleRate = 44100.0f;

    float combDelays[] = {25.0f, 20.0f, 35.0f, 65.0f};
    float combGains[] = {0.805f, 0.827f, 0.783f, 0.764f};
    size_t i;
    for (i = 0; i < reverb.numCombFilters; ++i) {
        initFilter(&reverb.combFilters[i], combDelays[i], combGains[i], reverb.sampleRate);
    }

    float allPassDelays[] = {5.0f, 1.7f};
    float allPassGains[] = {0.7f, 0.7f};
    for (i = 0; i < reverb.numAllPassFilters; ++i) {
        initFilterAllPass(&reverb.allPassFilters[i], allPassDelays[i], allPassGains[i], reverb.sampleRate);
    }
}

void initReverbStageBb(void){
    static FilterConfig combFilters[4];
    static FilterConfigAllPass allPassFilters[2];

    reverb.numCombFilters = 4;
    reverb.numAllPassFilters = 2;
    reverb.combFilters = combFilters;
    reverb.allPassFilters = allPassFilters;
    reverb.wetLevel = 0.5f;
    reverb.sampleRate = 44100.0f;

    float combDelays[] = {10.0f, 10.0f, 40.0f, 60.0f};
    float combGains[] = {0.805f, 0.827f, 0.783f, 0.764f};
    size_t i;
    for (i = 0; i < reverb.numCombFilters; ++i) {
        initFilter(&reverb.combFilters[i], combDelays[i], combGains[i], reverb.sampleRate);
    }

    float allPassDelays[] = {5.0f, 1.7f};
    float allPassGains[] = {0.7f, 0.7f};
    for (i = 0; i < reverb.numAllPassFilters; ++i) {
        initFilterAllPass(&reverb.allPassFilters[i], allPassDelays[i], allPassGains[i], reverb.sampleRate);
    }
}


void initReverbStageDb(void){
    static FilterConfig combFilters[4];
    static FilterConfigAllPass allPassFilters[2];

    reverb.numCombFilters = 4;
    reverb.numAllPassFilters = 2;
    reverb.combFilters = combFilters;
    reverb.allPassFilters = allPassFilters;
    reverb.wetLevel = 0.5f;
    reverb.sampleRate = 44100.0f;

    float combDelays[] = {15.0f, 30.0f, 45.0f, 50.0f};
    float combGains[] = {0.805f, 0.827f, 0.783f, 0.764f};
    size_t i;
    for (i = 0; i < reverb.numCombFilters; ++i) {
        initFilter(&reverb.combFilters[i], combDelays[i], combGains[i], reverb.sampleRate);
    }

    float allPassDelays[] = {5.0f, 1.7f};
    float allPassGains[] = {0.7f, 0.7f};
    for (i = 0; i < reverb.numAllPassFilters; ++i) {
        initFilterAllPass(&reverb.allPassFilters[i], allPassDelays[i], allPassGains[i], reverb.sampleRate);
    }
}

void initReverbStageFb(void){
    static FilterConfig combFilters[4];
    static FilterConfigAllPass allPassFilters[2];

    reverb.numCombFilters = 4;
    reverb.numAllPassFilters = 2;
    reverb.combFilters = combFilters;
    reverb.allPassFilters = allPassFilters;
    reverb.wetLevel = 0.5f;
    reverb.sampleRate = 44100.0f;

    float combDelays[] = {40.0f, 38.0f, 55.0f, 80.0f};
    float combGains[] = {0.805f, 0.827f, 0.783f, 0.764f};
    size_t i;
    for (i = 0; i < reverb.numCombFilters; ++i) {
        initFilter(&reverb.combFilters[i], combDelays[i], combGains[i], reverb.sampleRate);
    }

    float allPassDelays[] = {5.0f, 1.7f};
    float allPassGains[] = {0.7f, 0.7f};
    for (i = 0; i < reverb.numAllPassFilters; ++i) {
        initFilterAllPass(&reverb.allPassFilters[i], allPassDelays[i], allPassGains[i], reverb.sampleRate);
    }
}


void initReverbStageGTHT(void){
    static FilterConfig combFilters[4];
    static FilterConfigAllPass allPassFilters[2];

    reverb.numCombFilters = 4;
    reverb.numAllPassFilters = 2;
    reverb.combFilters = combFilters;
    reverb.allPassFilters = allPassFilters;
    reverb.wetLevel = 0.5f;
    reverb.sampleRate = 44100.0f;

    float combDelays[] = {2.0f, 10.0f, 32.0f, 40.0f};
    float combGains[] = {0.805f, 0.827f, 0.783f, 0.764f};
    size_t i;
    for (i = 0; i < reverb.numCombFilters; ++i) {
        initFilter(&reverb.combFilters[i], combDelays[i], combGains[i], reverb.sampleRate);
    }

    float allPassDelays[] = {5.0f, 1.7f};
    float allPassGains[] = {0.7f, 0.7f};
    for (i = 0; i < reverb.numAllPassFilters; ++i) {
        initFilterAllPass(&reverb.allPassFilters[i], allPassDelays[i], allPassGains[i], reverb.sampleRate);
    }
}
