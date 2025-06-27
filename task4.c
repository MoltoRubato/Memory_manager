#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "tasks.h"

TLBEntry tlb[TLB_SIZE] = {0}; // Global TLB since its only 32 entries

int tlb_lookup(int page_number, int *frame_number) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].valid && tlb[i].page_number == page_number) {
            *frame_number = tlb[i].frame_number; // Assign frame number from TLB
            return i; // TLB hit at index i
        }
    }
    return -1; // Miss
}

// Invalidate the evicted page in TLB
void tlb_flush(int evicted_page) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].valid && tlb[i].page_number == evicted_page) {
            tlb[i].valid = 0;
            printf("tlb-flush=%d,tlb-size=%d\n", evicted_page, TLB_SIZE - 1);
            break;
        }
    }
}

// Add new entry to TLB, get rid of invalid entries
void tlb_update(int page_number, int frame_number) {
    // Find an invalid spot for new entry
    int min_age = __INT_MAX__, lru_index = -1;
    for (int i = 0; i < TLB_SIZE; i++) {
        if (!tlb[i].valid) { // Invalid entry immediately gets replaced
            lru_index = i;
            break;
        }
        if (tlb[i].lru_age < min_age) {
            min_age = tlb[i].lru_age;
            lru_index = i; // If no invalid entries, find the least recently used entry
        }
    }

    char *rp = malloc(16);
    assert(rp != NULL);
    if (tlb[lru_index].valid) {
        sprintf(rp, "%d", tlb[lru_index].page_number);
    } else {
        strcpy(rp, "none");
    }
    
    printf("tlb-remove=%s,tlb-add=%d\n", rp, page_number);
    free(rp);

    // Update
    tlb[lru_index].page_number = page_number;
    tlb[lru_index].frame_number = frame_number;
    tlb[lru_index].valid = 1;
    tlb[lru_index].lru_age = TLB_SIZE; // Most recent

    // Age the others
    for (int i = 0; i < TLB_SIZE; i++) {
        if (i != lru_index && tlb[i].valid)
            tlb[i].lru_age--;
    }
}


void task4(FILE *file) {
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


        int frame_number;
        int tlb_index = tlb_lookup(page_number, &frame_number);

        if (tlb_index != -1) {
            // TLB Hit
            tlb[tlb_index].lru_age = TLB_SIZE;
            for (int i = 0; i < TLB_SIZE; i++)
                if (i != tlb_index && tlb[i].valid)
                    tlb[i].lru_age--; // aging

            uint32_t physical_address = (frame_number << PAGE_SHIFT) | offset;
            printf("tlb-hit=1,page-number=%u,frame=%u,physical-address=%u\n",
                   page_number, frame_number, physical_address);
            continue;
        }
        // TLB miss
        printf("tlb-hit=0,page-number=%u,frame=none,physical-address=none\n", page_number);


        // Check page from Page Table
        int page_fault = 0;
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

                tlb_flush(evicted_page); // update TLB
            }

        } else {
            // Page already present
            frame_number = page_table[page_number].frame_number;
        }

        tlb_update(page_number, frame_number); // Add to TLB

        uint32_t physical_address = (frame_number << PAGE_SHIFT) | offset; // Shift back 12 bits for the offset

        // Task 2 output
        printf("page-number=%u,page-fault=%d,frame-number=%u,physical-address=%u\n",
               page_number, page_fault, frame_number, physical_address);
    }

    free(page_table);
    free(fifo_pageQ);
}