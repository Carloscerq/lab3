#ifndef INCLUDE_DSP_EFFECTS_H_
#define INCLUDE_DSP_EFFECTS_H_

#include <stdlib.h>

#define EFFECTS_LENGTH 3

typedef struct {
    int8_t id;
    char file_name[20];
    void (*init)(void);
    void (*process)(const int16_t *input, int16_t *output, size_t blockSize);
} Effect;

extern Effect effects[EFFECTS_LENGTH];

#endif /* INCLUDE_DSP_EFFECTS_H_ */
