CC = gcc
CFLAGS = -Wall
BIN = bin
SRC = src
BUILD = build

all: $(BIN)/dining_philosophers $(BIN)/philosopher

$(BIN)/dining_philosophers: $(BUILD)/main.o $(BUILD)/ipclib.o
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN)/philosopher: $(BUILD)/philosopher.o	$(BUILD)/ipclib.o
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)/%.o: $(SRC)/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $< -c -o $@

clean: clear

clear:
	rm -rf $(BIN) $(BUILD)

run:
	./$(BIN)/dining_philosophers 5
