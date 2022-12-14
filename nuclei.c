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
            handle_start_state(token, automata, c);
            break;
        case in_literal:
            break;
        case in_variable:
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
            automata->token->literal_lexeme = (char*)allocate_space(LEXEMEMAXLEN, sizeof(char));
            automata->state = in_literal;
            break;
        case '"':
            automata->token = (Token*)allocate_space(1, sizeof(Token));
            automata->token->type = string;
            automata->token->string_lexeme = (char*)allocate_space(LEXEMEMAXLEN, sizeof(char));
            automata->state = in_string;
            break;
        case 'A'...'Z':
            if (c == 'C') {
                automata->state = C;
            }  else if (c == 'E') {
                automata->state = E;
            } else if (c == 'G') {
                automata->state = G;
            } || c == 'I' || c == 'L' || c == 'N' || c == 'P' || c == 'S' '|| c == W') {
                automata->state = 
            } else {
                automata->token = (Token*)allocate_space(1, sizeof(Token));
                automata->token->type = variable;
                automata->token->var_name = c;
                add_token(tokens, automata->token);
            }
            break;
        default:
            break;
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