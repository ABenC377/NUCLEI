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

all: parse parse_debug parse_valgrind interp interp_debug interp_valgrind extension extension_debug extension_valgrind
	
parse: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h
	$(CC) nuclei.c lexical_parser.c lisp.c -o parse $(PROD)

parse_debug: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h
	$(CC) nuclei.c lexical_parser.c lisp.c -o parse_s $(DEBUG)

parse_valgrind: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h
	$(CC) nuclei.c lexical_parser.c lisp.c -o parse_v $(VFLAGS)	
	
interp: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h
	$(CC) nuclei.c lexical_parser.c lisp.c -o interp -DINTERP $(PROD)

interp_debug: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h
	$(CC) nuclei.c lexical_parser.c lisp.c -o interp_s -DINTERP $(DEBUG)

interp_valgrind: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h 
	$(CC) nuclei.c lexical_parser.c lisp.c -o interp_v -DINTERP $(VFLAGS)	
	
extension: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h
	$(CC) nuclei.c lexical_parser.c lisp.c -o ext -DINTERP -DEXT $(PROD)

extension_debug: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h
	$(CC) nuclei.c lexical_parser.c lisp.c -o ext_s -DINTERP -DEXT $(DEBUG)

extension_valgrind: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h 
	$(CC) nuclei.c lexical_parser.c lisp.c -o ext_v -DINTERP -DEXT $(VFLAGS)	

zip: nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h
	zip -nuclei.zip nuclei.c nuclei.h lexical_parser.c lexical_parser.h lisp.c lisp.h

run: parse_s interp_s ext_s
	./parse_s test_code/basic_print.ncl
	./parse_s test_code/demo1.ncl
	./parse_s test_code/demo2.ncl
	./parse_s test_code/demo3.ncl
	./parse_s test_code/fibonacci.ncl
	./parse_s test_code/inf_loop.ncl
	./parse_s test_code/parse_fail.ncl
	./parse_s test_code/parse_pass_interp_fail.ncl
	./parse_s test_code/print_set.ncl
	./parse_s test_code/simple_loop.ncl
	./parse_s test_code/test.ncl
	./parse_s test_code/triv.ncl
	./interp_s test_code/basic_print.ncl
	./interp_s test_code/demo1.ncl
	./interp_s test_code/demo2.ncl
	./interp_s test_code/demo3.ncl
	./interp_s test_code/fibonacci.ncl
	./interp_s test_code/parse_fail.ncl
	./interp_s test_code/parse_pass_interp_fail.ncl
	./interp_s test_code/print_set.ncl
	./interp_s test_code/simple_loop.ncl
	./interp_s test_code/test.ncl
	./interp_s test_code/triv.ncl
	./ext_s test_code/basic_print.ncl
	./ext_s test_code/demo1.ncl
	./ext_s test_code/demo2.ncl
	./ext_s test_code/demo3.ncl
	./ext_s test_code/fibonacci.ncl
	./ext_s test_code/parse_fail.ncl
	./ext_s test_code/parse_pass_interp_fail.ncl
	./ext_s test_code/print_set.ncl
	./ext_s test_code/simple_loop.ncl
	./ext_s test_code/test.ncl
	./ext_s test_code/triv.ncl
	
