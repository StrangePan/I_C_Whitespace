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
#include "cmd_type.h"

// Global constants
#define STACK_MAX    512
#define LABEL_MAX    512
#define HEAP_MAX     512
#define CACHE_MIN    512



// Global typedefs and structs
typedef int num;				// Datatype for numbers and labels alike
typedef struct stmt {
	cmd_type cmd;
	num arg;
} stmt;


// Global variables (stack, heap, etc)
int pc;							// Program counter

num stack[STACK_MAX];			// Stack array
int sp;							// Index of next open spot in stack

num labels[LABEL_MAX];			// Map of labels (index corresponds to index in labelptrs
int labelbals[LABEL_MAX];		// Map of program counter points

num heap[HEAP_MAX];				// Map for heap addresses
num heapval[HEAP_MAX];			// Values in heap

struct stmt* cache;				// Dynamic array for cached values
int cp;							// Pointer into next open spot in cache



// Function prototypes



int main(int argc, char** argv)
{
	FILE *in;
	
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
	
	fclose(in);
	
	return 0;
}
