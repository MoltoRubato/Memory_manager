#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "tasks.h"

int main(int argc, char *argv[]){
    char *filename = NULL;
    char *task = NULL;

    // Assuming by the project specifications that all arguments are passed correctly
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[++i]; // filename after -f
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            task = argv[++i];     // task after -t
        }
    }

    // Open input file
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    // Delegate to the task
    if (strcmp(task, "task1") == 0) {
        task1(file);
    } else if (strcmp(task, "task2") == 0) {
        task2(file);
    } else if (strcmp(task, "task3") == 0) {
        task3(file);
    } else if (strcmp(task, "task4") == 0) {
        task4(file);
    } else {
        // Not in the task description?
        perror("Unknown Task");
        fclose(file);
        return 1;
    }

    fclose(file);
    return 0;
}
