#include "nuclei.h"

int main(int argc, char* argv[]) {
    test();
    check_inputs(argc, argv);
    // Set up the file to be scanned
    Token_list* list = run_lexical_analyser(argc, argv);

    parse_list(list);
    
    free_token_list(list);
}

void check_inputs(int argc, char* argv[]) {
    if (argc != 2) {
        throw_error("ERROR: invalid number of command line arguments\n");
    }
    (void)argv; // To get rid of warning for now
}

void parse_list(Token_list* list) {
    Syntax_tree* tree = (Syntax_tree*)allocate_space(1, sizeof(Syntax_tree));
    Token_node* current = list->start;
    Prog_log* log = (Prog_log*)allocate_space(1, sizeof(Prog_log));
    log->executing = true;
    tree->program = descend_recursively(&current, log);
    #ifdef INTERP
    if (log->interp_error) {
        printf("Interpreter failed\n");
    }
    #endif
    #ifdef EXT
    printf((log->parser_error) ? "Not parsed correctly\n" : "Parsed OK\n");
    if (log->parser_error || log->interp_error) {
        print_log(log);
    }
    #else
    printf((log->parser_error) ? "Not parsed correctly (run extension to see more detail)\n" : "Parsed OK\n");
    #endif
    free_tree(tree); free_log(log);
}

Tree_node* descend_recursively(Token_node** current, Prog_log* log) {
    Tree_node* program = make_node(PROG);
    if (!next_token_is(current, 1, t_l_parenthesis)) {
        return parser_fails(log, (*current)->value, 
        "Expecting opening parenthesis before instructions in program\n");
    } else {
        program->child1 = handle_INSTRCTS(current, log);
    }
    return program;
}

Tree_node* handle_INSTRCTS(Token_node** current, Prog_log* log) {
    if (!(*current)) {
        return parser_fails(log, (*current)->value, 
        "Expecting further instructions\n");
    } else if (next_token_is(current, 1, t_r_parenthesis)) {
        return NULL;
    } else {
        Tree_node* instructions = make_node(INSTRCTS);
        instructions->child1 = handle_INSTRCT(current, log);
        if (instructions->child1->type == ERROR_NODE) {
            *current = (*current)->next;
        }
        instructions->child2 = handle_INSTRCTS(current, log);
        return instructions;
    }
}

Tree_node* handle_INSTRCT(Token_node** current, Prog_log* log) {
    if (!next_token_is(current, 1, t_l_parenthesis)) {
        return parser_fails(log, (*current)->value, 
        "Expecting opening parenthesis to begin function\n");
    }
    Tree_node* instruction = make_node(INSTRCT);
    instruction->child1 = handle_FUNC(current, log);
    if (!next_token_is(current, 1, t_r_parenthesis)) {
        return parser_fails(log, (*current)->value, 
        "Expecting closing parenthesis after function in instruction\n");
    }
    return instruction;
}

Tree_node* handle_FUNC(Token_node** current, Prog_log* log) {
    Tree_node* function = make_node(FUNC);
    if (is_RETFUNC(*current)) {
        function->child1 = handle_RETFUNC(current, log);
    } else if (is_IOFUNC(*current)) {
        function->child1 = handle_IOFUNC(current, log);
    } else if (is_IF(*current)) {
        function->child1 = handle_IF(current, log);
    } else if (is_LOOP(*current)) {
        function->child1 = handle_LOOP(current, log);
    } else {
        return parser_fails(log, (*current)->value, 
        "Expecting function in instruction\n");
    }
    return function;
}

bool is_RETFUNC(Token_node* current) {
    return (is_LISTFUNC(current) || is_INTFUNC(current) || is_BOOLFUNC(current));
}

Tree_node* handle_RETFUNC(Token_node** current, Prog_log* log) {
    Tree_node* ret_function = make_node(RETFUNC);
    if (is_LISTFUNC(*current)) {
        ret_function->child1 = handle_LISTFUNC(current, log);
    } else if (is_INTFUNC(*current)) {
        ret_function->child1 = handle_INTFUNC(current, log);
    } else if (is_BOOLFUNC(*current)) {
        ret_function->child1 = handle_BOOLFUNC(current, log);
    } else {
        return parser_fails(log, (*current)->value, 
        "Expecting list, int, or bool function in return function\n");
    }
    #ifdef INTERP
    if (log->executing) {
        ret_function->list = lisp_copy(ret_function->child1->list);
        lisp_free(ret_function->child1->list);
        ret_function->child1->list = NULL;
    }
    #endif
    return ret_function;
}

