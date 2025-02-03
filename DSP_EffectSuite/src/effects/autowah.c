#include "effects/autowah.h"

typedef struct {
    float prev_env;
    float alpha;
    float base_freq;
    float max_freq;
    float q;
} AutoWah;

typedef struct {
    float low, band, high;
} SVF;

#define PI 3.141592653589793
#define SAMPLE_RATE 44100

static AutoWah autowah;
static SVF svf;


float envelope_follower(float input, float prev_envelope, float alpha) {
    float env = alpha * fabs(input) + (1 - alpha) * prev_envelope;
    return env;
}


void initAutoWah(void){
    autowah.prev_env = 0.0f;
    autowah.base_freq = 100.0f;
    autowah.max_freq = 2000.0f;
    autowah.q = 1.0f;
}

void bandpass_filter(float input, float fc, float Q, float sample_rate) {
    float w = 2.0f * PI * fc / sample_rate;
    float d = 1.0f / Q;
    float g = tanf(w / 2.0f);

    float hp = (input - svf.low) - d * svf.band;
    svf.low += g * svf.band;
    svf.band += g * hp;
    svf.high = hp;
}


void processAutoWah(const Int16 *input, Int16 *output, size_t blockSize){
    size_t i;
    for (i = 0; i < blockSize; i++) {
        autowah.prev_env = envelope_follower(input[i], autowah.prev_env, autowah.alpha);
        float modulated_fc = autowah.base_freq + autowah.prev_env * (autowah.max_freq - autowah.base_freq);
        bandpass_filter(input[i], modulated_fc, autowah.q, SAMPLE_RATE);
        output[i] = svf.band;  // Output bandpass signal
    }
}
