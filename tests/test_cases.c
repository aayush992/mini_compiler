#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "ir.h"
#include "codegen.h"

// Forward declarations
void create_arithmetic_test(BasicBlock* block);
void create_boolean_test(BasicBlock* block);
void create_comparison_test(BasicBlock* block);
void create_control_flow_test(IRFunction* func);

// Helper function to create a test block
void create_arithmetic_test(BasicBlock* block) {
    // Test arithmetic operations
    // int a = 42;
    IRInst* inst1 = create_ir_inst(IR_ASSIGN,
        create_temp_operand("a"),
        create_int_operand(42),
        (IROperand){.type = OP_NONE});
    add_inst_to_block(block, inst1);
    
    // int b = 10;
    IRInst* inst2 = create_ir_inst(IR_ASSIGN,
        create_temp_operand("b"),
        create_int_operand(10),
        (IROperand){.type = OP_NONE});
    add_inst_to_block(block, inst2);
    
    // int sum = a + b;
    IRInst* add = create_ir_inst(IR_ADD,
        create_temp_operand("sum"),
        create_temp_operand("a"),
        create_temp_operand("b"));
    add_inst_to_block(block, add);
    
    // int diff = a - b;
    IRInst* sub = create_ir_inst(IR_SUB,
        create_temp_operand("diff"),
        create_temp_operand("a"),
        create_temp_operand("b"));
    add_inst_to_block(block, sub);
    
    // int prod = a * b;
    IRInst* mul = create_ir_inst(IR_MUL,
        create_temp_operand("prod"),
        create_temp_operand("a"),
        create_temp_operand("b"));
    add_inst_to_block(block, mul);
    
    // int quot = a / b;
    IRInst* div = create_ir_inst(IR_DIV,
        create_temp_operand("quot"),
        create_temp_operand("a"),
        create_temp_operand("b"));
    add_inst_to_block(block, div);
}

void create_boolean_test(BasicBlock* block) {
    // Test boolean operations
    // bool x = true;
    IRInst* inst1 = create_ir_inst(IR_ASSIGN,
        create_temp_operand("x"),
        create_bool_operand(1),
        (IROperand){.type = OP_NONE});
    add_inst_to_block(block, inst1);
    
    // bool y = false;
    IRInst* inst2 = create_ir_inst(IR_ASSIGN,
        create_temp_operand("y"),
        create_bool_operand(0),
        (IROperand){.type = OP_NONE});
    add_inst_to_block(block, inst2);
    
    // bool and_result = x && y;
    IRInst* and = create_ir_inst(IR_AND,
        create_temp_operand("and_result"),
        create_temp_operand("x"),
        create_temp_operand("y"));
    add_inst_to_block(block, and);
    
    // bool or_result = x || y;
    IRInst* or = create_ir_inst(IR_OR,
        create_temp_operand("or_result"),
        create_temp_operand("x"),
        create_temp_operand("y"));
    add_inst_to_block(block, or);
    
    // bool not_result = !x;
    IRInst* not = create_ir_inst(IR_NOT,
        create_temp_operand("not_result"),
        create_temp_operand("x"),
        (IROperand){.type = OP_NONE});
    add_inst_to_block(block, not);
}

void create_comparison_test(BasicBlock* block) {
    // Test comparison operations
    // int a = 5;
    IRInst* inst1 = create_ir_inst(IR_ASSIGN,
        create_temp_operand("a"),
        create_int_operand(5),
        (IROperand){.type = OP_NONE});
    add_inst_to_block(block, inst1);
    
    // int b = 10;
    IRInst* inst2 = create_ir_inst(IR_ASSIGN,
        create_temp_operand("b"),
        create_int_operand(10),
        (IROperand){.type = OP_NONE});
    add_inst_to_block(block, inst2);
    
    // bool eq = a == b;
    IRInst* eq = create_ir_inst(IR_EQ,
        create_temp_operand("eq"),
        create_temp_operand("a"),
        create_temp_operand("b"));
    add_inst_to_block(block, eq);
    
    // bool neq = a != b;
    IRInst* neq = create_ir_inst(IR_NEQ,
        create_temp_operand("neq"),
        create_temp_operand("a"),
        create_temp_operand("b"));
    add_inst_to_block(block, neq);
    
    // bool lt = a < b;
    IRInst* lt = create_ir_inst(IR_LT,
        create_temp_operand("lt"),
        create_temp_operand("a"),
        create_temp_operand("b"));
    add_inst_to_block(block, lt);
    
    // bool gt = a > b;
    IRInst* gt = create_ir_inst(IR_GT,
        create_temp_operand("gt"),
        create_temp_operand("a"),
        create_temp_operand("b"));
    add_inst_to_block(block, gt);
}

