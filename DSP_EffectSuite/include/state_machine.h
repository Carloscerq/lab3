#ifndef INCLUDE_STATE_MACHINE_H_
#define INCLUDE_STATE_MACHINE_H_

#include "tistdtypes.h"

// Enumera��o dos estados poss�veis
typedef enum {
    STATE_WELCOME,         // Estado de boas-vindas
    STATE_SELECT_MODE,     // Sele��o do modo (tempo real ou arquivo)
    STATE_FILE_MODE,
    STATE_SELECT_EFFECT,   // Sele��o do efeito
    STATE_EFFECT_SELECT_RUNNING,  // Efeito em execu��o
    STATE_EFFECT_FILE_RUNNING
} State;

// Inicializa a m�quina de estados
void stateMachineInit(void);

// Executa a l�gica da m�quina de estados
void stateMachineRun(void);

Uint8 getSelectedMode();

#endif /* INCLUDE_STATE_MACHINE_H_ */
