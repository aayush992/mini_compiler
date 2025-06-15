#include <stdio.h>
#include "ir.h"
#include "codegen.h"

int main() {
    // Create IR program
    IRProgram* prog = create_ir_program();
    
    // Create main function
    IRFunction* main_func = create_ir_function("main");
    
    // Create entry block
    BasicBlock* entry = create_basic_block("entry");
    
    // Create some test instructions
    // int x = 42;
    IRInst* inst1 = create_ir_inst(IR_ASSIGN,
        create_temp_operand("x"),
        create_int_operand(42),
        (IROperand){.type = OP_NONE});
    add_inst_to_block(entry, inst1);
    
    // int y = 10;
    IRInst* inst2 = create_ir_inst(IR_ASSIGN,
        create_temp_operand("y"),
        create_int_operand(10),
        (IROperand){.type = OP_NONE});
    add_inst_to_block(entry, inst2);
    
    // int z = x + y;
    IRInst* inst3 = create_ir_inst(IR_ADD,
        create_temp_operand("z"),
        create_temp_operand("x"),
        create_temp_operand("y"));
    add_inst_to_block(entry, inst3);
    
    // Add block to function
    add_block_to_function(main_func, entry);
    
    // Add function to program
    add_function_to_program(prog, main_func);
    
    // Create output file
    FILE* output = fopen("output.asm", "w");
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
    
    printf("Assembly code generated in output.asm\n");
    return 0;
} 