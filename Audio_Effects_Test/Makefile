# Nome do executável
TARGET = audio_test

# Diretórios
SRC_DIR = src
INC_DIR = include
EFFECTS_SRC_DIR = $(SRC_DIR)/effects
EFFECTS_INC_DIR = $(INC_DIR)/effects
OBJ_DIR = build

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -I$(INC_DIR) -I$(EFFECTS_INC_DIR) -g
LDFLAGS = -lm

# Arquivos fontes e objetos
SRCS = $(wildcard $(SRC_DIR)/*.c $(EFFECTS_SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Regras
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compilar arquivos .c para .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/effects/%.o: $(EFFECTS_SRC_DIR)/%.c | $(OBJ_DIR)/effects
	$(CC) $(CFLAGS) -c $< -o $@

# Criar diretório de objetos
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Regra para criar o diretório build/effects
$(OBJ_DIR)/effects:
	mkdir -p $(OBJ_DIR)/effects


clean:
	rm -rf $(OBJ_DIR) $(TARGET)
