#include "commands.h"

// Variables defined in cwhitespace.c
extern int pc;
extern num stack[STACK_MAX];
extern int sp;
extern num labels[LABEL_MAX];
extern int labelbals[LABEL_MAX];
extern num heap[HEAP_MAX];
extern num heapval[HEAP_MAX];
extern struct stmt* cache;
extern int cp;
