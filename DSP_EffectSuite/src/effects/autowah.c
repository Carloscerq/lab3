#include "effects/autowah.h"

typedef struct {
    float lFoFreq;
    float qualityFactor;
    float minFreq;
    float maxFreq;
    float state[2];
} AutoWah;

#define PI 3.141592653589793
#define SAMPLE_RATE 44100

static AutoWah autowah;

void initAutoWah(void){
    autowah.lFoFreq = 6.0f;
    autowah.qualityFactor = 0.7f;
    autowah.minFreq = 300.0f;
    autowah.maxFreq = 1200.0f;
    memset(autowah.state, 0, sizeof(autowah.state));
}

float bandPassFilter(float sample, float centerFreq, float q, float sampleRate, float *state) {
    float omega = 2.0f * PI * centerFreq / sampleRate;
    float alpha = sinf(omega) / (2.0f * q);

    float a0 = 1.0f + alpha;
    float b0 = alpha;
    float b1 = 0.0f;
    float b2 = -alpha;
    float a1 = -2.0f * cosf(omega);
    float a2 = 1.0f - alpha;

      // Apply filter
    float output = (b0 / a0) * sample + state[0];
    state[0] = (b1 / a0) * sample + state[1] - (a1 / a0) * output;
    state[1] = (b2 / a0) * sample - (a2 / a0) * output;

    return output;
}

void processAutoWah(const Int16 *input, Int16 *output, size_t blockSize){
    size_t i;
    for (i = 0; i < blockSize; i++) {
        float sample = (float)input[i] / 32768.0f;
        float lfo = sinf(2.0f * PI * autowah.lFoFreq * (float)i / SAMPLE_RATE);

        float centerFreq = autowah.minFreq + (autowah.maxFreq - autowah.maxFreq) * ((lfo + 1.0f) / 2.0f);

        // Apply the band-pass filter
        float outputSample = bandPassFilter(sample, centerFreq, autowah.qualityFactor, (float)SAMPLE_RATE, autowah.state);

        // Scale the output back to 16-bit integer range and store it
        output[i] = (Int16)(outputSample * 32767.0f);
    }
}
