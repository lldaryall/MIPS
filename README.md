# MIPS Computer Architecture Projects

This repository contains two comprehensive MIPS (Microprocessor without Interlocked Pipeline Stages) projects developed for Computer Architecture coursework.

## Projects Overview

### [PA2: MIPS Pipeline Simulator](./PA2/)
A 5-stage MIPS pipeline simulator that implements instruction execution in a pipelined processor architecture.

**Key Features:**
- 5-stage pipeline: IF, ID, EX, MEM, WB
- Hazard detection and resolution
- Data forwarding support
- Branch prediction
- Support for multiple MIPS instructions

**Technologies:** C Programming

---

### [PA3: Cache Simulation](./PA3/)
A comprehensive cache simulation program that models different cache architectures and policies.

**Key Features:**
- Write-through and write-back cache policies
- Set-associative cache simulation
- LRU replacement policy
- Split instruction/data cache support (Extra Credit)
- Dynamic memory allocation

**Technologies:** C Programming

---

## Quick Start

### Prerequisites
- GCC compiler
- Linux/Unix environment (tested on linprog)

### Running the Projects

#### PA2 - Pipeline Simulator
```bash
cd PA2/
gcc -o PA2 PA2.c
./PA2 < input_file.in > output_file.out
```

#### PA3 - Cache Simulator
```bash
cd PA3/
gcc PA3_EC.c -lm
./a.out < test_file.in
```

## Project Structure

```
MIPS/
├── README.md                 # This file
├── PA2/                     # MIPS Pipeline Simulator
│   ├── PA2.c               # Main pipeline simulator
│   ├── README.md           # Detailed PA2 documentation
│   ├── *.asm               # Assembly test files
│   └── *.exe               # Test utilities
└── PA3/                     # Cache Simulation
    ├── PA3_EC.c            # Main cache simulator
    ├── README.md           # Detailed PA3 documentation
    └── test*.in            # Test input files
```

## Testing

Both projects include comprehensive test suites:

- **PA2**: 10+ test cases covering arithmetic, memory operations, hazards, and branching
- **PA3**: 8 test cases covering basic operations, associativity, write policies, and split caches

## Author

**Darya Pylypenko**  
*Computer Architecture Student*

## Course Information

- **Course:** CDA 3100 - Computer Architecture
- **Term:** Summer 2025

---

*These projects demonstrate advanced understanding of computer architecture principles including pipelining, caching, hazard detection, and memory management.*
