# Mini Compiler

A simplified compiler implementation featuring lexical analysis, parsing, and x86 assembly code generation.

## Language Features

### Data Types
- `int`: Integer values
- `float`: Floating-point values
- `bool`: Boolean values (true/false)
- `string`: Text strings with escape sequences
- `void`: Used for functions with no return value
- Arrays of all basic types

### Operators
- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Logical: `&&`, `||`, `!`
- Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Assignment: `=`
- Array indexing: `[]`

### Control Flow
- If-else statements
- While loops
- For loops
- Basic blocks
- Function calls

## Project Structure

```
mini_compiler/
├── src/
│   ├── lexer.l          # Flex lexical analyzer
│   ├── parser.y         # Bison parser
│   ├── ir.h/c          # Intermediate representation
│   ├── codegen.h/c     # x86 code generation
│   ├── main.c          # Main compiler driver
│   └── Makefile        # Build configuration
├── tests/
│   ├── arithmetic_test.txt
│   ├── array_ops.txt
│   ├── basic_arithmetic.txt
│   ├── control_flow.txt
│   ├── error_handling.txt
│   ├── functions.txt
│   ├── logical_ops_new.txt
│   └── type_checking.txt
└── docs/
    ├── language.md     # Language specification
    └── compiler.md     # Compiler architecture
```

## Building the Compiler

### Prerequisites
- GCC compiler
- Flex lexical analyzer
- Bison parser generator
- NASM assembler (for x86 output)

### Build Instructions
1. Clone the repository
2. Navigate to the src directory
3. Run make to build the compiler

```bash
git clone <repository-url>
cd mini_compiler/src
make
```

## Usage

### Basic Compilation
```bash
./mini_compiler input.txt -o output
```

### Example Program
```c
// Sample program
int main() {
    int x = 42;
    float y = 10.5;
    int arr[5];
    
    arr[0] = x;
    arr[1] = y;
    
    if (x > y) {
        return arr[0] + arr[1];
    } else {
        return arr[0] - arr[1];
    }
}
```

## Intermediate Representation

The compiler uses a three-address code IR with the following components:

### IR Operations
- Assignment: `dest = src`
- Arithmetic: `dest = src1 op src2`
- Array Operations: `dest = array[index]`
- Control Flow: `if cond goto label`
- Function: `call func(args)`

### Basic Blocks
- Linear sequences of instructions
- Single entry, single exit
- Connected by control flow edges

## Code Generation

### Register Allocation
- Simple stack-based allocation
- Fixed register assignments for operations
- Spilling to memory when needed

### Stack Frame
- Function prologue/epilogue
- Local variable storage
- Parameter passing
- Array storage

## Testing

The compiler includes a comprehensive test suite covering:
- Arithmetic operations
- Array operations
- Boolean logic
- Control flow
- Error handling
- Type checking

Run tests with:
```bash
./mini_compiler < tests/test_name.txt
```

## Known Limitations

1. Limited optimization support
2. Basic register allocation
3. No support for:
   - Structs
   - Global variables
   - Multiple source files

## Future Improvements

1. Enhanced optimization passes
2. Better register allocation
3. Support for additional language features:
   - Structure types
   - Global scope
   - Multiple source files
   - Standard library

## Contributing

1. Fork the repository
2. Create a feature branch
3. Submit a pull request

## License

This project is licensed under the MIT License - see LICENSE file for details.
