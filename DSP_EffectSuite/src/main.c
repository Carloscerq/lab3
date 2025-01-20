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
#include "dsp_effects.h"

//---------Global data definition---------
#pragma DATA_SECTION(buffReceive, "dmaMem")
#pragma DATA_ALIGN(buffReceive, 32)
Int16 buffReceive[BUFF_SIZE];

#pragma DATA_SECTION(buffTransmit, "dmaMem")
#pragma DATA_ALIGN(buffTransmit, 32)
Int16 buffTransmit[BUFF_SIZE];

//---------Global constants---------
#define DAC_GAIN        5       // 3dB range: -6dB to 29dB
#define ADC_GAIN        35      // 0dB range: 0dB to 46dB

//---------Extern definition---------
extern volatile int dmaInterruptFlag;
extern Uint8 selectedEffectIndex;
extern Int16 oled_start();

//---------Function prototypes---------
void cleanBuffers(void);
void processRealtime(Int16 *input, Int16 *output, Int16 size);
void processFile(const char *inputFile, const char *outputFile, int effectIndex);

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
            //dsp_process(buffReceive, buffTransmit, BUFF_SIZE);
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

void processRealtime(Int16 *input, Int16 *output, Int16 size) {
    Int16 i;
    for (i = 0; i < size; i++) {
        output[i] = effects[selectedEffectIndex].process(input[i]);
    }
}

// Create the wave file header
void wHeader(Uint8 *w, float f1, float f2, Uint32 bytes)
{
    Int32 t;

    t = bytes;
    w[40] = (t>>0)&0xff;    // Set up output file size
    w[41] = (t>>8)&0xff;
    w[42] = (t>>16)&0xff;
    w[43] = (t>>24)&0xff;
    t += 36;
    w[4] = (t>>0)&0xff;
    w[5] = (t>>8)&0xff;
    w[6] = (t>>16)&0xff;
    w[7] = (t>>24)&0xff;
    t = w[24]|(w[25]<<8)|((Int32)w[26]<<16)|((Int32)w[27]<<24);
    t = (Int32)((float)t*f2/f1);
    w[24] = (t>>0)&0xff;    // Set up output file frequency
    w[25] = (t>>8)&0xff;
    w[26] = (t>>16)&0xff;
    w[27] = (t>>24)&0xff;
    t = w[28]|(w[29]<<8)|((Int32)w[30]<<16)|((Int32)w[31]<<24);
    t = (Int32)((float)t*f2/f1);
    w[28] = (t>>0)&0xff;    // Set up output file byte rate
    w[29] = (t>>8)&0xff;
    w[30] = (t>>16)&0xff;
    w[31] = (t>>24)&0xff;
    return;
}

void processFile(const char *inputFile, const char *outputFile, int effectIndex) {
    FILE *fpIn, *fpOut;
    Int16 buffer[BUFF_SIZE];
    Uint8 temp[2 * BUFF_SIZE];
    Uint8 waveHeader[44];
    Uint32 cnt = 0;

    // Abrir arquivos
    fpIn = fopen(inputFile, "rb");
    fpOut = fopen(outputFile, "wb");

    if (!fpIn || !fpOut) {
        printf("Erro ao abrir arquivos!\n");
        if (fpIn) fclose(fpIn);
        if (fpOut) fclose(fpOut);
        return;
    }

    Int16 i, j;

    // Ler e escrever cabeçalho do arquivo de entrada
    fread(waveHeader, sizeof(Uint8), 44, fpIn);
    fwrite(waveHeader, sizeof(Uint8), 44, fpOut);

    // Processar dados de áudio
    while (fread(temp, sizeof(Uint8), 2 * BUFF_SIZE, fpIn) == 2 * BUFF_SIZE) {
        for (i = 0, j = 0; i < BUFF_SIZE; i++) {
            buffer[i] = temp[j++] | (temp[j++] << 8);  // Combinar bytes para formar Int16
            buffer[i] = effects[selectedEffectIndex].process(buffer[i]);  // Aplicar o efeito
        }
        for (i = 0, j = 0; i < BUFF_SIZE; i++) {
            temp[j++] = buffer[i] & 0xFF;  // Byte menos significativo
            temp[j++] = (buffer[i] >> 8) & 0xFF;  // Byte mais significativo
        }
        fwrite(temp, sizeof(Uint8), 2 * BUFF_SIZE, fpOut);
        cnt += BUFF_SIZE;
    }

    // Atualizar cabeçalho WAV com informações finais
    wHeader(waveHeader, 8000, 8000, cnt * 2);  // Atualizar com o tamanho do arquivo processado
    rewind(fpOut);  // Voltar ao início do arquivo de saída
    fwrite(waveHeader, sizeof(Uint8), 44, fpOut);  // Sobrescrever o cabeçalho

    fclose(fpIn);
    fclose(fpOut);

    printf("Processamento de arquivo concluído. %u amostras processadas.\n", cnt);
}

