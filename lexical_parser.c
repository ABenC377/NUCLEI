#include "nuclei.h"

Token_list* run_lexical_analyser(int argc, char* argv[]) {
    char* file_name = get_file_name(argc, argv);
    if (!file_name) {
        throw_error("ERROR: incorrect usage - please provide an input file\n");
    }
    
    FILE* fp = fopen(file_name, "r");
    
    Token_list* tokens = get_tokens_from_file(fp);
    
    fclose(fp);
    
    return tokens;
}

char* get_file_name(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        char* argument = argv[i];
        int arg_length = strlen(argument);
        for (int j = 0; j < (arg_length - 1); j++) {
            if (argument[j] == '.') {
                return argument;
            }
        }
    }
    return NULL;
}

Token_list* get_tokens_from_file(FILE* fp) {
    Token_list* tokens = (Token_list*)allocate_space(1, sizeof(Token_list));
    Automata* automata = (Automata*)allocate_space(1, sizeof(Automata));
    char c = (char)fgetc(fp);
    while (c != EOF) {
        update_tokens(tokens, automata, c);
        c = (char)fgetc(fp);
    }
    free(automata);
    return tokens;
}

void update_tokens(Token_list* tokens, Automata* automata, char c) {
    switch (automata->state) {
        case start:
            handle_start_state(tokens, automata, c);
            break;
        case in_literal:
        case in_string:
            handle_in_state(tokens, automata, c);
            break;
        case N:
            if (c == 'I') {
                automata->state = NI;
            } else {
                add_previous_chars(tokens, automata, 'N', 1, c);
            }
            break;
        case NI:
            if (c == 'L') {
                make_and_add_simple_token(tokens, automata, t_nil);
            } else {
                add_previous_chars(tokens, automata, 'N', 2, 'I', c);
            }
            break;
        case W:
            if (c == 'H') {
                automata->state = WH;
            } else {
                add_previous_chars(tokens, automata, 'W', 1, c);
            }
            break;
        case WH:
            if (c == 'I') {
                automata->state = WHI;
            } else {
                add_previous_chars(tokens, automata, 'W', 2, 'H', c);
            }
            break;
        case WHI:
            if (c == 'L') {
                automata->state = WHIL;
            } else {
                add_previous_chars(tokens, automata, 'W', 3, 'H', 'I', c);
            }
            break;
        case WHIL:
            if (c == 'E') {
                make_and_add_simple_token(tokens, automata, t_while);
            } else {
                add_previous_chars(tokens, automata, 'W', 4, 'H', 'I', 'L', c);
            }
            break;
        case I:
            if (c == 'F') {
                make_and_add_simple_token(tokens, automata, t_if);
            } else {
                add_previous_chars(tokens, automata, 'I', 1, c);
            }
            break;
        case P:
            if (c == 'R') {
                automata->state = PR;
            } else if (c == 'L') {
                automata->state = PL;
            } else {
                add_previous_chars(tokens, automata, 'P', 1, c);
            }
            break;
        case PR:
            if (c == 'I') {
                automata->state = PRI;
            } else {
                add_previous_chars(tokens, automata, 'P', 2, 'R', c);
            }
            break;
        case PRI:
            if (c == 'N') {
                automata->state = PRIN;
            } else {
                add_previous_chars(tokens, automata, 'P', 3, 'R', 'I', c);
            }
            break;
        case PRIN:
            if (c == 'T') {
                make_and_add_simple_token(tokens, automata, t_print);
            } else {
                add_previous_chars(tokens, automata, 'P', 4, 'R', 'I', 'N', c);
            }
            break;
        case PL:
            if (c == 'U') {
                automata->state = PLU;
            } else {
                add_previous_chars(tokens, automata, 'P', 2, 'L', c);
            }
            break;
        case PLU:
            if (c == 'S') {
                make_and_add_simple_token(tokens, automata, t_plus);
            } else {
                add_previous_chars(tokens, automata, 'P', 3, 'L', 'U', c);
            }
            break;
        case S:
            if (c == 'E') {
                automata->state = SE;
            } else {
                add_previous_chars(tokens, automata, 'S', 1, c);
            }
            break;
        case SE:
            if (c == 'T') {
                make_and_add_simple_token(tokens, automata, t_set);
            } else {
                add_previous_chars(tokens, automata, 'S', 2, 'E', c);
            }
            break;
        case L:
            if (c == 'E') {
                automata->state = LE;
            } else {
                add_previous_chars(tokens, automata, 'L', 1, c);
            }
            break;
        case LE:
            if (c == 'S') {
                automata->state = LES;
            } else if (c == 'N') {
                automata->state = LEN;
            } else {
                add_previous_chars(tokens, automata, 'L', 2, 'E', c);
            }
            break;
        case LES:
            if (c == 'S') {
                make_and_add_simple_token(tokens, automata, t_less);
            } else {
                add_previous_chars(tokens, automata, 'L', 3, 'E', 'S', c);
            }
            break;
        case LEN:
            if (c == 'G') {
                automata->state = LENG;
            } else {
                add_previous_chars(tokens, automata, 'L', 3, 'E', 'N', c);
            }
            break;
        case LENG:
            if (c == 'T') {
                automata->state = LENGT;
            } else {
                add_previous_chars(tokens, automata, 'L', 4, 'E', 'N', 'G', c);
            }
            break;
        case LENGT:
            if (c == 'H') {
                make_and_add_simple_token(tokens, automata, t_length);
            } else {
                add_previous_chars(tokens, automata, 'L', 5, 'E', 'N', 'G', 'T', c);
            }
            break;
        case G:
            if (c == 'R') {
                automata->state = GR;
            } else {
                add_previous_chars(tokens, automata, 'G', 1, c);
            }
            break;
        case GR:
            if (c == 'E') {
                automata->state = GRE;
            } else {
                add_previous_chars(tokens, automata, 'G', 2, 'R', c);
            }
            break;
        case GRE:
            if (c == 'A') {
                automata->state = GREA;
            } else {
                add_previous_chars(tokens, automata, 'G', 3, 'R', 'E', c);
            }
            break;
        case GREA:
            if (c == 'T') {
                automata->state = GREAT;
            } else {
                add_previous_chars(tokens, automata, 'G', 3, 'R', 'E', 'A', c);
            }
            break;
        case GREAT:
            if (c == 'E') {
                automata->state = GREATE;
            } else {
                add_previous_chars(tokens, automata, 'G', 5, 'R', 'E', 'A', 'T', c);
            }
            break;
        case GREATE:
            if (c == 'R') {
                make_and_add_simple_token(tokens, automata, t_greater);
            } else {
                add_previous_chars(tokens, automata, 'G', 6, 'R', 'E', 'A', 'T', 'E', c);
            }
            break;
        case E:
            if (c == 'Q') {
                automata->state = EQ;
            } else {
                add_previous_chars(tokens, automata, 'E', 1, c);
            }
            break;
        case EQ:
            if (c == 'U') {
                automata->state = EQU;
            } else {
                add_previous_chars(tokens, automata, 'E', 2, 'Q', c);
            }
            break;
        case EQU:
            if (c == 'A') {
                automata->state = EQUA;
            } else {
                add_previous_chars(tokens, automata, 'E', 3, 'Q', 'U', c);
            }
            break;
        case EQUA:
            if (c == 'L') {
                make_and_add_simple_token(tokens, automata, t_equal);
            } else {
                add_previous_chars(tokens, automata, 'E', 4, 'Q', 'U', 'A', c);
            }
            break;
        case C:
            if (c == 'A') {
                automata->state = CA;
            } else if (c == 'D') { 
                automata->state = CD;
            } else if (c == 'O') {
                automata->state = CO;
            } else {
                add_previous_chars(tokens, automata, 'C', 1, c);
            }
            break;
        case CA:
            if (c == 'R') {
                make_and_add_simple_token(tokens, automata, t_CAR);
            } else {
                add_previous_chars(tokens, automata, 'C', 2, 'A', c);
            }
            break;
        case CD:
            if (c == 'R') {
                make_and_add_simple_token(tokens, automata, t_CDR);
            } else {
                add_previous_chars(tokens, automata, 'C', 2, 'D', c);
            }
            break;
        case CO:
            if (c == 'N') {
                automata->state = CON;
            } else {
                add_previous_chars(tokens, automata, 'C', 2, 'O', c);
            }
            break;
        case CON:
            if (c == 'S') {
                make_and_add_simple_token(tokens, automata, t_CONS);
            } else {
                add_previous_chars(tokens, automata, 'C', 3, 'O', 'N', c);
            }
            break;
    }
}

