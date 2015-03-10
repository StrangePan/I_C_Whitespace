#include <stdio.h>
#include "defines.h"

int cmd_stack_push(num arg);
int cmd_stack_duplicate();
int cmd_stack_copy(num arg);
int cmd_stack_swap();
int cmd_stack_pop();
int cmd_stack_slide(num arg);
int cmd_math_add();
int cmd_math_subtract();
int cmd_math_multiply();
int cmd_math_divide();
int cmd_math_modulo();
int cmd_heap_store();
int cmd_heap_retrieve();
int cmd_flow_mark(num arg);
int cmd_flow_subroutine(num arg);
int cmd_flow_goto(num arg);
int cmd_flow_goto_zero(num arg);
int cmd_flow_goto_negative(num arg);
int cmd_flow_return();
int cmd_io_print_char();
int cmd_io_print_num();
int cmd_io_get_char();
int cmd_io_get_num();
