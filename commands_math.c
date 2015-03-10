#include "commands.h"

// Variables defined in cwhitespace.c
extern int pc;
extern int errcode;
extern num stack[STACK_MAX];
extern int sp;
extern int pcstack[PCSTACK_MAX];
extern int pcsp;
extern num labels[LABEL_MAX];
extern int labelvals[LABEL_MAX];
extern num heap[HEAP_MAX];
extern num heapvals[HEAP_MAX];
extern struct stmt* cache;
extern int cache_size;
extern int cp;

int cmd_math_add() { return 0; } // TODO
int cmd_math_subtract() { return 0; } // TODO
int cmd_math_multiply() { return 0; } // TODO
int cmd_math_divide() { return 0; } // TODO
int cmd_math_modulo() { return 0; } // TODO
