%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ir.h"

// Phase 3 Will add code generation functionality

#define TABLE_SIZE 100
#define MAX_NAME 100

// Symbol types
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_VOID
} SymbolType;

typedef struct Symbol {
    char* name;
    int type;
    int size;           // For arrays
    int* int_array;     // For int arrays
    float* float_array; // For float arrays
    char** str_array;   // For string arrays
    int int_val;
    float float_val;
    char* str_val;
    int is_array;
    struct Symbol* next;  // For hash table collision handling
} Symbol;

typedef struct {
    int int_val;
    float float_val;
    char* str_val;
    int is_float;
    int is_bool;
    int is_array;
    int array_index;
    char* temp_var;
} ExprValue;

// Hash table implementation
Symbol* symbol_table[TABLE_SIZE];
int temp_var_count = 0;
int label_count = 0;

void yyerror(const char *s);
int yylex(void);

// Hash function
unsigned int hash(const char* name) {
    unsigned int hash_val = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        hash_val = hash_val * 31 + name[i];
    }
    return hash_val % TABLE_SIZE;
}

// Insert symbol into hash table
Symbol* insert_symbol(char* name, int type, int size) {
    unsigned int index = hash(name);
    
    // Check if symbol already exists
    Symbol* current = symbol_table[index];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            yyerror("Symbol already declared");
            return NULL;
        }
        current = current->next;
    }
    
    // Create new symbol
    Symbol* sym = (Symbol*)malloc(sizeof(Symbol));
    if (!sym) {
        yyerror("Memory allocation failed");
        return NULL;
    }
    
    sym->name = strdup(name);
    sym->type = type;
    sym->is_array = (size > 0);
    sym->size = size;
    sym->next = NULL;
    
    if(sym->is_array) {
        if(type == TYPE_INT) {
            sym->int_array = (int*)calloc(size, sizeof(int));
        } else if(type == TYPE_FLOAT) {
            sym->float_array = (float*)calloc(size, sizeof(float));
        } else if(type == TYPE_STRING) {
            sym->str_array = (char**)calloc(size, sizeof(char*));
        }
    }
    
    // Insert at the beginning of the list
    sym->next = symbol_table[index];
    symbol_table[index] = sym;
    
    return sym;
}

// Look up symbol in hash table
Symbol* lookup_symbol(const char* name) {
    unsigned int index = hash(name);
    Symbol* current = symbol_table[index];
    
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

char* new_temp() {
    char* temp = (char*)malloc(10);
    sprintf(temp, "t%d", temp_var_count++);
    return temp;
}

char* new_label() {
    char* label = (char*)malloc(10);
    sprintf(label, "L%d", label_count++);
    return label;
}

void init_symbol_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        symbol_table[i] = NULL;
    }
}

void cleanup_symbol_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Symbol* current = symbol_table[i];
        while (current != NULL) {
            Symbol* to_delete = current;
            current = current->next;
            
            free(to_delete->name);
            if(to_delete->is_array) {
                if(to_delete->type == TYPE_INT)
                    free(to_delete->int_array);
                else if(to_delete->type == TYPE_FLOAT)
                    free(to_delete->float_array);
                else if(to_delete->type == TYPE_STRING)
                    free(to_delete->str_array);
            }
            free(to_delete);
        }
        symbol_table[i] = NULL;
    }
}

// Global IR program
IRProgram* program;
IRFunction* current_function;
BasicBlock* current_block;

// Function declarations
void add_symbol(const char* name, SymbolType type);
%}

%union {
    int intval;
    float floatval;
    char* strval;
    struct {
        int int_val;
        float float_val;
        char* str_val;
        int is_float;
        int is_bool;
        int is_array;
        int array_index;
        char* temp_var;
    } exprval;
}

%token <strval> IDENTIFIER
%token <intval> NUMBER
%token <floatval> FLOAT_LITERAL
%token <strval> STRING_LITERAL
%token <intval> BOOL_LITERAL
%token INT FLOAT IF ELSE RETURN WHILE FOR PRINT INPUT
%token '+' '-' '*' '/' '%' '=' '<' '>' '(' ')' '{' '}' ';' ',' '[' ']'
%token AND OR NOT
%token EQ NEQ LEQ GEQ
%token TRUE FALSE
%token STRING BOOL VOID

%type <exprval> expr bool_expr array_expr
%type <intval> condition

%left OR
%left AND
%right NOT
%left EQ NEQ
%left '<' '>' LEQ GEQ
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS
%nonassoc IFX
%nonassoc ELSE

%%

