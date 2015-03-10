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

int cmd_stack_push(num arg)
{
    // Make sure we have room
    if (sp >= STACK_MAX)
    {
        // TODO STACK OVERFLOW!
        return -1;
    }
    
    // Add argument to stack, increment stack pointer
    stack[sp] = arg;
    sp = sp + 1;
    return 0;
}

int cmd_stack_duplicate()
{
    // Make sure we even have something to duplicate
    if (sp < 1)
    {
        // TODO ILLEGAL ACCESS
        return -1;
    }
    
    return cmd_stack_push(stack[sp-1]);
}

int cmd_stack_copy(num arg)
{
    // Make sure we are operating within valid range
    if (arg < 0 || arg >= sp)
    {
        // TODO ILLEGAL ACCESS
        return -1;
    }
    
    return cmd_stack_push(stack[(sp-1) - arg]);
}

int cmd_stack_swap()
{
    num tmp;
    
    // Make sure we even have something to swap
    if (sp < 2)
    {
        // TODO ILLEGAL ACCESS
        return -1;
    }
    
    // Swap top two stack items
    tmp = stack[sp-2];
    stack[sp-2] = stack[sp-1];
    stack[sp-1] = tmp;
    return 0;
}

int cmd_stack_pop()
{
    // Make sure we event have something to pop
    if (sp < 1)
    {
        // TODO ILLEGAL ACCESS
        return -1;
    }
    
    sp = sp - 1;
    return 0;
}

int cmd_stack_slide(num arg)
{
    // Make sure we are operating within valid range
    if (arg < 0 || arg >= (sp - 1))
    {
        // TODO ILLEGAL ACCESS
        return -1;
    }
    
    // Remove n items, but preserve top item!
    stack[sp - arg - 1] = stack[sp - 1];
    sp = sp - arg;
    return 0;
}
