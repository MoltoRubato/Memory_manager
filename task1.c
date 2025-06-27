#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "tasks.h"

void task1(FILE *file) {
    uint32_t logical_address;
    while (fscanf(file, "%u", &logical_address) == 1) {
        uint32_t masked = logical_address & ADDRESS_MASK;          // Mask first 10 bits for parsing
        uint32_t offset = masked & OFFSET_MASK;                    // Keep the last 12 bits for the offset
        uint32_t page_number = (masked >> PAGE_SHIFT) & PAGE_MASK; // Keep the middle 10 bits for the page number
        
        printf("logical-address=%u,page-number=%u,offset=%u\n",
               logical_address, page_number, offset);
    }
}