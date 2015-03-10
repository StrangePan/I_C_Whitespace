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



// Our friendly neighborhood debug flag
//#define VERBOSE



// Global variables (stack, heap, etc)
int pc;                                 // Program counter
int errcode;

num stack[STACK_MAX];                   // Stack array
int sp = 0;                             // Index of next open spot in stack

int pcstack[PCSTACK_MAX];               // A program counter stack (for subroutine calls only)
int pcsp = 0;                           // Index of next open spot in pc stack

num labels[LABEL_MAX];                  // Map of labels (index corresponds to index in labelptrs
int labelvals[LABEL_MAX];               // Map of program counter points

num heap[HEAP_MAX];                     // Map for heap addresses
num heapvals[HEAP_MAX];                  // Values in heap

struct stmt* cache;                     // Dynamic array for cached values
int cp = -1;                            // Pointer into next open spot in cache
int cache_size;                         // Current number of items in cache

char buffer[BUFFER_MAX] = {0};          // Input buffer for efficiency
int bufferptr = 0;                      // Current position in buffer



// Fancy lookup tree for fast parsing
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
num ws_getnum(FILE* in);
void printError();


int main(int argc, char** argv)
{
    FILE *in;                           // Pointer to input file
    char c;                             // Temporary variable for input
    num n;                              // Temporary variable for input
    int ptreeptr = 0;                   // Current index into parse tree
    struct stmt* cache_temp;            // Temp var for array doubling
    int i;                              // For loopz
    
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
    
#ifdef VERBOSE
    printf("opened file, copmiling\n");
#endif
    
    // Allocate space for cache
    cache_size = CACHE_MIN;
    cache = malloc(sizeof(stmt) * cache_size);
    cp = 0;
    
    // Initialize heap and label maps
    for (i = 0; i < LABEL_MAX; i++)
    {
        labels[i] = -1;
        labelvals[i] = -1;
    }
    for (i = 0; i < HEAP_MAX; i++)
    {
        heap[i] = -1;
    }
    
    // Read until end of file or an error occures, precompiling code
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
            
        case FLOW_MARK:                 // Mark point in code
            n = ws_getnum(in);
            
            if (errcode != ERROR_NONE)
            {
                printError();
                exit(1);
                break;
            }
            
            cmd_flow_mark(n);
        
#ifdef VERBOSE
        printf("created label %d\n", n);
#endif
        
        // Reset ptree pointer if needed
        ptreeptr = 0;
            break;
            
            
        case STACK_PUSH:
        case STACK_COPY:
        case STACK_SLIDE:
        case FLOW_SUBROUTINE:
        case FLOW_GOTO:
        case FLOW_GOTO_ZERO:
        case FLOW_GOTO_NEGATIVE:        // These commands require number parameter
            n = ws_getnum(in);
            
#ifdef VERBOSE
        printf("parsed number %d  ", n);
#endif
        
            // Make sure no errors occured during parsing
            if (errcode != ERROR_NONE)
            {
                // TODO Display error
                exit(1);
                break;
            }
            
        case STACK_DUPLICATE:
        case STACK_SWAP:
        case STACK_POP:
        case MATH_ADD:
        case MATH_SUBTRACT:
        case MATH_MULTIPLY:
        case MATH_DIVIDE:
        case MATH_MODULO:
        case HEAP_STORE:
        case HEAP_RETRIEVE:
        case FLOW_RETURN:
        case FLOW_EXIT:
        case IO_PRINT_CHAR:
        case IO_PRINT_NUM:
        case IO_GET_CHAR:
        case IO_GET_NUM:
            
            // If needed, double cache size
            if (cp == cache_size)
            {
                cache_temp = malloc(sizeof(stmt) * (cache_size * 2));
                
                // copy everything over :(
                for (i = 0; i < cache_size; i = i + 1)
                {
                    cache_temp[i] = cache[i];
                }
                
                free(cache);
                cache = cache_temp;
                cache_size = cache_size * 2;
            }
            
            // Store command and argument in cache for future execution
            cache[cp].cmd = ptree[ptreeptr];
            cache[cp].arg = n;
            cp = cp + 1;
        
#ifdef VERBOSE
        printf("compiled command: %d\n", ptree[ptreeptr]);
#endif
        
        // Reset ptree pointer if needed
        ptreeptr = 0;
            break;
            
        case INVALID:
        default:
            // TODO Display error
            exit(1);
        
        // Reset ptree pointer if needed
        ptreeptr = 0;
            break;
        }
        
    
    }
    
