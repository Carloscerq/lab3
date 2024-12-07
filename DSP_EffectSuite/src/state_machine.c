#include <stdio.h>
#include "state_machine.h"
#include "lcd.h"
#include "buttons.h"

//---------Global data definition---------

#define REALTIME 0
#define ARCHIVE 1

static State currentState = STATE_WELCOME;
static Uint8 selectedEffectIndex = 0;
static Uint8 selectedMode = REALTIME; // 0 = Tempo Real, 1 = Arquivo

char message[10];

//---------Extern definition---------
extern void oled_writeText(const char* text, Uint8 page);
extern void oled_fillPage(Uint8 page, Uint8 value);

// Função interna para mudar de estado
static void changeState(State newState) {
    currentState = newState;

    // Ações associadas à transição de estado
    switch (newState) {
        case STATE_WELCOME:
            oled_writeText("WELCOME!", 0);
            oled_writeText("           2:START", 1);
            break;

        case STATE_SELECT_MODE:
            if (!selectedMode){
                oled_writeText("REAL TIME", 0);
            } else {
                oled_writeText("ARCHIVE", 0);
            }
            oled_writeText("1:NEXT     2:SELECT", 1);
            break;

        case STATE_ARCHIVE_MODE:
            oled_writeText("EFFECTS APPLIED!", 0);
            oled_writeText("           2:BACK", 1);
            break;

        case STATE_SELECT_EFFECT:
            if (selectedEffectIndex < 8) {// number of DSP effects + 1
                sprintf(message, "EFFECT: %u", selectedEffectIndex);
                oled_writeText(message, 0);
            } else {
                oled_writeText("-- PREVIOUS MENU", 0);
            }
            oled_writeText("1:NEXT     2:SELECT", 1);
            break;

        case STATE_EFFECT_RUNNING:
            sprintf(message, "EFFECT: %u", selectedEffectIndex);
            oled_writeText(message, 0);
            oled_writeText("RUNNING..!   2:BACK", 1);
            break;

        default:
            break;
    }
}

// Inicializa a máquina de estados
void stateMachineInit(void) {
    currentState = STATE_WELCOME;
    selectedEffectIndex = 0;
    selectedMode = 0;
    oled_fillPage(0x00, 0);
    oled_fillPage(0x00, 1);
    changeState(STATE_WELCOME);
}

// Lógica principal da máquina de estados
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
            } else if (getButtonState(BUTTON_SELECT) && selectedMode == ARCHIVE) {
                changeState(STATE_ARCHIVE_MODE);
            }
            break;

        case STATE_ARCHIVE_MODE:
            if (getButtonState(BUTTON_SELECT)) {
                changeState(STATE_SELECT_MODE);
            }
            break;

        case STATE_SELECT_EFFECT:
            if (getButtonState(BUTTON_NEXT)) {
                selectedEffectIndex = (selectedEffectIndex + 1) % 9; // number of DSP effects + 1
                changeState(STATE_SELECT_EFFECT);
            } else if (getButtonState(BUTTON_SELECT) && selectedEffectIndex < 8) {
                changeState(STATE_EFFECT_RUNNING);
            } else if (getButtonState(BUTTON_SELECT) && selectedEffectIndex == 8){
                changeState(STATE_SELECT_MODE);
            }
            break;

        case STATE_EFFECT_RUNNING:
            if (getButtonState(BUTTON_SELECT)) {
                changeState(STATE_SELECT_EFFECT);
            }
            break;

        default:
            break;
    }
}
