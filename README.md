# Virtual Memory Manager

A C program that simulates a virtual memory management system with TLB (Translation Lookaside Buffer) and page table for address translation, implementing FIFO page replacement and LRU TLB replacement algorithms.

## Project Overview

This virtual memory manager simulates address translation for a single process, converting logical addresses to physical addresses using a combination of TLB and page table lookups. The system handles page faults through frame allocation and implements page replacement when physical memory is full.

## System Specifications

### Memory Configuration
- **Logical Address Space**: 4MB (2²² bytes)
- **Physical Address Space**: 1MB (2²⁰ bytes)
- **Page/Frame Size**: 4KB (2¹² bytes)
- **Number of Pages**: 1024 (2¹⁰)
- **Number of Physical Frames**: 256 (2⁸)
- **TLB Entries**: 32

### Address Structure
- **Logical Address**: 22 bits (10-bit page number + 12-bit offset)
- **Input Format**: 32-bit unsigned integers (upper 10 bits masked)

## Features

### Task 1: Logical Address Parser
- Extracts page number and offset from 32-bit logical addresses
- Masks upper 10 bits to work with 22-bit address space
- Provides detailed address parsing output

### Task 2: Page Table Implementation
- Implements page table with 1024 entries
- Handles initial frame allocation in increasing frame order
- Manages page-to-frame mapping with present/absent bits
- Assumes sufficient frames available (≤256 pages referenced)

### Task 3: FIFO Page Replacement
- Implements First-In-First-Out page replacement algorithm
- Handles memory allocation when all frames are occupied
- Evicts oldest page when new page needs to be loaded
- Updates page table entries accordingly

### Task 4: TLB Integration
- 32-entry Translation Lookaside Buffer implementation
- LRU (Least Recently Used) replacement policy for TLB entries
- TLB flush functionality when pages are evicted from memory
- Optimized address translation through TLB hits

## Usage

```bash
./translate -f <filename> -t <task>
```

### Parameters
- `-f <filename>`: Path to input file containing logical addresses
- `-t <task>`: Task to execute (`task1`, `task2`, `task3`, or `task4`)

### Input Format
Input file contains one logical address per line (32-bit unsigned integers):
```
30023
10001
20007
```

## Output Examples

### Task 1 Output
```
logical-address=30023,page-number=7,offset=1351
logical-address=10001,page-number=2,offset=1809
logical-address=20007,page-number=4,offset=3623
```

### Task 2 Output
```
logical-address=30023,page-number=7,offset=1351
page-number=7,page-fault=1,frame-number=0,physical-address=1351
logical-address=10001,page-number=2,offset=1809
page-number=2,page-fault=1,frame-number=1,physical-address=5905
```

### Task 3 Output (with eviction)
```
logical-address=90048,page-number=21,offset=4032
evicted-page=7,freed-frame=0
page-number=21,page-fault=1,frame-number=0,physical-address=4032
```

### Task 4 Output (with TLB)
```
logical-address=30023,page-number=7,offset=1351
tlb-hit=0,page-number=7,frame=none,physical-address=none
tlb-remove=none,tlb-add=7
page-number=7,page-fault=1,frame-number=0,physical-address=1351
```

## Build Instructions

### Prerequisites
- GCC compiler
- Make utility
- Linux/Unix environment

### Compilation
```bash
make clean && make -B
```

### Clean Build Files
```bash
make clean
```

## Algorithm Details

### Address Translation Process
1. **Parse logical address** → Extract page number and offset
2. **Check TLB** → Search for existing page-frame mapping
3. **Access page table** → If TLB miss, consult page table
4. **Handle page fault** → Allocate frame or evict page if needed
5. **Update TLB** → Add new entry using LRU replacement
6. **Calculate physical address** → Combine frame number and offset

### FIFO Page Replacement
- Maintains queue of pages in memory
- Evicts oldest page when all frames occupied
- Updates both page table and TLB accordingly

### LRU TLB Management
- Tracks access order of TLB entries
- Replaces least recently used entry when TLB is full
- Flushes entries for evicted pages

## Testing

The program includes comprehensive test coverage:
- Basic address parsing validation
- Page table functionality verification
- FIFO replacement algorithm testing
- TLB hit/miss scenarios
- Edge cases and boundary conditions

Run tests using provided test cases in the `tests/` directory.

## Implementation Notes

- **Memory Management**: Proper allocation and deallocation of data structures
- **Error Handling**: Robust handling of invalid inputs and edge cases
- **Performance**: Efficient algorithms for TLB and page table operations
- **Code Quality**: Clean, well-documented, and modular code structure

## Contributors
- Kerui Huang

## License

This project is for educational purposes as part of COMP30023 coursework.