program: 
    { init_symbol_table(); }  // Initialize at start
    statement_list 
    { cleanup_symbol_table(); };  // Cleanup at end

statement_list: statement | statement_list statement;

statement:
      declaration ';'
    | assignment ';'
    | array_assignment ';'
    | if_statement
    | while_loop
    | for_loop
    | return_statement ';'
    | print_statement ';'
    | input_statement ';'
    | '{' statement_list '}'
    ;

declaration:
      INT IDENTIFIER '=' expr {
        if ($4.is_array) {
            yyerror("Cannot assign array to scalar");
        } else if ($4.is_bool) {
            yyerror("Cannot assign boolean to int");
        } else {
            Symbol* sym = insert_symbol($2, TYPE_INT, 0);
            if (!sym) {
                yyerror("Redeclaration of variable");
            } else {
                sym->int_val = $4.int_val;
                printf("%s = %s\n", $2, $4.temp_var);
            }
        }
    }
    | INT IDENTIFIER '[' NUMBER ']' {
        Symbol* sym = insert_symbol($2, TYPE_INT, $4);
        if (!sym) {
            yyerror("Redeclaration of variable");
        } else {
            printf("Array %s[%d] declared\n", $2, $4);
        }
    }
    | FLOAT IDENTIFIER '=' expr {
        if ($4.is_array) {
            yyerror("Cannot assign array to scalar");
        } else if ($4.is_bool) {
            yyerror("Cannot assign boolean to float");
        } else {
            float val = $4.is_float ? $4.float_val : (float)$4.int_val;
            Symbol* sym = insert_symbol($2, TYPE_FLOAT, 0);
            if (!sym) {
                yyerror("Redeclaration of variable");
            } else {
                sym->float_val = val;
                printf("%s = %s\n", $2, $4.temp_var);
            }
        }
    }
    | FLOAT IDENTIFIER '[' NUMBER ']' {
        Symbol* sym = insert_symbol($2, TYPE_FLOAT, $4);
        if (!sym) {
            yyerror("Redeclaration of variable");
        } else {
            printf("Array %s[%d] declared\n", $2, $4);
        }
    }
    | STRING IDENTIFIER '=' expr {
        if ($4.is_array) {
            yyerror("Cannot assign array to scalar");
        } else if (!$4.str_val) {
            yyerror("Cannot assign non-string to string");
        } else {
            Symbol* sym = insert_symbol($2, TYPE_STRING, 0);
            if (!sym) {
                yyerror("Redeclaration of variable");
            } else {
                sym->str_val = strdup($4.str_val);
                printf("%s = %s\n", $2, $4.temp_var);
            }
        }
    }
    | STRING IDENTIFIER '[' NUMBER ']' {
        Symbol* sym = insert_symbol($2, TYPE_STRING, $4);
        if (!sym) {
            yyerror("Redeclaration of variable");
        } else {
            printf("Array %s[%d] declared\n", $2, $4);
        }
    }
    | BOOL IDENTIFIER '=' bool_expr {
        if ($4.is_array) {
            yyerror("Cannot assign array to scalar");
        } else {
            Symbol* sym = insert_symbol($2, TYPE_BOOL, 0);
            if (!sym) {
                yyerror("Redeclaration of variable");
            } else {
                sym->int_val = $4.int_val;
                printf("%s = %s\n", $2, $4.temp_var);
            }
        }
    };

assignment:
    IDENTIFIER '=' expr {
        Symbol* sym = lookup_symbol($1);
        if (!sym) {
            yyerror("Undefined variable");
        } else if (sym->is_array) {
            yyerror("Cannot assign to array variable directly");
        } else {
            printf("%s = %s\n", $1, $3.temp_var);
        }
    };

array_assignment:
    IDENTIFIER '[' expr ']' '=' expr {
        Symbol* sym = lookup_symbol($1);
        if (!sym) {
            yyerror("Undefined variable");
        } else if (!$3.is_array && $3.int_val >= 0 && $3.int_val < sym->size) {
            printf("%s[%s] = %s\n", $1, $3.temp_var, $6.temp_var);
        } else {
            yyerror("Array index out of bounds");
        }
    };

array_expr:
    IDENTIFIER '[' expr ']' {
        Symbol* sym = lookup_symbol($1);
        if (!sym) {
            yyerror("Undefined variable");
            $$.int_val = 0;
            $$.float_val = 0.0;
            $$.is_float = 0;
        } else if (!$3.is_array && $3.int_val >= 0 && $3.int_val < sym->size) {
            $$.is_array = 1;
            $$.array_index = $3.int_val;
            $$.temp_var = new_temp();
            printf("%s = %s[%s]\n", $$.temp_var, $1, $3.temp_var);
        } else {
            yyerror("Array index out of bounds");
        }
    };

