#Name of the project
PROJ_NAME=cypher

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

$(PROJ_NAME): cypher.c
	gcc -o $@ $< $(CC_FLAGS)

run_1:
	./$(PROJ_NAME) < quote.txt

run_2:
	./$(PROJ_NAME) < quote.txt > quote_cyphered.txt

run_3:
	./$(PROJ_NAME) < quote_cyphered.txt

clean: 
	rm -rf *.o $(PROJ_NAME) *~ quote_cyphered.txt