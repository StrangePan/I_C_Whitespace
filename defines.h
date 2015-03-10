// Global constants
#define STACK_MAX    512
#define LABEL_MAX    512
#define HEAP_MAX     512
#define CACHE_MIN    512



// Global typedefs and structs
typedef int num;                        // Datatype for numbers and labels alike

typedef enum {                          // Numeric constants of commands
    INVALID,                            // Indicates invalid command (char sequence not defined by language)
    INCOMPLETE,                         // Indicates incomplete command (similar, used for parsing)
    
    STACK_PUSH,                         // Push number onto stack
    STACK_DUPLICATE,                    // Copy number on top of stack onto top of stack (again)
    STACK_COPY,                         // Copy nth item on stack (given by argument) onto top of stack
    STACK_SWAP,                         // Swap top two items on stack
    STACK_POP,                          // Discard top item on stack
    STACK_SLIDE,                        // Slide n items (given by argument) off stack, keeping top one
    
    MATH_ADD,                           // Add top item instack by second item in stack, replace them with result
    MATH_SUBTRACT,                      // Subtract top item in stack by second item in stack, replace them with result
    MATH_MULTIPLY,                      // Multiply top item in stack by second item in stack, replace them with result
    MATH_DIVIDE,                        // Integer divide top item in stack by second item in stack, replace them with result
    MATH_MODULO,                        // Mod top item in stack by second item in stack, replace them with result
    
    HEAP_STORE,                         // Store value at top of stack at location found immediately under it in stack
    HEAP_RETRIEVE,                      // Retrieve value in heap at location found at top of stack
    
    FLOW_MARK,                          // Mark location in program
    FLOW_SUBROUTINE,                    // Call subroutine
    FLOW_GOTO,                          // GOTO label unconditionally
    FLOW_GOTO_ZERO,                     // GOTO label if top of stack is zero
    FLOW_GOTO_NEGATIVE,                 // GOTO label if top of stack is negative
    FLOW_RETURN,                        // End subroutine, transfer control back to caller
    FLOW_EXIT,                          // Exit the program
    
    IO_PRINT_CHAR,                      // Output character at top of stack
    IO_PRINT_NUM,                       // Output number at top of stack
    IO_GET_CHAR,                        // Read character and place it at location given by top of stack
    IO_GET_NUM                          // Read number and place it at location given by top of stack
}
cmd_type;

typedef enum {
    SPACE = 0,
    TAB = 1,
    LINEFEED = 2
} char_type;

typedef struct stmt {
    cmd_type cmd;
    num arg;
} stmt;

typedef enum {
    ERROR_NONE = 0,
    ERROR_NUM_FORMAT,
    ERROR_NUM_SIZE,
    ERROR_EXPECT_NUM,
    ERROR_UNEXPECT_SPACE,
    ERROR_UNEXPECT_TAB,
    ERROR_UNEXPECT_LINEFEED,
    ERROR_UNEXPECT_EOF,
    ERROR_COMMAND_INVALID
} error_code;
