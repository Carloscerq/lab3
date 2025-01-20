#include <stdio.h>
#include "state_machine.h"
#include "dsp_effects.h"
#include "lcd.h"
#include "buttons.h"

//---------Global data definition---------

#define REALTIME 0
#define FILE 1

State currentState = STATE_WELCOME;
Uint8 selectedEffectIndex = 0;
Uint8 selectedMode = REALTIME; // 0 = Tempo Real, 1 = Arquivo

char message[10];

//---------Extern definition---------
extern void oled_writeText(const char* text, Uint8 page);
extern void oled_fillPage(Uint8 page, Uint8 value);

// Fun��o interna para mudar de estado
static void changeState(State newState) {
    currentState = newState;

    // A��es associadas � transi��o de estado
    switch (newState) {
        case STATE_WELCOME:
            oled_writeText("WELCOME!", 0);
            oled_writeText("           2:START", 1);
            break;

        case STATE_SELECT_MODE:
            if (!selectedMode){
                oled_writeText("REAL TIME", 0);
            } else {
                oled_writeText("FILE", 0);
            }
            oled_writeText("1:NEXT     2:SELECT", 1);
            break;

        case STATE_FILE_MODE:
            if (selectedEffectIndex < EFFECTS_LENGTH) {// number of DSP effects + 1
                sprintf(message, "%s", effects[selectedEffectIndex].file_name);
                oled_writeText(message, 0);
            } else if (selectedEffectIndex == EFFECTS_LENGTH) {
                oled_writeText("ALL", 0);
            } else {
                oled_writeText("-- PREVIOUS MENU", 0);
            }
            oled_writeText("1:NEXT     2:SELECT", 1);
            break;

        case STATE_SELECT_EFFECT:
            if (selectedEffectIndex < EFFECTS_LENGTH) {// number of DSP effects + 1
                sprintf(message, "%s", effects[selectedEffectIndex].file_name);
                oled_writeText(message, 0);
            } else {
                oled_writeText("-- PREVIOUS MENU", 0);
            }
            oled_writeText("1:NEXT     2:SELECT", 1);
            break;

        case STATE_EFFECT_SELECT_RUNNING:
            sprintf(message, "%s", effects[selectedEffectIndex].file_name);
            oled_writeText(message, 0);
            oled_writeText("RUNNING..!   2:BACK", 1);
            break;

        case STATE_EFFECT_FILE_RUNNING:
            sprintf(message, "%s", effects[selectedEffectIndex].file_name);
            oled_writeText(message, 0);
            oled_writeText("APPLYING.. 2:BACK", 1);
            break;

        default:
            break;
    }
}

// Inicializa a m�quina de estados
void stateMachineInit(void) {
    currentState = STATE_WELCOME;
    selectedEffectIndex = 0;
    selectedMode = 0;
    oled_fillPage(0x00, 0);
    oled_fillPage(0x00, 1);
    changeState(STATE_WELCOME);
}

// L�gica principal da m�quina de estados
void stateMachineRun(void) {
    checkButtons();

    switch (currentState) {
        case STATE_WELCOME:
            if (getButtonState(BUTTON_SELECT)) {
                changeState(STATE_SELECT_MODE);
            }
            break;

        case STATE_SELECT_MODE:
            if (getButtonState(BUTTON_NEXT)) {
                selectedMode = !selectedMode;
                changeState(STATE_SELECT_MODE);
            } else if (getButtonState(BUTTON_SELECT) && selectedMode == REALTIME) {
                changeState(STATE_SELECT_EFFECT);
            } else if (getButtonState(BUTTON_SELECT) && selectedMode == FILE) {
                changeState(STATE_FILE_MODE);
            }
            break;

        case STATE_FILE_MODE:
            if (getButtonState(BUTTON_NEXT)) {
                selectedEffectIndex = (selectedEffectIndex + 1) % (EFFECTS_LENGTH + 2); // number of DSP effects + 1
                changeState(STATE_FILE_MODE);
            } else if (getButtonState(BUTTON_SELECT) && selectedEffectIndex < EFFECTS_LENGTH) {
                changeState(STATE_EFFECT_FILE_RUNNING);
            } else if (getButtonState(BUTTON_SELECT) && selectedEffectIndex == EFFECTS_LENGTH){
                changeState(STATE_EFFECT_FILE_RUNNING); // ALL CASE
            } else if (getButtonState(BUTTON_SELECT) && selectedEffectIndex == EFFECTS_LENGTH + 1){
                changeState(STATE_SELECT_MODE);
            }
            break;

        case STATE_SELECT_EFFECT:
            if (getButtonState(BUTTON_NEXT)) {
                selectedEffectIndex = (selectedEffectIndex + 1) % (EFFECTS_LENGTH + 1); // number of DSP effects + 1
                changeState(STATE_SELECT_EFFECT);
            } else if (getButtonState(BUTTON_SELECT) && selectedEffectIndex < EFFECTS_LENGTH) {
                changeState(STATE_EFFECT_SELECT_RUNNING);
            } else if (getButtonState(BUTTON_SELECT) && selectedEffectIndex == EFFECTS_LENGTH){
                changeState(STATE_SELECT_MODE);
            }
            break;

        case STATE_EFFECT_SELECT_RUNNING:
            if (getButtonState(BUTTON_SELECT)) {
                changeState(STATE_SELECT_EFFECT);
            }
            break;
            
        case STATE_EFFECT_FILE_RUNNING:
            if (getButtonState(BUTTON_SELECT)) {
                changeState(STATE_FILE_MODE);
            }
            break;

        default:
            break;
    }
}
