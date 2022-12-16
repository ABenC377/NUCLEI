# In *my* code, I had the interpreter code added to the parser code.
# This additional code is protected by #ifdef INTERP / #endif clauses.
# This code is only "seen" by the compiler if you have #defined INTERP
# This #define would be annoying to change inside nuclei.c/.h, so instead it is
# set in the gcc/clang statement using -DINTERP
# In this way ./parse & ./interp can both be built from the same source file.

CC      := gcc
DEBUG   := -g3
CFLAGS  := -Wall -Wextra -Wpedantic -Wfloat-equal -Wvla -std=c99 -fsanitize=undefined -fsanitize=address


debug: nuclei.c nuclei.h
	$(CC) nuclei.c -o nuclei $(CFLAGS) $(DEBUG)

run: nuclei
	./nuclei test_code/demo1.ncl
	./nuclei test_code/demo2.ncl
	./nuclei test_code/demo3.ncl
	