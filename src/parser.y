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
    char name[MAX_NAME];
    int int_val;
    float float_val;
    char* str_val;
    int is_float;
    int scope;
    SymbolType type;  // Added type field
    struct Symbol* next;  // For hash table chaining
} Symbol;

typedef struct {
    int int_val;
    float float_val;
    int is_float;
    char* temp_var;
} ExprValue;

// Hash table implementation
Symbol* symbol_table[TABLE_SIZE];
int current_scope = 0;
int temp_var_count = 0;
int label_count = 0;

void yyerror(const char *s);
int yylex(void);

// Hash function
unsigned int hash(char* name, int scope) {
    unsigned int hash_val = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        hash_val = hash_val * 31 + name[i];
    }
    hash_val = (hash_val + scope) % TABLE_SIZE;
    return hash_val;
}

// Insert symbol into hash table
Symbol* insert_symbol(char* name, SymbolType type, int int_val, float float_val) {
    unsigned int index = hash(name, current_scope);
    
    // Check if symbol already exists in current scope
    Symbol* current = symbol_table[index];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && current->scope == current_scope) {
            return NULL;  // Already exists in current scope
        }
        current = current->next;
    }
    
    // Create new symbol
    Symbol* new_symbol = (Symbol*)malloc(sizeof(Symbol));
    if (!new_symbol) return NULL;
    
    strncpy(new_symbol->name, name, MAX_NAME - 1);
    new_symbol->name[MAX_NAME - 1] = '\0';
    new_symbol->int_val = int_val;
    new_symbol->float_val = float_val;
    new_symbol->str_val = NULL;
    new_symbol->is_float = (type == TYPE_FLOAT);
    new_symbol->type = type;
    new_symbol->scope = current_scope;
    
    // Insert at beginning of chain
    new_symbol->next = symbol_table[index];
    symbol_table[index] = new_symbol;
    
    return new_symbol;
}

// Look up symbol in all accessible scopes
Symbol* lookup_symbol(char* name) {
    // Search from current scope to global scope
    for (int scope = current_scope; scope >= 0; scope--) {
        unsigned int index = hash(name, scope);
        Symbol* current = symbol_table[index];
        
        while (current != NULL) {
            if (strcmp(current->name, name) == 0 && current->scope == scope) {
                return current;
            }
            current = current->next;
        }
    }
    return NULL;
}

void enter_scope() {
    current_scope++;
}

void exit_scope() {
    if (current_scope <= 0) return;
    
    // Remove all symbols in current scope
    for (int i = 0; i < TABLE_SIZE; i++) {
        Symbol* current = symbol_table[i];
        Symbol* prev = NULL;
        
        while (current != NULL) {
            if (current->scope == current_scope) {
                Symbol* to_delete = current;
                if (prev == NULL) {
                    symbol_table[i] = current->next;
                } else {
                    prev->next = current->next;
                }
                current = current->next;
                free(to_delete);
            } else {
                prev = current;
                current = current->next;
            }
        }
    }
    current_scope--;
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
    current_scope = 0;
}

void cleanup_symbol_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Symbol* current = symbol_table[i];
        while (current != NULL) {
            Symbol* to_delete = current;
            current = current->next;
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
Symbol* lookup_symbol(const char* name);
void add_symbol(const char* name, int kind, int type);
IROperand create_temp();
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
        char* temp_var;
    } exprval;
    IROperand operand;
}

%token <strval> IDENTIFIER
%token <intval> NUMBER
%token <floatval> FLOAT_LITERAL
%token INT FLOAT IF ELSE RETURN WHILE FOR
%token '+' '-' '*' '/' '=' '<' '>' '(' ')' '{' '}' ';'
%token <strval> STRING_LITERAL
%token <intval> BOOL_LITERAL
%token AND OR NOT
%token EQ NEQ LEQ GEQ
%token TRUE FALSE
%token STRING BOOL VOID

%type <exprval> expr bool_expr
%type <intval> condition

