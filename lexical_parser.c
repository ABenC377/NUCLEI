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
    bool in_comment = false;
    while (c != EOF) {
        if (c == '#') {
            in_comment = true;
        } else if (c == '\n') {
            in_comment = false;
        }
        if (!in_comment) {
            update_tokens(tokens, automata, c);
        }
        c = (char)fgetc(fp);
    }
    free(automata);
    return tokens;
}

void update_tokens(Token_list* tokens, Automata* automata, char c) {
    switch (automata->state) {
        case s_start:
            handle_start_state(tokens, automata, c);
            break;
        case s_in_literal:
        case s_in_string:
            handle_in_state(tokens, automata, c);
            break;
        case s_in_invalid:
            handle_invalid(tokens, automata, c);
            break;
        case s_in_variable:
            handle_variable(tokens, automata, c);
            break;
        case s_N:
            if (c == 'I') {
                automata->state = s_NI;
            } else if (is_white_space(c)) {
                add_variable(tokens, automata, 'N');
            } else if (c == ')') {
                add_variable(tokens, automata, 'N');
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_NI:
            if (c == 'L') {
                automata->state = s_NIL;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_NIL:
            if (is_white_space(c)) {
                make_and_add_simple_token(tokens, automata, t_nil);
            } else if (c == ')') {
                make_and_add_simple_token(tokens, automata, t_nil);
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_W:
            if (c == 'H') {
                automata->state = s_WH;
            } else if (is_white_space(c)) {
                add_variable(tokens, automata, 'W');
            } else if (c == ')') {
                add_variable(tokens, automata, 'W');
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_WH:
            if (c == 'I') {
                automata->state = s_WHI;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_WHI:
            if (c == 'L') {
                automata->state = s_WHIL;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_WHIL:
            if (c == 'E') {
                automata->state = s_WHILE;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_WHILE:
            if (is_white_space(c)) {
                make_and_add_simple_token(tokens, automata, t_while);
            } else if (c == ')') {
                make_and_add_simple_token(tokens, automata, t_while);
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_I:
            if (c == 'F') {
                automata->state = s_IF;
            } else if (is_white_space(c)) {
                add_variable(tokens, automata, 'I');
            } else if (c == ')') {
                add_variable(tokens, automata, 'I');
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_IF:
            if (is_white_space(c)) {
                make_and_add_simple_token(tokens, automata, t_if);
            } else if (c == ')') {
                make_and_add_simple_token(tokens, automata, t_if);
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_P:
            if (c == 'R') {
                automata->state = s_PR;
            } else if (c == 'L') {
                automata->state = s_PL;
            } else if (is_white_space(c)) {
                add_variable(tokens, automata, 'P');
            } else if (c == ')') {
                add_variable(tokens, automata, 'P');
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_PR:
            if (c == 'I') {
                automata->state = s_PRI;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_PRI:
            if (c == 'N') {
                automata->state = s_PRIN;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_PRIN:
            if (c == 'T') {
                automata->state = s_PRINT;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_PRINT:
            if (is_white_space(c)) {
                make_and_add_simple_token(tokens, automata, t_print);
            } else if (c == ')') {
                make_and_add_simple_token(tokens, automata, t_print);
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_PL:
            if (c == 'U') {
                automata->state = s_PLU;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_PLU:
            if (c == 'S') {
                automata->state = s_PLUS;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_PLUS:
            if (is_white_space(c)) {
                make_and_add_simple_token(tokens, automata, t_plus);
            } else if (c == ')') {
                make_and_add_simple_token(tokens, automata, t_plus);
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_S:
            if (c == 'E') {
                automata->state = s_SE;
            } else if (is_white_space(c)) {
                add_variable(tokens, automata, 'S');
            } else if (c == ')') {
                add_variable(tokens, automata, 'S');
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_SE:
            if (c == 'T') {
                automata->state = s_SET;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_SET:
            if (is_white_space(c)) {
                make_and_add_simple_token(tokens, automata, t_set);
            } else if (c == ')') {
                make_and_add_simple_token(tokens, automata, t_set);
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_L:
            if (c == 'E') {
                automata->state = s_LE;
            } else if (is_white_space(c)) {
                add_variable(tokens, automata, 'L');
            } else if (c == ')') {
                add_variable(tokens, automata, 'L');
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_LE:
            if (c == 'S') {
                automata->state = s_LES;
            } else if (c == 'N') {
                automata->state = s_LEN;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_LES:
            if (c == 'S') {
                automata->state = s_LESS;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_LESS:
            if (is_white_space(c)) {
                make_and_add_simple_token(tokens, automata, t_less);
            } else if (c == ')') {
                make_and_add_simple_token(tokens, automata, t_less);
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_LEN:
            if (c == 'G') {
                automata->state = s_LENG;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_LENG:
            if (c == 'T') {
                automata->state = s_LENGT;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_LENGT:
            if (c == 'H') {
                automata->state = s_LENGTH;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_LENGTH:
            if (is_white_space(c)) {
                make_and_add_simple_token(tokens, automata, t_length);
            } else if (c == ')') {
                make_and_add_simple_token(tokens, automata, t_length);
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_G:
            if (c == 'R') {
                automata->state = s_GR;
            } else if (is_white_space(c)) {
                add_variable(tokens, automata, 'G');
            } else if (c == ')') {
                add_variable(tokens, automata, 'G');
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_GR:
            if (c == 'E') {
                automata->state = s_GRE;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_GRE:
            if (c == 'A') {
                automata->state = s_GREA;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_GREA:
            if (c == 'T') {
                automata->state = s_GREAT;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_GREAT:
            if (c == 'E') {
                automata->state = s_GREATE;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_GREATE:
            if (c == 'R') {
                automata->state = s_GREATER;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_GREATER:
            if (is_white_space(c)) {
                make_and_add_simple_token(tokens, automata, t_greater);
            } else if (c == ')') {
                make_and_add_simple_token(tokens, automata, t_greater);
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_E:
            if (c == 'Q') {
                automata->state = s_EQ;
            } else if (is_white_space(c)) {
                add_variable(tokens, automata, 'E');
            } else if (c == ')') {
                add_variable(tokens, automata, 'E');
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_EQ:
            if (c == 'U') {
                automata->state = s_EQU;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_EQU:
            if (c == 'A') {
                automata->state = s_EQUA;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_EQUA:
            if (c == 'L') {
                automata->state = s_EQUAL;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_EQUAL:
            if (is_white_space(c)) {
                make_and_add_simple_token(tokens, automata, t_equal);
            } else if (c == ')') {
                make_and_add_simple_token(tokens, automata, t_equal);
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_C:
            if (c == 'A') {
                automata->state = s_CA;
            } else if (c == 'D') { 
                automata->state = s_CD;
            } else if (c == 'O') {
                automata->state = s_CO;
            } else if (is_white_space(c)) {
                add_variable(tokens, automata, 'C');
            } else if (c == ')') {
                add_variable(tokens, automata, 'C');
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_CA:
            if (c == 'R') {
                automata->state = s_CAR;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_CAR:
            if (is_white_space(c)) {
                make_and_add_simple_token(tokens, automata, t_CAR);
            } else if (c == ')') {
                make_and_add_simple_token(tokens, automata, t_CAR);
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_CD:
            if (c == 'R') {
                automata->state = s_CDR;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_CDR:
            if (is_white_space(c)) {
                make_and_add_simple_token(tokens, automata, t_CDR);
            } else if (c == ')') {
                make_and_add_simple_token(tokens, automata, t_CDR);
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_CO:
            if (c == 'N') {
                automata->state = s_CON;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_CON:
            if (c == 'S') {
                automata->state = s_CONS;
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
        case s_CONS:
            if (is_white_space(c)) {
                make_and_add_simple_token(tokens, automata, t_CONS);
            } else if (c == ')') {
                make_and_add_simple_token(tokens, automata, t_CONS);
                make_and_add_simple_token(tokens, automata, t_r_parenthesis);
            } else {
                handle_invalid(tokens, automata, c);
            }
            break;
    }
}

bool is_white_space(char c) {
    return (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r');
}

void add_variable(Token_list* tokens, Automata* automata, char name) {
    automata->state = s_start;
    Token* new_token = (Token*)allocate_space(1, sizeof(Token));
    new_token->type = t_variable;
    new_token->var_name = name;
    add_token(tokens, new_token);
}

// This only works for token types that do not have a lexeme or other value
void make_and_add_simple_token(Token_list* tokens, Automata* automata, token_type type) {
    automata->state = s_start;
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
            automata->state = s_in_literal;
            break;
        case '"':
            automata->token = (Token*)allocate_space(1, sizeof(Token));
            automata->token->type = t_string;
            automata->token->lexeme = (char*)allocate_space(LEXEMEMAXLEN, sizeof(char));
            automata->state = s_in_string;
            break;
        case 'A'...'Z':
            if (c == 'C') {
                automata->state = s_C;
            }  else if (c == 'E') {
                automata->state = s_E;
            } else if (c == 'G') {
                automata->state = s_G;
            } else if (c == 'I') {
                automata->state = s_I;
            } else if (c == 'L') {
                automata->state = s_L;
            } else if (c == 'N') {
                automata->state = s_N;
            } else if (c == 'P') {
                automata->state = s_P;
            } else if (c == 'S') {
                automata->state = s_S;
            } else if (c == 'W') {
                automata->state = s_W;
            } else {
                automata->token = (Token*)allocate_space(1, sizeof(Token));
                automata->token->type = t_variable;
                automata->token->var_name = c;
                automata->state = s_in_variable;
            }
            break;
        default:
            automata->token = (Token*)allocate_space(1, sizeof(Token));
            automata->token->type = t_invalid;
            add_token(tokens, automata->token);
            break;
    }
}


void handle_in_state(Token_list* tokens, Automata* automata, char c) {
    if ((automata->state == s_in_literal) && (c == SINGLEQUOTE)) {
        add_token(tokens, automata->token);
        automata->state = s_start;
    } else if ((automata->state == s_in_string) && (c == '"')) {
        add_token(tokens, automata->token);
        automata->state = s_start;
    } else {
        int index = 0;
        while ((index < LEXEMEMAXLEN) && (automata->token->lexeme[index] != '\0')) {
            index++;
        }
        if (index == LEXEMEMAXLEN) {
            throw_error("ERROR: literal lexeme greater than maximum allowed string length\n");
        }
        automata->token->lexeme[index] = c;
    }
}

void handle_invalid(Token_list* tokens, Automata* automata, char c) {
    automata->state = s_in_invalid;
    if (is_white_space(c)) {
        automata->state = s_start;
        make_and_add_simple_token(tokens, automata, t_invalid);
    } else if (c == ')') {
        automata->state = s_start;
        make_and_add_simple_token(tokens, automata, t_invalid);
        make_and_add_simple_token(tokens, automata, t_r_parenthesis);
    }
}

void handle_variable(Token_list* tokens, Automata* automata, char c) {
    if (is_white_space(c)) {
        add_token(tokens, automata->token);
        automata->state = s_start;
    } else if (c == ')') {
        add_variable(tokens, automata, automata->token->var_name);
        make_and_add_simple_token(tokens, automata, t_r_parenthesis);
    } else {
        automata->token->type = t_invalid;
        automata->state = s_in_invalid;
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
    if (node) {
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

void lexical_analysis_test(void) {
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
    test_automata->state = s_N;
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_NI);
    test_c = 'L';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_NIL);
    test_c = ' ';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_start);
    assert(test_tokens->end->value->type == t_nil);
    
    // States to WHILE
    test_c = 'W';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_W);
    test_c = 'H';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_WH);
    test_c = 'I';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_WHI);
    test_c = 'L';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_WHIL);
    test_c = 'E';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_WHILE);
    test_c = ' ';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_start);
    assert(test_tokens->end->value->type == t_while);
    
    // states to IF
    test_c = 'I';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_I);
    test_c = 'F';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_IF);
    test_c = ' ';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_start);
    assert(test_tokens->end->value->type == t_if);
    
    // states to PRINT
    test_c = 'P';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_P);
    test_c = 'R';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_PR);
    test_c = 'I';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_PRI);
    test_c = 'N';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_PRIN);
    test_c = 'T';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_PRINT);
    test_c = ' ';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_start);
    assert(test_tokens->end->value->type == t_print);
    
    // states to PLUS
    test_c = 'P';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_P);
    test_c = 'L';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_PL);
    test_c = 'U';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_PLU);
    test_c = 'S';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_PLUS);
    test_c = ' ';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_start);
    assert(test_tokens->end->value->type == t_plus);
    
    // states to SET
    test_c = 'S';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_S);
    test_c = 'E';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_SE);
    test_c = 'T';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_SET);
    test_c = ' ';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_start);
    assert(test_tokens->end->value->type == t_set);
    
    // states to LENGTH
    test_c = 'L';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_L);
    test_c = 'E';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_LE);
    test_c = 'N';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_LEN);
    test_c = 'G';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_LENG);
    test_c = 'T';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_LENGT);
    test_c = 'H';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_LENGTH);
    test_c = ' ';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_start);
    assert(test_tokens->end->value->type == t_length);
    
    // states to LESS
    test_c = 'L';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_L);
    test_c = 'E';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_LE);
    test_c = 'S';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_LES);
    test_c = 'S';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_LESS);
    test_c = ' ';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_start);
    assert(test_tokens->end->value->type == t_less);
    
    // states to GREATER
    test_c = 'G';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_G);
    test_c = 'R';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_GR);
    test_c = 'E';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_GRE);
    test_c = 'A';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_GREA);
    test_c = 'T';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_GREAT);
    test_c = 'E';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_GREATE);
    test_c = 'R';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_GREATER);
    test_c = ' ';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_start);
    assert(test_tokens->end->value->type == t_greater);
    
    // states to EQUAL
    test_c = 'E';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_E);
    test_c = 'Q';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_EQ);
    test_c = 'U';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_EQU);
    test_c = 'A';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_EQUA);
    test_c = 'L';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_EQUAL);
    test_c = ' ';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_start);
    assert(test_tokens->end->value->type == t_equal);
    
    // states to CAR
    test_c = 'C';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_C);
    test_c = 'A';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_CA);
    test_c = 'R';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_CAR);
    test_c = ' ';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_start);
    assert(test_tokens->end->value->type == t_CAR);
    
    // states to CDR
    test_c = 'C';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_C);
    test_c = 'D';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_CD);
    test_c = 'R';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_CDR);
    test_c = ' ';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_start);
    assert(test_tokens->end->value->type == t_CDR);
    
    // states to CONS
    test_c = 'C';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_C);
    test_c = 'O';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_CO);
    test_c = 'N';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_CON);
    test_c = 'S';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_CONS);
    test_c = ' ';
    update_tokens(test_tokens, test_automata, test_c);
    assert(test_automata->state == s_start);
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