void create_control_flow_test(IRFunction* func) {
    // Create blocks for if-else structure
    BasicBlock* entry = create_basic_block("entry");
    BasicBlock* then_block = create_basic_block("then");
    BasicBlock* else_block = create_basic_block("else");
    BasicBlock* exit_block = create_basic_block("exit");
    
    // Entry block: set up condition
    IRInst* cond = create_ir_inst(IR_ASSIGN,
        create_temp_operand("cond"),
        create_bool_operand(1),
        (IROperand){.type = OP_NONE});
    add_inst_to_block(entry, cond);
    
    // Add conditional jump
    IRInst* cjump = create_ir_inst(IR_CJUMP,
        create_label_operand("else"),
        create_temp_operand("cond"),
        (IROperand){.type = OP_NONE});
    add_inst_to_block(entry, cjump);
    
    // Then block
    IRInst* then_inst = create_ir_inst(IR_ASSIGN,
        create_temp_operand("result"),
        create_int_operand(1),
        (IROperand){.type = OP_NONE});
    add_inst_to_block(then_block, then_inst);
    
    // Jump to exit
    IRInst* then_jump = create_ir_inst(IR_JUMP,
        create_label_operand("exit"),
        (IROperand){.type = OP_NONE},
        (IROperand){.type = OP_NONE});
    add_inst_to_block(then_block, then_jump);
    
    // Else block
    IRInst* else_inst = create_ir_inst(IR_ASSIGN,
        create_temp_operand("result"),
        create_int_operand(0),
        (IROperand){.type = OP_NONE});
    add_inst_to_block(else_block, else_inst);
    
    // Add blocks to function
    add_block_to_function(func, entry);
    add_block_to_function(func, then_block);
    add_block_to_function(func, else_block);
    add_block_to_function(func, exit_block);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Allocate a console for output
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    // Create IR program
    IRProgram* prog = create_ir_program();
    
    // Create main function
    IRFunction* main_func = create_ir_function("main");
    
    // Create test blocks
    BasicBlock* arith_block = create_basic_block("arithmetic_test");
    create_arithmetic_test(arith_block);
    add_block_to_function(main_func, arith_block);
    
    BasicBlock* bool_block = create_basic_block("boolean_test");
    create_boolean_test(bool_block);
    add_block_to_function(main_func, bool_block);
    
    BasicBlock* comp_block = create_basic_block("comparison_test");
    create_comparison_test(comp_block);
    add_block_to_function(main_func, comp_block);
    
    // Add control flow test
    create_control_flow_test(main_func);
    
    // Add function to program
    add_function_to_program(prog, main_func);
    
    // Print IR for verification
    printf("Generated IR:\n");
    print_ir_program(prog);
    
    // Create output file
    FILE* output = fopen("test_output.asm", "w");
    if (!output) {
        fprintf(stderr, "Error: Cannot create output file\n");
        return 1;
    }
    
    // Create code generator context
    CodeGenContext* ctx = create_codegen_context(prog, output);
    
    // Generate code
    generate_code(ctx);
    
    // Clean up
    fclose(output);
    free_codegen_context(ctx);
    free_ir_program(prog);
    
    printf("\nAssembly code generated in test_output.asm\n");
    printf("\nPress any key to exit...");
    getchar();
    
    return 0;
} 