#include <buttons.h>
#include <stdio.h>
#include <stdlib.h>
#include "ezdsp5502.h"
#include "ezdsp5502_mcbsp.h"

#include "aic3204.h"
#include "dma.h"
#include "define.h"
#include "buttons.h"
#include "state_machine.h"

//---------Global data definition---------
#pragma DATA_SECTION(buffReceive, "dmaMem")
#pragma DATA_ALIGN(buffReceive, 32)
Int16 buffReceive[BUFF_SIZE];

#pragma DATA_SECTION(buffTransmit, "dmaMem")
#pragma DATA_ALIGN(buffTransmit, 32)
Int16 buffTransmit[BUFF_SIZE];

//---------Global constants---------
#define DAC_GAIN        5       // 3dB range: -6dB to 29dB
#define ADC_GAIN        35       // 0dB range: 0dB to 46dB

//---------Extern definition---------
extern volatile int dmaInterruptFlag;
extern Int16 oled_start();

//---------Function prototypes---------
void cleanBuffers(void);
Int16 dsp_process(Int16 *input, Int16 *output, Int16 size);

//---------main routine---------
void main(void){
    /* Initialize */
    EZDSP5502_init();  // Board Initialization

    Init_AIC3204(DAC_GAIN, ADC_GAIN);
    configAudioDma(buffReceive, buffTransmit);
    initI2CButtons();

    cleanBuffers();
    startAudioDma();

    EZDSP5502_MCBSP_init();  // Configure and start McBSP

    oled_start();
    stateMachineInit();

    while(1){
        stateMachineRun();
        if (dmaInterruptFlag) {
            dsp_process(buffReceive, buffTransmit, BUFF_SIZE);
            dmaInterruptFlag = 0;
            enableInterrupt();
        }
    }
}

void cleanBuffers(void){
    Int16 i;

    for (i = 0; i <= (BUFF_SIZE - 1); i++) {
        buffReceive[i] = 0;
        buffTransmit[i] = 0;
    }
}

Int16 dsp_process(Int16 *input, Int16 *output, Int16 size){
    Int16 i;

    for(i=0; i<size; i++)
    {
        *(output + i) = *(input + i);
    }
    return 1;
}
