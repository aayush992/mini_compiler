#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Register names for x86
static const char* reg_names[] = {
    "eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp"
};

CodeGenContext* create_codegen_context(IRProgram* ir, FILE* output) {
    CodeGenContext* ctx = (CodeGenContext*)malloc(sizeof(CodeGenContext));
    ctx->output = output;
    ctx->ir = ir;
    ctx->stack_offset = 0;
    ctx->label_count = 0;
    ctx->var_locations = NULL;
    ctx->var_count = 0;
    ctx->string_literals = NULL;
    ctx->string_count = 0;
    return ctx;
}

void emit_data_section(CodeGenContext* ctx) {
    fprintf(ctx->output, "section .data\n");
    fprintf(ctx->output, "true_val: dd 1\n");
    fprintf(ctx->output, "false_val: dd 0\n");
    
    // Emit string literals
    for (int i = 0; i < ctx->string_count; i++) {
        fprintf(ctx->output, "str_%d: db \"%s\", 0\n", i, ctx->string_literals[i]);
    }
    
    fprintf(ctx->output, "\n");
}

void emit_prologue(CodeGenContext* ctx) {
    fprintf(ctx->output, "section .text\n");
    fprintf(ctx->output, "global main\n\n");
    fprintf(ctx->output, "main:\n");
    fprintf(ctx->output, "    push ebp\n");
    fprintf(ctx->output, "    mov ebp, esp\n");
    
    // Align stack and reserve space for locals
    int aligned_offset = ((ctx->var_count * WORD_SIZE + STACK_ALIGN - 1) & ~(STACK_ALIGN - 1));
    fprintf(ctx->output, "    sub esp, %d\n", aligned_offset);
}

void emit_epilogue(CodeGenContext* ctx) {
    fprintf(ctx->output, "    mov esp, ebp\n");
    fprintf(ctx->output, "    pop ebp\n");
    fprintf(ctx->output, "    ret\n");
}

void emit_comparison(CodeGenContext* ctx, IRInst* inst, const char* jump_condition) {
    // Load first operand into eax
    fprintf(ctx->output, "    mov eax, ");
    if (inst->src1.type == OP_INT) {
        fprintf(ctx->output, "%d\n", inst->src1.val.int_val);
    } else if (inst->src1.type == OP_TEMP) {
        fprintf(ctx->output, "[ebp-%d]\n", 4); // Simplified: assume all temps are on stack
    }
    
    // Compare with second operand
    fprintf(ctx->output, "    cmp eax, ");
    if (inst->src2.type == OP_INT) {
        fprintf(ctx->output, "%d\n", inst->src2.val.int_val);
    } else if (inst->src2.type == OP_TEMP) {
        fprintf(ctx->output, "[ebp-%d]\n", 8);
    }
    
    // Generate unique labels
    char true_label[32], end_label[32];
    sprintf(true_label, ".L%d_true", ctx->label_count++);
    sprintf(end_label, ".L%d_end", ctx->label_count++);
    
    // Conditional jump
    fprintf(ctx->output, "    %s %s\n", jump_condition, true_label);
    
    // False case
    fprintf(ctx->output, "    mov dword [ebp-%d], 0\n", 12);
    fprintf(ctx->output, "    jmp %s\n", end_label);
    
    // True case
    fprintf(ctx->output, "%s:\n", true_label);
    fprintf(ctx->output, "    mov dword [ebp-%d], 1\n", 12);
    
    // End
    fprintf(ctx->output, "%s:\n", end_label);
}

void emit_binary_operation(CodeGenContext* ctx, IRInst* inst, const char* op) {
    // Load first operand into eax
    fprintf(ctx->output, "    mov eax, ");
    if (inst->src1.type == OP_INT) {
        fprintf(ctx->output, "%d\n", inst->src1.val.int_val);
    } else if (inst->src1.type == OP_TEMP) {
        fprintf(ctx->output, "[ebp-%d]\n", 4);
    }
    
    // Perform operation with second operand
    fprintf(ctx->output, "    %s eax, ", op);
    if (inst->src2.type == OP_INT) {
        fprintf(ctx->output, "%d\n", inst->src2.val.int_val);
    } else if (inst->src2.type == OP_TEMP) {
        fprintf(ctx->output, "[ebp-%d]\n", 8);
    }
    
    // Store result
    fprintf(ctx->output, "    mov [ebp-%d], eax\n", 12);
}

void emit_logical_operation(CodeGenContext* ctx, IRInst* inst, const char* op) {
    // Load operands
    fprintf(ctx->output, "    mov eax, [ebp-%d]\n", 4);
    fprintf(ctx->output, "    mov ebx, [ebp-%d]\n", 8);
    
    // Perform logical operation
    if (strcmp(op, "and") == 0) {
        fprintf(ctx->output, "    and eax, ebx\n");
    } else if (strcmp(op, "or") == 0) {
        fprintf(ctx->output, "    or eax, ebx\n");
    } else if (strcmp(op, "not") == 0) {
        fprintf(ctx->output, "    xor eax, 1\n");
    }
    
    // Store result
    fprintf(ctx->output, "    mov [ebp-%d], eax\n", 12);
}

