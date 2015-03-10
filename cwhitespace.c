/**
 * So you want to code in Whitespace, eh?
 * May God have mercy on your soul.
 *
 * Please note that everything's got hard limits.
 * Whitespace doesn't have limits on data size, so a virtual machine that
 * provides unlimited resources is unrealistic.
 */

// Includes
#include <stdlib.h>
#include <stdio.h>
#include "commands.h"



// Global variables (stack, heap, etc)
int pc;                                 // Program counter

num stack[STACK_MAX];                   // Stack array
int sp;                                 // Index of next open spot in stack

num labels[LABEL_MAX];                  // Map of labels (index corresponds to index in labelptrs
int labelbals[LABEL_MAX];               // Map of program counter points

num heap[HEAP_MAX];                     // Map for heap addresses
num heapval[HEAP_MAX];                  // Values in heap

struct stmt* cache;                     // Dynamic array for cached values
int cp;                                 // Pointer into next open spot in cache

// Fancy parse lookup tree for fast parsing
cmd_type ptree[120] = {
    // space            // tab              // lf
    INCOMPLETE,         INCOMPLETE,         INCOMPLETE,     //
    STACK_PUSH,         INCOMPLETE,         INCOMPLETE,     // space
    INCOMPLETE,         INCOMPLETE,         INCOMPLETE,     // tab
    INCOMPLETE,         INCOMPLETE,         INCOMPLETE,     // lf
    INVALID,            INVALID,            INVALID,        // space space
    STACK_COPY,         INVALID,            STACK_SLIDE,    // space tab
    STACK_DUPLICATE,    STACK_SWAP,         STACK_POP,      // space lf
    INCOMPLETE,         INCOMPLETE,         INVALID,        // tab space
    HEAP_STORE,         HEAP_RETRIEVE,      INVALID,        // tab tab
    INCOMPLETE,         INCOMPLETE,         INVALID,        // tab lf
    FLOW_MARK,          FLOW_SUBROUTINE,    FLOW_GOTO,      // lf space
    FLOW_GOTO_ZERO,     FLOW_GOTO_NEGATIVE, FLOW_RETURN,    // lf tab
    INVALID,            INVALID,            FLOW_EXIT,      // lf lf
    INVALID,            INVALID,            INVALID,        // space space space
    INVALID,            INVALID,            INVALID,        // space space tab
    INVALID,            INVALID,            INVALID,        // space space lf
    INVALID,            INVALID,            INVALID,        // space tab space
    INVALID,            INVALID,            INVALID,        // space tab tab
    INVALID,            INVALID,            INVALID,        // space tab lf
    INVALID,            INVALID,            INVALID,        // space lf space
    INVALID,            INVALID,            INVALID,        // space lf tab
    INVALID,            INVALID,            INVALID,        // space lf lf
    MATH_ADD,           MATH_SUBTRACT,      MATH_MULTIPLY,  // tab space space
    MATH_DIVIDE,        MATH_MODULO,        INVALID,        // tab space tab
    INVALID,            INVALID,            INVALID,        // tab space lf
    INVALID,            INVALID,            INVALID,        // tab tab space
    INVALID,            INVALID,            INVALID,        // tab tab tab
    INVALID,            INVALID,            INVALID,        // tab tab lf
    IO_PRINT_CHAR,      IO_PRINT_NUM,       INVALID,        // tab lf space
    IO_GET_CHAR,        IO_GET_NUM,         INVALID,        // tab lf tab
    INVALID,            INVALID,            INVALID,        // tab lf lf
    INVALID,            INVALID,            INVALID,        // lf space space
    INVALID,            INVALID,            INVALID,        // lf space tab
    INVALID,            INVALID,            INVALID,        // lf space lf
    INVALID,            INVALID,            INVALID,        // lf tab space
    INVALID,            INVALID,            INVALID,        // lf tab tab
    INVALID,            INVALID,            INVALID,        // lf tab lf
    INVALID,            INVALID,            INVALID,        // lf lf space
    INVALID,            INVALID,            INVALID,        // lf lf tab
    INVALID,            INVALID,            INVALID         // lf lf lf
};

// Function prototypes
char ws_fgetc(FILE* in);


int main(int argc, char** argv)
{
    FILE *in;                           // Pointer to input file
    char c;                             // Temporary variable for input
    int treeptr = 0;                    // Current index into parse tree
    
    // Print help if incorrect number of arguments were provided
    if (argc != 2)
    {
        fprintf(stderr, "Usage:\n  %s <whitespace file>\n", argv[0]);
        exit(1);
    }
    
    // Open input file
    in = fopen(argv[1], "r");
    
    // Check for errors
    if (in == NULL)
    {
        fprintf(stderr, "%s: Unable to open file for input\n", argv[1]);
        exit(1);
    }
    
    // Read until end of file or an error occures
    while (1)
    {
        c = ws_fgetc(in);
        
        // Check if error/end of file, break if so
        if (c == EOF) break;
        
        
        
    }
    
    // Check if we broke because of an error
    if (ferror(in))
    {
        fprintf(stderr, "Error occured while reading from file\n");
        exit(1);
    }

    fclose(in);
    
    return 0;
}


// Read in characters until error, end of file, or valid whitespace char hit
char ws_fgetc(FILE* in)
{
    char c;
    
    // Loop forever
    do
    {
        c = fgetc(in);
        
        switch(c)
        {
        case ' ':
        case '\t':
        case '\n':
        case EOF:
            return c;
            break;
        
        default:
            break;
        }
    }
    while (1);
    
    return c;
}
