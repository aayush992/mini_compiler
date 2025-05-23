#include "ir.h"

IRProgram* create_ir_program() {
    IRProgram* prog = (IRProgram*)malloc(sizeof(IRProgram));
    prog->first_func = NULL;
    prog->string_literals = NULL;
    prog->string_count = 0;
    return prog;
}

IRFunction* create_ir_function(const char* name) {
    IRFunction* func = (IRFunction*)malloc(sizeof(IRFunction));
    func->name = strdup(name);
    func->entry_block = NULL;
    func->next = NULL;
    return func;
}

BasicBlock* create_basic_block(const char* label) {
    BasicBlock* block = (BasicBlock*)malloc(sizeof(BasicBlock));
    block->label = strdup(label);
    block->first_inst = NULL;
    block->last_inst = NULL;
    block->next = NULL;
    return block;
}

IRInst* create_ir_inst(IROpType op, IROperand dest, IROperand src1, IROperand src2) {
    IRInst* inst = (IRInst*)malloc(sizeof(IRInst));
    inst->op = op;
    inst->dest = dest;
    inst->src1 = src1;
    inst->src2 = src2;
    inst->next = NULL;
    return inst;
}

IROperand create_int_operand(int value) {
    IROperand op;
    op.type = OP_INT;
    op.val.int_val = value;
    return op;
}

IROperand create_float_operand(float value) {
    IROperand op;
    op.type = OP_FLOAT;
    op.val.float_val = value;
    return op;
}

IROperand create_string_operand(const char* value) {
    IROperand op;
    op.type = OP_STRING;
    op.val.str_val = strdup(value);
    return op;
}

IROperand create_bool_operand(int value) {
    IROperand op;
    op.type = OP_BOOL;
    op.val.bool_val = value;
    return op;
}

IROperand create_temp_operand(const char* name) {
    IROperand op;
    op.type = OP_TEMP;
    op.val.temp_name = strdup(name);
    return op;
}

IROperand create_label_operand(const char* name) {
    IROperand op;
    op.type = OP_LABEL;
    op.val.label_name = strdup(name);
    return op;
}

void add_inst_to_block(BasicBlock* block, IRInst* inst) {
    if (!block->first_inst) {
        block->first_inst = inst;
    } else {
        block->last_inst->next = inst;
    }
    block->last_inst = inst;
}

void add_block_to_function(IRFunction* func, BasicBlock* block) {
    if (!func->entry_block) {
        func->entry_block = block;
    } else {
        BasicBlock* current = func->entry_block;
        while (current->next) {
            current = current->next;
        }
        current->next = block;
    }
}

void add_function_to_program(IRProgram* prog, IRFunction* func) {
    if (!prog->first_func) {
        prog->first_func = func;
    } else {
        IRFunction* current = prog->first_func;
        while (current->next) {
            current = current->next;
        }
        current->next = func;
    }
}

void print_ir_operand(IROperand op) {
    switch (op.type) {
        case OP_INT:
            printf("%d", op.val.int_val);
            break;
        case OP_FLOAT:
            printf("%f", op.val.float_val);
            break;
        case OP_STRING:
            printf("\"%s\"", op.val.str_val);
            break;
        case OP_BOOL:
            printf("%s", op.val.bool_val ? "true" : "false");
            break;
        case OP_TEMP:
            printf("%s", op.val.temp_name);
            break;
        case OP_LABEL:
            printf("%s", op.val.label_name);
            break;
        case OP_NONE:
            printf("none");
            break;
    }
}

void print_ir_inst(IRInst* inst) {
    switch (inst->op) {
        case IR_ASSIGN:
            printf("  ");
            print_ir_operand(inst->dest);
            printf(" = ");
            print_ir_operand(inst->src1);
            break;
        case IR_ADD:
            printf("  ");
            print_ir_operand(inst->dest);
            printf(" = ");
            print_ir_operand(inst->src1);
            printf(" + ");
            print_ir_operand(inst->src2);
            break;
        // Add other operations as needed
        case IR_LABEL:
            print_ir_operand(inst->dest);
            printf(":");
            break;
        default:
            printf("  <unknown operation>");
    }
    printf("\n");
}

void print_ir_program(IRProgram* prog) {
    printf("IR Program:\n");
    
    // Print string literals
    printf("String literals:\n");
    for (int i = 0; i < prog->string_count; i++) {
        printf("  str_%d: \"%s\"\n", i, prog->string_literals[i]);
    }
    printf("\n");

    // Print functions
    IRFunction* func = prog->first_func;
    while (func) {
        printf("function %s:\n", func->name);
        BasicBlock* block = func->entry_block;
        while (block) {
            printf("%s:\n", block->label);
            IRInst* inst = block->first_inst;
            while (inst) {
                print_ir_inst(inst);
                inst = inst->next;
            }
            block = block->next;
        }
        func = func->next;
    }
}

void free_ir_operand(IROperand op) {
    switch (op.type) {
        case OP_STRING:
            free(op.val.str_val);
            break;
        case OP_TEMP:
            free(op.val.temp_name);
            break;
        case OP_LABEL:
            free(op.val.label_name);
            break;
        default:
            break;
    }
}

void free_ir_inst(IRInst* inst) {
    if (!inst) return;
    free_ir_operand(inst->dest);
    free_ir_operand(inst->src1);
    free_ir_operand(inst->src2);
    free(inst);
}

void free_basic_block(BasicBlock* block) {
    if (!block) return;
    IRInst* inst = block->first_inst;
    while (inst) {
        IRInst* next = inst->next;
        free_ir_inst(inst);
        inst = next;
    }
    free(block->label);
    free(block);
}

void free_ir_function(IRFunction* func) {
    if (!func) return;
    BasicBlock* block = func->entry_block;
    while (block) {
        BasicBlock* next = block->next;
        free_basic_block(block);
        block = next;
    }
    free(func->name);
    free(func);
}

void free_ir_program(IRProgram* prog) {
    if (!prog) return;
    
    // Free string literals
    for (int i = 0; i < prog->string_count; i++) {
        free(prog->string_literals[i]);
    }
    free(prog->string_literals);

    // Free functions
    IRFunction* func = prog->first_func;
    while (func) {
        IRFunction* next = func->next;
        free_ir_function(func);
        func = next;
    }
    
    free(prog);
} 