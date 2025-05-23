# CustomLang Compiler Architecture

## 1. Overview

The CustomLang compiler is a multi-pass compiler that translates CustomLang source code into x86 assembly. It follows a traditional compiler pipeline:

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
IF, ELSE, RETURN, TRUE, FALSE, INT, BOOL, STRING, VOID, FUNCTION

// Operators
PLUS, MINUS, MULTIPLY, DIVIDE
AND, OR, NOT
EQ, NEQ, LT, GT, LEQ, GEQ
ASSIGN

// Literals
INTEGER_LITERAL
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
- Performs basic type checking
- Manages symbol table

### 3.2 Grammar Rules
```yacc
program     : function_list
function    : type IDENTIFIER LPAREN param_list RPAREN compound_stmt
stmt        : if_stmt | return_stmt | expr_stmt | compound_stmt
if_stmt     : IF LPAREN expr RPAREN stmt ELSE stmt
expr        : binary_expr | unary_expr | literal | IDENTIFIER
```

### 3.3 Symbol Table
- Tracks variable declarations and types
- Manages scope information
- Validates variable usage

## 4. Intermediate Representation

### 4.1 Three-Address Code IR (ir.h/c)
```c
typedef enum {
    IR_ASSIGN, IR_ADD, IR_SUB, IR_MUL, IR_DIV,
    IR_AND, IR_OR, IR_NOT,
    IR_EQ, IR_NEQ, IR_LT, IR_GT, IR_LEQ, IR_GEQ,
    IR_LABEL, IR_JUMP, IR_CJUMP,
    IR_PARAM, IR_CALL, IR_RETURN
} IROpType;
```

### 4.2 Basic Block Structure
```c
typedef struct BasicBlock {
    char* label;
    IRInst* first_inst;
    IRInst* last_inst;
    struct BasicBlock* next;
} BasicBlock;
```

### 4.3 Function Structure
```c
typedef struct IRFunction {
    char* name;
    BasicBlock* entry_block;
    struct IRFunction* next;
} IRFunction;
```

## 5. Code Generation

### 5.1 Implementation (codegen.h/c)
- Generates x86 assembly code
- Manages register allocation
- Handles stack frame layout
- Implements calling convention

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
| Temp values    |
+----------------+
| Saved regs     |
+----------------+
Low addresses
```

### 5.4 Instruction Selection
- Maps IR operations to x86 instructions
- Optimizes common patterns
- Handles memory operands

## 6. Error Handling

### 6.1 Error Categories
1. Lexical errors
2. Syntax errors
3. Type errors
4. Scope errors
5. Code generation errors

### 6.2 Error Recovery
- Synchronization points in parser
- Error message generation
- Source location tracking

## 7. Optimization

### 7.1 Current Optimizations
- Constant folding
- Dead code elimination
- Basic block optimization

### 7.2 Planned Optimizations
- Common subexpression elimination
- Loop optimization
- Strength reduction

## 8. Testing

### 8.1 Test Components
- Lexer tests
- Parser tests
- IR generation tests
- Code generation tests

### 8.2 Test Infrastructure
```c
// Test case structure
void create_arithmetic_test(BasicBlock* block);
void create_boolean_test(BasicBlock* block);
void create_comparison_test(BasicBlock* block);
void create_control_flow_test(IRFunction* func);
```

## 9. Build System

### 9.1 Dependencies
- GCC compiler
- Flex lexical analyzer
- Bison parser generator
- NASM assembler

### 9.2 Build Process
1. Generate lexer (flex)
2. Generate parser (bison)
3. Compile C source files
4. Link object files
5. Run tests

## 10. Future Enhancements

1. Optimization passes:
   - Dataflow analysis
   - Register allocation graphs
   - Peephole optimization

2. Language features:
   - Loop constructs
   - Array support
   - Structure types
   - Global variables

3. Development tools:
   - Debugger support
   - Profile-guided optimization
   - Source-level error messages 