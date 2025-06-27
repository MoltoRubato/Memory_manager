#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "tasks.h"


void task2(FILE *file) {
    PageTableEntry *page_table = calloc(NUM_PAGES, sizeof(PageTableEntry));
    assert(page_table != NULL);
    uint8_t next_free_frame = 0; // Max 256 frames 

    uint32_t logical_address;
    while (fscanf(file, "%u", &logical_address) == 1) {
        // First do task 1
        int32_t masked = logical_address & ADDRESS_MASK;
        uint32_t offset = masked & OFFSET_MASK;
        uint32_t page_number = (masked >> PAGE_SHIFT) & PAGE_MASK;
        printf("logical-address=%u,page-number=%u,offset=%u\n",
               logical_address, page_number, offset);

        // Then assign frame number
        int page_fault = 0;
        uint8_t frame_number;
        if (!page_table[page_number].present) { // New page number
            page_fault = 1;
            frame_number = next_free_frame++;
            page_table[page_number].present = 1;
            page_table[page_number].frame_number = frame_number;
        } else {
            frame_number = page_table[page_number].frame_number;
        }

        uint32_t physical_address = (frame_number << PAGE_SHIFT) | offset; // Shift back 12 bits for the offset

        printf("page-number=%u,page-fault=%d,frame-number=%u,physical-address=%u\n",
               page_number, page_fault, frame_number, physical_address);
    }
    free(page_table);
}