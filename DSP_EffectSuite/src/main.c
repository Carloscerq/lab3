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
extern Int16 oled_start();

//---------Function prototypes---------
void cleanBuffers(void);
void processRealtime(Int16 *input, Int16 *output, Int16 size);
void processFile(const char *inputFile, const char *outputFile, Int16 effectIndex);

//---------main routine---------
void main(void){
    /* Initialize */
    EZDSP5502_init();  // Board Initialization

    Init_AIC3204(DAC_GAIN, ADC_GAIN);
    configAudioDma(buffReceive, buffTransmit);
    initI2CButtons();
    cleanBuffers();
    oled_start();
    stateMachineInit();

    while(1){
        stateMachineRun();

        if (getSelectedMode() == REALTIME_MODE && getCurrentState() == STATE_EFFECT_SELECT_RUNNING) {
            startAudioDma();
            EZDSP5502_MCBSP_init();  // Configure and start McBSP

            while (getCurrentState() == STATE_EFFECT_SELECT_RUNNING) {
                stateMachineRun();
                if (dmaInterruptFlag) {
                    processRealtime(buffReceive, buffTransmit, BUFF_SIZE);
                    dmaInterruptFlag = 0;
                    enableInterrupt();
                }
            }

            stopAudioDma();
            EZDSP5502_MCBSP_close();
        }

        if (getSelectedMode() == FILE_MODE && getCurrentState() == STATE_EFFECT_FILE_RUNNING) {
            const char *inputFile = "../data/original.wav";
            char outputFile[50];

            const char *effectFileName = effects[getSelectedEffectIndex()].file_name;

            snprintf(outputFile, sizeof(outputFile), "../data/%s_output.wav", effectFileName);

            processFile(inputFile, outputFile, getSelectedEffectIndex());

            // Notificar a FSM de que a operação foi concluída
            stateMachineNotifyFileComplete();
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
    effects[getSelectedEffectIndex()].process(input, output, size);
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

void processFile(const char *inputFile, const char *outputFile, Int16 effectIndex) {
    FILE *fpIn, *fpOut;
    Int16 inputBuffer[BUFF_SIZE];
    Int16 outputBuffer[BUFF_SIZE];
    Uint8 temp[2 * BUFF_SIZE];
    Uint8 waveHeader[44];
    Int32 cnt = 0;

    // Imprimir nomes dos arquivos e índice do efeito
    printf("Input File: %s\n", inputFile);
    printf("Output File: %s\n", outputFile);

    // Abrir arquivos
    fpIn = fopen(inputFile, "rb");
    fpOut = fopen(outputFile, "wb");

    if (!fpIn || !fpOut) {
        printf("Erro ao abrir arquivos!\n");
        if (fpIn) fclose(fpIn);
        if (fpOut) fclose(fpOut);
        return;
    }

    // Ler e escrever cabeçalho do arquivo de entrada
    fread(waveHeader, sizeof(Uint8), 44, fpIn);
    fwrite(waveHeader, sizeof(Uint8), 44, fpOut);

    // Processar dados de áudio em blocos
    Uint16 i, j;
    while (fread(temp, sizeof(Uint8), 2 * BUFF_SIZE, fpIn) == 2 * BUFF_SIZE) {
        // Combinar bytes para formar Int16 no buffer de entrada
        for ( i = 0, j = 0; i < BUFF_SIZE; i++) {
            inputBuffer[i] = temp[j++] | (temp[j++] << 8);
        }

        // Aplicar o efeito ao bloco de amostras
        effects[effectIndex].process(inputBuffer, outputBuffer, BUFF_SIZE);

        // Dividir Int16 em bytes para o buffer de saída
        for (i = 0, j = 0; i < BUFF_SIZE; i++) {
            temp[j++] = outputBuffer[i] & 0xFF;
            temp[j++] = (outputBuffer[i] >> 8) & 0xFF;
        }

        // Escrever bloco processado no arquivo de saída
        fwrite(temp, sizeof(Uint8), 2 * BUFF_SIZE, fpOut);
        cnt += BUFF_SIZE;
        printf("%ld data samples processed\n", cnt);
    }

    // Atualizar cabeçalho WAV com informações finais
    wHeader(waveHeader, 8000, 8000, cnt * 2);  // Atualizar com o tamanho do arquivo processado
    rewind(fpOut);  // Voltar ao início do arquivo de saída
    fwrite(waveHeader, sizeof(Uint8), 44, fpOut);  // Sobrescrever o cabeçalho

    fclose(fpIn);
    fclose(fpOut);

    printf("Processamento de arquivo concluído. %u amostras processadas.\n", cnt);
}

