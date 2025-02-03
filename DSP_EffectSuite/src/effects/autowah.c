#include "effects/autowah.h"
#include <math.h>
#include <stdlib.h>

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

#define PI 3.141592653589793f
#define SAMPLE_RATE 44100

static AutoWah autowah;
static SVF svf;

// -----------------------------------------------------------------------------
// Envelope Follower
//
// This function smooths the absolute value of the normalized input
// sample. The input is assumed to be in the range [-1, 1].
// -----------------------------------------------------------------------------
float envelope_follower(float input, float prev_envelope, float alpha) {
    return alpha * fabsf(input) + (1.0f - alpha) * prev_envelope;
}

// -----------------------------------------------------------------------------
// Initialization Function
//
// This function sets up the autowah parameters and clears the filter state.
// -----------------------------------------------------------------------------
void initAutoWah(void) {
    autowah.prev_env = 0.0f;
    autowah.base_freq = 100.0f;   // Base cutoff frequency (Hz)
    autowah.max_freq  = 3500.0f;  // Maximum cutoff frequency (Hz)
    autowah.q         = 0.5f;     // Resonance factor (try tweaking this)
    autowah.alpha     = 0.03f;    // Envelope smoothing factor (experiment between 0.01-0.05)

    svf.low = 0.0f;
    svf.band = 0.0f;
}

// -----------------------------------------------------------------------------
// Bandpass Filter (State Variable Filter - SVF)
//
// This version of the SVF uses a sine-based coefficient calculation.
// It calculates a highpass component and updates the bandpass and lowpass states.
// -----------------------------------------------------------------------------
void bandpass_filter(float input, float fc, float Q, float sample_rate) {
    // Calculate the filter coefficient
    float f = 2.0f * sinf(PI * fc / sample_rate);

    // Compute the highpass output
    float hp = input - svf.low - Q * svf.band;

    // Update the filter states
    svf.band += f * hp;
    svf.low  += f * svf.band;
}

// -----------------------------------------------------------------------------
// Process Function
//
// This function applies the autowah effect on a block of samples.
// - `input`  : Pointer to the input sample block (Int16).
// - `output` : Pointer to the output sample block (Int16).
// - `blockSize` : Number of samples in the block.
// -----------------------------------------------------------------------------
void processAutoWah(const Int16 *input, Int16 *output, size_t blockSize) {
    size_t i;
    for (i = 0; i < blockSize; i++) {
        // Normalize the input sample to [-1, 1]
        float in = input[i] / 32768.0f;

        // Update the envelope follower
        autowah.prev_env = envelope_follower(in, autowah.prev_env, autowah.alpha);

        // Calculate the modulated cutoff frequency
        // The envelope value (prev_env) is used to sweep the cutoff between base_freq and max_freq.
        float modulated_fc = autowah.base_freq + autowah.prev_env * (autowah.max_freq - autowah.base_freq);

        // Process the input sample through the bandpass filter
        bandpass_filter(in, modulated_fc, autowah.q, SAMPLE_RATE);

        // Scale the filter output (bandpass output) back to 16-bit integer range.
        float temp = svf.band * 32767.0f;

        // Manual clipping in case of overflow
        if (temp > 32767.0f)
            temp = 32767.0f;
        if (temp < -32768.0f)
            temp = -32768.0f;

        output[i] = (Int16) temp;
    }
}
