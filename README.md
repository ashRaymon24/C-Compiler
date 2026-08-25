# Mini C Compiler

A compiler for a subset of the C language written in C++.

## Features

- Integer variables
- Variable assignments
- Arithmetic expressions
- Comparison operators
- If statements
- If-else statements
- While loops
- Semantic analysis
- x86-64 assembly generation

## Example

Input:

int main() {
    int x = 5;

    while (x > 0) {
        x = x - 1;
    }

    return x;
}

Output:

x86-64 assembly executable returning 0.