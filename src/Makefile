CC = gcc
CFLAGS = -Wall -I.
FLEX = flex
BISON = bison

OBJS = lexer.o parser.tab.o ir.o codegen.o

all: compiler test_cases

compiler: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

test_cases: test_cases.o ir.o codegen.o
	$(CC) $(CFLAGS) -o $@ $^

lexer.c: lexer.l parser.tab.h
	$(FLEX) -o $@ $<

parser.tab.c parser.tab.h: parser.y
	$(BISON) -d $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f compiler test_cases *.o lexer.c parser.tab.c parser.tab.h *.asm

.PHONY: all clean 