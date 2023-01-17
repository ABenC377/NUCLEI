#include "tokentype.h"
#include "syntaxtree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>


void parse_list(Token_list* list);
Tree_node* descend_recursively(Syntax_tree* tree, Token_node** current, bool* parses_correctly);
Tree_node* handle_INSTRCTS(Syntax_tree* tree, Token_node** current, bool* parses_correctly);
Tree_node* handle_INSTRCT(Syntax_tree* tree, Token_node** current, bool* parses_correctly);
Tree_node* handle_FUNC(Syntax_tree* tree, Token_node** current, bool* parses_correctly);
bool is_RETFUNC(Token_node* current);
Tree_node* handle_RETFUNC(Syntax_tree* tree, Token_node** current, bool* parses_correctly);
bool is_IOFUNC(Token_node* current);
Tree_node* handle_IOFUNC(Syntax_tree* tree, Token_node** current, bool* parses_correctly);
bool is_IF(Token_node* current);
Tree_node* handle_IF(Syntax_tree* tree, Token_node** current, bool* parses_correctly);
bool is_LOOP(Token_node* current);
Tree_node* handle_LOOP(Syntax_tree* tree, Token_node** current, bool* parses_correctly);
bool is_LISTFUNC(Token_node* current);
Tree_node* handle_LISTFUNC(Syntax_tree* tree, Token_node** current, bool* parses_correctly);
bool is_INTFUNC(Token_node* current);
Tree_node* handle_INTFUNC(Syntax_tree* tree, Token_node** current, bool* parses_correctly);
bool is_BOOLFUNC(Token_node* current);
Tree_node* handle_BOOLFUNC(Syntax_tree* tree, Token_node** current, bool* parses_correctly);
Tree_node* handle_LIST(Syntax_tree* tree, Token_node** current, bool* parses_correctly);
Tree_node* handle_VAR(Syntax_tree* tree, Token_node** current, bool* parses_correctly);
Tree_node* handle_LITERAL(Syntax_tree* tree, Token_node** current, bool* parses_correctly);
Tree_node* handle_NIL(Syntax_tree* tree, Token_node** current, bool* parses_correctly);
Tree_node* make_node(grammar_type type);Tree_node* parser_fails(bool* parses_correctly);
bool next_token_is(Token_node** current, int num_possible_tokens, ...);

