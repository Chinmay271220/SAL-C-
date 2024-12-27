### README.txt Contents

Project: Assembly Language Interpreter (ALI)

Description:
The Assembly Language Interpreter (ALI) is a virtual machine designed to execute programs written in the Simple Assembly Language (SAL). It simulates basic hardware components, including memory, registers, and a program counter, to process SAL instructions. The project implements SAL's instruction set and supports functionalities like arithmetic operations, conditional jumps, and memory manipulation.

ALI allows users to execute programs step-by-step for debugging or all at once until completion.

Instructions for Use:
1. Compilation:
Ensure you have a C++ compiler that supports the C++11 standard or later. Compile the project using the following command:

```bash
g++ -std=c++11 main.cpp ALI.cpp -o ALI
```

2. Running the Program:
After compilation, run the program with:

```bash
./ALI
```
The interpreter will prompt you to provide the file name of a SAL program. Use a valid SAL file (e.g., sample.txt) to load and execute it.

3. Files Included in the Project:

ALI.h: Defines the ALI class and instruction subclasses.
ALI.cpp: Implements the ALI class and the SAL instruction set.
main.cpp: Entry point of the program, handles initialization and command loop.
sample.txt: Example SAL program for testing.
