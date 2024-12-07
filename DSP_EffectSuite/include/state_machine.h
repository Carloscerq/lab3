#ifndef INCLUDE_STATE_MACHINE_H_
#define INCLUDE_STATE_MACHINE_H_


// Enumeração dos estados possíveis
typedef enum {
    STATE_WELCOME,         // Estado de boas-vindas
    STATE_SELECT_MODE,     // Seleção do modo (tempo real ou arquivo)
    STATE_ARCHIVE_MODE,
    STATE_SELECT_EFFECT,   // Seleção do efeito
    STATE_EFFECT_RUNNING   // Efeito em execução
} State;

// Inicializa a máquina de estados
void stateMachineInit(void);

// Executa a lógica da máquina de estados
void stateMachineRun(void);


#endif /* INCLUDE_STATE_MACHINE_H_ */
