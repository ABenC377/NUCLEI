// #include "specific.h"

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


char* get_file_name(int argc, char* argv[]);
void check_inputs(int argc, char* argv[]);
Token_list* run_lexical_analyser(char* file_name);
Token_list* get_tokens_from_file(FILE* fp);
void update_tokens(Token_list* tokens, Automata* automata, char c);
void make_and_add_simple_token(Token_list* tokens, Automata* automata, token_type type);
void handle_start_state(Token_list* tokens, Automata* automata, char c);
void add_variable_token(Token_list* tokens, Automata* automata, char name);
void handle_in_state(Token_list* tokens, Automata* automata, char c);
void print_tokens(Token_list* tokens);
void print_token(Token_node node);
void add_token(Token_list* tokens, Token* token);
void* allocate_space(int num, int size);
void throw_error(const char* error_message);
void free_token_list(Token_list* list);