bool is_IOFUNC(Token_node* current) {
    return (current->value->type == t_set || current->value->type == t_print);
}

Tree_node* handle_IOFUNC(Token_node** current, Prog_log* log) {
    Tree_node* io_function = make_node(IOFUNC);
    if ((*current)->value->type == t_set) {
        io_function->child1 = handle_SET(current, log);
    } else if ((*current)->value->type == t_print) {
        io_function->child1 = handle_PRINT(current, log);
    } else {
        return parser_fails(log, (*current)->value, 
        "Expecting 'SET' or 'PRINT' in I/O function\n");
    }
    return io_function;
}

Tree_node* handle_SET(Token_node** current, Prog_log* log) {
    if (!next_token_is(current, 1, t_set)) {
        return parser_fails(log, (*current)->value, 
        "Expecting 'SET' in set statement\n");
    }
    Tree_node* set = make_node(SET); 
    set->child1 = handle_VAR(current, log);
    set->child2 = handle_LIST(current, log);
    #ifdef INTERP
    if (log->executing) {
        lisp_free(set->child1->list); set->child1->list = NULL;
        Lisp** var_p = &(log->variables[set->child1->var_name - 'A']);
        if (*var_p != NULL) {
            lisp_free(*var_p); *var_p = NULL;
        }
        *var_p = lisp_copy(set->child2->list);
        lisp_free(set->child2->list); set->child2->list = NULL;
    }
    #endif
    return set;
}

bool is_IF(Token_node* current) {
    return (current->value->type == t_if);
}

Tree_node* handle_IF(Token_node** current, Prog_log* log) {
    if (!next_token_is(current, 1, t_if) || !next_token_is(current, 1, '(')) {
        return parser_fails(log, (*current)->value, 
        "Expecting IF before parenthesis in if statement.\n");
    }
    Tree_node* if_node = make_node(IF);
    if_node->child1 = handle_BOOLFUNC(current, log);
    #ifdef INTERP
        bool execution_state = log->executing;
        bool execute = (lisp_get_val(if_node->child1->list) == 1);
        lisp_free(if_node->child1->list); if_node->child1->list = NULL;
    #endif
    if (!next_token_is(current, 1, ')') || !next_token_is(current, 1, '(')) {
        free_node(if_node);
        return parser_fails(log, (*current)->value, 
        "Expecting parentheses between bool and first statement in if statement.\n");
    }
    #ifdef INTERP
        log->executing = (execution_state == true && execute == true);
    #endif
    if_node->child2 = handle_INSTRCTS(current, log);
    if (!next_token_is(current, 1, t_l_parenthesis)) {
        free_node(if_node);
        return parser_fails(log, (*current)->value, 
        "Expecting opening parenthesis before second instructions in if statement\n");
    }
    #ifdef INTERP
        log->executing = (execution_state == true && execute == false);
    #endif 
    if_node->child3 = handle_INSTRCTS(current, log);
    #ifdef INTERP 
        log->executing = execution_state;
    #endif 
    return if_node;
}

bool is_LOOP(Token_node* current) {
    return (current->value->type == t_while);
}

Tree_node* handle_LOOP(Token_node** current, Prog_log* log) {
    #ifdef INTERP
        Token_node* loop_start = (*current);
    #endif
    if (!next_token_is(current, 1, t_while) || !next_token_is(current, 1, '(')) {
        return parser_fails(log, (*current)->value, 
        "Expecting WHILE and opening parenthesis before bool function in loop\n");
    }
    Tree_node* loop = make_node(LOOP);
    loop->child1 = handle_BOOLFUNC(current, log);
    #ifdef INTERP
        bool execution_state = log->executing;
        bool execute = (lisp_get_val(loop->child1->list) == 1);
        lisp_free(loop->child1->list);
        loop->child1->list = NULL;
    #endif
    if (!next_token_is(current, 1, ')') || !next_token_is(current, 1, '(')) {
        free_node(loop);
        return parser_fails(log, (*current)->value, 
        "Expecting parentheses between bool and instructions within loop\n");
    }
    #ifdef INTERP
        log->executing = (execution_state && !execute) ? false : log->executing;
    #endif
    loop->child2 = handle_INSTRCTS(current, log);
    #ifdef INTERP
        if (execution_state && execute) {
            *current = loop_start;
            free_node(loop); // To ensure that the replaced nodes are not just hanging in the heap
            loop = handle_LOOP(current, log);
            log->executing = execution_state;
        }
        log->executing = (execution_state && !execute) ? true : log->executing;
    #endif
    return loop;
}

