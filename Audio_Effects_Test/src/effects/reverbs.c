#include "effects/reverbs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

static ReverbConfig reverb;

// Static buffers for Comb filters
static float comb1_buffer[6000]; // 29.7ms @44.1kHz
static float comb2_buffer[6000];  // 18ms
static float comb3_buffer[6000]; // 45ms
static float comb4_buffer[6000]; // 60ms

// Static buffers for All-Pass filters
static float allPass1_buffer[400]; // 5ms
static float allPass2_buffer[400];  // 1.7ms

static void initFilter(FilterConfig *filter, float delayMs, float decayGain, float sampleRate, float *buffer) {
    filter->delayMs = delayMs;
    filter->decayGain = decayGain;
    filter->delaySamples = (size_t)(delayMs * sampleRate / 1000.0f);
    filter->bufferSize = filter->delaySamples + 1;
    filter->buffer = buffer;
    filter->writeIndex = 0;
    memset(filter->buffer, 0, filter->bufferSize * sizeof(float));
}

static float processCombFilter(FilterConfig *filter, float input) {
    size_t readIndex = filter->writeIndex + 1;
    if (readIndex >= filter->bufferSize) readIndex = 0;

    float output = input + filter->buffer[readIndex] * filter->decayGain;

    filter->buffer[filter->writeIndex] = output;
    filter->writeIndex++;
    if (filter->writeIndex >= filter->bufferSize) filter->writeIndex = 0;

    return output;
}

static float processAllPassFilter(FilterConfig *filter, float input) {
    size_t readIndex = filter->writeIndex + 1;
    if (readIndex >= filter->bufferSize) readIndex = 0;

    float delayed = filter->buffer[readIndex];
    float v = input - filter->decayGain * delayed;
    float output = delayed + filter->decayGain * v;

    filter->buffer[filter->writeIndex] = v;
    filter->writeIndex++;
    if (filter->writeIndex >= filter->bufferSize) filter->writeIndex = 0;

    return output;
}

void processReverbBlock(const int16_t *input, int16_t *output, size_t blockSize) {
    static const float scale = 1.0f / 32768.0f;
    static const float invCombCount = 1.0f / 4.0f; // 4 comb filters

    for (size_t i = 0; i < blockSize; ++i) {
        float dry = input[i] * scale;
        float wet = 0.0f;

        // Process Comb filters in parallel
        wet += processCombFilter(&reverb.combFilters[0], dry);
        wet += processCombFilter(&reverb.combFilters[1], dry);
        wet += processCombFilter(&reverb.combFilters[2], dry);
        wet += processCombFilter(&reverb.combFilters[3], dry);
        wet *= invCombCount;

        // Process All-Pass filters in series
        wet = processAllPassFilter(&reverb.allPassFilters[0], wet);
        wet = processAllPassFilter(&reverb.allPassFilters[1], wet);

        // Mix wet/dry and clamp
        float out = (1.0f - reverb.wetLevel) * dry + reverb.wetLevel * wet;
        out = out < -1.0f ? -1.0f : out > 1.0f ? 1.0f : out;
        output[i] = (int16_t)(out * 32768.0f);
    }
}

void initReverbHall2(void) {
    static FilterConfig combFilters[4];
    static FilterConfig allPassFilters[2];

    reverb.sampleRate = 44100.0f;
    reverb.wetLevel = 0.8f;
    reverb.numCombFilters = 4;
    reverb.combFilters = combFilters;
    reverb.numAllPassFilters = 2;
    reverb.allPassFilters = allPassFilters;

    // Initialize Comb filters
    initFilter(&combFilters[0], 50.0f, 0.705f, reverb.sampleRate, comb1_buffer);
    initFilter(&combFilters[1], 25.0f, 0.727f, reverb.sampleRate, comb2_buffer);
    initFilter(&combFilters[2], 75.0f, 0.783f, reverb.sampleRate, comb3_buffer);
    initFilter(&combFilters[3], 90.0f, 0.764f, reverb.sampleRate, comb4_buffer);

    // Initialize All-Pass filters
    initFilter(&allPassFilters[0], 2.5f, 0.6f, reverb.sampleRate, allPass1_buffer);
    initFilter(&allPassFilters[1], 1.2f, 0.7f, reverb.sampleRate, allPass2_buffer);
}