
#ifndef INCLUDE_DSP_EFFECTS_H_
#define INCLUDE_DSP_EFFECTS_H_

#define EFFECTS_LENGTH 8

typedef struct {
    int id;
    char file_name[20];
} Effect;

Effect effects[EFFECTS_LENGTH] = {
    {0, "REV-HALL2"},
    {1, "REV-STAGE A"},
    {2, "REV-STAGE Bb"}, // TODO: Bb
    {3, "REV-STAGE Db"}, // TODO: Db
    {4, "REV-STAGE Fb"}, // TODO: Fb
    {5, "RET-STAGE GTHT"},
    {6, "PHASER"},
    {7, "AUTO-WAH"}
};


#endif /* INCLUDE_DSP_EFFECTS_H_ */
