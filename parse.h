#include "tokentype.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void parse_list(Token_list* list);
bool is_prog(Token_node* start);
bool is_instrcts(Token_node** start);
bool is_instrct(Token_node** start);
bool is_func(Token_node** start);
bool is_retfunc(Token_node** start);
bool is_listfunc(Token_node** start);
bool is_intfunc(Token_node** start);
bool is_boolfunc(Token_node** start);
bool is_iofunc(Token_node** start);
bool is_set(Token_node** start);
bool is_print(Token_node** start);
bool is_if(Token_node** start);
bool is_loop(Token_node** start);
bool is_list(Token_node** start);
void parse_test(void);

