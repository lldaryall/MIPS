# PA3: Cache Simulation

A comprehensive cache simulation program that models different cache architectures and replacement policies, implemented in C.

## Overview

This project simulates the operation of various cache configurations including write-through and write-back policies, set-associative caches, and split instruction/data caches. It demonstrates advanced understanding of memory hierarchy and caching mechanisms.

## Features

### Cache Policies
- **Write-through with No Write Allocate**
- **Write-back with Write Allocate**
- **LRU (Least Recently Used) Replacement Policy**

### Cache Configurations
- **Unified Cache**: Single cache for both instructions and data
- **Split Cache**: Separate instruction and data caches (Extra Credit)
- **Set-Associative**: Configurable associativity levels
- **Dynamic Sizing**: Support for various block sizes and set counts

### Advanced Features
- **Dynamic Memory Allocation**: Proper memory management with malloc/free
- **Comprehensive Statistics**: Hit/miss ratios, memory references
- **Flexible Input Format**: Supports various test scenarios
- **Extra Credit Implementation**: Split I/D cache functionality

## Compilation & Usage

### Compile the Simulator
```bash
gcc PA3_EC.c -lm
```

### Run the Simulator
```bash
./a.out < test_file.in
```

### Example Usage
```bash
gcc PA3_EC.c -lm
./a.out < test1.basic.in
```

## Input Format

The input file contains:
- **Line 1**: Block size
- **Line 2**: Number of sets  
- **Line 3**: Associativity
- **Lines 4-103**: Memory references in format `R/W<tab>address`
  - `R` for read operations
  - `W` for write operations
  - Address as 32-bit positive integer

### Extra Credit Input Format
For split cache testing, format is:
```
R/W<tab>address<tab>I/D
```
Where `I` indicates instruction reference and `D` indicates data reference.

## Output Format

### Standard Output
```
Block size: [size]
Number of sets: [sets]
Associativity: [associativity]
Number of offset bits: [offset_bits]
Number of index bits: [index_bits]
Number of tag bits: [tag_bits]
****************************************
Write-through with No Write Allocate
****************************************
Total number of references: [count]
Hits: [hits]
Misses: [misses]
Memory References: [memory_refs]
****************************************
Write-back with Write Allocate
****************************************
Total number of references: [count]
Hits: [hits]
Misses: [misses]
Memory References: [memory_refs]
```

### Split Cache Output
Additional output section for split I/D cache:
```
============= Split I vs. D =============
[Similar format for instruction and data caches separately]
```

## Test Cases

The project includes 8 comprehensive test cases:

1. **test1.basic.in** - Basic cache operations
2. **test2.associative.in** - 2-way set associative cache testing
3. **test3.writeTandB.in** - Write-through vs write-back policy comparison
4. **test4.dirty.in** - Dirty block testing
5. **test5.splitCache.in** - Split I/D cache functionality
6. **test6.large.in** - Large cache size testing
7. **test7.readWrite.in** - Read and write operation testing
8. **test8.repeated.in** - Repeated access pattern testing

## Technical Implementation

### Key C Concepts Used
- **Dynamic Allocation**: `malloc()` and `free()` for memory management
- **Structures**: Custom cache data structures
- **Bit Manipulation**: Calculating tag, index, and offset bits
- **File I/O**: Reading input from stdin
- **Mathematical Functions**: Using `-lm` flag for math operations

### Data Structures
```c
typedef struct {
    int valid;
    int dirty;
    int tag;
    int last_used;
} CacheBlock;

typedef struct {
    CacheBlock *blocks;
    int associativity;
} CacheSet;
```

## Learning Outcomes

This project demonstrates understanding of:
- Cache organization and addressing
- Write policies and their trade-offs
- Replacement algorithms (LRU)
- Memory hierarchy concepts
- Set-associative cache design
- Split cache architectures
- C programming for systems development

## Files

- `PA3_EC.c` - Main cache simulator with extra credit
- `test*.in` - Test input files
- `README_old` - Original detailed documentation

## Extra Credit Features

- **Split I/D Caches**: Separate instruction and data caches
- **Half-set Allocation**: Each sub-cache gets s/2 sets
- **Instruction-only I-cache**: No write policies needed for instructions
- **Comprehensive Testing**: Additional test cases for split cache functionality

## Author

**Darya Pylypenko**  
*CDA 3100 - Computer Architecture*  
*Summer 2025*

## Testing Status

All test cases have been successfully implemented and tested:
- Basic cache operations
- Associativity testing
- Write policy comparison
- Dirty block handling
- Split cache functionality
- Large cache performance
- Read/write operations
- Repeated access patterns

*The program has been thoroughly tested and produces correct output matching expected results.*
