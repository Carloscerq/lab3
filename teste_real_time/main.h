#include <math.h>
#include "ezdsp5502.h"
#include "ezdsp5502_aic23.h"
#include "csl_dma.h"

#define BUFFER_SIZE 256
#define SAMPLING_RATE 44100 // 44.1 kHz
#define MOD_FREQ 5.0        // Tremolo frequency: 5 Hz
#define DEPTH 0.5           // Tremolo depth: 50%

#pragma DATA_ALIGN(inputBuffer, 2);  // Align buffers for DMA
#pragma DATA_ALIGN(outputBuffer, 2);

Int16 inputBuffer[BUFFER_SIZE];
Int16 outputBuffer[BUFFER_SIZE];

float phase = 0.0f;
const float twoPi = 2.0f * 3.14159265359f;

EZDSP5502_AIC23_Config config = {
    0x0017, // Left line input volume
    0x0017, // Right line input volume
    0x01f9, // Left headphone volume
    0x01f9, // Right headphone volume
    0x0011, // Analog audio path control
    0x0000, // Digital audio path control
    0x0001, // Power down control
    0x0043, // Digital audio interface format
    0x0081, // Sample rate control
    0x0001  // Digital interface activation
};