bool is_LISTFUNC(Token_node* current) {
    token_type type = current->value->type;
    return (type == t_CAR || type == t_CDR || type == t_CONS);
}

Tree_node* handle_LISTFUNC(Token_node** current, Prog_log* log) {
    token_type type = (*current)->value->type;
    if (!next_token_is(current, 3, t_CAR, t_CDR, t_CONS)) {
        return parser_fails(log, (*current)->value, 
        "Expecting 'CAR', 'CDR', or 'CONS' in list function");
    }
    Tree_node* list_function = make_node(LISTFUNC);
    list_function->func_type = type;
    list_function->child1 = handle_LIST(current, log);
    list_function->child2 = (type == t_CONS) ? handle_LIST(current, log) : NULL;
    #ifdef INTERP
    if (log->executing) {
        list_function->list = evaluate_list_function(type, &(list_function->child1->list), 
        (type == t_CONS) ? &(list_function->child2->list) : NULL);
    }
    #endif
    return list_function;
}

Lisp* evaluate_list_function(token_type type, Lisp** arg1, Lisp** arg2) {
    Lisp* output;
    if (type == t_CAR) {
        output = lisp_copy(lisp_car(*arg1));
        lisp_free(*arg1);
        *arg1 = NULL;
    } else if (type == t_CDR) {
        output = lisp_copy(lisp_cdr(*arg1));
        lisp_free(*arg1);
        *arg1 = NULL;
    } else {
        Lisp* cons = lisp_cons(*arg1, *arg2);
        *arg1 = NULL; *arg2 = NULL;
        output = lisp_copy(cons);
        lisp_free(cons);
    }
    return output;
}
    

bool is_INTFUNC(Token_node* current) {
    token_type type = current->value->type;
    return (type == t_plus || type == t_length);
}

Tree_node* handle_INTFUNC(Token_node** current, Prog_log* log) {
    Tree_node* int_function = make_node(INTFUNC);
    token_type type = (*current)->value->type;
    if (!next_token_is(current, 2, t_plus, t_length)) {
        return parser_fails(log, (*current)->value, 
        "Expecting 'PLUS' or 'LENGTH' in unteger function\n");
    } else {
        int_function->func_type = type;
        int_function->child1 = handle_LIST(current, log);
    }
    if (type == t_plus) {
        int_function->child2 = handle_LIST(current, log);
    }
    #ifdef INTERP
        int_function->list = evaluate_int_function((*current), type, log, &(int_function->child1->list),
        (type == t_plus) ? &(int_function->child2->list) : NULL);
    #endif
    return int_function;
}

Lisp* evaluate_int_function(Token_node* node, token_type type, Prog_log* log, Lisp** arg1, Lisp** arg2) {
    Lisp* output;
    if (type == t_plus && log->executing) {
        output = evaluate_plus(node, *arg1, *arg2, log);
        lisp_free(*arg1);
        lisp_free(*arg2);
        *arg1 = *arg2 = NULL;
    } else if (log->executing) {
        output = evaluate_length(*arg1);
        lisp_free(*arg1);
        *arg1 = NULL;
    } else {
        output = NULL;
    }
    return output;
}

Lisp* evaluate_plus(Token_node* node, Lisp* arg1, Lisp* arg2, Prog_log* log) {
    if (!lisp_is_atomic(arg1) || !lisp_is_atomic(arg2)) {
        Error* error = (Error*)allocate_space(1, sizeof(Error));
        error->message = "INTERPRETER ERROR: arguments for PLUS are not atomic, so PLUS cannot be evaluated.\n";
        error->line = node->value->line;
        error->col = node->value->col;
        add_error(log, error, false);
        return NULL;
    } else {
        int result = lisp_get_val(arg1) + lisp_get_val(arg2);
        return lisp_atom(result);
    }
}

Lisp* evaluate_length(Lisp* arg) {
    int result = lisp_length(arg);
    return lisp_atom(result);
}

bool is_BOOLFUNC(Token_node* current) {
    token_type type = current->value->type;
    return (type == t_less || type == t_greater || type == t_equal);
}

Tree_node* handle_BOOLFUNC(Token_node** current, Prog_log* log) {
    Token_node* operator = (*current);
    if (!next_token_is(current, 3, t_less, t_greater, t_equal)) {
        return parser_fails(log, (*current)->value, 
        "Expecting 'LESS', 'GREATER', or 'EQUAL' in bool function\n");
    }
    Tree_node* bool_function = make_node(BOOLFUNC);
    bool_function->func_type = operator->value->type;
    bool_function->child1 = handle_LIST(current, log);
    bool_function->child2 = handle_LIST(current, log);
    #ifdef INTERP
    if (log->executing) {
        bool evaluates = evaluate_bool(operator, bool_function->child1->list, 
        bool_function->child2->list, log);
        bool_function->list = (evaluates) ? lisp_atom(1) : lisp_atom(0);
        lisp_free(bool_function->child1->list); lisp_free(bool_function->child2->list);
        bool_function->child1->list = bool_function->child2->list = NULL;
    }
    #endif
    return bool_function;
}

