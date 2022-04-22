CC = gcc
CFLAGS = -Wall -g

SRC = scanner.h parser.h lex.h token.h tree.h stack.h generator.h

OBJ = scanner.o parser.o tree.o stack.o generator.o
MASTER_OBJ = main.o

MASTER = compfs

OUTPUT = $(MASTER)

all: $(OUTPUT)

%.o: %.c $(STANDARD) $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(MASTER): $(MASTER_OBJ) $(OBJ)
	$(CC) $(CFLAGS) $(MASTER_OBJ) $(OBJ) -o $(MASTER)

clean: 
	rm -f $(OUTPUT) *.o *.output.buffer a.out *.asm 
