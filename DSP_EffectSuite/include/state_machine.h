#ifndef INCLUDE_STATE_MACHINE_H_
#define INCLUDE_STATE_MACHINE_H_


// Enumera��o dos estados poss�veis
typedef enum {
    STATE_WELCOME,         // Estado de boas-vindas
    STATE_SELECT_MODE,     // Sele��o do modo (tempo real ou arquivo)
    STATE_ARCHIVE_MODE,
    STATE_SELECT_EFFECT,   // Sele��o do efeito
    STATE_EFFECT_RUNNING   // Efeito em execu��o
} State;

// Inicializa a m�quina de estados
void stateMachineInit(void);

// Executa a l�gica da m�quina de estados
void stateMachineRun(void);


#endif /* INCLUDE_STATE_MACHINE_H_ */