bool evaluate_bool(Token_node* node, Lisp* arg1, Lisp* arg2, Prog_log* log) {
    if (!lisp_is_atomic(arg1) || !lisp_is_atomic(arg2)) {
        Error* error = (Error*)allocate_space(1, sizeof(Error));
        error->message = "INTERPRETER ERROR: arguments for boolean function are not atomic, so boolean function cannot be evaluated.\n";
        error->line = node->value->line;
        error->col = node->value->col;
        add_error(log, error, false);
        return false;
    }
    int val1 = lisp_get_val(arg1);
    int val2 = lisp_get_val(arg2);
    if (node->value->type == t_less) {
        return (val1 < val2);
    } else if (node->value->type == t_equal) {
        return (val1 == val2);
    } else {
        return (val1 < val2); 
    }
}

Tree_node* handle_LIST(Token_node** current, Prog_log* log) {
    Tree_node* list = make_node(LIST);
    token_type type = (*current)->value->type;
    if (type == t_variable) {
        list->child1 = handle_VAR(current, log);
        #ifdef EXT
        check_initialised((*current), list->child1, log);
        #endif
    } else if (type == t_literal) {
        list->child1 = handle_LITERAL(current, log);
    } else if (type == t_nil) {
        list->child1 = handle_NIL(current, log);
    } else if (next_token_is(current, 1, t_l_parenthesis)) {
        list->child1 = handle_RETFUNC(current, log);
        if (!next_token_is(current, 1, t_r_parenthesis)) {
            return parser_fails(log, (*current)->value, 
            "Expecting closing parenthesis after return function\n");
        }
    } else {
        free_node(list);
        return parser_fails(log, (*current)->value, 
        "Expecting variable, literal, 'nil' or return function in list\n");
    }
    #ifdef INTERP
    list->list = (log->executing && (type != t_nil)) ? move_list(&(list->child1->list)) : NULL;
    #endif
    return list;
}

void check_initialised(Token_node* token_node, Tree_node* tree_node, Prog_log* log) {
    if (tree_node->var_name) {
        int index = tree_node->var_name - 'A';
        if (log->variables[index] == NULL) {
            tree_node->type = ERROR_NODE;
            Error* error = (Error*)allocate_space(1, sizeof(Error));
            error->line = token_node->value->line;
            error->col = token_node->value->col;
            error->message = "INTERPRETER ERROR: uninitialised variable used\n";
            add_error(log, error, false);
        }
    }
    
}

Lisp* move_list(Lisp** original) {
    Lisp* output = lisp_copy(*(original));
    lisp_free(*(original));
    *(original) = NULL;
    return output;
}

Tree_node* handle_VAR(Token_node** current, Prog_log* log) {
    if ((*current)->value->type != t_variable) {
        return parser_fails(log, (*current)->value, 
        "Expecting a variable\n");
    }
    Tree_node* var = make_node(VAR);
    var->var_name = (*current)->value->var_name;
    
    if (log->executing) {
        #ifdef INTERP
        int index = var->var_name - 'A';
        var->list = lisp_copy(log->variables[index]);
        #endif
    }
    *current = (*current)->next;
    return var;
}

Tree_node* handle_LITERAL(Token_node** current, Prog_log* log) {
    if ((*current)->value->type == t_literal) {
        Tree_node* literal = make_node(LITERAL);
        #ifdef INTERP
        if (log->executing && is_invalid((*current)->value->lexeme)) {
            literal->type = ERROR_NODE;
            Error* error = (Error*)allocate_space(1, sizeof(Error));
            error->line = (*current)->value->line;
            error->col = (*current)->value->line;
            error->message = "INTERPRETER ERROR: invalid literal\n";
            add_error(log, error, false);
        } else if (log->executing) {
            literal->list = lisp_from_string((*current)->value->lexeme);
        }
        #endif
        *current = (*current)->next;
        return literal;
    } else {
        return parser_fails(log, (*current)->value, 
        "Expecting literal\n");
    }
}

