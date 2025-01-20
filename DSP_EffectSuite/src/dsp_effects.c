#include "dsp_effects.h"
#include "effects/reverbs.h"
#include "effects/phaser.h"
#include "effects/autowah.h"

// Definição dos efeitos
Effect effects[EFFECTS_LENGTH] = {
    {0, "REV-HALL2", initReverbHall2, processReverbBlock},
    {1, "REV-STAGE A", initReverbStageA, processReverbBlock},
    {2, "REV-STAGE Bb", initReverbStageBb, processReverbBlock},
    {3, "REV-STAGE Db", initReverbStageDb, processReverbBlock},
    {4, "REV-STAGE Fb", initReverbStageFb, processReverbBlock},
    {5, "RET-STAGE GTHT", initReverbStageGTHT, processReverbBlock},
    {6, "PHASER", initPhaser, processPhaser},
    {7, "AUTO-WAH", initAutoWah, processAutoWah}
};