void add_previous_chars(Token_list* tokens, Automata* automata, char var, int n, ...) {
    add_variable_token(tokens, automata, var);
    va_list arg_ptr;
    va_start(arg_ptr, n);
    for (int i = 0; i < n; i++) {
        update_tokens(tokens, automata, va_arg(arg_ptr, int));
    }
    va_end(arg_ptr);
}

// This only works for token types that do not have a lexeme or other value
void make_and_add_simple_token(Token_list* tokens, Automata* automata, token_type type) {
    automata->state = start;
    Token* new_token = (Token*)allocate_space(1, sizeof(Token));
    new_token->type = type;
    add_token(tokens, new_token);
}

void handle_start_state(Token_list* tokens, Automata* automata, char c) {
    switch (c) {
        // White space should be ignored
        case ' ':
        case '\n':
        case '\t':
        case '\v':
        case '\f':
        case '\r':
            break;
        case ')':
            make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            break;
        case '(':
            make_and_add_simple_token(tokens, automata, t_l_parenthesis);
            break;
        case SINGLEQUOTE:
            automata->token = (Token*)allocate_space(1, sizeof(Token));
            automata->token->type = t_literal;
            automata->token->lexeme = (char*)allocate_space(LEXEMEMAXLEN, sizeof(char));
            automata->state = in_literal;
            break;
        case '"':
            automata->token = (Token*)allocate_space(1, sizeof(Token));
            automata->token->type = t_string;
            automata->token->lexeme = (char*)allocate_space(LEXEMEMAXLEN, sizeof(char));
            automata->state = in_string;
            break;
        case 'A'...'Z':
            if (c == 'C') {
                automata->state = C;
            }  else if (c == 'E') {
                automata->state = E;
            } else if (c == 'G') {
                automata->state = G;
            } else if (c == 'I') {
                automata->state = I;
            } else if (c == 'L') {
                automata->state = L;
            } else if (c == 'N') {
                automata->state = N;
            } else if (c == 'P') {
                automata->state = P;
            } else if (c == 'S') {
                automata->state = S;
            } else if (c == 'W') {
                automata->state = W;
            } else {
                add_variable_token(tokens, automata, c);
            }
            break;
        default:
            automata->token = (Token*)allocate_space(1, sizeof(Token));
            automata->token->type = t_invalid;
            add_token(tokens, automata->token);
            break;
    }
}