Tree_node* handle_NIL(Token_node** current, Prog_log* log)  {
    if ((*current)->value->type != t_nil) {
        return parser_fails(log, (*current)->value, "Expecting 'NIL'\n");
    }
    Tree_node* nil = make_node(NIL);
    *current = (*current)->next;
    return nil;
}

Tree_node* handle_PRINT(Token_node** current, Prog_log* log) {
    if (!next_token_is(current, 1, t_print)) {
        return parser_fails(log, (*current)->value, "Expecting 'PRINT' in print statement\n");
    } 
    Tree_node* print = make_node(PRINT);
    if ((*current)->value->type == t_string) {
        print->child1 = handle_STRING(current, log);
        #ifdef INTERP
            if (log->executing) {
                printf("%s\n", print->child1->string_value);
            }
        #endif
    } else if (is_LIST(*current)) {
        print->child1 = handle_LIST(current, log);
        #ifdef INTERP
        print_lisp(&(print->child1->list), log);
        #endif
    }
    return print;
}

void print_lisp(Lisp** lisp, Prog_log* log) {
    if (log->executing) {
        char string_to_print[MAXSTR];
        lisp_to_string(*lisp, string_to_print);
        printf("%s\n", string_to_print);
        lisp_free(*lisp);
        *lisp = NULL;
    }
}

bool is_LIST(Token_node* current) {
    token_type type = current->value->type;
    return (type == t_variable || type == t_literal || type == t_nil || type == t_l_parenthesis);
}

Tree_node* handle_STRING(Token_node** current, Prog_log* log) {
    if ((*current)->value->type == t_string) {
        Tree_node* string = make_node(STRING);
        #ifdef INTERP
            string->string_value = (*current)->value->lexeme;
        #endif
        *current = (*current)->next;
        return string;
    } else {
        return parser_fails(log, (*current)->value, 
        "Expecting string\n");
    }
}

Tree_node* make_node(grammar_type type) {
    Tree_node* new_node = (Tree_node*)allocate_space(1, sizeof(Tree_node));
    new_node->type = type;
    return new_node;
}

Tree_node* parser_fails(Prog_log* log, Token* token, char* error_message) {
    Error* error = (Error*)allocate_space(1, sizeof(Error));
    error->message = error_message;
    error->line = token->line;
    error->col = token->col;
    add_error(log, error, true);
    Tree_node* error_node = (Tree_node*)allocate_space(1, sizeof(Tree_node));
    error_node->type = ERROR_NODE;
    return error_node;
}

bool next_token_is(Token_node** current, int num_possible_tokens, ...) {
    if ((num_possible_tokens < 1) || !(*current)) {
        return false;
    }
    va_list token_list;
    va_start(token_list, num_possible_tokens);
    for (int i = 0; i < num_possible_tokens; i++) {
        token_type expected = va_arg(token_list, token_type);
        if ((*current)->value->type == expected) {
            *current = (*current)->next;
            return true;
        }
    }
    return false;
}

void print_tree(Syntax_tree* tree) {
    if (tree->program) {
        print_tree_node(tree->program);
    } else {
        printf("EMPTY TREE");
    }
    printf("\n");
}

void print_tree_node(Tree_node* node) {
    char* node_type = get_node_type(node);
    printf("(%s", node_type);
    if (node->child1) {
        print_tree_node(node->child1);
    } else {
        printf("()"); 
    }
    if (node->child2) {
        print_tree_node(node->child2);
    }
    if (node->child3) {
        print_tree_node(node->child3);
    }
    printf(")");
}

char* get_node_type(Tree_node* node) {
    switch (node->type) {
        case PROG:
            return "PROG";
        case INSTRCTS:
            return "INSTRCTS";
        case INSTRCT:
            return "INSTRCT";
        case FUNC:
            return "FUNC";
        case RETFUNC:
            return "RETFUNC";
        case LISTFUNC:
            return "LISTFUNC";
        case INTFUNC:
            return "INTFUNC";
        case BOOLFUNC:
            return "BOOLFUNC";
        case IOFUNC:
            return "IOFUNC";
        case SET:
            return "SET";
        case PRINT:
            return "PRINT";
        case IF:
            return "IF";
        case LOOP:
            return "LOOP";
        case LIST:
            return "LIST";
        case VAR:
            return "VAR";
        case STRING:
            return "STRING";
        case LITERAL:
            return "LITERAL";
        case NIL:
            return "NIL";
        case ERROR_NODE:
            return "ERROR";
        default:
            return "null";
    }
}

void free_tree(Syntax_tree* tree) {
    if (tree->program) {
        free_node(tree->program);
    }
    free(tree);
}

