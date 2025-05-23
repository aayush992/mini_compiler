# CustomLang Mini Compiler

A simplified compiler implementation for the CustomLang programming language, featuring lexical analysis, parsing, and x86 assembly code generation.

## Language Features

### Data Types
- `int`: Integer values
- `bool`: Boolean values (true/false)
- `string`: Text strings with escape sequences
- `void`: Used for functions with no return value

### Operators
- Arithmetic: `+`, `-`, `*`, `/`
- Logical: `&&`, `||`, `!`
- Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Assignment: `=`

### Control Flow
- If-else statements
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
│   └── test_cases.c    # Test suite
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
./compiler input.cl -o output
```

### Example Program
```c
// Sample CustomLang program
int main() {
    int x = 42;
    int y = 10;
    
    if (x > y) {
        return x + y;
    } else {
        return x - y;
    }
}
```

## Intermediate Representation

The compiler uses a three-address code IR with the following components:

### IR Operations
- Assignment: `dest = src`
- Arithmetic: `dest = src1 op src2`
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

## Testing

The compiler includes a comprehensive test suite covering:
- Arithmetic operations
- Boolean logic
- Control flow
- Error handling

Run tests with:
```bash
./test_cases
```

## Known Limitations

1. Limited optimization support
2. Basic register allocation
3. No support for:
   - Loops
   - Arrays
   - Structs
   - Global variables

## Future Improvements

1. Enhanced optimization passes
2. Better register allocation
3. Support for additional language features:
   - For/while loops
   - Array operations
   - Structure types
   - Global scope

## Contributing

1. Fork the repository
2. Create a feature branch
3. Submit a pull request

## License

This project is licensed under the MIT License - see LICENSE file for details. 