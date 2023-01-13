#pragma once

#include tokentype.h

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
    LITERAL
} grammar_type

typedef struct Tree_node {
    grammar_type type;
    char var_name;
    char* string_value;
    token_type func_type;
    struct Tree_node* child1;
    struct Tree_node* child2;
} Tree_node;

typedef struct {
    Tree_node* program;
} Syntax_tree;

