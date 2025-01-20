#include "ezdsp5502.h"
#include "ezdsp5502_mcbsp.h"

#include "main.h"

void codec_init() {
    EZDSP5502_init();
    EZDSP5502_AIC23_init(&config);
}


void dma_init() {
    // Initialize DMA controller
    CSL_DMA_Handle hDmaRx, hDmaTx;

    // Configure DMA for receiving (ADC -> inputBuffer)
    hDmaRx = DMA_open(DMA_CHA0, NULL);
    DMA_configArgs(hDmaRx,
                   CSL_DMA_ADDR_INC,         // Source address increment
                   CSL_DMA_ADDR_CONST,       // Destination address constant
                   CSL_DMA_DATASIZE_16BIT,   // 16-bit data
                   CSL_DMA_TRIGGER_EE,       // Triggered by event
                   BUFFER_SIZE);

    // Configure DMA for transmitting (outputBuffer -> DAC)
    hDmaTx = DMA_open(DMA_CHA1, NULL);
    DMA_configArgs(hDmaTx,
                   CSL_DMA_ADDR_CONST,       // Source address constant
                   CSL_DMA_ADDR_INC,         // Destination address increment
                   CSL_DMA_DATASIZE_16BIT,   // 16-bit data
                   CSL_DMA_TRIGGER_EE,       // Triggered by event
                   BUFFER_SIZE);
}


interrupt void dma_isr() {
    static int currentPhaseIndex = 0;

    // Apply tremolo effect
    for (int i = 0; i < BUFFER_SIZE; i++) {
        outputBuffer[i] = (Int16)(inputBuffer[i]);
    }
}

void enable_interrupts() {
    DMA_enableInterrupt(DMA_CHA0); // Enable RX interrupt
    DMA_enableInterrupt(DMA_CHA1); // Enable TX interrupt
    IRQ_enable(DMA_EVENT);         // Enable global DMA interrupt
}

void start_dma() {
    // Start ADC -> inputBuffer transfer
    DMA_start(DMA_CHA0, EZDSP5502_AIC23_rget16());

    // Start outputBuffer -> DAC transfer
    DMA_start(DMA_CHA1, EZDSP5502_AIC23_wset16());
}

/**
 * main.c
 */
void main() {
    codec_init();     // Initialize codec
    dma_init();       // Initialize DMA
    enable_interrupts(); // Enable interrupts

    start_dma();      // Start audio transfer

    while (1) {
        // Main loop can remain empty as processing is handled by DMA and ISR
    }
}

