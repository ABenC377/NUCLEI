# In *my* code, I had the interpreter code added to the parser code.
# This additional code is protected by #ifdef INTERP / #endif clauses.
# This code is only "seen" by the compiler if you have #defined INTERP
# This #define would be annoying to change inside nuclei.c/.h, so instead it is
# set in the gcc/clang statement using -DINTERP
# In this way ./parse & ./interp can both be built from the same source file.

CC      := gcc
CFLAGS  := -Wall -Wextra -Wpedantic -Wfloat-equal -Wvla -std=c99 
DEBUG   := $(CFLAGS) -g3 -fsanitize=undefined -fsanitize=address
VFLAGS  := $(CFLAGS) -g3
PROD    := $(CFLAGS) -O3

all: parse parse_debug parse_valgrind interp interp_debug interp_valgrind
	
parse: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h
	$(CC) nuclei.c lexical_parser.c lisp.c -o nuclei_p $(PROD)

parse_debug: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h
	$(CC) nuclei.c lexical_parser.c lisp.c -o nuclei_pd $(DEBUG)

parse_valgrind: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h
	$(CC) nuclei.c lexical_parser.c lisp.c -o nuclei_pv $(VFLAGS)	
	
interp: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h
	$(CC) nuclei.c lexical_parser.c lisp.c -o nuclei_i -DINTERP $(PROD)

interp_debug: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h
	$(CC) nuclei.c lexical_parser.c lisp.c -o nuclei_id -DINTERP $(DEBUG)

interp_valgrind: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h 
	$(CC) nuclei.c lexical_parser.c lisp.c -o nuclei_iv -DINTERP $(VFLAGS)	
	
extension: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h
	$(CC) nuclei.c lexical_parser.c lisp.c -o nuclei_i -DINTERP -DEXT $(PROD)

extension_debug: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h
	$(CC) nuclei.c lexical_parser.c lisp.c -o nuclei_id -DINTERP -DEXT $(DEBUG)

extension_valgrind: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h 
	$(CC) nuclei.c lexical_parser.c lisp.c -o nuclei_iv -DINTERP -DEXT $(VFLAGS)	

run: nuclei_pd
	./nuclei_pd test_code/basic_print.ncl
	./nuclei_pd test_code/demo1.ncl
	./nuclei_pd test_code/demo2.ncl
	./nuclei_pd test_code/demo3.ncl
	./nuclei_pd test_code/fibonacci.ncl
	./nuclei_pd test_code/inf_loop.ncl
	./nuclei_pd test_code/parse_fail.ncl
	./nuclei_pd test_code/parse_pass_interp_fail.ncl
	./nuclei_pd test_code/print_set.ncl
	./nuclei_pd test_code/simple_loop.ncl
	./nuclei_pd test_code/test.ncl
	./nuclei_pd test_code/triv.ncl
	./nuclei_id test_code/basic_print.ncl
	./nuclei_id test_code/demo1.ncl
	./nuclei_id test_code/demo2.ncl
	./nuclei_id test_code/demo3.ncl
	./nuclei_id test_code/fibonacci.ncl
	./nuclei_id test_code/inf_loop.ncl
	./nuclei_id test_code/parse_fail.ncl
	./nuclei_id test_code/parse_pass_interp_fail.ncl
	./nuclei_id test_code/print_set.ncl
	./nuclei_id test_code/simple_loop.ncl
	./nuclei_id test_code/test.ncl
	./nuclei_id test_code/triv.ncl
	