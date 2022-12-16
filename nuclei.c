#include "nuclei.h"

int main(int argc, char* argv[]) {
    
    // Set up the file to be scanned
    Token_list* list = run_lexical_analyser(argc, argv);
    print_tokens(list);
    
    free_token_list(list);
}

void* allocate_space(int num, int size) {
    void* pointer = calloc((unsigned long)num, (unsigned long)size);
    if (!pointer) {
        throw_error("ERROR: unable to allocate space\n");
    }
    return pointer;
}

void throw_error(const char* error_message) {
    fputs(error_message, stderr);
    exit(EXIT_FAILURE);
}

void test(void) {
    lexical_parse_test();
    
}
