CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LIBS = -lncurses

SRC = src/main.c src/globals.c src/buffer.c src/fileio.c src/ui.c src/commands.c src/gc.c
TARGET = editor

all: $(TARGET)

$(TARGET): $(SRC) include/editor.h
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)