#ifdef VERBOSE
    printf ("compiled, running\n");
#endif
    
    
    // Now we actually execute the code
    for (pc = 0; pc <= cp && cache[pc].cmd != FLOW_EXIT; pc++)
    {
        switch (cache[pc].cmd)
        {
        case STACK_PUSH:
            cmd_stack_push(cache[pc].arg);
            break;
            
        case STACK_DUPLICATE:
            cmd_stack_duplicate();
            break;
            
        case STACK_COPY:
            cmd_stack_copy(cache[pc].arg);
            break;
            
        case STACK_SWAP:
            cmd_stack_swap();
            break;
            
        case STACK_POP:
            cmd_stack_pop();
            break;
            
        case STACK_SLIDE:
            cmd_stack_slide(cache[pc].arg);
            break;
            
        case MATH_ADD:
            cmd_math_add();
            break;
            
        case MATH_SUBTRACT:
            cmd_math_subtract();
            break;
            
        case MATH_MULTIPLY:
            cmd_math_multiply();
            break;
            
        case MATH_DIVIDE:
            cmd_math_divide();
            break;
            
        case MATH_MODULO:
            cmd_math_modulo();
            break;
            
        case HEAP_STORE:
            cmd_heap_store();
            break;
            
        case HEAP_RETRIEVE:
            cmd_heap_retrieve();
            break;

        case FLOW_MARK:
            cmd_flow_mark(cache[pc].arg);
            break;
            
        case FLOW_SUBROUTINE:
            cmd_flow_subroutine(cache[pc].arg);
            break;
            
        case FLOW_GOTO:
            cmd_flow_goto(cache[pc].arg);
            break;
            
        case FLOW_GOTO_ZERO:
            cmd_flow_goto_zero(cache[pc].arg);
            break;
            
        case FLOW_GOTO_NEGATIVE:
            cmd_flow_goto_negative(cache[pc].arg);
            break;
            
        case FLOW_RETURN:
            cmd_flow_return();
            break;
            
        case FLOW_EXIT:
            // Already handled in loop
            break;
            
        case IO_PRINT_CHAR:
            cmd_io_print_char();
            break;
            
        case IO_PRINT_NUM:
            cmd_io_print_num();
            break;
            
        case IO_GET_CHAR:
            cmd_io_get_char();
            break;
            
        case IO_GET_NUM:
            cmd_io_get_num();
            break;
            
        default:                        // We shouldn't ever make it here
            break;
        }
        
        // Break out if any error is encountered
    if (errcode != ERROR_NONE)
        break;
    }
    
    
    // Check if we broke because of an IO error
    if (ferror(in))
    {
        fprintf(stderr, "Error occured while reading from file\n");
        exit(1);
    }
    
    // Determine whether the program cleaned up after itself or if it was terminated properly
    if (errcode == ERROR_NONE && (pc >= cp || cache[pc].cmd != FLOW_EXIT))
        errcode = ERROR_UNEXPECT_EXIT;
    
    // Print required error message
    if (errcode != ERROR_NONE)
        printError();
    
    // Program unexpectedly terminated, print reason
    if (errcode == ERROR_UNEXPECT_EXIT)
        fprintf(stderr, "Program reached end of code without encountering end-of-program character sequence.\n");
    
    // Print out items remaining on stack at time of termination
    if (sp > 0)
        fprintf(stderr, "Items remaining on stack at exit: %d\n", sp);
    
    
    // Clean up and go home!
    fclose(in);
    return 0;
}


