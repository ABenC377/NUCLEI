#include "parse.h"

void parse_list(Token_list* list) {
    Syntax_tree* tree = (Syntax_tree*)allocate_space(1, sizeof(Syntax_tree));
    Token_node* current = list->start;
    bool parses_correctly = true;
    tree->program = descend_recursively(tree, &current, &parses_correctly);
}

Tree_node* descend_recursively(Syntax_tree* tree, Token_node** current, bool* parses_correctly) {
    Tree_node* program = make_node(PROG);
    if ((current*)->value->type != t_l_parenthesis) {
        parses_correctly* = false;
    } else {
        (current*) = (current*)->next;
        program->child1 = handle_INSTRCTS(tree, current, parses_correctly);
    }
    return program;
}

Tree_node* handle_INSTRCTS(Syntax_tree* tree, Token_node** current, bool* parses_correctly) {
    Tree_node* instructions = make_node(INSTRCTS);
    if ((current*)->value->type == t_r_parenthesis) {
        current* = (current*)->next;
    } else {
        instructions->child1 = handle_INSTRCT(tree, current, parses_correctly);
        instructions->child2 = handle_INSTRCTS(tree, current, parses_correctly);
    }
    return instructions;
}

Tree_node* handle_INSTRCT(Syntax_tree* tree, Token_node** current, bool* parses_correctly) {
    Tree_node* instruction = make_node(INSTRCT);
    if ((current*)->value->type == t_l_parenthesis) {
        current* = (current*)->next;
    } else {
        parses_correctly* = false;
    }
    instruction->child1 = handle_FUNC(tree, current, parses_correctly);
    if ((current*)->value->type == t_r_parenthesis) {
        current* = (current*)->next;
    } else {
        parses_correctly* = false;
    }
    return instruction;
}

Tree_node* handle_FUNC(Syntax_tree* tree, Token_node** current, bool* parses_correctly) {
    Tree_node* function = make_node(FUNC);
    if (is_RETFUNC(current*)) {
        function->child1 = handle_RETFUNC(tree, current, parses_correctly);
    } else if (is_IOFUNC(current*)) {
        function->child1 = handle_IOFUNC(tree, current, parses_correctly);
    } else if (is_IF(current*)) {
        function->child1 = handle_IF(tree, current, parses_correctly);
    } else if (is_LOOP(current*)) {
        function->child1 = handle_LOOP(tree, current, parses_correctly);
    } else {
        parses_correctly* = false;
    }
    return function;
}

bool is_RETFUNC(Token_node* current) {
    return (is_LISTFUNC(current) || is_INTFUNC(current) || is_BOOLFUNC(current));
}

Tree_node* handle_RETFUNC(Syntax_tree* tree, Token_node** current, bool* parses_correctly) {
    Tree_node* ret_function = make_node(RETFUNC);
    if (is_LISTFUNC(current*)) {
        ret_function->child1 = handle_LISTFUNC(tree, current, parses_correctly);
    } else if (is_INTFUNC(current*)) {
        ret_function->child1 = handle_INTFUNC(tree, current, parses_correctly);
    } else if (is_BOOLFUNC(current*)) {
        ret_function->child1 = handle_BOOLFUNC(tree, current, parses_correctly);
    } else {
        parses_correctly* = false;
    }
    return ret_function;
}

bool is_IOFUNC(Token_node* current) {
    
}

Tree_node* handle_IOFUNC(Syntax_tree* tree, Token_node** current, bool* parses_correctly) {
    
}

bool is_IF(Token_node* current) {
    
}

Tree_node* handle_IF(Syntax_tree* tree, Token_node** current, bool* parses_correctly) {
    
}

bool is_LOOP(Token_node* current) {
    
}

Tree_node* handle_LOOP(Syntax_tree* tree, Token_node** current, bool* parses_correctly) {
    
}

bool is_LISTFUNC(Token_node* current) {
    token_type type = current->value->type;
    return (type == t_CAR || type == t_CDR || type == t_CONS);
}

Tree_node* handle_LISTFUNC(Syntax_tree* tree, Token_node** current, bool* parses_correctly) {
    Tree_node* list_function = make_node(LISTFUNC);
    token_type type = (current*)->value->type;
    if (type != t_CAR && type != t_CDR && type != t_CONS) {
        parses_correctly* = false;
        return NULL;
    } else {
        list_function->func_type = type;
        current* = (current*)->next;
    }
    if (is_LIST(current*)) {
        list_function->child1 = handle_LIST(tree, current, parses_correctly);
    }
    if (type == t_CONS && is_LIST(current*)) {
        list_function->child2 = handle_LIST(tree, current, parses_correctly);
    }
    return list_function;
}
    

bool is_INTFUNC(Token_node* current) {
    token_type type = current->value->type;
    return (type == t_plus || type == t_length);
}

Tree_node* handle_INTFUNC(Syntax_tree* tree, Token_node** current, bool* parses_correctly) {
    
}

bool is_BOOLFUNC(Token_node* current) {
    token_type = current->value->type;
    return (type == t_less || type == t_greater || type == t_equal);
}

Tree_node* handle_BOOLFUNC(Syntax_tree* tree, Token_node** current, bool* parses_correctly) {
    
}

bool is_LIST(Token_node* current) {
    token_type type = current->value->type;
    return (type == t_variable || type == t_literal || type == t_nil || type == t_l_parenthesis);
}

Tree_node* handle_LIST(Syntax_tree* tree, Token_node** current, bool* parses_correctly) {
    Tree_node* list = make_node(LIST);
    token_type type = (current*)->value->type;
    if (type == t_variable) {
        list->child1 = handle_VAR(tree, current, parses_correctly);
    } else if (type == t_literal) {
        list->child1 = handle_LITERAL(tree, current, parses_correctly);
    } else if (type == t_nil) {
        list->child1 = handle_NIL(tree, current, parses_correctly);
    } else if (type != t_l_parenthesis) {
        parses_correctly* = false;
        return NULL;
    }
    current* = (current*)->next;
    if (is_RETFUNC(current*)) {
        list->child1 = handle_RETFUNC(tree, current, parses_correctly);
    }
    if ((current*)->value->type == t_r_parenthesis) {
        current* = (current*)->next;
        return list;
    } else {
        parses_correctly* = false;
        return NULL;
    }
}

Tree_node* handle_VAR(Syntax_tree* tree, Token_node** current, bool* parses_correctly) {
    Tree_node* var = make_node(VAR);
    
}

Tree_node* handle_LITERAL(Syntax_tree* tree, Token_node** current, bool* parses_correctly)  {
    
}

Tree_node* handle_NIL(Syntax_tree* tree, Token_node** current, bool* parses_correctly)  {
    
}

Tree_node* make_node(grammar_type type) {
    Tree_node* new_node = (Tree_node*)allocate_space(1, sizeof(Tree_node));
    new_node->type = type;
    return new_node;
}
