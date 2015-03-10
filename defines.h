// Global constants
#define STACK_MAX    512
#define LABEL_MAX    512
#define HEAP_MAX     512
#define CACHE_MIN    512



// Global typedefs and structs
typedef int num;				// Datatype for numbers and labels alike

typedef enum {					// Numeric constants of commands
	TEST
}
cmd_type;

typedef struct stmt {
	cmd_type cmd;
	num arg;
} stmt;

