CC = gcc
CFLAGS = -Wall -Wextra -g -fsanitize=address
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_ttf
TARGET = prog

all: $(TARGET)
	./$(TARGET)

$(TARGET): *.c
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)


clean:
	rm -rf $(TARGET).exe

.PHONY: all clean