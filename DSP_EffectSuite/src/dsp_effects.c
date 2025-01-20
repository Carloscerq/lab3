#include "dsp_effects.h"
#include "effects/reverbs.h"
#include "effects/phaser.h"
#include "effects/autowah.h"

// Definição dos efeitos
Effect effects[EFFECTS_LENGTH] = {
    {0, "REV-HALL2", initReverbHall2, processReverbHall2},
    {1, "REV-STAGE A", initReverbStageA, processReverbStageA},
    {2, "REV-STAGE Bb", initReverbStageBb, processReverbStageBb},
    {3, "REV-STAGE Db", initReverbStageDb, processReverbStageDb},
    {4, "REV-STAGE Fb", initReverbStageFb, processReverbStageFb},
    {5, "RET-STAGE GTHT", initReverbStageGTHT, processReverbStageGTHT},
    {6, "PHASER", initPhaser, processPhaser},
    {7, "AUTO-WAH", initAutoWah, processAutoWah}
};
