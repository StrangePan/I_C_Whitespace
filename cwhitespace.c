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
    int ptreeptr = 0;                    // Current index into parse tree
    
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
        
        // Offset tree pointer
        switch (c)
        {
        case ' ':
            ptreeptr = ptreeptr + SPACE;
            break;
            
        case '\t':
            ptreeptr = ptreeptr + TAB;
            break;
            
        case '\n':
            ptreeptr = ptreeptr + LINEFEED;
            break;
        }
        
        // Determine action to take based on current position in parse tree
        switch (ptree[ptreeptr])
        {
        case INCOMPLETE:
            ptreeptr = (ptreeptr + 1) * 3;      // DEEPER INTO THE TREE WE GO
            break;
            
            
            
        case INVALID:
        default:
            // TODO Display error
            break;
        }
        
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


// Parse a number in whitespace
error_code ws_getnum(FILE* in, int& num)
{
    char c;                             // temporary variable for input
    int neg = 0;                        // flag to make value negative
    int bits = 0;                       // Count number of bits required to store number
    int count = 0;                      // Bits read from file (to enforce minimum of 1 bit)
    int n = 0;                          // Reset given number
    
    // Determine if number is positive/negative
    c = ws_fgetc(in);
    switch (c)
    {
    case ' ':                           // Space means positive
        neg = 0;
        break;
        
    case '\t':                          // Tab means negative
        neg = 1;
        break;
        
    case '\n':                          // Uh oh!
        return ERROR_EXPECT_NUM;
        
    case EOF
        return ERROR_UNEXPECT_EOF;
    }
    
    // Start reading in number until newline is hit
    do
    {
        c = ws_fgetc(in);
        
        switch(c)
        {
        case ' ':                       // Space means 0 bit
            n = n << 1;
            count = count + 1;
            
            // Increment bits if not only getting leading 0's
            if (bits > 0)
                bits = bits + 1;
            break;
            
        case '\t':                      // Tab means 1 bit
            n = n << 1;
            n = n & 1;
            count = count + 1;
            bits = bits + 1
            break;
            
        case EOF:                       // Uh oh!
            return ERROR_UNEXPECT_EOF;
        }
        
        // Make sure we aren't overflowing
        if (bits >= (sizeof(n) * 8))
            return ERROR_NUM_SIZE;
    }
    while (c != '\n');
    
    // Make sure at least 1 bit was read after the sign bit
    if (count == 0)
        return ERROR_NUM_FORMAT;
    
    // Negatize number if necessary
    if (neg == 1) n = 0 - n;
    
    num = n;
    
    // Return success
    return ERROR_NONE;
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
