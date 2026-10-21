```markdown
# Three Address Code (TAC) Generator for Arithmetic Expressions

This repository contains a C++ program developed for the Compiler Design Lab (BCSE307P) project. The tool accepts arithmetic expressions and converts them into Three Address Code (TAC). It also displays the generated intermediate code across Quadruples, Triples, and Indirect Triples representations, along with a Symbol Table.

## Project Description

The project demonstrates key concepts of Intermediate Code Generation in compilers. It validates arithmetic expressions, enforces operator precedence and associativity using a stack-based parser, and generates temporary variables to break down complex expressions into simple instructions with at most three addresses.

Key components included:
- Lexical and semantic checks for invalid expressions or unbalanced parentheses.
- Three Address Code generation using temporary variables (`t1`, `t2`, etc.).
- Quadruples table representation (`Operator`, `Arg1`, `Arg2`, `Result`).
- Triples table representation (using instruction position references).
- Indirect Triples representation (using pointer arrays).
- Symbol Table for tracking variables and constants.

## How to Run in VS Code

### Prerequisites
A C++ compiler (`g++`) must be installed on your system.

### Steps

1. Open the project folder in VS Code (`File` > `Open Folder...`).
2. Open the terminal in VS Code using `Ctrl + ~` (or `Cmd + ~` on Mac).
3. Compile `main.cpp` using `g++`:

```bash
g++ main.cpp -o tac_generator

```

4. Run the executable:

* On Linux / macOS:

```bash
./tac_generator

```

* On Windows (CMD / PowerShell):

```cmd
.\tac_generator.exe

```

5. Enter any valid arithmetic expression when prompted by the terminal.

## Author

Neeraj Korrapati

Reg No: 24BCE2393

VIT Vellore

```

```
