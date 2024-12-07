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

//---------Global constants---------
#define SF48KHz         48000
#define SF24KHz         24000
#define SF16KHz         16000
#define SF12KHz         12000
#define SF8KHz          8000

#define DAC_GAIN        20       // 3dB range: -6dB to 29dB
#define ADC_GAIN        20       // 0dB range: 0dB to 46dB

//---------Global data definition---------
#pragma DATA_SECTION(buffReceive, "dmaMem")
Int16 buffReceive[BUFF_SIZE];

#pragma DATA_SECTION(buffTransmit, "dmaMem")
Int16 buffTransmit[BUFF_SIZE];

//---------Extern definition---------
extern Int16 oled_start();

//---------Function prototypes---------
Int16 dsp_process(Int16 *input, Int16 *output, Int16 size);

//---------main routine---------
void main(void)
{
    /* Initialize */
    EZDSP5502_init( );  // Board Initialization

    Init_AIC3204(SF48KHz, DAC_GAIN, ADC_GAIN);
    configAudioDma(buffReceive, buffTransmit);
    initI2CButtons();

    Int16 i;
    /* Initialize source and destination buffers */
    for (i = 0; i <= (BUFF_SIZE - 1); i++) {
        buffReceive[i] = 0;
        buffTransmit[i] = 0;
    }

    startAudioDma();
    EZDSP5502_MCBSP_init( );  // Configure and start McBSP
    oled_start();
    stateMachineInit();

    while(1){
        stateMachineRun();
        dsp_process(buffReceive, buffTransmit, BUFF_SIZE);
    }
}

Int16 dsp_process(Int16 *input, Int16 *output, Int16 size)
{
    Int16 i;

    for(i=0; i<size; i++)
    {
        *(output + i) = *(input + i);
    }
    return 1;
}
