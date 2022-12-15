int main(int argc, char* argv[]) {
    
    // Set up the file to be scanned
    
    Token_list* list = run_lexical_analyser(file_name);
    
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
        update_tokens(tokens, automata, c);
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
        case in_variable:
            handle_in_state(tokens, automata, c);
            break;
        case N:
            if (c == 'I') {
                automata->state = NI;
            } else {
                automata->state = start;
                update_tokens(tokens, automata, 'N') // Make sure this doesn't lead to an infinite loop
                update_tokens(tokens, automata, c);
            }
            break;
        case NI:
            if (c == 'L') {
                make_and_add_simple_token(token, automata, t_nil);
            } else {
                automata->state = start;
                update_tokens(tokens, automata, 'N');
                update_tokens(tokens, automata, 'I');
                update_tokens(tokens, automata, c);
            }
            break;
        case W:
            if (c == 'H') {
                automata->state = WH;
            } else {
                automata->state = start;
                update_tokens(tokens, automata, 'W');
                update_tokens(tokens, automata, c);
            }
            break;
        case WH:
            if (c == 'I') {
                automata->state = WHI;
            } else {
                automata->state = start;
                update_tokens(tokens, automata, 'W');
                update_tokens(tokens, automata, 'H');
                update_tokens(tokens, automata, c);
            }
            break;
        case WHI:
            if (c == 'L') {
                automata->state = WHIL;
            } else {
                automata->state = start;
                update_tokens(tokens, automata, 'W');
                update_tokens(tokens, automata, 'H');
                update_tokens(tokens, automata, 'I');
                update_tokens(tokens, automata, c);
            }
            break;
        case WHIL:
            if (c == 'E') {
                make_and_add_simple_token(tokens, automata, t_while);
            } else {
                automata->state = start;
                update_tokens(tokens, automata, 'W');
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
                automata->state = start;
                udpate_tokens(tokens, automata, 'I');
                update_tokens(tokens, automata, c);
            }
            break;
        case P:
            if (c == 'R') {
                automata->state = PR;
            } else if (c == 'L') {
                automata->state = PL;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'P');
                update_tokens(tokens, automata, c);
            }
            break;
        case PR:
            if (c == 'I') {
                autoamta->sate = PRI;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'P');
                udpate_tokens(tokens, automata, 'R');
                update_tokens(tokens, automata, c);
            }
            break;
        case PRI:
            if (c == 'N') {
                automata->state = PRIN;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'P');
                udpate_tokens(tokens, automata, 'R');
                udpate_tokens(tokens, automata, 'I');
                update_tokens(tokens, automata, c);
            }
            break;
        case PRIN:
            if (c == 'T') {
                make_and_add_simple_token(tokens, automata, t_print);
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'P');
                udpate_tokens(tokens, automata, 'R');
                udpate_tokens(tokens, automata, 'I');
                udpate_tokens(tokens, automata, 'N');
                update_tokens(tokens, automata, c);
            }
            break;
        case PL:
            if (c == 'U') {
                automata->state = PLU;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'P');
                udpate_tokens(tokens, automata, 'L');
                update_tokens(tokens, automata, c);
            }
            break;
        case PLU:
            if (c == 'S') {
                make_and_add_simple_token(tokens, automata, t_plus);
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'P');
                udpate_tokens(tokens, automata, 'L');
                udpate_tokens(tokens, automata, 'U');
                update_tokens(tokens, automata, c);
            }
            break;
        case S:
            if (c == 'E') {
                automata->state = SE;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'S');
                update_tokens(tokens, automata, c);
            }
            break;
        case SE:
            if (c == 'T') {
                make_and_add_simple_token(tokens, automata, t_set);
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'S');
                udpate_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, c);
            }
            break;
        case L:
            if (c == 'E') {
                automata->state = LE;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'L');
                update_tokens(tokens, automata, c);
            }
            break;
        case LE:
            if (c == 'S') {
                automata->state = LES;
            } else if (c == 'N') {
                automata->state = LEN;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'L');
                udpate_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, c);
            }
            break;
        case LES:
            if (c == 'S') {
                make_and_add_simple_token(tokens, automata, t_less);
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'L');
                udpate_tokens(tokens, automata, 'E');
                udpate_tokens(tokens, automata, 'S');
                update_tokens(tokens, automata, c);
            }
            break;
        case LEN:
            if (c == 'G') {
                automata->state = LENG;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'L');
                udpate_tokens(tokens, automata, 'E');
                udpate_tokens(tokens, automata, 'N');
                update_tokens(tokens, automata, c);
            }
            break;
        case LENG:
            if (c == 'T') {
                automata->state = LENGT;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'L');
                udpate_tokens(tokens, automata, 'E');
                udpate_tokens(tokens, automata, 'N');
                udpate_tokens(tokens, automata, 'G');
                update_tokens(tokens, automata, c);
            }
            break;
        case LENGT:
            if (c == 'H') {
                make_and_add_simple_token(tokens, automata, t_length);
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'L');
                udpate_tokens(tokens, automata, 'E');
                udpate_tokens(tokens, automata, 'N');
                udpate_tokens(tokens, automata, 'G');
                udpate_tokens(tokens, automata, 'T');
                update_tokens(tokens, automata, c);
            }
            break;
        case G:
            if (c == 'R') {
                automata->state = GR;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'G');
                update_tokens(tokens, automata, c);
            }
            break;
        case GR:
            if (c == 'E') {
                automata->state = GRE;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'G');
                udpate_tokens(tokens, automata, 'R');
                update_tokens(tokens, automata, c);
            }
            break;
        case GRE:
            if (c == 'A') {
                automata->state = GREA;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'G');
                udpate_tokens(tokens, automata, 'R');
                udpate_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, c);
            }
            break;
        case GREA:
            if (c == 'T') {
                automata->state = GREAT;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'G');
                udpate_tokens(tokens, automata, 'R');
                udpate_tokens(tokens, automata, 'E');
                udpate_tokens(tokens, automata, 'A');
                update_tokens(tokens, automata, c);
            }
            break;
        case GREAT:
            if (c == 'E') {
                automata->state = GREATE;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'G');
                udpate_tokens(tokens, automata, 'R');
                udpate_tokens(tokens, automata, 'E');
                udpate_tokens(tokens, automata, 'A');
                udpate_tokens(tokens, automata, 'T');
                update_tokens(tokens, automata, c);
            }
            break;
        case GREATE:
            if (c == 'R') {
                make_and_add_simple_token(tokens, automata, t_greater)
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'G');
                udpate_tokens(tokens, automata, 'R');
                udpate_tokens(tokens, automata, 'E');
                udpate_tokens(tokens, automata, 'A');
                udpate_tokens(tokens, automata, 'T');
                udpate_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, c);
            }
            break;
        case E:
            if (c == 'Q') {
                automata->state = EQ;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'E');
                update_tokens(tokens, automata, c);
            }
            break;
        case EQ:
            if (c == 'U') {
                automata->state = EQU;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'E');
                udpate_tokens(tokens, automata, 'Q');
                update_tokens(tokens, automata, c);
            }
            break;
        case EQU:
            if (c == 'A') {
                automata->state = EQUA;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'E');
                udpate_tokens(tokens, automata, 'Q');
                udpate_tokens(tokens, automata, 'U');
                update_tokens(tokens, automata, c);
            }
            break;
        case EQUA:
            if (c == 'L') {
                make_and_add_simple_token(tokens, automata, t_equal);
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'E');
                udpate_tokens(tokens, automata, 'Q');
                udpate_tokens(tokens, automata, 'U');
                udpate_tokens(tokens, automata, 'A');
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
                automata->state = start;
                udpate_tokens(tokens, automata, 'C');
                update_tokens(tokens, automata, c);
            }
            break;
        case CA:
            if (c == 'R') {
                make_and_add_simple_token(tokens, automata, t_CAR);
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'C');
                udpate_tokens(tokens, automata, 'A');
                update_tokens(tokens, automata, c);
            }
            break;
        case CD:
            if (c == 'R') {
                make_and_add_simple_token(tokens, automata, t_CDR);
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'C');
                udpate_tokens(tokens, automata, 'D');
                update_tokens(tokens, automata, c);
            }
            break;
        case CO:
            if (c == 'N') {
                automata->state = CON;
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'C');
                udpate_tokens(tokens, automata, 'O');
                update_tokens(tokens, automata, c);
            }
            break;
        case CON:
            if (c == 'N') {
                make_and_add_simple_token(tokens, automata, t_CONS);
            } else {
                automata->state = start;
                udpate_tokens(tokens, automata, 'C');
                udpate_tokens(tokens, automata, 'O');
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
                automata->token = (Token*)allocate_space(1, sizeof(Token));
                automata->token->type = t_variable;
                automata->token->var_name = c;
                add_token(tokens, automata->token);
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
            throw_error("ERROR: literal lexeme greater than maximum allowed string length\n")
        }
        automata->token->lexeme[index] = c;
    }
}

void print_tokens(Token_list* tokens) {
    
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
