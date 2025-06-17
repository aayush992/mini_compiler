# Compiler Architecture

## 1. Overview

The compiler is a multi-pass compiler that translates source code into x86 assembly. It follows a traditional compiler pipeline:

1. Lexical Analysis (Flex)
2. Parsing (Bison)
3. Intermediate Representation
4. Code Generation (x86)

## 2. Lexical Analysis

### 2.1 Implementation (lexer.l)
- Uses Flex lexical analyzer generator
- Recognizes tokens through regular expressions
- Handles comments and whitespace
- Tracks line numbers for error reporting

### 2.2 Token Categories
```c
// Keywords
IF, ELSE, RETURN, TRUE, FALSE
INT, FLOAT, BOOL, STRING, VOID
WHILE, FOR, PRINT, INPUT

// Operators
PLUS, MINUS, MULTIPLY, DIVIDE, MODULO
AND, OR, NOT
EQ, NEQ, LT, GT, LEQ, GEQ
ASSIGN, LBRACKET, RBRACKET

// Literals
INTEGER_LITERAL
FLOAT_LITERAL
BOOLEAN_LITERAL
STRING_LITERAL

// Identifiers
IDENTIFIER

// Delimiters
LPAREN, RPAREN    // ( )
LBRACE, RBRACE    // { }
SEMICOLON         // ;
COMMA            // ,
```

## 3. Parsing

### 3.1 Implementation (parser.y)
- Uses Bison parser generator
- Builds Abstract Syntax Tree (AST)
- Performs type checking
- Manages symbol table
- Handles array operations

### 3.2 Grammar Rules
```yacc
program     : statement_list
statement   : declaration | assignment | array_assignment
            | if_statement | while_loop | for_loop
            | print_statement | input_statement
declaration : type IDENTIFIER | type IDENTIFIER '[' NUMBER ']'
assignment  : IDENTIFIER '=' expr | array_access '=' expr
expr        : binary_expr | unary_expr | literal | array_access
```

### 3.3 Symbol Table
- Tracks variable declarations and types
- Manages scope information
- Validates variable usage
- Handles array types and sizes

## 4. Intermediate Representation

### 4.1 Three-Address Code IR (ir.h)
```c
typedef enum {
    IR_TEMP,
    IR_VAR,
    IR_CONST,
    IR_LABEL
} IROperandType;

typedef struct {
    IROperandType type;
    union {
        int temp_num;
        char* var_name;
        int const_val;
        char* label_name;
    } value;
} IROperand;

typedef enum {
    IR_ASSIGN,
    IR_ADD,
    IR_SUB,
    IR_MUL,
    IR_DIV,
    IR_MOD,
    IR_JUMP,
    IR_COND_JUMP,
    IR_CALL,
    IR_RETURN,
    IR_ARRAY_ACCESS,
    IR_ARRAY_ASSIGN
} IRInstructionType;
```

### 4.2 Basic Block Structure
```c
typedef struct BasicBlock {
    char* label;
    IRInstruction* instructions;
    struct BasicBlock* next;
} BasicBlock;
```

### 4.3 Function Structure
```c
typedef struct IRFunction {
    char* name;
    BasicBlock* blocks;
    struct IRFunction* next;
} IRFunction;
```

## 5. Code Generation

### 5.1 Implementation (codegen.h/c)
- Generates x86 assembly code
- Manages register allocation
- Handles stack frame layout
- Implements calling convention
- Supports array operations

### 5.2 Register Allocation
```c
typedef enum {
    REG_EAX, REG_EBX, REG_ECX, REG_EDX,
    REG_ESI, REG_EDI, REG_ESP, REG_EBP
} Register;
```

### 5.3 Stack Frame Layout
```
High addresses
+----------------+
| Return address |
+----------------+
| Saved EBP      |
+----------------+
| Local vars     |
+----------------+
| Array storage  |
+----------------+
| Temp values    |
+----------------+
| Saved regs     |
+----------------+
Low addresses
```

### 5.4 Instruction Selection
- Maps IR operations to x86 instructions
- Handles array access and assignment
- Manages control flow
- Implements function calls

## 6. Error Handling

### 6.1 Error Categories
1. Lexical errors
2. Syntax errors
3. Type errors
4. Scope errors
5. Array errors
6. Code generation errors

### 6.2 Error Recovery
- Synchronization points in parser
- Error message generation
- Source location tracking
- Array bounds checking

## 7. Testing

### 7.1 Test Components
- Lexer tests
- Parser tests
- IR generation tests
- Code generation tests
- Array operation tests

### 7.2 Test Infrastructure
```c
// Test case structure
void test_arithmetic();
void test_arrays();
void test_control_flow();
void test_functions();
void test_error_handling();
```

## 8. Build System

### 8.1 Dependencies
- GCC compiler
- Flex lexical analyzer
- Bison parser generator
- NASM assembler

### 8.2 Build Process
1. Generate lexer (flex)
2. Generate parser (bison)
3. Compile C source files
4. Link object files
5. Run tests

## 9. Future Enhancements

1. Optimization passes:
   - Dataflow analysis
   - Register allocation graphs
   - Peephole optimization
   - Array access optimization

2. Language features:
   - Structure types
   - Global variables
   - Multiple source files
   - Dynamic arrays

3. Development tools:
   - Debugger support
   - Profile-guided optimization
   - Source-level error messages
   - Array bounds checking 