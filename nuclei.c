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
            break;
        case NI:
            break;
        case W:
            break;
        case WH:
            break;
        case WHI:
            break;
        case WHIL:
            break;
        case I:
            break;
        case P:
            break;
        case PR:
            break;
        case PRI:
            break;
        case PRIN:
            break;
        case PL:
            break;
        case PLU:
            break;
        case S:
            break;
        case SE:
            break;
        case L:
            break;
        case LE:
            break;
        case LES:
            break;
        case LEN:
            break;
        case LENG:
            break;
        case LENGT:
            break;
        case G:
            break;
        case GR:
            break;
        case GRE:
            break;
        case GREA:
            break;
        case GREAT:
            break;
        case GREATE:
            break;
        case E:
            break;
        case EQ:
            break;
        case EQU:
            break;
        case EQUA:
            break;
        case C:
            break;
        case CA:
            break;
        case CD:
            break;
        case CO:
            break;
        case CON:
            break;
        default:
            throw_error("ERROR: invalid automata state\n");
    }
}

void handle_start_state(Token_list* tokens, Automata* automata, char c) {
    switch (c) {
        case ' ':
            break;
        case ')':
            automata->token = (Token*)allocate_space(1, sizeof(Token));
            automata->token->type = r_parenthesis;
            add_token(tokens, automata->token);
            break;
        case '(':
            automata->token = (Token*)allocate_space(1, sizeof(Token));
            automata->token->type = l_parenthesis;
            add_token(tokens, automata->token);
            break;
        case SINGLEQUOTE:
            automata->token = (Token*)allocate_space(1, sizeof(Token));
            automata->token->type = literal;
            automata->token->lexeme = (char*)allocate_space(LEXEMEMAXLEN, sizeof(char));
            automata->state = in_literal;
            break;
        case '"':
            automata->token = (Token*)allocate_space(1, sizeof(Token));
            automata->token->type = string;
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
                automata->token->type = variable;
                automata->token->var_name = c;
                add_token(tokens, automata->token);
            }
            break;
        default:
            automata->token = (Token*)allocate_space(1, sizeof(Token));
            automata->token->type = INVALID;
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