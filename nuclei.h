#pragma once

// #include "specific.h"
#include "./lexical_parser.h"
#include "./tokentype.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#define MAXERR 20

typedef enum {
    PROG,
    INSTRCTS,
    INSTRCT,
    FUNC,
    RETFUNC,
    LISTFUNC,
    INTFUNC,
    BOOLFUNC,
    IOFUNC,
    SET,
    PRINT,
    IF,
    LOOP,
    LIST,
    VAR,
    STRING,
    LITERAL,
    NIL,
    ERROR_NODE
} grammar_type;

typedef struct Tree_node {
    grammar_type type;
    char var_name;
    char* string_value;
    token_type func_type;
    struct Tree_node* child1;
    struct Tree_node* child2;
    struct Tree_node* child3;
} Tree_node;

typedef struct {
    Tree_node* program;
} Syntax_tree;

typedef struct {
    int num_errors;
    char* errors[MAXERR];
    bool overflow;
} Error_log;

void check_inputs(int argc, char* argv[]);
void parse_list(Token_list* list);
Tree_node* descend_recursively(Token_node** current, Error_log* error_log);
Tree_node* handle_INSTRCTS(Token_node** current, Error_log* error_log);
Tree_node* handle_INSTRCT(Token_node** current, Error_log* error_log);
Tree_node* handle_FUNC(Token_node** current, Error_log* error_log);
bool is_RETFUNC(Token_node* current);
Tree_node* handle_RETFUNC(Token_node** current, Error_log* error_log);
bool is_IOFUNC(Token_node* current);
Tree_node* handle_IOFUNC(Token_node** current, Error_log* error_log);
Tree_node* handle_SET(Token_node** current, Error_log* error_log);
bool is_IF(Token_node* current);
Tree_node* handle_IF(Token_node** current, Error_log* error_log);
bool is_LOOP(Token_node* current);
Tree_node* handle_LOOP(Token_node** current, Error_log* error_log);
bool is_LISTFUNC(Token_node* current);
Tree_node* handle_LISTFUNC(Token_node** current, Error_log* error_log);
bool is_INTFUNC(Token_node* current);
Tree_node* handle_INTFUNC(Token_node** current, Error_log* error_log);
bool is_BOOLFUNC(Token_node* current);
Tree_node* handle_BOOLFUNC(Token_node** current, Error_log* error_log);
Tree_node* handle_LIST(Token_node** current, Error_log* error_log);
Tree_node* handle_VAR(Token_node** current, Error_log* error_log);
Tree_node* handle_LITERAL(Token_node** current, Error_log* error_log);
Tree_node* handle_NIL(Token_node** current, Error_log* error_log);
Tree_node* handle_PRINT(Token_node** current, Error_log* error_log);
bool is_LIST(Token_node* current);
Tree_node* handle_STRING(Token_node** current, Error_log* error_log);
Tree_node* make_node(grammar_type type);
Tree_node* parser_fails(Error_log* error_log, char* error_message);
bool next_token_is(Token_node** current, int num_possible_tokens, ...);
void print_tree(Syntax_tree* tree);
void print_tree_node(Tree_node* node);
char* get_node_type(Tree_node* node);
void free_tree(Syntax_tree* tree);
void free_node(Tree_node* node);
void throw_error(const char* error_message);
void* allocate_space(int num, int size);
void print_log(Error_log* log);
void free_log(Error_log* log);
void add_error(Error_log* error_log, char* error_message);
void test(void);
