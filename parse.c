#include "parse.h"

void parse_list(Token_list* list) {
    //parse_print_tokens(list); // debugging
    if (is_prog(&(list->start))) {
        printf("Parsed OK\n");
    } else {
        fprintf(stderr, "Parser failed - this file does not contain a syntactically valid NUCLEI program\n");
        exit(EXIT_FAILURE);
    }
}

bool is_prog(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type != t_l_parenthesis) {
        return false;
    } 
    start_node = start_node->next;
//    printf("Open parenthesis found (program)\n"); // debugging
//    parse_print_remaining_tokens(start_node); // debugging
    if (is_instrcts(&start_node)) {
//        printf("Program found\n"); // debugging
//        parse_print_remaining_tokens(start_node); // debugging
        return true;
    }
    return false;
}

bool is_instrcts(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type == t_r_parenthesis) {
        *start = start_node->next;
//        printf("Instructions found (closing parenthesis)\n"); // debugging
//        parse_print_remaining_tokens(start_node); // debugging
        return true;
    } else {
        if (!is_instrct(&start_node)) {
            return false;
        } else if (is_instrcts(&start_node)) {
//            printf("Instructions found\n"); // debugging
//            parse_print_remaining_tokens(start_node); // debugging
            *start = start_node;
            return true;
        }
    }
    return false;
}

bool is_instrct(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type != t_l_parenthesis) {
        return false;
    }
    start_node = start_node->next;
//    printf("in_instrct - open parenthesis found\n"); // debugging
//    parse_print_remaining_tokens(start_node); // debugging
    if (!is_func(&start_node)) {
        return false;
    }
//    printf("in_instrc - function found\n"); // debugging
//    parse_print_remaining_tokens(start_node); // debugging
    if (start_node->value->type != t_r_parenthesis) {
//        printf("in_instrc - no close bracket found\n"); // debugging
//        parse_print_remaining_tokens(start_node); // debugging
        return false;
    } else {
        *start = start_node->next;
//        printf("Instruction found (i.e., close parenthesis found)\n"); // debugging
//        parse_print_remaining_tokens(start_node); // debugging
        return true;
    }
}

bool is_func(Token_node** start) {
    if (is_retfunc(start) || is_iofunc(start) || is_if(start) || is_loop(start)) {
//        printf("Function found\n"); // debugging
//        parse_print_remaining_tokens(*start); // debugging
        return true;
    }
    return false;
}

bool is_retfunc(Token_node** start) {
    if (is_listfunc(start) || is_intfunc(start) || is_boolfunc(start)) {
//        printf("Return function found\n"); // debugging
//        parse_print_remaining_tokens(*start); // debugging
        return true;
    }
    return false;
}

bool is_listfunc(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type == t_CAR) {
        start_node = start_node->next;
        if (is_list(&start_node)) {
            *start = start_node;
//            printf("List function found (CAR)\n"); // debugging
//            parse_print_remaining_tokens(start_node); // debugging
            return true;
        }
    } else if (start_node->value->type == t_CDR) {
        start_node = start_node->next;
        if (is_list(&start_node)) {
            *start = start_node;
//            printf("List function found (CDR)\n"); // debugging
//            parse_print_remaining_tokens(start_node); // debugging
            return true;
        }
    } else if (start_node->value->type == t_CONS) {
        start_node = start_node->next;
        if (is_list(&start_node)) {
                if (is_list(&start_node)) {
                *start = start_node;
//                printf("List function found (CONS)\n"); // debugging
//                parse_print_remaining_tokens(start_node); // debugging
                return true;
            }
        }
    }
    return false;
}

bool is_intfunc(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type == t_plus) {
        start_node = start_node->next;
        if (is_list(&start_node)) {
            if (is_list(&start_node)) {
                *start = start_node;
//                printf("Int function found (plus)\n"); // debugging
//                parse_print_remaining_tokens(start_node); // debugging
                return true;
            }
        }
    } else if (start_node->value->type == t_length) {
        start_node = start_node->next;
        if (is_list(&start_node)) {
            *start = start_node;
//            printf("Int function found (length)\n"); // debugging
//            parse_print_remaining_tokens(start_node); // debugging
            return true;
        }
    }
    return false;
}

