#include "parse.h"

void parse_list(Token_list* list) {
    if (is_prog(list->start)) {
        printf("Parse: yes\n");
    } else {
        printf("Parse: no\n");
    }
}

bool is_prog(Token_node* start) {
    if (start->value->type != t_l_parenthesis) {
        return false;
    } else {
        return is_instrcts(&(start->next));
    }
}

bool is_instrcts(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type == t_r_parenthesis) {
        *start = start_node->next;
        return true;
    } else {
        if (!is_instrct(&start_node)) {
            return false;
        } else {
            return is_instrcts(&start_node);
        }
    }
}

bool is_instrct(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type != t_l_parenthesis) {
        return false;
    }
    start_node = start_node->next;
    if (!is_func(&start_node)) {
        return false;
    }
    if (start_node->value->type != t_r_parenthesis) {
        return false;
    } else {
        *start = start_node->next;
        return true;
    }
}

bool is_func(Token_node** start) {
    return (is_retfunc(start) || is_iofunc(start) || is_if(start) || is_loop(start));
}

bool is_retfunc(Token_node** start) {
    return (is_listfunc(start) || is_intfunc(start) || is_boolfunc(start));
}

bool is_listfunc(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type == t_CAR) {
        start_node = start_node->next;
        if (is_list(&start_node)) {
            *start = start_node;
            return true;
        }
    } else if (start_node->value->type == t_CDR) {
        start_node = start_node->next;
        if (is_list(&start_node)) {
            *start = start_node;
            return true;
        }
    } else if (start_node->value->type == t_CONS) {
        start_node = start_node->next;
        if (is_list(&start_node)) {
                if (is_list(&start_node)) {
                *start = start_node;
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
                return true;
            }
        }
    } else if (start_node->value->type == t_length) {
        start_node = start_node->next;
        if (is_list(&start_node)) {
            *start = start_node;
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
                return true;
            }
        }
    } else if (start_node->value->type == t_greater) {
        start_node = start_node->next;
        if (is_list(&start_node)) {
                if (is_list(&start_node)) {
                *start = start_node;
                return true;
            }
        }
    } else if (start_node->value->type == t_greater) {
        start_node = start_node->next;
        if (is_list(&start_node)) {
                if (is_list(&start_node)) {
                *start = start_node;
                return true;
            }
        }
    }
    return false;
}

bool is_iofunc(Token_node** start) {
    return (is_set(start) || is_print(start));
}

bool is_set(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type == t_set) {
        start_node = start_node->next;
        if (start_node->value->type == t_variable) {
            start_node = start_node->next;
            if (is_list(&start_node)) {
                *start = start_node;
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
            return true;
        } else if (start_node->value->type == t_string) {
            start_node = start_node->next;
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
    return true;
}

bool is_list(Token_node** start) {
    Token_node* start_node = *start;
    if (start_node->value->type == t_variable) {
        *start = (*start)->next;
        return true;
    } else if (start_node->value->type == t_literal) {
        *start = (*start)->next;
        return true;
    } else if (start_node->value->type == t_nil) {
        *start = (*start)->next;
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
    }
    return false;
}

void parse_test(void) {
    
}