void emit_instruction(CodeGenContext* ctx, IRInst* inst) {
    switch (inst->op) {
        case IR_ASSIGN:
            if (inst->src1.type == OP_INT || inst->src1.type == OP_BOOL) {
                fprintf(ctx->output, "    mov dword [ebp-%d], %d\n", 
                    4, // Simplified: fixed offset
                    inst->src1.val.int_val);
            }
            break;
            
        case IR_ADD:
            emit_binary_operation(ctx, inst, "add");
            break;
            
        case IR_SUB:
            emit_binary_operation(ctx, inst, "sub");
            break;
            
        case IR_MUL:
            emit_binary_operation(ctx, inst, "imul");
            break;
            
        case IR_DIV:
            // Save ebx as it will be clobbered
            fprintf(ctx->output, "    push ebx\n");
            // Load dividend into eax
            fprintf(ctx->output, "    mov eax, [ebp-%d]\n", 4);
            // Sign extend eax into edx
            fprintf(ctx->output, "    cdq\n");
            // Load divisor into ebx
            fprintf(ctx->output, "    mov ebx, [ebp-%d]\n", 8);
            // Perform division
            fprintf(ctx->output, "    idiv ebx\n");
            // Store result
            fprintf(ctx->output, "    mov [ebp-%d], eax\n", 12);
            // Restore ebx
            fprintf(ctx->output, "    pop ebx\n");
            break;
            
        case IR_AND:
            emit_logical_operation(ctx, inst, "and");
            break;
            
        case IR_OR:
            emit_logical_operation(ctx, inst, "or");
            break;
            
        case IR_NOT:
            emit_logical_operation(ctx, inst, "not");
            break;
            
        case IR_EQ:
            emit_comparison(ctx, inst, "je");
            break;
            
        case IR_NEQ:
            emit_comparison(ctx, inst, "jne");
            break;
            
        case IR_LT:
            emit_comparison(ctx, inst, "jl");
            break;
            
        case IR_GT:
            emit_comparison(ctx, inst, "jg");
            break;
            
        case IR_LEQ:
            emit_comparison(ctx, inst, "jle");
            break;
            
        case IR_GEQ:
            emit_comparison(ctx, inst, "jge");
            break;
            
        case IR_LABEL:
            fprintf(ctx->output, "%s:\n", inst->dest.val.label_name);
            break;
            
        case IR_JUMP:
            fprintf(ctx->output, "    jmp %s\n", inst->dest.val.label_name);
            break;
            
        case IR_CJUMP:
            fprintf(ctx->output, "    mov eax, [ebp-%d]\n", 4);
            fprintf(ctx->output, "    test eax, eax\n");
            fprintf(ctx->output, "    jz %s\n", inst->dest.val.label_name);
            break;
            
        default:
            fprintf(stderr, "Warning: Unhandled IR instruction type\n");
            break;
    }
}

void emit_basic_block(CodeGenContext* ctx, BasicBlock* block) {
    fprintf(ctx->output, "%s:\n", block->label);
    
    IRInst* inst = block->first_inst;
    while (inst) {
        emit_instruction(ctx, inst);
        inst = inst->next;
    }
}

void emit_function(CodeGenContext* ctx, IRFunction* func) {
    fprintf(ctx->output, "%s:\n", func->name);
    
    // Function prologue
    fprintf(ctx->output, "    push ebp\n");
    fprintf(ctx->output, "    mov ebp, esp\n");
    
    // Emit each basic block
    BasicBlock* block = func->entry_block;
    while (block) {
        emit_basic_block(ctx, block);
        block = block->next;
    }
    
    // Function epilogue
    fprintf(ctx->output, "    mov esp, ebp\n");
    fprintf(ctx->output, "    pop ebp\n");
    fprintf(ctx->output, "    ret\n");
}

void generate_code(CodeGenContext* ctx) {
    // Emit data section
    emit_data_section(ctx);
    
    // Emit text section
    fprintf(ctx->output, "section .text\n");
    fprintf(ctx->output, "global main\n\n");
    
    // Emit each function
    IRFunction* func = ctx->ir->first_func;
    while (func) {
        emit_function(ctx, func);
        func = func->next;
    }
}

void free_codegen_context(CodeGenContext* ctx) {
    if (!ctx) return;
    
    // Free string literals
    for (int i = 0; i < ctx->string_count; i++) {
        free(ctx->string_literals[i]);
    }
    free(ctx->string_literals);
    
    // Free variable locations
    free(ctx->var_locations);
    
    free(ctx);
} 