void free_node(Tree_node* node) {
    if (node->child1) {
        free_node(node->child1);
        node->child1 = NULL;
    }
    if (node->child2) {
        free_node(node->child2);
        node->child2 = NULL;
    }
    if (node->child3) {
        free_node(node->child3);
        node->child3 = NULL;
    }
    if (node->list) {
        lisp_free(node->list);
    }
    free(node);
}

void free_log(Prog_log* log) {
    for (int i = 0; i < NUMVARS; i++) {
        if (log->variables[i]) {
            lisp_free(log->variables[i]);
        }
    }
    for (int j = 0; j < log->num_errors; j++) {
        free(log->errors[j]);
    }
    free(log);
}

void print_log(Prog_log* log) {
    if (log->num_errors > 0) {
        fprintf(stderr, "ERROR LOG:\n");
    }
    for (int i = 0; i < log->num_errors; i++) {
        fprintf(stderr, "Er%i, L%i C%i - %s", i + 1, 
        log->errors[i]->line, log->errors[i]->col, log->errors[i]->message);
    }
    if (log->overflow) {
        fprintf(stderr, "More than 20 parsing errors.  Not printing any more.\n");
    }
}


void add_error(Prog_log* log, Error* error, bool parsing) {
    if (log->num_errors < 20) {
        log->errors[log->num_errors] = error;
        (log->num_errors)++;
    } else {
        log->overflow = true;
    }
    if (parsing) {
        log->parser_error = true;
    } else {
        log->interp_error = true;
    }
}

void test(void) {
    lexical_analysis_test();
    parse_test();
}

