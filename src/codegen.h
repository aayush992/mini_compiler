#ifndef CODEGEN_H
#define CODEGEN_H

#include "ir.h"

// Target architecture specific
#define WORD_SIZE 4
#define STACK_ALIGN 16

// Register allocation
typedef enum {
    REG_EAX,
    REG_EBX,
    REG_ECX,
    REG_EDX,
    REG_ESI,
    REG_EDI,
    REG_ESP,
    REG_EBP,
    REG_COUNT
} Register;

// Variable location
typedef struct {
    enum { LOC_REG, LOC_STACK, LOC_GLOBAL } type;
    union {
        Register reg;
        int offset;     // Stack offset or global index
    } loc;
} VarLocation;

// Code generator context
typedef struct {
    FILE* output;
    IRProgram* ir;
    int stack_offset;
    int label_count;
    VarLocation* var_locations;
    int var_count;
    char** string_literals;
    int string_count;
} CodeGenContext;

// Function declarations
CodeGenContext* create_codegen_context(IRProgram* ir, FILE* output);
void generate_code(CodeGenContext* ctx);
void free_codegen_context(CodeGenContext* ctx);

// Assembly generation helpers
void emit_prologue(CodeGenContext* ctx);
void emit_epilogue(CodeGenContext* ctx);
void emit_function(CodeGenContext* ctx, IRFunction* func);
void emit_basic_block(CodeGenContext* ctx, BasicBlock* block);
void emit_instruction(CodeGenContext* ctx, IRInst* inst);
void emit_data_section(CodeGenContext* ctx);
void emit_string_literal(CodeGenContext* ctx, int index, const char* str);

#endif /* CODEGEN_H */ 