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

int cmd_flow_mark(num arg)
{
    int i, j = (arg % LABEL_MAX);
    
    while (j < 0) j = j + LABEL_MAX;
    
    // Attempt to create new label
    // 1. Must be at least 1 open slot
    // 2. Must not already exist
    for (i = 0; i <= LABEL_MAX; i++, j++)
    {
        if (j > LABEL_MAX) j = j % LABEL_MAX;
        
        // If we made full loop, no open spots
        if (i == LABEL_MAX)
        {
            // TODO TOO MANY LABELS DECLARED
            return -1;
        }

        // If we find duplicate, throw error
        if (labels[j] == arg && labelvals[j] != -1)
        {
            // TODO LABEL ALREADY DECLARED
            return -1;
        }

        // Check for clear spot. If found, take it!
        if (labels[j] == -1 && labelvals[j] == -1)
        {
            labels[j] = arg;
            labelvals[j] = cp;
            return 0;
        }
    }
    
    // We shouldn't ever get here. If we do, something's wrong.
    return -1;
}

int cmd_flow_subroutine(num arg)
{
    // Make sure we don't overflow our PC stack
    if (pcsp >= PCSTACK_MAX)
    {
        // TODO STACK OVERFLOW!
        return -1;
    }
    
    // Push program counter onto pc stack and goto label
    pcstack[pcsp] = pc;
    pcsp = pcsp + 1;
    return cmd_flow_goto(arg);
}

int cmd_flow_goto(num arg)
{
    int i, j = (arg % LABEL_MAX);
    
    while (j < 0) j = j + LABEL_MAX;
    
    // Find matching label
    for (i = 0; i <= LABEL_MAX; i++, j++)
    {
        if (j > LABEL_MAX) j = j % LABEL_MAX;
        
        // If we made a full loop, label not found
        if (i == LABEL_MAX)
        {
            // TODO NO SUCH LABEL
            return -1;
        }
        
        // Match found; move program counter
        if (labels[j] == arg && labelvals[j] != -1)
        {
            pc = labelvals[j];
            return 0;
        }
    }
    
    return 0;
}

int cmd_flow_goto_zero(num arg)
{
    // Make sure we even have something on the stack
    if (sp < 1)
    {
        // TODO ILLEGAL ACCESS EXCEPTION
        return -1;
    }
    
    if (stack[sp-1] == 0)
        return cmd_flow_goto(arg);
    else
        return 0;
}

int cmd_flow_goto_negative(num arg)
{
    // Make sure we even have something on the stack
    if (sp < 1)
    {
        // TODO ILLEGAL ACCESS EXCEPTION
        return -1;
    }
    
    if (stack[sp-1] < 0)
        return cmd_flow_goto(arg);
    else
        return 0;
}

int cmd_flow_return()
{
    // Make sure we have something to pop from
    if (pcsp < 1)
    {
        // TODO RETURNING FROM NOWHERE!
        return -1;
    }
    
    // Pop program counter off top of pc stack
    pcsp = pcsp - 1;
    pc = pcstack[pcsp];
    return 0;
}
