#Name of the project
PROJ_NAME=phrases

# .c files
C_SOURCE=$(wildcard *.c)
 
# Object files
OBJ=$(C_SOURCE:.c=.o)

# Flags for compiler
CC_FLAGS=-Wall

#
# Compilation and linking
#

all: $(PROJ_NAME)

$(PROJ_NAME): phrases.c
	gcc -o $@ $< $(CC_FLAGS)

run_1:
	./$(PROJ_NAME) quote.txt

run_2:
	./$(PROJ_NAME) -l quote.txt

clean: 
	rm -rf *.o $(PROJ_NAME) *~