# PA2: MIPS Pipeline Simulator

A comprehensive 5-stage MIPS pipeline simulator implemented in C that demonstrates advanced computer architecture concepts.

## Overview

This project implements a complete 5-stage MIPS pipeline simulator that reads machine instructions and simulates their execution through the pipeline stages: Instruction Fetch (IF), Instruction Decode (ID), Execute (EX), Memory Access (MEM), and Write Back (WB).

## Features

### Pipeline Architecture
- **5-Stage Pipeline**: IF, ID, EX, MEM, WB
- **Hazard Detection**: Automatic detection of load-use hazards
- **Data Forwarding**: Implements forwarding to resolve data hazards
- **Branch Prediction**: Supports branch prediction mechanisms

### Supported Instructions
- `add` - Addition
- `sub` - Subtraction  
- `lw` - Load Word
- `sw` - Store Word
- `andi` - AND Immediate
- `ori` - OR Immediate
- `sll` - Shift Left Logical
- `bne` - Branch Not Equal
- `halt` - Halt execution
- `noop` - No operation

### Advanced Features
- **Stall Insertion**: Automatically inserts stalls for hazard resolution
- **Memory Management**: Proper handling of data and instruction memory
- **Statistics Tracking**: Comprehensive cycle counting and performance metrics

## Compilation & Usage

### Compile the Simulator
```bash
gcc -o PA2 PA2.c
```

### Run the Simulator
```bash
./PA2 < input_file.in > output_file.out
```

### Using the Test Generator
Convert assembly files to machine code:
```bash
chmod 744 testGen.exe
./testGen.exe < assembly_file.asm > input_file.in
```

## Test Cases

The project includes 10 comprehensive test cases:

1. **arithmetic.asm** - Basic arithmetic operations
2. **loadStore.asm** - Memory load and store operations
3. **dataHazard.asm** - Load-use hazard scenarios
4. **forwarding.asm** - Data forwarding test cases
5. **branch.asm** - Branch instruction testing
6. **logical.asm** - Logical operation testing
7. **shift.asm** - Shift operation testing
8. **noop.asm** - No-operation instruction testing
9. **manyHazards.asm** - Complex hazard scenarios
10. **branchPrediction.asm** - Branch prediction testing

## Testing Workflow

1. **Set permissions**:
   ```bash
   chmod 744 testGen.exe Forwarding.exe NoHazards.exe HaltAndNoop.exe
   ```

2. **Compile the simulator**:
   ```bash
   gcc -o PA2 PA2.c
   ```

3. **Test individual cases**:
   ```bash
   ./testGen.exe < dataHazard.asm > dataHazard.in
   ./Forwarding.exe < dataHazard.in > dataHazard.out  
   ./PA2 < dataHazard.in > dataHazard_my.out
   ```

4. **Compare outputs** (if needed):
   ```bash
   diff file1 file2
   ```

## Output Format

The simulator provides detailed output including:
- Pipeline state at each cycle
- Register values throughout execution
- Memory contents
- Final statistics (cycles, stalls, branches, mispredictions)

## Learning Outcomes

This project demonstrates understanding of:
- Pipeline architecture and stage interactions
- Hazard detection and resolution techniques
- Data forwarding mechanisms
- Branch prediction strategies
- Memory management in processors
- C programming for systems-level development

## Files

- `PA2.c` - Main simulator implementation
- `*.asm` - Assembly test files
- `testGen.exe` - Assembly to machine code converter
- `Forwarding.exe` - Reference implementation for forwarding
- `NoHazards.exe` - Reference implementation without hazards
- `HaltAndNoop.exe` - Reference implementation for halt/noop

## Author

**Darya Pylypenko**  
*CDA 3100 - Computer Architecture*  
*Summer 2025*
