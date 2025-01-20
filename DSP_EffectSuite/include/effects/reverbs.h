#ifndef INCLUDE_EFFECTS_REVERBS_H_
#define INCLUDE_EFFECTS_REVERBS_H_

#include "tistdtypes.h"

// Estrutura para armazenar os parâmetros do reverb
typedef struct {
    float decay;       // Decaimento do reverb
    float preDelay;    // Pré-delay
    float damping;     // Fator de amortecimento
    float roomSize;    // Tamanho da sala
} ReverbParams;

void initReverbHall2(void);
Int16 processReverbHall2(Int16 sample);

void initReverbStageA(void);
Int16 processReverbStageA(Int16 sample);

void initReverbStageBb(void);
Int16 processReverbStageBb(Int16 sample);

void initReverbStageDb(void);
Int16 processReverbStageDb(Int16 sample);

void initReverbStageFb(void);
Int16 processReverbStageFb(Int16 sample);

void initReverbStageGTHT(void);
Int16 processReverbStageGTHT(Int16 sample);

#endif /* INCLUDE_EFFECTS_REVERBS_H_ */
