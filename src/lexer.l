%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

int line_number = 1;
%}

%option noyywrap

%%

[ \t\r]     ; /* Ignore whitespace */
\n          { line_number++; }

"//".*      ; /* Ignore single-line comments */
"/*"([^*]|\*+[^*/])*\*+"/" ; /* Ignore multi-line comments */

"if"        { return IF; }
"else"      { return ELSE; }
"return"    { return RETURN; }
"int"       { return INT; }
"float"     { return FLOAT; }
"bool"      { return BOOL; }
"string"    { return STRING; }
"void"      { return VOID; }
"while"     { return WHILE; }
"for"       { return FOR; }
"print"     { return PRINT; }
"input"     { return INPUT; }
"true"      { yylval.intval = 1; return BOOL_LITERAL; }
"false"     { yylval.intval = 0; return BOOL_LITERAL; }

"+"         { return '+'; }
"-"         { return '-'; }
"*"         { return '*'; }
"/"         { return '/'; }
"%"         { return '%'; }
"&&"        { return AND; }
"||"        { return OR; }
"!"         { return NOT; }
"=="        { return EQ; }
"!="        { return NEQ; }
"<"         { return '<'; }
">"         { return '>'; }
"<="        { return LEQ; }
">="        { return GEQ; }
"="         { return '='; }
"["         { return '['; }
"]"         { return ']'; }
"("         { return '('; }
")"         { return ')'; }
"{"         { return '{'; }
"}"         { return '}'; }
";"         { return ';'; }
","         { return ','; }

[0-9]+      { 
    yylval.intval = atoi(yytext); 
    return NUMBER; 
}

[0-9]+\.[0-9]+ {
    yylval.floatval = atof(yytext);
    return FLOAT_LITERAL;
}

\"([^\"\\]|\\.)*\" {
    int len = strlen(yytext) - 2;
    char* str = malloc(len + 1);
    strncpy(str, yytext + 1, len);
    str[len] = '\0';
    yylval.strval = str;
    return STRING_LITERAL;
}

[a-zA-Z][a-zA-Z0-9_]* {
    yylval.strval = strdup(yytext);
    return IDENTIFIER;
}

.           { 
    printf("Error: Unexpected character '%s' at line %d\n", yytext, line_number);
    return 0;
}

%% 