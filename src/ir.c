#include "ir.h"

// Create a new IR program
IRProgram* create_ir_program() {
    IRProgram* prog = (IRProgram*)malloc(sizeof(IRProgram));
    prog->functions = NULL;
    return prog;
}

// Create a new IR function
IRFunction* create_ir_function(const char* name) {
    IRFunction* func = (IRFunction*)malloc(sizeof(IRFunction));
    func->name = strdup(name);
    func->blocks = NULL;
    func->next = NULL;
    return func;
}

// Create a new basic block
BasicBlock* create_basic_block(const char* label) {
    BasicBlock* block = (BasicBlock*)malloc(sizeof(BasicBlock));
    block->label = strdup(label);
    block->instructions = NULL;
    block->next = NULL;
    return block;
}

// Create a new IR instruction
IRInstruction* create_ir_instruction(IRInstructionType type, IROperand result, IROperand op1, IROperand op2) {
    IRInstruction* inst = (IRInstruction*)malloc(sizeof(IRInstruction));
    inst->type = type;
    inst->result = result;
    inst->op1 = op1;
    inst->op2 = op2;
    inst->next = NULL;
    return inst;
}

// Create a temporary operand
IROperand create_temp() {
    static int temp_count = 0;
    IROperand op;
    op.type = IR_TEMP;
    op.value.temp_num = temp_count++;
    return op;
}

// Create a label operand
IROperand create_label() {
    static int label_count = 0;
    IROperand op;
    op.type = IR_LABEL;
    char* label = malloc(32);
    sprintf(label, "L%d", label_count++);
    op.value.label_name = label;
    return op;
}

// Create a variable operand
IROperand create_var(const char* name) {
    IROperand op;
    op.type = IR_VAR;
    op.value.var_name = strdup(name);
    return op;
}

// Create a constant operand
IROperand create_const(int value) {
    IROperand op;
    op.type = IR_CONST;
    op.value.const_val = value;
    return op;
}

// Add an instruction to a basic block
void add_instruction(BasicBlock* block, IRInstruction* inst) {
    if (!block->instructions) {
        block->instructions = inst;
    } else {
        IRInstruction* current = block->instructions;
        while (current->next) {
            current = current->next;
        }
        current->next = inst;
    }
}

// Add a basic block to a function
void add_block(IRFunction* func, BasicBlock* block) {
    if (!func->blocks) {
        func->blocks = block;
    } else {
        BasicBlock* current = func->blocks;
        while (current->next) {
            current = current->next;
        }
        current->next = block;
    }
}

// Add a function to a program
void add_function(IRProgram* prog, IRFunction* func) {
    if (!prog->functions) {
        prog->functions = func;
    } else {
        IRFunction* current = prog->functions;
        while (current->next) {
            current = current->next;
        }
        current->next = func;
    }
}

// Create code generation context
CodeGenContext* create_codegen_context(IRProgram* program, FILE* output) {
    CodeGenContext* ctx = (CodeGenContext*)malloc(sizeof(CodeGenContext));
    ctx->program = program;
    ctx->output = output;
    ctx->temp_count = 0;
    ctx->label_count = 0;
    return ctx;
}

// Generate code
void generate_code(CodeGenContext* ctx) {
    IRFunction* func = ctx->program->functions;
    while (func) {
        fprintf(ctx->output, "function %s:\n", func->name);
        
        BasicBlock* block = func->blocks;
        while (block) {
            fprintf(ctx->output, "%s:\n", block->label);
            
            IRInstruction* inst = block->instructions;
            while (inst) {
                switch (inst->type) {
                    case IR_ASSIGN:
                        fprintf(ctx->output, "  %s = %s\n", 
                            inst->result.value.var_name,
                            inst->op1.value.var_name);
                        break;
                    case IR_ADD:
                        fprintf(ctx->output, "  %s = %s + %s\n",
                            inst->result.value.var_name,
                            inst->op1.value.var_name,
                            inst->op2.value.var_name);
                        break;
                    case IR_SUB:
                        fprintf(ctx->output, "  %s = %s - %s\n",
                            inst->result.value.var_name,
                            inst->op1.value.var_name,
                            inst->op2.value.var_name);
                        break;
                    case IR_MUL:
                        fprintf(ctx->output, "  %s = %s * %s\n",
                            inst->result.value.var_name,
                            inst->op1.value.var_name,
                            inst->op2.value.var_name);
                        break;
                    case IR_DIV:
                        fprintf(ctx->output, "  %s = %s / %s\n",
                            inst->result.value.var_name,
                            inst->op1.value.var_name,
                            inst->op2.value.var_name);
                        break;
                    case IR_JUMP:
                        fprintf(ctx->output, "  jump %s\n",
                            inst->result.value.label_name);
                        break;
                    case IR_COND_JUMP:
                        fprintf(ctx->output, "  if %s jump %s\n",
                            inst->op1.value.var_name,
                            inst->result.value.label_name);
                        break;
                    case IR_CALL:
                        fprintf(ctx->output, "  call %s\n",
                            inst->result.value.var_name);
                        break;
                    case IR_RETURN:
                        fprintf(ctx->output, "  return %s\n",
                            inst->result.value.var_name);
                        break;
                }
                inst = inst->next;
            }
            block = block->next;
        }
        func = func->next;
    }
}

// Free code generation context
void free_codegen_context(CodeGenContext* ctx) {
    free(ctx);
}

// Free IR program
void free_ir_program(IRProgram* program) {
    if (!program) return;
    
    IRFunction* func = program->functions;
    while (func) {
        IRFunction* next_func = func->next;
        
        BasicBlock* block = func->blocks;
        while (block) {
            BasicBlock* next_block = block->next;
            
            IRInstruction* inst = block->instructions;
            while (inst) {
                IRInstruction* next_inst = inst->next;
                free(inst);
                inst = next_inst;
            }
            
            free(block->label);
            free(block);
            block = next_block;
        }
        
        free(func->name);
        free(func);
        func = next_func;
    }
    
    free(program);
}

// Create a new IR program
IRProgram* create_ir_program() {
    IRProgram* prog = (IRProgram*)malloc(sizeof(IRProgram));
    prog->functions = NULL;
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
    if (!prog->functions) {
        prog->functions = func;
    } else {
        IRFunction* current = prog->functions;
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
    IRFunction* func = prog->functions;
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

void free_ir_program(IRProgram* program) {
    if (!program) return;
    
    // Free functions
    IRFunction* func = program->functions;
    while (func) {
        IRFunction* next = func->next;
        
        // Free basic blocks
        BasicBlock* block = func->entry_block;
        while (block) {
            BasicBlock* next_block = block->next;
            
            // Free instructions
            IRInst* inst = block->first_inst;
            while (inst) {
                IRInst* next_inst = inst->next;
                free_ir_inst(inst);
                inst = next_inst;
            }
            
            free(block->label);
            free(block);
            block = next_block;
        }
        
        free(func->name);
        free(func);
        func = next;
    }
    
    free(program);
} 