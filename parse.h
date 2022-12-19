#include "tokentype.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_list(Token_list* list);
bool is_prog(Token_node** start);
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
bool is_valid_literal(char* literal_string);
bool even_parentheses(char* literal_string);
bool valid_chars(char* literal_string);
bool valid_char(char c);
bool no_hanging_atoms(char* literal_string);
void parse_test(void);




void parse_print_tokens(Token_list* tokens);
void parse_print_remaining_tokens(Token_node* node);
void parse_print_token(Token_node* node);
