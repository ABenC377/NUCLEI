#include "nuclei.h"

Token_list* run_lexical_analyser(int argc, char* argv[]) {
    char* file_name = get_file_name(argc, argv);
    FILE* fp = fopen(file_name, "r");
    
    Token_list* tokens = get_tokens_from_file(fp);
    
    fclose(fp);
    
    return tokens;
}

char* get_file_name(int argc, char* argv[]) {
    check_inputs(argc, argv);
    return argv[1];
}

void check_inputs(int argc, char* argv[]) {
    if (argc != 2) {
        throw_error("ERROR: invalid number of command line arguments\n");
    }
    (void)argv; // To get rid of warning for now
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
                add_previous_chars(1, tokens, automata, 'N', c);
            }
            break;
        case NI:
            if (c == 'L') {
                make_and_add_simple_token(tokens, automata, t_nil);
            } else {
                add_previous_chars(2, tokens, automata, 'N', 'I', c);
            }
            break;
        case W:
            if (c == 'H') {
                automata->state = WH;
            } else {
                add_previous_chars(1, tokens, automata, 'W', c);
            }
            break;
        case WH:
            if (c == 'I') {
                automata->state = WHI;
            } else {
                add_previous_chars(2, tokens, automata, 'W', 'H', c);
            }
            break;
        case WHI:
            if (c == 'L') {
                automata->state = WHIL;
            } else {
                add_previous_chars(3, tokens, automata, 'W', 'H', 'I', c);
            }
            break;
        case WHIL:
            if (c == 'E') {
                make_and_add_simple_token(tokens, automata, t_while);
            } else {
                add_previous_chars(4, tokens, automata, 'W', 'H', 'I', 'L', c);
            }
            break;
        case I:
            if (c == 'I') {
                make_and_add_simple_token(tokens, automata, t_if);
            } else {
                add_previous_chars(1, tokens, automata, 'I', c);
            }
            break;
        case P:
            if (c == 'R') {
                automata->state = PR;
            } else if (c == 'L') {
                automata->state = PL;
            } else {
                add_previous_chars(1, tokens, automata, 'P', c);
            }
            break;
        case PR:
            if (c == 'I') {
                automata->state = PRI;
            } else {
                add_previous_chars(2, tokens, automata, 'P', 'R', c);
            }
            break;
        case PRI:
            if (c == 'N') {
                automata->state = PRIN;
            } else {
                add_previous_chars(3, tokens, automata, 'P', 'R', 'I', c);
            }
            break;
        case PRIN:
            if (c == 'T') {
                make_and_add_simple_token(tokens, automata, t_print);
            } else {
                add_previous_chars(4, tokens, automata, 'P', 'R', 'I', 'N', c);
            }
            break;
        case PL:
            if (c == 'U') {
                automata->state = PLU;
            } else {
                add_previous_chars(2, tokens, automata, 'P', 'L', c);
            }
            break;
        case PLU:
            if (c == 'S') {
                make_and_add_simple_token(tokens, automata, t_plus);
            } else {
                add_previous_chars(3, tokens, automata, 'P', 'L', 'U', c);
            }
            break;
        case S:
            if (c == 'E') {
                automata->state = SE;
            } else {
                add_previous_chars(1, tokens, automata, 'S', c);
            }
            break;
        case SE:
            if (c == 'T') {
                make_and_add_simple_token(tokens, automata, t_set);
            } else {
                add_previous_chars(2, tokens, automata, 'S', 'E', c);
            }
            break;
        case L:
            if (c == 'E') {
                automata->state = LE;
            } else {
                add_previous_chars(1, tokens, automata, 'L', c);
            }
            break;
        case LE:
            if (c == 'S') {
                automata->state = LES;
            } else if (c == 'N') {
                automata->state = LEN;
            } else {
                add_previous_chars(2, tokens, automata, 'L', 'E', c);
            }
            break;
        case LES:
            if (c == 'S') {
                make_and_add_simple_token(tokens, automata, t_less);
            } else {
                add_previous_chars(3, tokens, automata, 'L', 'E', 'S', c);
            }
            break;
        case LEN:
            if (c == 'G') {
                automata->state = LENG;
            } else {
                add_previous_chars(3, tokens, automata, 'L', 'E', 'N', c);
            }
            break;
        case LENG:
            if (c == 'T') {
                automata->state = LENGT;
            } else {
                add_previous_chars(4, tokens, automata, 'L', 'E', 'N', 'G', c);
            }
            break;
        case LENGT:
            if (c == 'H') {
                make_and_add_simple_token(tokens, automata, t_length);
            } else {
                add_previous_chars(5, tokens, automata, 'L', 'E', 'N', 'G', 'T', c);
            }
            break;
        case G:
            if (c == 'R') {
                automata->state = GR;
            } else {
                add_previous_chars(1, tokens, automata, 'G', c);
            }
            break;
        case GR:
            if (c == 'E') {
                automata->state = GRE;
            } else {
                add_previous_chars(2, tokens, automata, 'G', 'R', c);
            }
            break;
        case GRE:
            if (c == 'A') {
                automata->state = GREA;
            } else {
                add_previous_chars(3, tokens, automata, 'G', 'R', 'E', c);
            }
            break;
        case GREA:
            if (c == 'T') {
                automata->state = GREAT;
            } else {
                add_previous_chars(4, tokens, automata, 'G', 'R', 'E', 'A', c);
            }
            break;
        case GREAT:
            if (c == 'E') {
                automata->state = GREATE;
            } else {
                add_previous_chars(5, tokens, automata, 'G', 'R', 'E', 'A', 'T', c);
            }
            break;
        case GREATE:
            if (c == 'R') {
                make_and_add_simple_token(tokens, automata, t_greater);
            } else {
                add_previous_chars(6, tokens, automata, 'G', 'R', 'E', 'A', 'T', 'E', c);
            }
            break;
        case E:
            if (c == 'Q') {
                automata->state = EQ;
            } else {
                add_previous_chars(1, tokens, automata, 'E', c);
            }
            break;
        case EQ:
            if (c == 'U') {
                automata->state = EQU;
            } else {
                add_previous_chars(2, tokens, automata, 'E', 'Q', c);
            }
            break;
        case EQU:
            if (c == 'A') {
                automata->state = EQUA;
            } else {
                add_previous_chars(3, tokens, automata, 'E', 'Q', 'U', c);
            }
            break;
        case EQUA:
            if (c == 'L') {
                make_and_add_simple_token(tokens, automata, t_equal);
            } else {
                add_previous_chars(4, tokens, automata, 'E', 'Q', 'U', 'A', c);
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
                add_previous_chars(1, tokens, automata, 'C', c);
            }
            break;
        case CA:
            if (c == 'R') {
                make_and_add_simple_token(tokens, automata, t_CAR);
            } else {
                add_previous_chars(2, tokens, automata, 'C', 'A', c);
            }
            break;
        case CD:
            if (c == 'R') {
                make_and_add_simple_token(tokens, automata, t_CDR);
            } else {
                add_previous_chars(2, tokens, automata, 'C', 'D', c);
            }
            break;
        case CO:
            if (c == 'N') {
                automata->state = CON;
            } else {
                add_previous_chars(2, tokens, automata, 'C', 'O', c);
            }
            break;
        case CON:
            if (c == 'S') {
                make_and_add_simple_token(tokens, automata, t_CONS);
            } else {
                add_previous_chars(3, tokens, automata, 'C', 'O', 'N', c);
            }
            break;
    }
}

