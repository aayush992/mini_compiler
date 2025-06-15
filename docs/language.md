# CustomLang Language Specification

## 1. Introduction

CustomLang is a simple, statically-typed programming language designed for educational purposes. It features basic programming constructs while maintaining a clean and straightforward syntax.

## 2. Lexical Elements

### 2.1 Comments
```c
// Single-line comment
/* Multi-line
   comment */
```

### 2.2 Identifiers
- Begin with a letter or underscore
- Followed by letters, digits, or underscores
- Case-sensitive
```c
valid_name
_temp
myVariable123
```

### 2.3 Keywords
```c
if      else    return  true
false   int     bool    string
void    function
```

### 2.4 Operators
```c
Arithmetic: +  -  *  /
Logical:    && ||  !
Comparison: == !=  <  >  <=  >=
Assignment: =
```

### 2.5 Literals
```c
// Integer literals
42
-17
0

// Boolean literals
true
false

// String literals
"Hello, world!"
"String with \"quotes\""
"Escape sequences: \n \t \r \\"
```

## 3. Types

### 3.1 Basic Types
```c
int     // 32-bit signed integer
bool    // Boolean value (true/false)
string  // Text string
void    // Used for functions with no return value
```

### 3.2 Type Rules
- No implicit type conversion
- Boolean operations require boolean operands
- Arithmetic operations require integer operands
- Comparison operations work on integers and strings

## 4. Expressions

### 4.1 Arithmetic Expressions
```c
x + y
a - b
m * n
p / q
```

### 4.2 Boolean Expressions
```c
x && y      // Logical AND
a || b      // Logical OR
!condition  // Logical NOT
```

### 4.3 Comparison Expressions
```c
x == y      // Equal to
a != b      // Not equal to
m < n       // Less than
p > q       // Greater than
i <= j      // Less than or equal to
u >= v      // Greater than or equal to
```

## 5. Statements

### 5.1 Declaration Statements
```c
int x;
bool flag;
string message;
```

### 5.2 Assignment Statements
```c
x = 42;
flag = true;
message = "Hello";
```

### 5.3 Conditional Statements
```c
if (condition) {
    // then branch
} else {
    // else branch
}
```

### 5.4 Return Statement
```c
return expression;
return;  // For void functions
```

## 6. Functions

### 6.1 Function Declaration
```c
return_type function_name(parameter_list) {
    // function body
}
```

### 6.2 Function Parameters
```c
int add(int x, int y) {
    return x + y;
}
```

### 6.3 Function Call
```c
result = add(5, 3);
```

## 7. Scope Rules

- Block scope for variables
- No global variables
- Variables must be declared before use
- Functions must be declared before use

## 8. Program Structure

### 8.1 Basic Program
```c
int main() {
    // Program entry point
    return 0;
}
```

### 8.2 Function Definitions
```c
int factorial(int n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

int main() {
    int result = factorial(5);
    return result;
}
```

## 9. Error Handling

The compiler reports the following types of errors:
1. Lexical errors (invalid tokens)
2. Syntax errors (invalid program structure)
3. Type errors (type mismatches)
4. Scope errors (undefined variables)
5. Function errors (undefined functions, parameter mismatches)

## 10. Implementation Limitations

1. No support for:
   - Loops
   - Arrays
   - Structures
   - Global variables
   - Multiple source files
2. Limited optimization
3. Basic error recovery 