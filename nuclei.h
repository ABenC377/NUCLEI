#include "specific.h"

#define SINGLEQUOTE 39
#define LEXEMEMAXLEN 100000

typedef enum {
    literal, 
    string, 
    variable, 
    nil, 
    while, 
    if, 
    print, 
    set,
    equal,
    greater,
    less,
    length,
    plus,
    CONS,
    CDR,
    CAR,
    l_parenthesis, 
    r_parenthesis
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
    char* string_lexeme;
    char* literal_lexeme;
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

