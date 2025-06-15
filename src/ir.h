#ifndef IR_H
#define IR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// IR Operand types
typedef enum {
    IR_TEMP,
    IR_VAR,
    IR_CONST,
    IR_LABEL
} IROperandType;

// IR Operand structure
typedef struct {
    IROperandType type;
    union {
        int temp_num;
        char* var_name;
        int const_val;
        char* label_name;
    } value;
} IROperand;

// IR Instruction types
typedef enum {
    IR_ASSIGN,
    IR_ADD,
    IR_SUB,
    IR_MUL,
    IR_DIV,
    IR_JUMP,
    IR_COND_JUMP,
    IR_CALL,
    IR_RETURN
} IRInstructionType;

// IR Instruction structure
typedef struct IRInstruction {
    IRInstructionType type;
    IROperand result;
    IROperand op1;
    IROperand op2;
    struct IRInstruction* next;
} IRInstruction;

// Basic Block structure
typedef struct BasicBlock {
    char* label;
    IRInstruction* instructions;
    struct BasicBlock* next;
} BasicBlock;

// Function structure
typedef struct IRFunction {
    char* name;
    BasicBlock* blocks;
    struct IRFunction* next;
} IRFunction;

// Program structure
typedef struct {
    IRFunction* functions;
} IRProgram;

// Code Generation Context
typedef struct {
    IRProgram* program;
    FILE* output;
    int temp_count;
    int label_count;
} CodeGenContext;

// Function declarations
IROperand create_temp();
IROperand create_label();
IROperand create_var(const char* name);
IROperand create_const(int value);

IRProgram* create_ir_program();
void free_ir_program(IRProgram* program);

CodeGenContext* create_codegen_context(IRProgram* program, FILE* output);
void generate_code(CodeGenContext* ctx);
void free_codegen_context(CodeGenContext* ctx);

#endif // IR_H 