print_statement:
    PRINT expr {
        if ($2.is_array) {
            yyerror("Cannot print array directly");
        } else {
            printf("print %s\n", $2.temp_var);
        }
    }
    | PRINT STRING_LITERAL {
        printf("print \"%s\"\n", $2);
    };

input_statement:
    INPUT IDENTIFIER {
        Symbol* sym = lookup_symbol($2);
        if (!sym) {
            yyerror("Undefined variable");
        } else if (sym->is_array) {
            yyerror("Cannot input to array variable directly");
        } else {
            printf("input %s\n", $2);
        }
    }
    | INPUT IDENTIFIER '[' expr ']' {
        Symbol* sym = lookup_symbol($2);
        if (!sym) {
            yyerror("Undefined variable");
        } else if (!$4.is_array && $4.int_val >= 0 && $4.int_val < sym->size) {
            printf("input %s[%s]\n", $2, $4.temp_var);
        } else {
            yyerror("Array index out of bounds");
        }
    };

if_statement:
    IF '(' condition ')' '{' statement_list '}' {
        printf("If condition true\n");
    }
    | IF '(' condition ')' '{' statement_list '}' ELSE '{' statement_list '}' {
        printf("If-Else executed\n");
    };

while_loop:
    WHILE '(' condition ')' '{' statement_list '}' {
        printf("While loop executed\n");
    };

for_loop:
    FOR '(' assignment ';' condition ';' assignment ')' '{' statement_list '}' {
        printf("For loop executed\n");
    };

return_statement:
    RETURN expr {
        if($2.is_float)
            printf("Return %.2f\n", $2.float_val);
        else
            printf("Return %d\n", $2.int_val);
    };

condition:
      expr '>' expr { $$ = 1; printf("if %s > %s\n", $1.temp_var, $3.temp_var); }
    | expr '<' expr { $$ = 1; printf("if %s < %s\n", $1.temp_var, $3.temp_var); }
    | expr EQ expr { $$ = 1; printf("if %s == %s\n", $1.temp_var, $3.temp_var); }
    ;

expr:
      expr '+' expr {
        $$ = $1;
        if ($1.is_float || $3.is_float) {
            $$.is_float = 1;
            $$.float_val = ($1.is_float ? $1.float_val : $1.int_val) +
                           ($3.is_float ? $3.float_val : $3.int_val);
        } else {
            $$.int_val = $1.int_val + $3.int_val;
        }
        $$.temp_var = new_temp();
        printf("%s = %s + %s\n", $$.temp_var, $1.temp_var, $3.temp_var);
    }
    | expr '-' expr {
        $$ = $1;
        if ($1.is_float || $3.is_float) {
            $$.is_float = 1;
            $$.float_val = ($1.is_float ? $1.float_val : $1.int_val) -
                           ($3.is_float ? $3.float_val : $3.int_val);
        } else {
            $$.int_val = $1.int_val - $3.int_val;
        }
        $$.temp_var = new_temp();
        printf("%s = %s - %s\n", $$.temp_var, $1.temp_var, $3.temp_var);
    }
    | expr '*' expr {
        $$ = $1;
        if ($1.is_float || $3.is_float) {
            $$.is_float = 1;
            $$.float_val = ($1.is_float ? $1.float_val : $1.int_val) *
                           ($3.is_float ? $3.float_val : $3.int_val);
        } else {
            $$.int_val = $1.int_val * $3.int_val;
        }
        $$.temp_var = new_temp();
        printf("%s = %s * %s\n", $$.temp_var, $1.temp_var, $3.temp_var);
    }
    | expr '/' expr {
        $$ = $1;
        if ($1.is_float || $3.is_float) {
            $$.is_float = 1;
            $$.float_val = ($1.is_float ? $1.float_val : $1.int_val) /
                           ($3.is_float ? $3.float_val : $3.int_val);
        } else {
            $$.int_val = $1.int_val / $3.int_val;
        }
        $$.temp_var = new_temp();
        printf("%s = %s / %s\n", $$.temp_var, $1.temp_var, $3.temp_var);
    }
    | expr '%' expr {
        if ($1.is_float || $3.is_float) {
            yyerror("Modulo operation not allowed on floating point numbers");
        } else {
            $$.int_val = $1.int_val % $3.int_val;
            $$.is_float = 0;
            $$.temp_var = new_temp();
            printf("%s = %s %% %s\n", $$.temp_var, $1.temp_var, $3.temp_var);
        }
    }
    | NUMBER {
        $$.int_val = $1;
        $$.is_float = 0;
        $$.temp_var = new_temp();
        printf("%s = %d\n", $$.temp_var, $1);
    }
    | FLOAT_LITERAL {
        $$.float_val = $1;
        $$.is_float = 1;
        $$.temp_var = new_temp();
        printf("%s = %.2f\n", $$.temp_var, $1);
    }
    | IDENTIFIER {
        Symbol* s = lookup_symbol($1);
        if (!s) {
            yyerror("Undefined variable");
            $$.int_val = 0;
            $$.float_val = 0.0;
            $$.is_float = 0;
        } else if (s->is_array) {
            yyerror("Cannot use array variable directly");
        } else {
            $$.is_float = (s->type == TYPE_FLOAT);
            $$.int_val = s->int_val;
            $$.float_val = s->float_val;
        }
        $$.temp_var = strdup($1);
    }
    | array_expr {
        $$ = $1;
    }
    | STRING_LITERAL {
        $$.str_val = $1;
        $$.is_float = 0;
        $$.is_bool = 0;
        $$.temp_var = new_temp();
        printf("%s = \"%s\"\n", $$.temp_var, $1);
    };

