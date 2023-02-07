# NUCLEI-interpreter

## Project parameters
An interpreter and parser for a programming language called 'NUCLEI'.  This was the final project for the 'Programming in C' module that I took as a part of my MSc.

## Summary of approach

The parser/interpretter runs by doing a first lexical analysis-parse of the file to turn it into a linked list of tokens.  The code for doing this is in the 'lexical_parser.c' and 'lexical_parser.h' files.  From this list of tokens, the parser/interpretter is run.  The parser and interpretter are different programs compiled from the same source file by using different flags.

There is also an extension flag which, when used in compilation, reults in the program keeping a complete error log for parser/interpretter errors which are caught, and then shows these to the user along with line and column locations of the source of the error. 

The lexical parser is basically a finite state automata, which takes the characters of the input file as inputs, and then adds tokens to the linked list of tokens as an output.  Because of the variety of possible characters and tokens, there are 39 states to the automata.  The easiest way for me to code this automata was with a switch statement.  This switch statement is very large, but I think easy enough to understand.

## NUCLEI grammar

NUCLEI is a new prgramming language that is inspired by LISP, though has a more limited set of intructions.
Like LISP, NUCLEI uses a CAR/CDR structure for storing its data.

The formal grammar of NUCLEI is as follows:

*PROG ::= "(" INSTRCTS*

*INSTRCTS ::= INSTRCT INSTRCTS | ")"*

*INSTRCT ::= "(" FUNC ")"*

*FUNC ::= RETFUNC | IOFUNC | IF | LOOP*

*RETFUNC ::= LISTFUNC | INTFUNC | BOOLFUNC*

*LISTFUNC ::= "CAR" LIST | "CDR" LIST | "CONS" LIST LIST*

*INTFUNC ::= "PLUS" LIST LIST | "LENGTH" LIST*

*BOOLFUNC ::= "LESS" LIST LIST | "GREATER" LIST LIST | "EQUAL" LIST LIST*

*IOFUNC ::= SET | PRINT*

*SET ::= "SET" VAR LIST*

*PRINT ::= "PRINT" LIST | "PRINT" STRING*

*IF ::= "IF" "(" BOOLFUNC ")" "(" INSTRCTS "(" INSTRCTS*

*LOOP ::= "WHILE" "(" BOOLFUNC ")" "(" INSRCTS*

*LIST ::= VAR | LITERAL | "NIL" | "(" RETFUNC ")"*

*VAR ::= [A-Z]*

*STRING ::= Double-quoted string constant e.g. "Hello, World!", or "FAILURE ?"*

*LITERAL ::= Single-quoted list e.g. ’(1)’, ’(1 2 3 (4 5))’, or ’2’*

  
NUCLEI ignores white space.



