CC = gcc
CFLAGS = -Wall
BIN = bin
SRC = src
BUILD = build

all: $(BIN)/dining_philosophers $(BIN)/philosopher $(BIN)/philosopher_asymm

$(BIN)/dining_philosophers: $(BUILD)/main.o $(BUILD)/ipclib.o
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN)/philosopher: $(BUILD)/philosopher.o	$(BUILD)/ipclib.o
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN)/philosopher_asymm: $(BUILD)/philosopher_asymm.o $(BUILD)/ipclib.o
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)/%.o: $(SRC)/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $< -c -o $@

clean: clear

clear:
	rm -rf $(BIN) $(BUILD)

run:
	./$(BIN)/dining_philosophers 5 bin/philosopher

run_asymm:
	./$(BIN)/dining_philosophers 5 bin/philosopher_asymm
