#ifndef INCLUDE_DMA_H_
#define INCLUDE_DMA_H_

#include "tistdtypes.h"

void configAudioDma(Int16 *buffReceive, Int16 *buffTransmit);
void startAudioDma (void);
void stopAudioDma (void);
void resetSinal();
Uint8 getSinal();

#endif /* INCLUDE_DMA_H_ */
