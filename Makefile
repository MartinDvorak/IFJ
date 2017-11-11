
CFLAGS = -std=c99 -Wall -Wextra -pedantic

parser: semantic.c semantic.h stack.h stack.c symbtab.h symbtab.c token.h token.c expr.h expr.c parser.h parser.c codegen.h codegen.c