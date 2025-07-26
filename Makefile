# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -g -std=c99

# Output binary
OUTPUT = jc

# Default target
all: $(OUTPUT)

# Link the final executable
$(OUTPUT): jc.o token.o generate.o Deque.o 
	$(CC) jc.o token.o generate.o Deque.o  -o $(OUTPUT)

# Compile object files and specify dependencies
jc.o: jc.c token.h generate.h Deque.h
	$(CC) $(CFLAGS) -c jc.c


generate.o: generate.c generate.h token.h Deque.h
	$(CC) $(CFLAGS) -c generate.c

token.o: token.c token.h
	$(CC) $(CFLAGS) -c token.c

Deque.o: Deque.c Deque.h 
	$(CC) $(CFLAGS) -c Deque.c

# Clean up build files
clean:
	rm -f jc.o token.o generate.o $(OUTPUT)