#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "tasks.h"

void task3(FILE *file) {
    PageTableEntry *page_table = calloc(NUM_PAGES, sizeof(PageTableEntry));
    assert(page_table != NULL);
    int next_free_frame = 0;

    int *fifo_pageQ = malloc(NUM_FRAMES * sizeof(int)); // Use circular queue for FIFO
    assert(fifo_pageQ != NULL);
    int fifo_front = 0, fifo_back = 0; 

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
        int frame_number;
        if (!page_table[page_number].present) { // New page number
            page_fault = 1;
            
            if (next_free_frame < NUM_FRAMES) {
                // Still have free frames
                frame_number = next_free_frame++;

                // Initialise the page table entry
                page_table[page_number].present = 1;
                page_table[page_number].frame_number = frame_number;

                // Add to FIFO queue
                fifo_pageQ[fifo_back] = page_number;
                fifo_back = (fifo_back + 1) % NUM_FRAMES; // Move back pointer

            } else {
                // Evict a page using FIFO
                int evicted_page = fifo_pageQ[fifo_front];
                fifo_front = (fifo_front + 1) % NUM_FRAMES; // Move front pointer

                frame_number = page_table[evicted_page].frame_number; // Free frame
                page_table[evicted_page].present = 0; // Mark as not present

                // initialise the page table entry
                page_table[page_number].present = 1;
                page_table[page_number].frame_number = frame_number;

                // Add new page to FIFO queue
                fifo_pageQ[fifo_back] = page_number;
                fifo_back = (fifo_back + 1) % NUM_FRAMES; // Move back pointer

                printf("evicted-page=%u,freed-frame=%u\n", evicted_page, frame_number);
            }

        } else {
            // Page already present
            frame_number = page_table[page_number].frame_number;
        }

        uint32_t physical_address = (frame_number << PAGE_SHIFT) | offset; // Shift back 12 bits for the offset
        
        // Task 2 output
        printf("page-number=%u,page-fault=%d,frame-number=%u,physical-address=%u\n",
               page_number, page_fault, frame_number, physical_address);
    }

    free(page_table);
    free(fifo_pageQ);
}