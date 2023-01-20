#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#define SINGLEQUOTE 39
#define LEXEMEMAXLEN 100000

typedef enum {
    t_literal, 
    t_string, 
    t_variable, 
    t_nil, 
    t_while, 
    t_if, 
    t_print, 
    t_set,
    t_equal,
    t_greater,
    t_less,
    t_length,
    t_plus,
    t_CONS,
    t_CDR,
    t_CAR,
    t_l_parenthesis, 
    t_r_parenthesis,
    t_invalid
} token_type; 

typedef struct {
    token_type type;
    char* lexeme;
    char var_name;
} Token;

typedef struct Token_node {
    Token* value;
    struct Token_node* next;
    struct Token_node* previous;
} Token_node;

typedef struct Token_list {
    Token_node* start;
    Token_node* end;
} Token_list;

typedef enum {
    s_start,
    s_in_literal,
    s_in_string,
    s_in_invalid,
    s_in_variable,
    s_C = 'C', s_E = 'E', s_G = 'G', s_I = 'I', s_L = 'L', 
    s_N = 'N', s_P = 'P', s_S = 'S', s_W = 'W', s_NI, 
    s_NIL, s_WH, s_WHI, s_WHIL, s_WHILE, s_IF, s_PR, 
    s_PRI, s_PRIN, s_PRINT, s_PL, s_PLU, s_PLUS, s_SE, 
    s_SET, s_LE, s_LES, s_LESS, s_LEN, s_LENG, s_LENGT, 
    s_LENGTH, s_GR, s_GRE, s_GREA, s_GREAT, s_GREATE, 
    s_GREATER, s_EQ, s_EQU, s_EQUA, s_EQUAL, s_CA, s_CAR, 
    s_CD, s_CDR, s_CO, s_CON, s_CONS
} automata_state;

typedef struct {
    automata_state state;
    Token* token;
} Automata;

Token_list* run_lexical_analyser(int argc, char* argv[]);
char* get_file_name(int argc, char* argv[]);
Token_list* get_tokens_from_file(FILE* fp);
void update_tokens(Token_list* tokens, Automata* automata, char c);
bool is_white_space(char c) ;
void add_variable(Token_list* tokens, Automata* automata, char name) ;
void make_and_add_simple_token(Token_list* tokens, Automata* automata, token_type type);
void handle_s_start(Token_list* tokens, Automata* automata, char c);
void start_literal(Automata* automata);
void start_string(Automata* automata);
void check_for_var(Token_list* tokens, Automata* automata, char name, char c);
void check_end_of_token(Token_list* tokens, Automata* automata, token_type type, char c);
void handle_s_in_state(Token_list* tokens, Automata* automata, char c);
void handle_s_invalid(Token_list* tokens, Automata* automata, char c);
void handle_s_variable(Token_list* tokens, Automata* automata, char c);
void print_tokens(Token_list* tokens);
void print_token(Token_node* node);
void add_token(Token_list* tokens, Token* token);
void free_token_list(Token_list* list);
void free_token_node(Token_node* node);
void lexical_analysis_test(void);