void add_variable_token(Token_list* tokens, Automata* automata, char name) {
    automata->token = (Token*)allocate_space(1, sizeof(Token));
    automata->token->type = t_variable;
    automata->token->var_name = name;
    add_token(tokens, automata->token);
    automata->state = start;
}

void handle_in_state(Token_list* tokens, Automata* automata, char c) {
    if ((automata->state == in_literal) && (c == SINGLEQUOTE)) {
        add_token(tokens, automata->token);
        automata->state = start;
    } else if ((automata->state == in_string) && (c == '"')) {
        add_token(tokens, automata->token);
        automata->state = start;
    } else {
        int index = 0;
        while ((++index < LEXEMEMAXLEN) && (automata->token->lexeme[index] != '\0')) {}
        if (index == LEXEMEMAXLEN) {
            throw_error("ERROR: literal lexeme greater than maximum allowed string length\n");
        }
        automata->token->lexeme[index] = c;
    }
}

void print_tokens(Token_list* tokens) {
    if (!(tokens) || !(tokens->start)) {
        printf("Empty list - cannot print\n");
    }
    Token_node* current = tokens->start;
    while (current) {
        print_token(current);
        printf(" -> ");
        current = current->next;
    }
    printf("END\n");
}

void print_token(Token_node* node) {
    switch (node->value->type) {
        case t_literal:
            printf("<LITERAL>");
            break;
        case t_string:
            printf("<STRING>");
            break;
        case t_variable:
            printf("<VARIABLE>");
            break;
        case t_nil:
            printf("NIL");
            break;
        case t_while:
            printf("WHILE");
            break;
        case t_if:
            printf("IF");
            break;
        case t_print:
            printf("PRINT");
            break;
        case t_set:
            printf("SET");
            break;
        case t_equal:
            printf("EQUAL");
            break;
        case t_greater:
            printf("GREATER");
            break;
        case t_less:
            printf("LESS");
            break;
        case t_length:
            printf("LENGTH");
            break;
        case t_plus:
            printf("PLUS");
            break;
        case t_CONS:
            printf("CONS");
            break;
        case t_CDR:
            printf("CDR");
            break;
        case t_CAR:
            printf("CAR");
            break;
        case t_l_parenthesis:
            printf("(");
            break;
        case t_r_parenthesis:
            printf(")");
            break;
        case t_invalid:
            printf("INVALID");
            break;
    }
}

void add_token(Token_list* tokens, Token* token) {
    if (token) {
        Token_node* new_token_node = (Token_node*)allocate_space(1, sizeof(Token_node));
        new_token_node->value = token;
        if (!(tokens->start)) {
            tokens->start = tokens->end = new_token_node;
        } else {
            Token_node* temp = tokens->end;
            tokens->end = tokens->end->next = new_token_node;
            tokens->end->previous = temp;
        }
    }
}

void free_token_list(Token_list* list) {
    if (list) {
        free_token_node(list->start);
    }
}

void free_token_node(Token_node* node) {
    if (node) {
        if (node->next) {
            free_token_node(node->next);
            node->next = NULL;
        }
        free(node);
    }
}

