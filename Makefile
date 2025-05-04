CC = gcc
CFLAGS = -I./include -Wall -fPIC
LDFLAGS = -pthread -lm

# Target to compile the source files into object files
SRC = src/main.c src/parser.c src/analysis.c src/thread.c src/output.c
OBJ = main.o parser.o analysis.o thread.o output.o

# Target to compile the final program
all: program

# Compile the final program
program: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o program

# Compile the source files into object files
main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o main.o

parser.o: src/parser.c
	$(CC) $(CFLAGS) -c src/parser.c -o parser.o

analysis.o: src/analysis.c
	$(CC) $(CFLAGS) -c src/analysis.c -o analysis.o

thread.o: src/thread.c
	$(CC) $(CFLAGS) -c src/thread.c -o thread.o

output.o: src/output.c
	$(CC) $(CFLAGS) -c src/output.c -o output.o

# Clean up object files and executable
clean:
	rm -f $(OBJ) program
