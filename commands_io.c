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

int cmd_io_print_char()
{
    // Make sure we even have something in the stack
    if (sp < 1)
    {
        // TODO ILLEGAL ACCESS
        return -1;
    }
    
    fprintf(stdout, "%c", (char) stack[sp-1]);
    cmd_stack_pop();
    return 0;
}

int cmd_io_print_num()
{
    // Make sure we even have something in the stack
    if (sp < 1)
    {
        // TODO ILLEGAL ACCESS
        return -1;
    }
    
    fprintf(stdout, "%d", (int) stack[sp-1]);
    cmd_stack_pop();
    return 0;
}

int cmd_io_get_char()
{
    char c;
    
    // Make sure we even have something in the stack
    if (sp < 1)
    {
        // TODO ILLEGAL ACCESS
        return -1;
    }
    
    c = fgetc(stdin);
    
    // Use prewritten functions to avoid duplicate code
    // May be a tad inefficient, but at least it's the same code base
    cmd_stack_push((num) c);
    cmd_heap_store();
    cmd_stack_pop();
    return 0;
}

int cmd_io_get_num()
{
    int d;
    
    // Make sure we even have something in the stack
    if (sp < 1)
    {
        // TODO ILLEGAL ACCESS
        return -1;
    }
    
    fscanf(stdin, "%d", &d);
    
    // Use prewritten functions to avoid duplicate code
    // May be a tad inefficient, but at least it's the same code base
    cmd_stack_push((num) d);
    cmd_heap_store();
    cmd_stack_pop();
    return 0;
}
