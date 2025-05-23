// Test program in CustomLang

int factorial(int n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

int main() {
    // Test arithmetic
    int x = 42;
    int y = 10;
    int sum = x + y;
    int diff = x - y;
    int prod = x * y;
    int quot = x / y;

    // Test boolean operations
    bool a = true;
    bool b = false;
    bool and_result = a && b;
    bool or_result = a || b;
    bool not_result = !a;

    // Test comparisons
    bool eq = x == y;
    bool neq = x != y;
    bool lt = x < y;
    bool gt = x > y;

    // Test function call
    int fact5 = factorial(5);

    return fact5;
} 