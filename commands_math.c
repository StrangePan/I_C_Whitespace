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

int cmd_math_add()
{
    // Make sure we have something to work with
    if (sp < 2)
    {
        // TODO ILLEGAL ACCESS
        return -1;
    }
    
    // Add top two items on stack together, pop first, replace second with result
    stack[sp-2] = stack[sp-1] + stack[sp-2];
    sp = sp - 1;
    return 0;
}

int cmd_math_subtract()
{
    // Make sure we have something to work with
    if (sp < 2)
    {
        // TODO ILLEGAL ACCESS
        return -1;
    }
    
    // Subtract top two items on stack from each other, pop first, replace second with result
    stack[sp-2] = stack[sp-1] - stack[sp-2];
    sp = sp - 1;
    return 0;
}

int cmd_math_multiply()
{
    // Make sure we have something to work with
    if (sp < 2)
    {
        // TODO ILLEGAL ACCESS
        return -1;
    }
    
    // Multiply top two items on stack together, pop first, replace second with result
    stack[sp-2] = stack[sp-1] * stack[sp-2];
    sp = sp - 1;
    return 0;
}

int cmd_math_divide()
{
    // Make sure we have something to work with
    if (sp < 2)
    {
        // TODO ILLEGAL ACCESS
        return -1;
    }
    
    // Divide top two items on stack together, pop first, replace second with result
    stack[sp-2] = stack[sp-1] / stack[sp-2];
    sp = sp - 1;
    return 0;
}

int cmd_math_modulo()
{
    // Make sure we have something to work with
    if (sp < 2)
    {
        // TODO ILLEGAL ACCESS
        return -1;
    }
    
    // Modulo top two items on stack together, pop first, replace second with result
    stack[sp-2] = stack[sp-1] % stack[sp-2];
    sp = sp - 1;
    return 0;
}
