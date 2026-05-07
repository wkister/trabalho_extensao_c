CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include -g
LDFLAGS = -lm

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/csv_reader.c $(SRC_DIR)/helpers.c $(SRC_DIR)/pivot.c
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
EXEC = analise_dados

.PHONY: all clean run help

all: $(BIN_DIR)/$(EXEC)

$(BIN_DIR)/$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(BIN_DIR)/$(EXEC)
	./$(BIN_DIR)/$(EXEC)

help:
	@echo "Comandos disponíveis:"
	@echo "  make            - Compila o programa"
	@echo "  make run        - Compila e executa com padrão (2020-2025)"
	@echo "  make clean      - Remove arquivos compilados"
	@echo "  make help       - Exibe esta mensagem"

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)/$(EXEC)
