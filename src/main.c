#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

// Phase 3 Will add code generation headers and functionality here
extern int yyparse();
extern int yylineno;
extern FILE* yyin;
extern char* yytext;
extern int line_number;

void print_token(int token) {
    switch(token) {
        case INT: printf("INT\n"); break;
        case FLOAT: printf("FLOAT\n"); break;
        case STRING: printf("STRING\n"); break;
        case BOOL: printf("BOOL\n"); break;
        case VOID: printf("VOID\n"); break;
        case IF: printf("IF\n"); break;
        case ELSE: printf("ELSE\n"); break;
        case WHILE: printf("WHILE\n"); break;
        case FOR: printf("FOR\n"); break;
        case RETURN: printf("RETURN\n"); break;
        case NUMBER: printf("NUMBER(%d)\n", yylval.intval); break;
        case FLOAT_LITERAL: printf("FLOAT_LITERAL(%f)\n", yylval.floatval); break;
        case STRING_LITERAL: printf("STRING_LITERAL(\"%s\")\n", yylval.strval); break;
        case BOOL_LITERAL: printf("BOOL_LITERAL(%s)\n", yylval.intval ? "true" : "false"); break;
        case IDENTIFIER: printf("IDENTIFIER(%s)\n", yylval.strval); break;
        case AND: printf("AND\n"); break;
        case OR: printf("OR\n"); break;
        case NOT: printf("NOT\n"); break;
        case EQ: printf("EQ\n"); break;
        case NEQ: printf("NEQ\n"); break;
        case LEQ: printf("LEQ\n"); break;
        case GEQ: printf("GEQ\n"); break;
        default:
            if (token >= 0 && token <= 127)
                printf("CHAR('%c')\n", token);
            else
                printf("Unknown token: %d\n", token);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s input_file\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", argv[1]);
        return 1;
    }

    yyin = input;
    printf("Lexical Analysis of %s:\n", argv[1]);
    printf("=====================================\n");

    int token;
    while ((token = yylex())) {
        printf("Line %d: ", line_number);
        print_token(token);
    }

    fclose(input);
    return 0;
} 