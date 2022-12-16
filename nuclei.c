#include "nuclei.h"

int main(int argc, char* argv[]) {
    
    // Set up the file to be scanned
    char* file_name = get_file_name(argc, argv);
    
    Token_list* list = run_lexical_analyser(file_name);
    print_tokens(list);
    
    free_token_list(list);
}

char* get_file_name(int argc, char* argv[]) {
    check_inputs(argc, argv);
    return argv[1];
}

void check_inputs(int argc, char* argv[]) {
    if (argc != 2) {
        throw_error("ERROR: invalid number of command line arguments\n");
    }
}

Token_list* run_lexical_analyser(char* file_name) {
    FILE* fp = fopen(file_name, "r");
    
    // ungetc() is going to be an important function here!!!!
    Token_list* tokens = get_tokens_from_file(fp);
    
    fclose(fp);
    
    return tokens;
}

Token_list* get_tokens_from_file(FILE* fp) {
    Token_list* tokens = (Token_list*)allocate_space(1, sizeof(Token_list));
    Automata* automata = (Automata*)allocate_space(1, sizeof(Automata));
    char c = fgetc(fp);
    while (c != EOF) {
        printf("Reading character '%c' (%i)\n", c, c); // debugging
        update_tokens(tokens, automata, c);
        c = fgetc(fp);
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
                add_variable_token(tokens, automata, 'N');
                update_tokens(tokens, automata, c);
            }
            break;
        case NI:
            if (c == 'L') {
                make_and_add_simple_token(tokens, automata, t_nil);
            } else {
                add_variable_token(tokens, automata, 'N');
                update_tokens(tokens, automata, 'I');
                update_tokens(tokens, automata, c);
            }
            break;
        case W:
            if (c == 'H') {
                automata->state = WH;
            } else {
                add_variable_token(tokens, automata, 'W');
                update_tokens(tokens, automata, c);
            }
            break;
        case WH:
            if (c == 'I') {
                automata->state = WHI;
            } else {
                add_variable_token(tokens, automata, 'W');
                update_tokens(tokens, automata, 'H');
                update_tokens(tokens, automata, c);
            }
            break;
        case WHI:
            if (c == 'L') {
                automata->state = WHIL;
            } else {
                add_variable_token(tokens, automata, 'W');
                update_tokens(tokens, automata, 'H');
                update_tokens(tokens, automata, 'I');
                update_tokens(tokens, automata, c);
            }
            break;
        case WHIL:
            if (c == 'E') {
                make_and_add_simple_token(tokens, automata, t_while);
            } else {
                add_variable_token(tokens, automata, 'W');
                update_tokens(tokens, automata, 'H');
                update_tokens(tokens, automata, 'I');
                update_tokens(tokens, automata, 'L');
                update_tokens(tokens, automata, c);
            }
            break;
        case I:
            if (c == 'I') {
                make_and_add_simple_token(tokens, automata, t_if);
            } else {
                add_variable_token(tokens, automata, 'I');
                update_tokens(tokens, automata, c);
            }
            break;
        case P:
            if (c == 'R') {
                automata->state = PR;
            } else if (c == 'L') {
                automata->state = PL;
            } else {
                add_variable_token(tokens, automata, 'P');
                update_tokens(tokens, automata, c);
            }
            break;
        case PR:
            if (c == 'I') {
                automata->state = PRI;
            } else {
                add_variable_token(tokens, automata, 'P');
                update_tokens(tokens, automata, 'R');
                update_tokens(tokens, automata, c);
            }
            break;
        case PRI:
            if (c == 'N') {
                automata->state = PRIN;
            } else {
                add_variable_token(tokens, automata, 'P');
                update_tokens(tokens, automata, 'R');
                update_tokens(tokens, automata, 'I');
                update_tokens(tokens, automata, c);
            }
            break;
        case PRIN:
            if (c == 'T') {
                make_and_add_simple_token(tokens, automata, t_print);
            } else {
                add_variable_token(tokens, automata, 'P');
                update_tokens(tokens, automata, 'R');
                update_tokens(tokens, automata, 'I');
                update_tokens(tokens, automata, 'N');
                update_tokens(tokens, automata, c);
            }
            break;
        case PL:
            if (c == 'U') {
                automata->state = PLU;
            } else {
                add_variable_token(tokens, automata, 'P');
                update_tokens(tokens, automata, 'L');
                update_tokens(tokens, automata, c);
            }
            break;
        case PLU:
            if (c == 'S') {
                make_and_add_simple_token(tokens, automata, t_plus);
            } else {
                add_variable_token(tokens, automata, 'P');
                update_tokens(tokens, automata, 'L');
                update_tokens(tokens, automata, 'U');
                update_tokens(tokens, automata, c);
            }
            break;
        case S:
            if (c == 'E') {
                automata->state = SE;
            } else {
                add_variable_token(tokens, automata, 'S');
                update_tokens(tokens, automata, c);
            }
            break;
        case SE:
            if (c == 'T') {
                make_and_add_simple_token(tokens, automata, t_set);
            } else {
                add_variable_token(tokens, automata, 'S');
                update_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, c);
            }
            break;
        case L:
            if (c == 'E') {
                automata->state = LE;
            } else {
                add_variable_token(tokens, automata, 'L');
                update_tokens(tokens, automata, c);
            }
            break;
        case LE:
            if (c == 'S') {
                automata->state = LES;
            } else if (c == 'N') {
                automata->state = LEN;
            } else {
                add_variable_token(tokens, automata, 'L');
                update_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, c);
            }
            break;
        case LES:
            if (c == 'S') {
                make_and_add_simple_token(tokens, automata, t_less);
            } else {
                add_variable_token(tokens, automata, 'L');
                update_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, 'S');
                update_tokens(tokens, automata, c);
            }
            break;
        case LEN:
            if (c == 'G') {
                automata->state = LENG;
            } else {
                add_variable_token(tokens, automata, 'L');
                update_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, 'N');
                update_tokens(tokens, automata, c);
            }
            break;
        case LENG:
            if (c == 'T') {
                automata->state = LENGT;
            } else {
                add_variable_token(tokens, automata, 'L');
                update_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, 'N');
                update_tokens(tokens, automata, 'G');
                update_tokens(tokens, automata, c);
            }
            break;
        case LENGT:
            if (c == 'H') {
                make_and_add_simple_token(tokens, automata, t_length);
            } else {
                add_variable_token(tokens, automata, 'L');
                update_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, 'N');
                update_tokens(tokens, automata, 'G');
                update_tokens(tokens, automata, 'T');
                update_tokens(tokens, automata, c);
            }
            break;
        case G:
            if (c == 'R') {
                automata->state = GR;
            } else {
                add_variable_token(tokens, automata, 'G');
                update_tokens(tokens, automata, c);
            }
            break;
        case GR:
            if (c == 'E') {
                automata->state = GRE;
            } else {
                add_variable_token(tokens, automata, 'G');
                update_tokens(tokens, automata, 'R');
                update_tokens(tokens, automata, c);
            }
            break;
        case GRE:
            if (c == 'A') {
                automata->state = GREA;
            } else {
                add_variable_token(tokens, automata, 'G');
                update_tokens(tokens, automata, 'R');
                update_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, c);
            }
            break;
        case GREA:
            if (c == 'T') {
                automata->state = GREAT;
            } else {
                add_variable_token(tokens, automata, 'G');
                update_tokens(tokens, automata, 'R');
                update_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, 'A');
                update_tokens(tokens, automata, c);
            }
            break;
        case GREAT:
            if (c == 'E') {
                automata->state = GREATE;
            } else {
                add_variable_token(tokens, automata, 'G');
                update_tokens(tokens, automata, 'R');
                update_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, 'A');
                update_tokens(tokens, automata, 'T');
                update_tokens(tokens, automata, c);
            }
            break;
        case GREATE:
            if (c == 'R') {
                make_and_add_simple_token(tokens, automata, t_greater);
            } else {
                add_variable_token(tokens, automata, 'G');
                update_tokens(tokens, automata, 'R');
                update_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, 'A');
                update_tokens(tokens, automata, 'T');
                update_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, c);
            }
            break;
        case E:
            if (c == 'Q') {
                automata->state = EQ;
            } else {
                add_variable_token(tokens, automata, 'E');
                update_tokens(tokens, automata, c);
            }
            break;
        case EQ:
            if (c == 'U') {
                automata->state = EQU;
            } else {
                add_variable_token(tokens, automata, 'E');
                update_tokens(tokens, automata, 'Q');
                update_tokens(tokens, automata, c);
            }
            break;
        case EQU:
            if (c == 'A') {
                automata->state = EQUA;
            } else {
                add_variable_token(tokens, automata, 'E');
                update_tokens(tokens, automata, 'Q');
                update_tokens(tokens, automata, 'U');
                update_tokens(tokens, automata, c);
            }
            break;
        case EQUA:
            if (c == 'L') {
                make_and_add_simple_token(tokens, automata, t_equal);
            } else {
                add_variable_token(tokens, automata, 'E');
                update_tokens(tokens, automata, 'Q');
                update_tokens(tokens, automata, 'U');
                update_tokens(tokens, automata, 'A');
                update_tokens(tokens, automata, c);
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
                add_variable_token(tokens, automata, 'C');
                update_tokens(tokens, automata, c);
            }
            break;
        case CA:
            if (c == 'R') {
                make_and_add_simple_token(tokens, automata, t_CAR);
            } else {
                add_variable_token(tokens, automata, 'C');
                update_tokens(tokens, automata, 'A');
                update_tokens(tokens, automata, c);
            }
            break;
        case CD:
            if (c == 'R') {
                make_and_add_simple_token(tokens, automata, t_CDR);
            } else {
                add_variable_token(tokens, automata, 'C');
                update_tokens(tokens, automata, 'D');
                update_tokens(tokens, automata, c);
            }
            break;
        case CO:
            if (c == 'N') {
                automata->state = CON;
            } else {
                add_variable_token(tokens, automata, 'C');
                update_tokens(tokens, automata, 'O');
                update_tokens(tokens, automata, c);
            }
            break;
        case CON:
            if (c == 'N') {
                make_and_add_simple_token(tokens, automata, t_CONS);
            } else {
                add_variable_token(tokens, automata, 'C');
                update_tokens(tokens, automata, 'O');
                update_tokens(tokens, automata, c);
            }
            break;
        default:
            throw_error("ERROR: invalid automata state\n");
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
    Token_node* current = tokens->start;
    while (current) {
        print_token(current);
        printf(" -> ");
        current = current->next;
    }
    printf("\n");
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

void* allocate_space(int num, int size) {
    void* pointer = calloc(num, size);
    if (!pointer) {
        throw_error("ERROR: unable to allocate space\n");
    }
    return pointer;
}

void throw_error(const char* error_message) {
    fputs(error_message, stderr);
    exit(EXIT_FAILURE);
}

void free_token_list(Token_list* list) {
    Token_node* current = list->start;
    while (current) {
        if (current->value) {
            if (current->value->lexeme) {
                free(current->value->lexeme);
            }
            free(current->value);
        }
    }
    free(list);
}
