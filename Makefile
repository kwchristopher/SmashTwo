#-------Define names of all the object files in this project
OBJS = commands.o history.o smash.o

#-------Define names of the executable file
EXE = smash

#-------Define options passed to the compiler
CFLAGS = -Wall -std=c99
CC = gcc

#-------Define target "all" for building the executable(s)
all: rules.d $(EXE)

rules.d: $(wildcard *.c) $(wildcard *.h)
	gcc -MM $(wildcard *.c) >rules.d

#-------Include the rules for rebuilding each *.o file
-include rules.d

debug: CFLAGS += -g -O0
debug: $(EXE)

#-------Rule for linking the executable product
$(EXE): $(OBJS)
	$(CC) $^ -o $@

#------This target builds a debug version and executes the product under valgrind
valgrind: debug
	valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all smash 

#-------Rule for cleaning build and vi artifacts
clean:
	rm -f $(OBJS) $(EXE) *.d *~