bool is_boolfunc(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type == t_less) {
        start_node = start_node->next;
        if (is_list(&start_node)) {
            if (is_list(&start_node)) {
                *start = start_node;
//                printf("Bool function found (less)\n"); // debugging
//                parse_print_remaining_tokens(start_node); // debugging
                return true;
            }
        }
    } else if (start_node->value->type == t_greater) {
        start_node = start_node->next;
        if (is_list(&start_node)) {
            if (is_list(&start_node)) {
                *start = start_node;
//                printf("Bool function found (greater)\n"); // debugging
//                parse_print_remaining_tokens(start_node); // debugging
                return true;
            }
        }
    } else if (start_node->value->type == t_equal) {
        start_node = start_node->next;
        if (is_list(&start_node)) {
            if (is_list(&start_node)) {
                *start = start_node;
//                printf("Bool function found (equal)\n"); // debugging
//                parse_print_remaining_tokens(start_node); // debugging
                return true;
            }
        }
    }
    return false;
}

bool is_iofunc(Token_node** start) {
    if (is_set(start) || is_print(start)) {
//        printf("IO function found\n"); // debugging
//        parse_print_remaining_tokens(*start); // debugging
        return true;
    }
    return false;
}

bool is_set(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type == t_set) {
        start_node = start_node->next;
        if (start_node->value->type == t_variable) {
            start_node = start_node->next;
            if (is_list(&start_node)) {
                *start = start_node;
//                printf("Set found\n"); // debugging
//                parse_print_remaining_tokens(start_node); // debugging
                return true;
            }
        }
    }
    return false;
}

bool is_print(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type == t_print) {
        start_node = start_node->next;
        if (is_list(&start_node)) {
            *start = start_node;
//            printf("Print found (list)\n"); // debugging
//            parse_print_remaining_tokens(start_node); // debugging
            return true;
        } else if (start_node->value->type == t_string) {
            start_node = start_node->next;
//            printf("Printf found string\n"); // debugging
//            parse_print_remaining_tokens(start_node); // debugging
            *start = start_node;
            return true;
        }
    }
    return false;
}

bool is_if(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type != t_if) {
        return false;
    }
    start_node = start_node->next;
    if (start_node->value->type != t_l_parenthesis) {
        return false;
    }
    start_node = start_node->next;
    if (!is_boolfunc(&start_node)) {
        return false;
    }
    if (start_node->value->type != t_r_parenthesis) {
        return false;
    }
    start_node = start_node->next;
    if (start_node->value->type != t_l_parenthesis) {
        return false;
    }
    start_node = start_node->next;
    if (!is_instrcts(&start_node)) {
        return false;
    }
    if (start_node->value->type != t_l_parenthesis) {
        return false;
    }
    start_node = start_node->next;
    if (!is_instrcts(&start_node)) {
        return false;
    }
    *start = start_node;
//    printf("If found\n"); // debugging
//    parse_print_remaining_tokens(start_node); // debugging
    return true;
}

bool is_loop(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type != t_while) {
        return false;
    }
    start_node = start_node->next;
    if (start_node->value->type != t_l_parenthesis) {
        return false;
    }
    start_node = start_node->next;
    if (!is_boolfunc(&start_node)) {
        return false;
    }
    if (start_node->value->type != t_r_parenthesis) {
        return false;
    }
    start_node = start_node->next;
    if (start_node->value->type != t_l_parenthesis) {
        return false;
    }
    start_node = start_node->next;
    if (!is_instrcts(&start_node)) {
        return false;
    }
    *start = start_node;
//    printf("Loop found\n"); // debugging
//    parse_print_remaining_tokens(start_node); // debugging
    return true;
}

