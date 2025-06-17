# Language Specification

## 1. Introduction

A simple, statically-typed programming language designed for educational purposes. It features basic programming constructs while maintaining a clean and straightforward syntax.

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
false   int     float   bool
string  void    while   for
print   input
```

### 2.4 Operators
```c
Arithmetic: +  -  *  /  %
Logical:    && ||  !
Comparison: == !=  <  >  <=  >=
Assignment: =
Array:      []
```

### 2.5 Literals
```c
// Integer literals
42
-17
0

// Float literals
3.14
-0.5
1.0

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
float   // 32-bit floating point
bool    // Boolean value (true/false)
string  // Text string
void    // Used for functions with no return value
```

### 3.2 Array Types
```c
int arr[5];        // Integer array
float farr[10];    // Float array
string sarr[3];    // String array
```

### 3.3 Type Rules
- No implicit type conversion
- Boolean operations require boolean operands
- Arithmetic operations work on integers and floats
- Comparison operations work on integers, floats, and strings
- Array indices must be integers
- Array elements must match array type

## 4. Expressions

### 4.1 Arithmetic Expressions
```c
x + y
a - b
m * n
p / q
r % s
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

### 4.4 Array Expressions
```c
arr[0]      // Array access
arr[i]      // Variable index
arr[i + 1]  // Expression index
```

## 5. Statements

### 5.1 Declaration Statements
```c
int x;
float y;
bool flag;
string message;
int arr[5];
float farr[10];
```

### 5.2 Assignment Statements
```c
x = 42;
y = 3.14;
flag = true;
message = "Hello";
arr[0] = x;
farr[i] = y;
```

### 5.3 Conditional Statements
```c
if (condition) {
    // then branch
} else {
    // else branch
}
```

### 5.4 Loop Statements
```c
while (condition) {
    // loop body
}

for (i = 0; i < 10; i = i + 1) {
    // loop body
}
```

### 5.5 Input/Output Statements
```c
print x;           // Print variable
print "Hello";     // Print string
input x;           // Read into variable
```

### 5.6 Return Statement
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

float average(float arr[10]) {
    // function body
}
```

### 6.3 Function Call
```c
result = add(5, 3);
avg = average(scores);
```

## 7. Scope Rules

- Block scope for variables
- No global variables
- Variables must be declared before use
- Functions must be declared before use
- Array sizes must be constant

## 8. Program Structure

### 8.1 Basic Program
```c
int main() {
    // Program entry point
    return 0;
}
```

### 8.2 Example Program
```c
int factorial(int n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

int main() {
    int arr[5];
    arr[0] = 1;
    
    for (i = 1; i < 5; i = i + 1) {
        arr[i] = factorial(i);
    }
    
    return arr[4];
}
```

## 9. Error Handling

The compiler reports the following types of errors:
1. Lexical errors (invalid tokens)
2. Syntax errors (invalid program structure)
3. Type errors (type mismatches)
4. Scope errors (undefined variables)
5. Array errors (invalid indices, type mismatches)
6. Function errors (undefined functions, parameter mismatches)

## 10. Implementation Limitations

1. No support for:
   - Structs
   - Global variables
   - Multiple source files
   - Dynamic arrays
2. Limited optimization
3. Basic error recovery
4. Fixed array sizes 