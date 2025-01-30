#include "effects/phaser.h"

#define SAMPLE_RATE 44100
#define MAX_DELAY 44100  // Buffer máximo para 1 segundo de atraso

typedef struct {
    float delayBuffer[MAX_DELAY];
    size_t writeIndex;
    float feedback;
    float rate;
} AllpassFilter;

typedef struct {
    int numStages;
    float rate;
    float depth;
    float feedback;
    float feedbackBuffer;
    AllpassFilter *allpassFilters;
} Phaser;

static Phaser phaser;

// Função auxiliar: inicializa um filtro all-pass
static void initAllpassFilter(AllpassFilter *filter, float rate, float feedback) {
    size_t i;
    for (i = 0; i < MAX_DELAY; i++) {
        filter->delayBuffer[i] = 0.0f;
    }
    filter->writeIndex = 0;
    filter->feedback = feedback;
    filter->rate = rate;
}

// Função auxiliar: processa uma amostra com um filtro all-pass
static float processAllpassFilter(AllpassFilter *filter, float input) {
    float delayedSample = filter->delayBuffer[filter->writeIndex];
    float output = -input + delayedSample;
    filter->delayBuffer[filter->writeIndex] = input + output * filter->feedback;

    filter->writeIndex = (filter->writeIndex + 1) % MAX_DELAY;

    return output;
}

// Inicializa o Phaser
void initPhaser(void) {
    phaser.numStages = 4;  // Número de estágios padrão
    phaser.rate = 0.5f;    // Taxa de modulação padrão (Hz)
    phaser.depth = 0.7f;   // Profundidade padrão
    phaser.feedback = 0.6f; // Realimentação padrão
    phaser.feedbackBuffer = 0.0f;

    phaser.allpassFilters = malloc(phaser.numStages * sizeof(AllpassFilter));
    int32_t i;
    for (i = 0; i < phaser.numStages; i++) {
        initAllpassFilter(&phaser.allpassFilters[i], phaser.rate, phaser.feedback);
    }
}

// Processa um bloco de amostras com o Phaser
void processPhaser(const int16_t *input, int16_t *output, size_t blockSize) {
    size_t i;
    int32_t j;
    for (i = 0; i < blockSize; i++) {
        float sample = (float)input[i] / 32768.0f;  // Normaliza para -1 a 1
        float processedSample = sample;

        for (j = 0; j < phaser.numStages; j++) {
            processedSample = processAllpassFilter(&phaser.allpassFilters[j], processedSample);
        }

        // Realimentação
        phaser.feedbackBuffer = processedSample * phaser.feedback + sample * (1 - phaser.feedback);

        // Converte de volta para 16 bits
        output[i] = (int16_t)(processedSample * 32768.0f);
    }
}