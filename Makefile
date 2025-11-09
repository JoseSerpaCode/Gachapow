CC = gcc
CFLAGS = -I include -I include/extern -Wall -Wextra
LDFLAGS = -L lib -lraylib -lgdi32 -lwinmm

SRC = $(wildcard src/**/*.c src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = build/game.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf build
