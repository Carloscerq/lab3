#include "dsp_effects.h"
#include "effects/reverbs.h"
#include "effects/phaser.h"
#include "effects/autowah.h"

// Definição dos efeitos
Effect effects[EFFECTS_LENGTH] = {
    {0, "REV-HALL2", initReverbHall2, processReverbBlock},
    {1, "PHASER", initPhaser, processPhaser},
    {2, "AUTO-WAH", initAutoWah, processAutoWah}
};