// Parse a number in whitespace
num ws_getnum(FILE* in)
{
    char c;                             // temporary variable for input
    int neg = 0;                        // flag to make value negative
    int bits = 0;                       // Count number of bits required to store number
    int count = 0;                      // Bits read from file (to enforce minimum of 1 bit)
    num n = 0;                          // Reset given number
    
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
        errcode = ERROR_EXPECT_NUM;
        return 0;
        
    case EOF:
        errcode = ERROR_UNEXPECT_EOF;
        return 0;
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
            n = n | 1;
            count = count + 1;
            bits = bits + 1;
            break;
            
        case EOF:                       // Uh oh!
            errcode = ERROR_UNEXPECT_EOF;
            return 0;
        }
        
        // Make sure we aren't overflowing
        if (bits >= (sizeof(n) * 8))
        {
            errcode = ERROR_NUM_SIZE;
            return 0;
        }
    }
    while (c != '\n');
    
    // Make sure at least 1 bit was read after the sign bit
    if (count == 0)
    {
        errcode = ERROR_NUM_FORMAT;
        return 0;
    }
    
    // Negatize number if necessary
    if (neg == 1) n = 0 - n;
    
    // Return success
    errcode = ERROR_NONE;
    return n;
}


// Read in characters until error, end of file, or valid whitespace char hit
char ws_fgetc(FILE* in)
{
    char c;
    
    // Loop forever
    do
    {
        // Fill buffer if needed
        if (buffer[bufferptr] == '\0')
        {
            // Read in checking for EOF
            if (fgets(buffer, BUFFER_MAX, in) == NULL)
            {
                c = EOF;
                break;
            }
            bufferptr = 0;
        }
        
        c = buffer[bufferptr];
        bufferptr = bufferptr + 1;
        
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

void printError()
{
    if (errcode == ERROR_NONE) return;
    
    // Output line number
    if (pc == -1)
        fprintf(stderr, "Compile error, command %d: ", cp);
    else
        fprintf(stderr, "Runtime error, command %d: ", pc);
    
    switch(errcode)
    {
    case ERROR_NONE:
        // :)
        break;
    
    case ERROR_NUM_FORMAT:
        fprintf(stderr, "Unable to parse numerical constant");
        break;
    
    case ERROR_NUM_SIZE:
        fprintf(stderr, "Number too large (greater than %d bits)", (sizeof(num) * 8) - 1);
        break;
    
    case ERROR_EXPECT_NUM:
        fprintf(stderr, "Number expected but not received");
        break;
    
    case ERROR_UNEXPECT_SPACE:
        fprintf(stderr, "Unexpected space character");
        break;
    
    case ERROR_UNEXPECT_TAB:
        fprintf(stderr, "Unexpected tab character");
        break;
    
    case ERROR_UNEXPECT_LINEFEED:
        fprintf(stderr, "Unexpected linefeed character");
        break;
    
    case ERROR_UNEXPECT_EOF:
        fprintf(stderr, "Unexpected end of file");
        break;
    
    case ERROR_COMMAND_INVALID:
        fprintf(stderr, "Unrecognize/invalid command");
        break;

    case ERROR_UNEXPECT_EXIT:
        fprintf(stderr, "Unexpected end of program");
        break;
    
    case ERROR_STACK_EMPTY:
        fprintf(stderr, "Attempt to pop from an empty stack");
        break;
    
    case ERROR_STACK_FULL:
        fprintf(stderr, "Attempt to push onto an already full stack");
        break;
    
    case ERROR_HEAP_NOT_ALLOC:
        fprintf(stderr, "Attempt to access uninitialized heap address");
        break;
    
    case ERROR_HEAP_FULL:
        fprintf(stderr, "Attempt to write to an already full heap");
        break;
    
    case ERROR_LABEL_NOT_DEC:
        fprintf(stderr, "Label not found");
        break;
    
    case ERROR_LABEL_DUP:
        fprintf(stderr, "Label already declared in file");
        break;
    
    case ERROR_LABEL_FULL:
        fprintf(stderr, "Maximum limit of unique labels reached");
        break;
    
    case ERROR_SUBROUTINE_DEPTH:
        fprintf(stderr, "Maximum subroutine depth reached");
        break;
    
    case ERROR_RETURN_NOWHERE:
        fprintf(stderr, "Attempt to return from a non-subroutine");
        break;
        
    case ERROR_OUT_OF_BOUNDS:
        fprintf(stderr, "Attempt to access location out of bounds");
        break;
    
    }
    
    fprintf(stderr, "\n");
}
