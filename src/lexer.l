%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

int line_number = 1;
%}

%option noyywrap

/* Definitions */
DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          {LETTER}({LETTER}|{DIGIT}|_)*
INTEGER     {DIGIT}+
WHITESPACE  [ \t\r]+
STRING      \"([^\"\\]|\\.)*\"

%%

{WHITESPACE}    { /* Ignore whitespace */ }
\n             { line_number++; }

"//".*         { /* Ignore single-line comments */ }
"/*"([^*]|\*+[^*/])*\*+"/" { /* Ignore multi-line comments */ }

/* Keywords */
"if"          { return IF; }
"else"        { return ELSE; }
"return"      { return RETURN; }
"int"         { return INT; }
"bool"        { return BOOL; }
"string"      { return STRING; }
"void"        { return VOID; }
"function"    { return FUNCTION; }
"true"        { yylval.bool_val = 1; return BOOL_LITERAL; }
"false"       { yylval.bool_val = 0; return BOOL_LITERAL; }

/* Operators */
"+"           { return PLUS; }
"-"           { return MINUS; }
"*"           { return MULTIPLY; }
"/"           { return DIVIDE; }
"&&"          { return AND; }
"||"          { return OR; }
"!"           { return NOT; }
"=="          { return EQ; }
"!="          { return NEQ; }
"<"           { return LT; }
">"           { return GT; }
"<="          { return LEQ; }
">="          { return GEQ; }
"="           { return ASSIGN; }

/* Delimiters */
"("           { return LPAREN; }
")"           { return RPAREN; }
"{"           { return LBRACE; }
"}"           { return RBRACE; }
";"           { return SEMICOLON; }
","           { return COMMA; }

/* Literals */
{INTEGER}     { 
    yylval.int_val = atoi(yytext); 
    return INTEGER_LITERAL; 
}

{STRING}      {
    /* Remove quotes and handle escapes */
    int len = strlen(yytext) - 2;
    char* str = malloc(len + 1);
    strncpy(str, yytext + 1, len);
    str[len] = '\0';
    yylval.string_val = str;
    return STRING_LITERAL;
}

{ID}          {
    yylval.string_val = strdup(yytext);
    return IDENTIFIER;
}

.             { 
    printf("Error: Unexpected character '%s' at line %d\n", yytext, line_number);
    return ERROR;
}

%%

/* Optional user code */
void yyerror(const char* msg) {
    fprintf(stderr, "Error at line %d: %s\n", line_number, msg);
} 