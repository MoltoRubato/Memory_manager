#ifndef TASKS_H
#define TASKS_H

#include <stdio.h>
#include <stdint.h>

#define ADDRESS_MASK 0b00000000001111111111111111111111  // 22 bits logical address mask
#define OFFSET_MASK  0b00000000000000000000111111111111  // 12 bits offset mask
#define PAGE_MASK    0b00000000000000000000001111111111  // 10 bits page number mask
#define PAGE_SHIFT   12                                  // page number shift length

#define NUM_PAGES 1024
#define NUM_FRAMES 256
#define TLB_SIZE 32

typedef struct {
    int present;           // 1 if page is mapped
    uint8_t frame_number;  // 0–255
} PageTableEntry;

typedef struct {
    int page_number;
    int frame_number;
    int lru_age; // Higher = recently used
    int valid;
} TLBEntry;

void task1(FILE *file);
void task2(FILE *file);
void task3(FILE *file);
void task4(FILE *file);

#endif


