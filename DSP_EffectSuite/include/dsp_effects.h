#ifndef INCLUDE_DSP_EFFECTS_H_
#define INCLUDE_DSP_EFFECTS_H_

#include "tistdtypes.h"

#define EFFECTS_LENGTH 8

typedef struct {
    int id;
    char file_name[20];
    void (*init)(void);
    Int16 (*process)(Int16 sample);
} Effect;

extern Effect effects[EFFECTS_LENGTH];

#endif /* INCLUDE_DSP_EFFECTS_H_ */
