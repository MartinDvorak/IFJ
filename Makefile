CFLAGS = -std=c99 -Wall -Wextra -pedantic

ifj17_compilator: ifj17_compilator.c semantic.c semantic.h stack.h stack.c symbtab.h symbtab.c token.h token.c expr.h expr.c parser.h parser.c
