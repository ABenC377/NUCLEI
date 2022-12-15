#include "specific.h"

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

typedef enum {
    start,
    in_literal,
    in_string,
    N, NI,
    W, WH, WHI, WHIL,
    I,
    P, PR, PRI, PRIN, PL, PLU,
    S, SE,
    L, LE, LES, LEN, LENG, LENGT,
    G, GR, GRE, GREA, GREAT, GREATE,
    E, EQ, EQU, EQUA,
    C, CA, CD, CO, CON,
} automata_state;

typedef struct Cons {
    struct Cons* car; // points to this element of the list
    struct Cons* cdr; // points to the remainder of the list
    atomtype value;
} Cons;

typedef struct {
    token_type type;
    char* lexeme;
    char var_name;
} Token;

typedef struct Token_node {
    Token* value;
    struct Token_node* next;
} Token_node;

typedef struct Token_list {
    Token_node* start;
    Token_node* end;
} Token_list;

typedef struct {
    automata_state state;
    Token* token;
} Automata;

