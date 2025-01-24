#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "dsp_effects.h"

//---------Global data definition---------
#define BUFF_SIZE 512
#define INDEX_EFFECT 0

int16_t buffReceive[BUFF_SIZE];
int16_t buffTransmit[BUFF_SIZE];

void processFile(const char *inputFile, const char *outputFile, int16_t effectIndex);

//---------main routine---------
int main(void){
    /* Initialize */
    const char *inputFile = "data/original.wav";
    char outputFile[50];

    const char *effectFileName = effects[INDEX_EFFECT].file_name;

    snprintf(outputFile, sizeof(outputFile), "data/%s_output.wav", effectFileName);

    effects[INDEX_EFFECT].init();
    processFile(inputFile, outputFile, INDEX_EFFECT);

    return 0;
}

// Create the wave file header
void wHeader(uint8_t *w, float f1, float f2, uint32_t bytes)
{
    int32_t t;

    t = bytes;
    w[40] = (t>>0)&0xff;	// Set up output file size
    w[41] = (t>>8)&0xff;
    w[42] = (t>>16)&0xff;
    w[43] = (t>>24)&0xff;
    t += 36;
    w[4] = (t>>0)&0xff;
    w[5] = (t>>8)&0xff;
    w[6] = (t>>16)&0xff;
    w[7] = (t>>24)&0xff;
    t = w[24]|(w[25]<<8)|((int32_t)w[26]<<16)|((int32_t)w[27]<<24);
    t = (int32_t)((float)t*f2/f1);
    w[24] = (t>>0)&0xff;	// Set up output file frequency
    w[25] = (t>>8)&0xff;
    w[26] = (t>>16)&0xff;
    w[27] = (t>>24)&0xff;
    t = w[28]|(w[29]<<8)|((int32_t)w[30]<<16)|((int32_t)w[31]<<24);
    t = (int32_t)((float)t*f2/f1);
    w[28] = (t>>0)&0xff;	// Set up output file byte rate
    w[29] = (t>>8)&0xff;
    w[30] = (t>>16)&0xff;
    w[31] = (t>>24)&0xff;
    return;
}

void processFile(const char *inputFile, const char *outputFile, int16_t effectIndex) {
    FILE *fpIn, *fpOut;
    int16_t inputBuffer[BUFF_SIZE];
    int16_t outputBuffer[BUFF_SIZE];
    uint8_t temp[2 * BUFF_SIZE];
    uint8_t temp2[2 * BUFF_SIZE];
    uint8_t waveHeader[44];
    int32_t cnt = 0;

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
    fread(waveHeader, sizeof(uint8_t), 44, fpIn);
    fwrite(waveHeader, sizeof(uint8_t), 44, fpOut);

    // Processar dados de áudio em blocos
    uint16_t i, j;
    while (fread(&temp, sizeof(uint8_t), 2 * BUFF_SIZE, fpIn) == 2 * BUFF_SIZE) {
        // Combinar bytes para formar Int16 no buffer de entrada
        for ( i = 0, j = 0; i < BUFF_SIZE; i++) {
            inputBuffer[i] = (temp[j]&0xFF)|(temp[j+1]<<8);;
            j += 2;
        }

        // Aplicar o efeito ao bloco de amostras
        effects[effectIndex].process(inputBuffer, outputBuffer, BUFF_SIZE);

        // Dividir Int16 em bytes para o buffer de saída
        for (i = 0, j = 0; i < BUFF_SIZE; i++) {
            temp2[j++] = outputBuffer[i] & 0xFF;
            temp2[j++] = (outputBuffer[i] >> 8) & 0xFF;
        }

        // Escrever bloco processado no arquivo de saída
        fwrite(temp2, sizeof(uint8_t), 2 * BUFF_SIZE, fpOut);
        cnt += BUFF_SIZE;
        //printf("%d data samples processed\n", cnt);
    }

    // Atualizar cabeçalho WAV com informações finais
    wHeader(waveHeader, 8000, 8000, cnt<<1);  // Atualizar com o tamanho do arquivo processado
    rewind(fpOut);  // Voltar ao início do arquivo de saída
    fwrite(waveHeader, sizeof(int8_t), 44, fpOut);  // Sobrescrever o cabeçalho

    fclose(fpIn);
    fclose(fpOut);

    printf("Processamento de arquivo concluído. %u amostras processadas.\n", cnt);
}
