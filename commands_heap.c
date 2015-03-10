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

int cmd_heap_store()
{
    int i, j, addr;
    
    // Make sure we have stuff to work with
    if (sp < 2)
    {
        errcode = ERROR_STACK_EMPTY;    // ILLEGAL ACCESS
        return -1;
    }
    
    // Get the heap address
    addr = stack[sp-2];
    
    // Verify address is valid
    if (addr < 0)
    {
        errcode = ERROR_OUT_OF_BOUNDS;  // ILLEGAL ACCESS
        return -1;
    }
    
    j = addr % HEAP_MAX;
    
    // Attempt to store top item in stack into heap
    // 1. Must be at least 1 open slot
    // 2. Must not already exist
    for (i = 0; i <= HEAP_MAX; i++, j++)
    {
        if (j > HEAP_MAX) j = j % HEAP_MAX;
        
        // If we made full loop, no open spots
        if (i == HEAP_MAX)
        {
            errcode = ERROR_HEAP_FULL;  // HEAP FULL
            return -1;
        }
        
        // Check for clear spot or a matching address. If found, take it!
        if (heap[j] == -1 || heap[j] == addr)
        {
            heap[j] = addr;
            heapvals[j] = stack[sp-1];
            return 0;
        }
    }
    
    // We shouldn't ever get here. If we do, something's wrong.
    return -1;
}

int cmd_heap_retrieve()
{
    int i, j, addr;
    
    // Make sure we have stuff to work with
    if (sp < 1)
    {
        errcode = ERROR_STACK_EMPTY;    // ILLEGAL ACCESS
        return -1;
    }
    
    // Get the heap address
    addr = stack[sp-1];
    
    // Verify address is valid
    if (addr < 0)
    {
        errcode = ERROR_OUT_OF_BOUNDS;  // ILLEGAL ACCESS
        return -1;
    }
    
    j = addr % HEAP_MAX;
    
    // Attempt to store top item in stack into heap
    // 1. Must be at least 1 open slot
    // 2. Must not already exist
    for (i = 0; i <= HEAP_MAX; i++, j++)
    {
        if (j > HEAP_MAX) j = j % HEAP_MAX;
        
        // If we made full loop, address not allocated
        if (i == HEAP_MAX)
        {
            errcode = ERROR_HEAP_NOT_ALLOC; // ADDRESS NOT ALLOCATED
            return -1;
        }
        
        // Check for matching address. If found, push to stack!
        if (heap[j] == addr)
        {
            return cmd_stack_push(heapvals[j]);
        }
    }
    
    // We shouldn't ever get here. If we do, something's wrong.
    return -1;
}