void parse_test(void) {
    // make_node()
    
    Tree_node* test_node = make_node(PROG);
    assert(test_node);
    assert(test_node->type == PROG);
    free(test_node);
    test_node = make_node(INSTRCTS);
    assert(test_node);
    assert(test_node->type == INSTRCTS);
    free(test_node);
    test_node = make_node(IOFUNC);
    assert(test_node);
    assert(test_node->type == IOFUNC);
    free(test_node);
    test_node = make_node(SET);
    assert(test_node);
    assert(test_node->type == SET);
    free(test_node);
    test_node = make_node(RETFUNC);
    assert(test_node);
    assert(test_node->type == RETFUNC);
    free(test_node);
    test_node = make_node(ERROR_NODE);
    assert(test_node);
    assert(test_node->type == ERROR_NODE);
    free(test_node);
    

    // Setting up token list for the following assert testing
    Token_list* test_tokens = (Token_list*)allocate_space(1, sizeof(Token_list));
    Prog_log* test_log = (Prog_log*)allocate_space(1, sizeof(Prog_log));
    Token* test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_nil;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_literal;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_variable;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_string;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_variable;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_literal;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_nil;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_l_parenthesis;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_CONS;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_literal;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_literal;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_r_parenthesis;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_while;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_print;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_string;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_print;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_literal;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_set;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_variable;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_literal;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_greater;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_literal;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_literal;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_less;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_literal;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_literal;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_equal;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_literal;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_literal;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_length;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_nil;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_nil;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_nil;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_nil;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_nil;
    add_token(test_tokens, test_token);
    test_token = (Token*)allocate_space(1, sizeof(Token));
    test_token->type = t_nil;
    add_token(test_tokens, test_token);
    
    Token_node* current = test_tokens->start;


    // handle_NIL()
    // current is at a NIL token, so should make a valid tree node
    // and move the current pointer on one.
    Tree_node* test_nil_node = handle_NIL(&current, test_log);
    assert(test_nil_node->type == NIL);
    assert(current->value->type == t_literal);
    assert(test_log->num_errors == 0);
    free_node(test_nil_node);
    // current is now at a literal token, so should return a NULL pointer, add
    // an error to the log, and not move the current pointer on.
    test_nil_node = handle_NIL(&current, test_log);
    assert(test_nil_node->type == ERROR_NODE);
    assert(current->value->type == t_literal);
    assert(test_log->num_errors == 1);
    assert(strcmp(test_log->errors[0]->message, "Expecting 'NIL'\n") == 0);
    free_node(test_nil_node);
    
    // handle_LITERAL()
    // current is at a LITERAL token, so should make a valid tree node
    // and move the current pointer on one.
    Tree_node* test_literal_node = handle_LITERAL(&current, test_log);
    assert(test_literal_node->type == LITERAL);
    assert(current->value->type == t_variable);
    assert(test_log->num_errors == 1);
    free_node(test_literal_node);
    // current is now at a variable token, so should return a NULL pointer, add
    // an error to the log, and not move the current pointer on.
    test_literal_node = handle_LITERAL(&current, test_log);
    assert(test_literal_node->type == ERROR_NODE);
    assert(current->value->type == t_variable);
    assert(test_log->num_errors == 2);
    assert(strcmp(test_log->errors[1]->message, "Expecting literal\n") == 0);
    free_node(test_literal_node);
    
    // handle_VAR()
    // current is at a variable token, so should make a valid tree node
    // and move the current pointer on one.
    Tree_node* test_variable_node = handle_VAR(&current, test_log);
    assert(test_variable_node->type == VAR);
    assert(current->value->type == t_string);
    assert(test_log->num_errors == 2);
    free_node(test_variable_node);
    // current is now at a string token, so should return a NULL pointer, add
    // an error to the log, and not move the current pointer on.
    test_variable_node = handle_VAR(&current, test_log);
    assert(test_variable_node->type == ERROR_NODE);
    assert(current->value->type == t_string);
    assert(test_log->num_errors == 3);
    assert(strcmp(test_log->errors[2]->message, "Expecting a variable\n") == 0);
    free_node(test_variable_node);
     
    // handle_STRING()
    // current is at a string token, so should make a valid tree node
    // and move the current pointer on one.
    Tree_node* test_string_node = handle_STRING(&current, test_log);
    assert(test_string_node->type == STRING);
    assert(current->value->type == t_variable);
    assert(test_log->num_errors == 3);
    free_node(test_string_node);
    // current is now at a variable token, so should return a NULL pointer, add
    // an error to the log, and not move the current pointer on.
    test_string_node = handle_STRING(&current, test_log);
    assert(test_string_node->type == ERROR_NODE);
    assert(current->value->type == t_variable);
    assert(test_log->num_errors == 4);
    assert(strcmp(test_log->errors[3]->message, "Expecting string\n") == 0);
    free_node(test_string_node);
    
    
    // handle_LIST()
    // token is variable -> should return a LIST node with a VAR node at child1
    Tree_node* test_list_node = handle_LIST(&current, test_log);
    assert(test_list_node->type == LIST);
    assert(test_list_node->child1->type == VAR);
    assert(current->value->type == t_literal);
    assert(test_log->num_errors == 4);
    free_node(test_list_node);
    // token is literal -> should return a LIST node with a LITERAL node at child1
    test_list_node = handle_LIST(&current, test_log);
    assert(test_list_node->type == LIST);
    assert(test_list_node->child1->type == LITERAL);
    assert(current->value->type == t_nil);
    assert(test_log->num_errors == 4);
    free_node(test_list_node);
    // token is nil -> should return a LIST node with a NIL node at child1
    test_list_node = handle_LIST(&current, test_log);
    assert(test_list_node->type == LIST);
    assert(test_list_node->child1->type == NIL);
    assert(current->value->type == t_l_parenthesis);
    assert(test_log->num_errors == 4);
    free_node(test_list_node);
    // token is (, followed by CONS LITERAL, LITERAL, ) -> should return a LIST node with a RETFUNC node at child1, LISTFUNC node at child1 of that, LIST nodes at child1 and child2 of that, and LITERAL nodes at child1 of those.
    test_list_node = handle_LIST(&current, test_log);
    assert(test_list_node->type == LIST);
    assert(test_list_node->child1->type == RETFUNC);
    assert(test_list_node->child1->child1->type == LISTFUNC);
    assert(test_list_node->child1->child1->child1->type == LIST);
    assert(test_list_node->child1->child1->child2->type == LIST);
    assert(test_list_node->child1->child1->child1->child1->type == LITERAL);
    assert(test_list_node->child1->child1->child2->child1->type == LITERAL);
    assert(current->value->type == t_while);
    assert(test_log->num_errors == 4);
    free_node(test_list_node);
    // Now the next token is a WHILE token.  Therefore, handle_LIST() should fail.  As we've seen, this should return an ERROR node, and add an error message to the log
    test_list_node = handle_LIST(&current, test_log);
    assert(test_list_node->type == ERROR_NODE);
    assert(current->value->type == t_while);
    assert(test_log->num_errors == 5);
    assert(strcmp(test_log->errors[4]->message, 
    "Expecting variable, literal, 'nil' or return function in list\n") == 0);
    free_node(test_list_node);
    
    current = current->next;
    
    // handle_PRINT()
    // token is print-string -> should return a PRINT node with a STRING node at child1
    Tree_node* test_print_node = handle_PRINT(&current, test_log);
    assert(test_print_node->type == PRINT);
    assert(test_print_node->child1->type == STRING);
    assert(current->value->type == t_print);
    assert(test_log->num_errors == 5);
    free_node(test_print_node);
    // token is print-literal -> should return a PRINT node with a LIST node at child1, and a LITERAL node at child1 of that
    test_print_node = handle_PRINT(&current, test_log);
    assert(test_print_node->type == PRINT);
    assert(test_print_node->child1->type == LIST);
    assert(test_print_node->child1->child1->type == LITERAL);
    assert(current->value->type == t_set);
    assert(test_log->num_errors == 5);
    free_node(test_print_node);
    // token is nil -> should return an ERROR node, and add an error to the log
    test_print_node = handle_PRINT(&current, test_log);
    assert(test_print_node->type == ERROR_NODE);
    assert(current->value->type == t_set);
    assert(test_log->num_errors == 6);
    assert(strcmp(test_log->errors[5]->message, 
    "Expecting 'PRINT' in print statement\n") == 0);
    free_node(test_print_node);
    
    // handle_SET()
    // token is set-variable-literal -> should return a SET node with a VAR node at child1 and a LIST node at child2, which in turn has a LITERAL node at child1
    Tree_node* test_set_node = handle_SET(&current, test_log);
    assert(test_set_node->type == SET);
    assert(test_set_node->child1->type == VAR);
    assert(test_set_node->child2->type == LIST);
    assert(test_set_node->child2->child1->type == LITERAL);
    assert(current->value->type == t_greater);
    assert(test_log->num_errors == 6);
    free_node(test_set_node);
    // token is greater -> should return an ERROR node, and add an error to the log
    test_set_node = handle_SET(&current, test_log);
    assert(test_set_node->type == ERROR_NODE);
    assert(current->value->type == t_greater);
    assert(test_log->num_errors == 7);
    assert(strcmp(test_log->errors[6]->message, 
    "Expecting 'SET' in set statement\n") == 0);
    free_node(test_set_node);
    
    
    // handle_BOOLFUNC()
    // token is greater-literal-literal -> should return a BOOL node with LIST nodes at child1 and child2, which each, in turn, have a LITERAL node at child1
    Tree_node* test_bool_node = handle_BOOLFUNC(&current, test_log);
    assert(test_bool_node->type == BOOLFUNC);
    assert(test_bool_node->func_type == t_greater)
    assert(test_bool_node->child1->type == LIST);
    assert(test_bool_node->child1->child1->type == LITERAL);
    assert(test_bool_node->child2->type == LIST);
    assert(test_bool_node->child2->child1->type == LITERAL);
    assert(current->value->type == t_less);
    assert(test_log->num_errors == 7);
    free_node(test_bool_node);
    // token is less-literal-literal -> should return a BOOL node with LIST nodes at child1 and child2, which each, in turn, have a LITERAL node at child1
    test_bool_node = handle_BOOLFUNC(&current, test_log);
    assert(test_bool_node->type == BOOLFUNC);
    assert(test_bool_node->func_type == t_less)
    assert(test_bool_node->child1->type == LIST);
    assert(test_bool_node->child1->child1->type == LITERAL);
    assert(test_bool_node->child2->type == LIST);
    assert(test_bool_node->child2->child1->type == LITERAL);
    assert(current->value->type == t_eqaul);
    assert(test_log->num_errors == 7);
    free_node(test_bool_node);
    // token is equal-literal-literal -> should return a BOOL node with LIST nodes at child1 and child2, which each, in turn, have a LITERAL node at child1
    test_bool_node = handle_BOOLFUNC(&current, test_log);
    assert(test_bool_node->type == BOOLFUNC);
    assert(test_bool_node->func_type == t_equal)
    assert(test_bool_node->child1->type == LIST);
    assert(test_bool_node->child1->child1->type == LITERAL);
    assert(test_bool_node->child2->type == LIST);
    assert(test_bool_node->child2->child1->type == LITERAL);
    assert(current->value->type == t_length);
    assert(test_log->num_errors == 7);
    free_node(test_bool_node);
    // token is length -> should return an ERROR node, and add an error to the log
    test_bool_node = handle_BOOLFUNC(&current, test_log);
    assert(test_bool_node->type == ERROR_NODE);
    assert(current->value->type == t_length);
    assert(test_log->num_errors == 8);
    assert(strcmp(test_log->errors[7]->message, 
    "Expecting 'LESS', 'GREATER', or 'EQUAL' in bool function\n") == 0);
    free_node(test_bool_node);
    
    
   
    free_token_list(test_tokens);
    free_log(test_log);
}
