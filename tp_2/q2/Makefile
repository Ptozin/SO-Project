#Name of the project
PROJ_NAME=addmx

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

$(PROJ_NAME): addmx.c
	gcc -o $@ $< $(CC_FLAGS)

clean: 
	rm -rf *.o $(PROJ_NAME) *~