bool_expr:
      expr EQ expr {
        $$.is_bool = 1;
        $$.int_val = ($1.is_float || $3.is_float) ?
            ($1.is_float ? $1.float_val : $1.int_val) == ($3.is_float ? $3.float_val : $3.int_val) :
            $1.int_val == $3.int_val;
        $$.temp_var = new_temp();
        printf("%s = %s == %s\n", $$.temp_var, $1.temp_var, $3.temp_var);
    }
    | expr NEQ expr {
        $$.is_bool = 1;
        $$.int_val = ($1.is_float || $3.is_float) ?
            ($1.is_float ? $1.float_val : $1.int_val) != ($3.is_float ? $3.float_val : $3.int_val) :
            $1.int_val != $3.int_val;
        $$.temp_var = new_temp();
        printf("%s = %s != %s\n", $$.temp_var, $1.temp_var, $3.temp_var);
    }
    | expr LEQ expr {
        $$.is_bool = 1;
        $$.int_val = ($1.is_float || $3.is_float) ?
            ($1.is_float ? $1.float_val : $1.int_val) <= ($3.is_float ? $3.float_val : $3.int_val) :
            $1.int_val <= $3.int_val;
        $$.temp_var = new_temp();
        printf("%s = %s <= %s\n", $$.temp_var, $1.temp_var, $3.temp_var);
    }
    | expr GEQ expr {
        $$.is_bool = 1;
        $$.int_val = ($1.is_float || $3.is_float) ?
            ($1.is_float ? $1.float_val : $1.int_val) >= ($3.is_float ? $3.float_val : $3.int_val) :
            $1.int_val >= $3.int_val;
        $$.temp_var = new_temp();
        printf("%s = %s >= %s\n", $$.temp_var, $1.temp_var, $3.temp_var);
    }
    | bool_expr AND bool_expr {
        $$.is_bool = 1;
        $$.int_val = $1.int_val && $3.int_val;
        $$.temp_var = new_temp();
        printf("%s = %s && %s\n", $$.temp_var, $1.temp_var, $3.temp_var);
    }
    | bool_expr OR bool_expr {
        $$.is_bool = 1;
        $$.int_val = $1.int_val || $3.int_val;
        $$.temp_var = new_temp();
        printf("%s = %s || %s\n", $$.temp_var, $1.temp_var, $3.temp_var);
    }
    | NOT bool_expr {
        $$.is_bool = 1;
        $$.int_val = !$2.int_val;
        $$.temp_var = new_temp();
        printf("%s = !%s\n", $$.temp_var, $2.temp_var);
    }
    | BOOL_LITERAL {
        $$.is_bool = 1;
        $$.int_val = $1;
        $$.temp_var = new_temp();
        printf("%s = %s\n", $$.temp_var, $1 ? "true" : "false");
    }
    | '(' bool_expr ')' { $$ = $2; };

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

/* Symbol table functions */
void add_symbol(const char* name, SymbolType type) {
    unsigned int index = hash(name);
    Symbol* sym = (Symbol*)malloc(sizeof(Symbol));
    if (!sym) return;
    
    strncpy(sym->name, name, MAX_NAME - 1);
    sym->name[MAX_NAME - 1] = '\0';
    sym->type = type;
    sym->next = symbol_table[index];
    symbol_table[index] = sym;
} 