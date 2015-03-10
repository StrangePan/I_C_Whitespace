#include "commands.h"

// Variables defined in cwhitespace.c
extern int pc;
extern int errcode;
extern num stack[STACK_MAX];
extern int sp;
extern num labels[LABEL_MAX];
extern int labelbals[LABEL_MAX];
extern num heap[HEAP_MAX];
extern num heapval[HEAP_MAX];
extern struct stmt* cache;
extern int cache_size;
extern int cp;

int cmd_heap_store() { return 0; } // TODO
int cmd_heap_retrieve() { return 0; } // TODO