%left OR
%left AND
%right NOT
%left EQ NEQ
%left '<' '>' LEQ GEQ
%left '+' '-'
%left '*' '/'
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
    | if_statement
    | while_loop
    | for_loop
    | return_statement ';'
    | '{' { enter_scope(); } statement_list '}' { exit_scope(); }
    ;

declaration:
      INT IDENTIFIER '=' expr {
        if ($4.is_bool) {
            yyerror("Cannot assign boolean to int");
        } else {
            Symbol* sym = insert_symbol($2, TYPE_INT, $4.int_val, 0.0);
            if (!sym) {
                yyerror("Redeclaration of variable");
            } else {
                printf("%s = %s\n", $2, $4.temp_var);
            }
        }
    }
    | FLOAT IDENTIFIER '=' expr {
        if ($4.is_bool) {
            yyerror("Cannot assign boolean to float");
        } else {
            float val = $4.is_float ? $4.float_val : (float)$4.int_val;
            Symbol* sym = insert_symbol($2, TYPE_FLOAT, 0, val);
            if (!sym) {
                yyerror("Redeclaration of variable");
            } else {
                printf("%s = %s\n", $2, $4.temp_var);
            }
        }
    }
    | STRING IDENTIFIER '=' expr {
        if (!$4.str_val) {
            yyerror("Cannot assign non-string to string");
        } else {
            Symbol* sym = insert_symbol($2, TYPE_STRING, 0, 0.0);
            if (!sym) {
                yyerror("Redeclaration of variable");
            } else {
                sym->str_val = strdup($4.str_val);
                printf("%s = %s\n", $2, $4.temp_var);
            }
        }
    }
    | BOOL IDENTIFIER '=' bool_expr {
        Symbol* sym = insert_symbol($2, TYPE_BOOL, $4.int_val, 0.0);
        if (!sym) {
            yyerror("Redeclaration of variable");
        } else {
            printf("%s = %s\n", $2, $4.temp_var);
        }
    };

assignment:
    IDENTIFIER '=' expr {
        Symbol* sym = lookup_symbol($1);
        if (!sym) {
            yyerror("Undefined variable");
        } else {
            printf("%s = %s\n", $1, $3.temp_var);
            printf("Assigned %s = %s\n", $1, $3.temp_var);
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
    | expr '=' expr { $$ = 1; printf("if %s == %s\n", $1.temp_var, $3.temp_var); }
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
        } else {
            $$.is_float = (s->type == TYPE_FLOAT);
            $$.int_val = s->int_val;
            $$.float_val = s->float_val;
        }
        $$.temp_var = strdup($1);
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
Symbol* lookup_symbol(const char* name) {
    Symbol* sym = symbol_table;
    while (sym) {
        if (strcmp(sym->name, name) == 0) {
            return sym;
        }
        sym = sym->next;
    }
    return NULL;
}

void add_symbol(const char* name, int kind, int type) {
    Symbol* sym = malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->kind = kind;
    sym->type = type;
    sym->next = symbol_table;
    symbol_table = sym;
}

/* Temporary variable creation */
static int temp_counter = 0;

IROperand create_temp() {
    char temp_name[32];
    sprintf(temp_name, "t%d", temp_counter++);
    return create_temp_operand(temp_name);
}

/* Main function */
int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s input_file\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Cannot open input file %s\n", argv[1]);
        return 1;
    }

    // Initialize IR program
    program = create_ir_program();

    // Parse input
    yyparse();

    // Generate output filename
    char output_name[256];
    strcpy(output_name, argv[1]);
    char* dot = strrchr(output_name, '.');
    if (dot) *dot = '\0';
    strcat(output_name, ".asm");

    // Open output file
    FILE* output = fopen(output_name, "w");
    if (!output) {
        fprintf(stderr, "Cannot create output file %s\n", output_name);
        return 1;
    }

    // Create code generator context
    CodeGenContext* ctx = create_codegen_context(program, output);

    // Generate code
    generate_code(ctx);

    // Clean up
    fclose(input);
    fclose(output);
    free_codegen_context(ctx);
    free_ir_program(program);

    return 0;
} 