void lexical_parse_test(void) {
    // char* get_file_name(int argc, char* argv[]) 
    int test_argc = 3;
    char* test_argv[3];
    test_argv[0] = "./nuclei";
    test_argv[1] = "folder/file.ncl";
    test_argv[2] = "folder/other.ncl";
    char* test_file_name = get_file_name(test_argc, test_argv);
    assert(strcmp(test_file_name, test_argv[1]) == 0);
    
    test_argv[1] = "not_a_file.";
    test_file_name = get_file_name(test_argc, test_argv);
    assert(strcmp(test_file_name, test_argv[2]) == 0);
    
    test_argv[2] = "also_not_a_file_name";
    test_file_name = get_file_name(test_argc, test_argv);
    assert(!test_file_name);
    
    // void update_tokens(Token_list* tokens, Automata* automata, char c);
    Token_list* test_tokens = (Token_list*)allocate_space(1, sizeof(Token_list));
    Automata* test_automata = (Automata*)allocate_space(1, sizeof(Automata));
    
    // States to NIL
    char test_c = 'I';
    test_automata->state = N;
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == NI);
    test_c = 'L';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == start);
    assert(test_tokens->end->value->type == t_nil);
    
    // States to WHILE
    test_c = 'W';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == W);
    test_c = 'H';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == WH);
    test_c = 'I';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == WHI);
    test_c = 'L';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == WHIL);
    test_c = 'E';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == start);
    assert(test_tokens->end->value->type == t_while);
    
    // states to IF
    test_c = 'I';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == I);
    test_c = 'F';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == start);
    assert(test_tokens->end->value->type == t_if);
    
    // states to PRINT
    test_c = 'P';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == P);
    test_c = 'R';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == PR);
    test_c = 'I';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == PRI);
    test_c = 'N';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == PRIN);
    test_c = 'T';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == start);
    assert(test_tokens->end->value->type == t_print);
    
    // states to PLUS
    test_c = 'P';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == P);
    test_c = 'L';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == PL);
    test_c = 'U';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == PLU);
    test_c = 'S';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == start);
    assert(test_tokens->end->value->type == t_plus);
    
    // states to SET
    test_c = 'S';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == S);
    test_c = 'E';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == SE);
    test_c = 'T';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == start);
    assert(test_tokens->end->value->type == t_set);
    
    // states to LENGTH
    test_c = 'L';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == L);
    test_c = 'E';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == LE);
    test_c = 'N';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == LEN);
    test_c = 'G';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == LENG);
    test_c = 'T';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == LENGT);
    test_c = 'H';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == start);
    assert(test_tokens->end->value->type == t_length);
    
    // states to LESS
    test_c = 'L';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == L);
    test_c = 'E';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == LE);
    test_c = 'S';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == LES);
    test_c = 'S';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == start);
    assert(test_tokens->end->value->type == t_less);
    
    // states to GREATER
    test_c = 'G';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == G);
    test_c = 'R';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == GR);
    test_c = 'E';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == GRE);
    test_c = 'A';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == GREA);
    test_c = 'T';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == GREAT);
    test_c = 'E';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == GREATE);
    test_c = 'R';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == start);
    assert(test_tokens->end->value->type == t_greater);
    
    // states to EQUAL
    test_c = 'E';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == E);
    test_c = 'Q';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == EQ);
    test_c = 'U';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == EQU);
    test_c = 'A';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == EQUA);
    test_c = 'L';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == start);
    assert(test_tokens->end->value->type == t_equal);
    
    // states to CAR
    test_c = 'C';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == C);
    test_c = 'A';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == CA);
    test_c = 'R';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == start);
    assert(test_tokens->end->value->type == t_CAR);
    
    // states to CDR
    test_c = 'C';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == C);
    test_c = 'D';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == CD);
    test_c = 'R';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == start);
    assert(test_tokens->end->value->type == t_CDR);
    
    // states to CONS
    test_c = 'C';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == C);
    test_c = 'O';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == CO);
    test_c = 'N';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == CON);
    test_c = 'S';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == start);
    assert(test_tokens->end->value->type == t_CONS);
    
    // void add_previous_chars(int n, Token_list* tokens, Automata* automata, char var, ...);
    
    
    // void make_and_add_simple_token(Token_list* tokens, Automata* automata, // token_type type);
    // void handle_start_state(Token_list* tokens, Automata* automata, char c);
    // void add_variable_token(Token_list* tokens, Automata* automata, char name);
    // void handle_in_state(Token_list* tokens, Automata* automata, char c);
    // void add_token(Token_list* tokens, Token* token);
    
    free_token_list(test_tokens);
    free(test_automata);
}
