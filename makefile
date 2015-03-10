CC = gcc
FLAGS = -g -Wall -std=c99
EXECS = cwhitespace

all: $(EXECS)

cwhitespace: cwhitespace.c commands.c
	$(CC) $(FLAGS) -o $@ $? $(LIBS)

clean:
	rm -rf $(EXECS) *.o

