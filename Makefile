CC = gcc
CFLAGS = -I./include -Wall -fPIC

# Target to compile the source files into object files
SRC = src/main.c src/parser.c
OBJ = main.o parser.o

# Target to compile the final program
all: program

# Compile the final program
program: $(OBJ)
	$(CC) $(OBJ) -o program

# Compile the source files into object files
main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o main.o

parser.o: src/parser.c
	$(CC) $(CFLAGS) -c src/parser.c -o parser.o

# Clean up object files and executable
clean:
	rm -f $(OBJ) program
