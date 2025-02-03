#ifndef INCLUDE_DSP_EFFECTS_H_
#define INCLUDE_DSP_EFFECTS_H_

#include <stdlib.h>
#include "tistdtypes.h"

#define EFFECTS_LENGTH 8

typedef struct {
    Int8 id;
    char file_name[20];
    void (*init)(void);
    void (*process)(const Int16 *input, Int16 *output, size_t blockSize);
} Effect;

extern Effect effects[EFFECTS_LENGTH];

#endif /* INCLUDE_DSP_EFFECTS_H_ */
