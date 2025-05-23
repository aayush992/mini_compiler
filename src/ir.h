#ifndef IR_H
#define IR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// IR Operation Types
typedef enum {
    IR_ASSIGN,      // Simple assignment
    IR_ADD,         // Addition
    IR_SUB,         // Subtraction
    IR_MUL,         // Multiplication
    IR_DIV,         // Division
    IR_AND,         // Logical AND
    IR_OR,          // Logical OR
    IR_NOT,         // Logical NOT
    IR_EQ,          // Equal comparison
    IR_NEQ,         // Not equal comparison
    IR_LT,          // Less than
    IR_GT,          // Greater than
    IR_LEQ,         // Less than or equal
    IR_GEQ,         // Greater than or equal
    IR_LABEL,       // Label for jumps
    IR_JUMP,        // Unconditional jump
    IR_CJUMP,       // Conditional jump
    IR_STRING,      // String operation
    IR_PARAM,       // Function parameter
    IR_CALL,        // Function call
    IR_RETURN       // Return statement
} IROpType;

// IR Operand Types
typedef enum {
    OP_NONE,
    OP_INT,
    OP_FLOAT,
    OP_STRING,
    OP_BOOL,
    OP_TEMP,
    OP_LABEL
} OperandType;

// IR Operand structure
typedef struct {
    OperandType type;
    union {
        int int_val;
        float float_val;
        char* str_val;
        int bool_val;
        char* temp_name;
        char* label_name;
    } val;
} IROperand;

// IR Instruction structure
typedef struct IRInst {
    IROpType op;
    IROperand dest;
    IROperand src1;
    IROperand src2;
    struct IRInst* next;
} IRInst;

// IR Basic Block structure
typedef struct BasicBlock {
    char* label;
    IRInst* first_inst;
    IRInst* last_inst;
    struct BasicBlock* next;
} BasicBlock;

// IR Function structure
typedef struct IRFunction {
    char* name;
    BasicBlock* entry_block;
    struct IRFunction* next;
} IRFunction;

// IR Program structure
typedef struct {
    IRFunction* first_func;
    char** string_literals;
    int string_count;
} IRProgram;

// Function declarations
IRProgram* create_ir_program();
IRFunction* create_ir_function(const char* name);
BasicBlock* create_basic_block(const char* label);
IRInst* create_ir_inst(IROpType op, IROperand dest, IROperand src1, IROperand src2);
IROperand create_int_operand(int value);
IROperand create_float_operand(float value);
IROperand create_string_operand(const char* value);
IROperand create_bool_operand(int value);
IROperand create_temp_operand(const char* name);
IROperand create_label_operand(const char* name);
void add_inst_to_block(BasicBlock* block, IRInst* inst);
void add_block_to_function(IRFunction* func, BasicBlock* block);
void add_function_to_program(IRProgram* prog, IRFunction* func);
void print_ir_program(IRProgram* prog);
void free_ir_program(IRProgram* prog);

#endif /* IR_H */ 