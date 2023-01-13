#pragma once

#include tokentype.h

typedef struct Tree_node {
    Token* token;
    int num_children;
    struct Tree_node* children;
} Tree_node;

typedef struct {
    Tree_node* program;
} Syntax_tree;