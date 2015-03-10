#include "commands.h"

// Variables defined in cwhitespace.c
extern int pc;
extern int errcode;
extern num stack[STACK_MAX];
extern int sp;
extern num labels[LABEL_MAX];
extern int labelvals[LABEL_MAX];
extern num heap[HEAP_MAX];
extern num heapvals[HEAP_MAX];
extern struct stmt* cache;
extern int cache_size;
extern int cp;

int cmd_stack_push(num arg) { return 0; } // TODO
int cmd_stack_duplicate() { return 0; } // TODO
int cmd_stack_copy(num arg) { return 0; } // TODO
int cmd_stack_swap() { return 0; } // TODO
int cmd_stack_pop() { return 0; } // TODO
int cmd_stack_slide(num arg) { return 0; } // TODO
