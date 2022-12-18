// #include "specific.h"
#include "./lexical_parser.h"
#include "./parse.h"
#include "./interpret.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

void check_inputs(int argc, char* argv[]);
void* allocate_space(int num, int size);
void throw_error(const char* error_message);
void test(void);