bool is_list(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type == t_variable) {
        *start = (*start)->next;
//        printf("List found (variable)\n"); // debugging
//        parse_print_remaining_tokens(start_node); // debugging
        return true;
    } else if ((start_node->value->type == t_literal) && is_valid_literal(start_node->value->lexeme)) {
        *start = (*start)->next;
//        printf("List found (literal)\n"); // debugging
//        parse_print_remaining_tokens(start_node); // debugging
        return true;
    } else if (start_node->value->type == t_nil) {
        *start = (*start)->next;
//        printf("List found (nil)\n"); // debugging
//        parse_print_remaining_tokens(start_node); // debugging
        return true;
    } else if (start_node->value->type == t_l_parenthesis) {
        start_node = start_node->next;
        if (!is_retfunc(&start_node)) {
            return false;
        }
        if (start_node->value->type != t_r_parenthesis) {
            return false;
        }
        start_node = start_node->next;
        *start = start_node;
//        printf("List found (return function)\n"); // debugging
//        parse_print_remaining_tokens(start_node); // debugging
        return true;
    }
    return false;
}

bool is_valid_literal(char* literal_string) {
    return (even_parentheses(literal_string) && valid_chars(literal_string) && no_hanging_atoms(literal_string));
}

bool even_parentheses(char* literal_string) {
    int open_parentheses = 0;
    int str_len = strlen(literal_string);
    for (int i = 0; i < str_len; i++) {
        if (literal_string[i] == '(') {
            open_parentheses++;
        } else if (literal_string[i] == ')') {
            open_parentheses--;
        }
        
        if (open_parentheses < 0) {
            return false;
        }
    }
    
    return (open_parentheses == 0);
}

bool valid_chars(char* literal_string) {
    int str_len = strlen(literal_string);
    for (int i = 0; i < str_len; i++) {
        if (!valid_char(literal_string[i])) {
            return false;
        }
    }
    return true;
}

bool valid_char(char c) {
    switch (c) {
        case '0' ... '9':
        case '(':
        case ')':
        case '-':
        case ' ':
        case '\n':
        case '\t':
        case '\v':
        case '\f':
        case '\r':
            return true;
            break;
        default:
            return false;
            break;
    }
}

bool no_hanging_atoms(char* literal_string) {
    // printf("literal string is %s\n", literal_string); // debugging
    int str_len = strlen(literal_string);
    if (literal_string[0] == '(') {
        int open_parentheses = 1;
        for (int i = 1; i < str_len; i++) {
            if (open_parentheses == 0) {
                // printf("hanging atom - chars after final closed parenthesis\n"); // debugging
                return false;
            }
            if (literal_string[i] == '(') {
                open_parentheses++;
            } else if (literal_string[i] == ')') {
                open_parentheses--;
            }
        }
        return true;
    } else if ((literal_string[0] != '-') && !(isdigit(literal_string[0]))) {
        // printf("hanging atom - first char not ( or a number\n"); // debugging
        return false;
    } else {
        for (int i = 1; i < str_len; i++) {
            if (!(isdigit(literal_string[i]))) {
                // printf("hanging atom - characters after first number\n"); // debugging
                return false;
            }
        }
        return true;
    }
    
}

void parse_test(void) {
    
}






void parse_print_tokens(Token_list* tokens) {
    if (!(tokens) || !(tokens->start)) {
        printf("Empty list - cannot print\n");
    }
    Token_node* current = tokens->start;
    while (current) {
        parse_print_token(current);
        printf(" -> ");
        current = current->next;
    }
    printf("END\n");
}

void parse_print_remaining_tokens(Token_node* node) {
    Token_node* current = node;
    while (current) {
        parse_print_token(current);
        printf(" -> ");
        current = current->next;
    }
    printf("END\n");
}

void parse_print_token(Token_node* node) {
    switch (node->value->type) {
        case t_literal:
            printf("<LITERAL - %s>", node->value->lexeme);
            break;
        case t_string:
            printf("<STRING - %s>", node->value->lexeme);
            break;
        case t_variable:
            printf("<VARIABLE - %c>", node->value->var_name);
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