void add_previous_chars(int n, Token_list* tokens, Automata* automata, char var, ...) {
    add_variable_token(tokens, automata, var);
    if (n > 0) {
        va_list arg_ptr;
        int index = 0;
        va_start(arg_ptr, n);
        while (index < n) {
            update_tokens(tokens, automata, va_arg(arg_ptr, char));
        }
        va_end(arg_ptr);
    }
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
            automata->token = (Token*)allocate_space(1, sizeof(Token));
            automata->token->type = t_r_parenthesis;
            add_token(tokens, automata->token);
            break;
        case '(':
            automata->token = (Token*)allocate_space(1, sizeof(Token));
            automata->token->type = t_l_parenthesis;
            add_token(tokens, automata->token);
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
    Token_node* new_token_node = (Token_node*)allocate_space(1, sizeof(Token_node));
    new_token_node->value = token;
    if (!(tokens->start)) {
        tokens->start = tokens->end = new_token_node;
    } else {
        tokens->end = tokens->end->next = new_token_node;
    }
}

void free_token_list(Token_list* list) {
    free_token_node(list->start);
}

void free_token_node(Token_node* node) {
    if (node->next) {
        free_token_node(node->next);
        node->next = NULL;
    }
    free(node);
}

void lexical_parse_test(void) {
    // Token_list* run_lexical_analyser(int argc, char* argv[]);
    // char* get_file_name(int argc, char* argv[]);
    // void check_inputs(int argc, char* argv[]);
    // Token_list* get_tokens_from_file(FILE* fp);
    // void update_tokens(Token_list* tokens, Automata* automata, char c);
    // void add_previous_chars(int n, Token_list* tokens, Automata* automata, char var, ...);
    // void make_and_add_simple_token(Token_list* tokens, Automata* automata, // token_type type);
    // void handle_start_state(Token_list* tokens, Automata* automata, char c);
    // void add_variable_token(Token_list* tokens, Automata* automata, char name);
    // void handle_in_state(Token_list* tokens, Automata* automata, char c);
    // void print_tokens(Token_list* tokens);
    // void print_token(Token_node* node);
    // void add_token(Token_list* tokens, Token* token);
    // void free_token_list(Token_list* list);
    // void free_token_node(Token_node* node);
}
